#include "AudioManager.h"

AudioManager& AudioManager::instance() {
   static AudioManager *instance = new AudioManager();
   return *instance;
}

AudioManager::AudioManager() :
   system_(NULL),
   stChannel_(NULL),
   eChannel_(NULL) {
   FMOD_RESULT result;
   unsigned int version;
   int numDrivers;

   result = FMOD::System_Create(&system_);
   FMODErrorCheck(result);

   system_->getVersion(&version);
   if (version < FMOD_VERSION) {
      std::cout << "FMOD lib version " << version << " doesn't match header";
      std::cout << " version " << FMOD_VERSION << std::endl;
      exit(-1);
   }
   std::cout << "FMOD Version " << version << std::endl;

   // Check how many sound cards are present.
   result = system_->getNumDrivers(&numDrivers);
   FMODErrorCheck(result);

   // If there are no sound cards, disable all sounds.
   if (numDrivers == 0) {
      result = system_->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
      FMODErrorCheck(result);
   }

   system_->init(MAX_CHANNELS, FMOD_INIT_NORMAL, 0);
   FMODErrorCheck(result);
}

AudioManager::~AudioManager() {
   system_->close();
   system_->release();

   while (!soundtrack_.empty()) {
      soundtrack_.front()->release();
      soundtrack_.pop();
   }

   for (auto it = soundeffects_.begin(); it != soundeffects_.end(); ++it) {
      it->second->release();
   }
   soundeffects_.clear();
}

void AudioManager::update() {
   FMOD_RESULT result;
   system_->update();

   if (stChannel_) {
      bool playing;

      /* If the current song is done get the next one. I don't believe there
       * is a function to check if a channel is valid, so the only way to check
       * is to call a function and check the result lol... */
      result = stChannel_->isPlaying(&playing);

      // If invalid handle the song has finished and the channel was released 
      if (result == FMOD_ERR_INVALID_HANDLE) {
         soundtrack_.push(soundtrack_.front());
         soundtrack_.pop();

         result = system_->playSound(soundtrack_.front(), 0, false, &stChannel_);
         FMODErrorCheck(result);
         stChannel_->setMode(FMOD_LOOP_OFF);
      } else {
         FMODErrorCheck(result);
      }
   }
}

void AudioManager::addTrack(std::string filename) {
   FMOD_RESULT result;
   FMOD::Sound *sound = NULL;

   /* `createStream` should be used for any compressed files (mp3/wav) as it
    * will buffer the file in chunks whereas `createSound` loads the entire
    * file into memory and uncompresses it. That is fine for simple sounds. */
   result = system_->createStream((audioPath_ + filename).c_str(),
      FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);
   FMODErrorCheck(result);

   soundtrack_.push(sound);
}

void AudioManager::startSoundtrack() {
   FMOD_RESULT result;

   if (stChannel_) {
      result = stChannel_->setPaused(false);
      if (result != FMOD_OK && result != FMOD_ERR_INVALID_HANDLE) {
         FMODErrorCheck(result);
      }
   } else {
      if (!soundtrack_.empty()) {
         result = system_->playSound(soundtrack_.front(), 0, false, &stChannel_);
         FMODErrorCheck(result);
         stChannel_->setMode(FMOD_LOOP_OFF);
      } 
   }
}

void AudioManager::pauseSoundtrack() {
   FMOD_RESULT result;

   if (stChannel_) {
      result = stChannel_->setPaused(true);
      if (result != FMOD_OK && result != FMOD_ERR_INVALID_HANDLE) {
         FMODErrorCheck(result);
      }
   }
}

void AudioManager::addEffect(std::string name, std::string filename) {
   FMOD_RESULT result;
   FMOD::Sound *sound = NULL;

   result = system_->createSound((audioPath_ + filename).c_str(),
      FMOD_LOOP_OFF | FMOD_2D, 0, &sound);
   FMODErrorCheck(result);

   std::pair<std::string, FMOD::Sound*> effectToInsert(name, sound);
   soundeffects_.insert(effectToInsert);
}

void AudioManager::playEffect(std::string effectName) {
   FMOD_RESULT result;
   FMOD::Sound* sound = soundeffects_.at(effectName);
   bool isPlaying = false;

   if (eChannel_) {
      result = eChannel_->isPlaying(&isPlaying);
      if (result != FMOD_OK && result != FMOD_ERR_INVALID_HANDLE) {
         FMODErrorCheck(result);
      }
   }

   if (!isPlaying) {
      result = system_->playSound(sound, 0, false, &eChannel_);
      FMODErrorCheck(result);
   }
}
