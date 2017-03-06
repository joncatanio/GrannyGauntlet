#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "fmod.hpp"
#include "fmod_errors.h"

#include <cstdlib>
#include <iostream>
#include <queue>

class AudioManager {
public:
   static AudioManager& instance();

   ~AudioManager();

   // Main update call for all sounds in the current audio system.
   void update();

   // Get the FMOD system in order to create/play sounds.
   FMOD::System* getSystem();

   // Adds a song to the internal game soundtrack.
   void addTrack(std::string filename);

   // Starts the soundtrack.
   void startSoundtrack();

   // Pauses the soundtrack.
   void pauseSoundtrack();
private:
   AudioManager();

   inline void FMODErrorCheck(FMOD_RESULT result) {
      if (result != FMOD_OK) {
         std::cout << "FMOD error: " << FMOD_ErrorString(result) << std::endl;
         exit(-1);
      }
   };

   const std::string audioPath_ = "../resources/audio/";

   /* The main system for FMOD to utilize for managing sounds. There must be
      a system object. */
   FMOD::System* system_;

   // Soundtrack of the game. Usually specified in the level loader.
   std::queue<FMOD::Sound*> soundtrack_;

   // The channel of the current soundtrack song.
   FMOD::Channel* stChannel_;

   // Maximum number of audio channels.
   const int MAX_CHANNELS = 512;
};

#endif
