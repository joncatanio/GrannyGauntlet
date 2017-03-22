#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "fmod.hpp"
#include "fmod_errors.h"

#include <cstdlib>
#include <iostream>
#include <queue>
#include <unordered_map>

class AudioManager {
public:
   static AudioManager& instance();

   ~AudioManager();

   // Main update call for all sounds in the current audio system.
   void update();

   // Adds a song to the internal game soundtrack.
   void addTrack(std::string filename);

   // Adds a song to the alternative internal game soundtrack.
   void addAltTrack(std::string filename);

   // Starts the soundtrack.
   void startSoundtrack();

   // Pauses the soundtrack.
   void pauseSoundtrack();

   // Swaps between alternative and regular soundtracks.
   void swapSoundtrack();

   // Adds a sound effect. This loads the sound into memory so keep it small.
   void addEffect(std::string name, std::string filename);

   // Plays an effect from the `soundeffects_` map.
   void playEffect(std::string effectName);
private:
   AudioManager();

   bool stPlaylistToggle; // True for regular playlist, false for alt.

   inline void FMODErrorCheck(FMOD_RESULT result) {
      if (result != FMOD_OK) {
         std::cout << "FMOD error: " << FMOD_ErrorString(result) << std::endl;
         exit(-1);
      }
   };

   // TODO (noj) possibly move this to resource manager or someshit.
   const std::string audioPath_ = "../resources/audio/";

   /* The main system for FMOD to utilize for managing sounds. There must be
      a system object. */
   FMOD::System* system_;

   // Soundtrack of the game. Usually specified in the level loader.
   std::queue<FMOD::Sound*> soundtrack_;

   // Alternative oundtrack of the game. Usually specified in the level loader.
   std::queue<FMOD::Sound*> altSoundtrack_;

   // The channel of the current soundtrack song.
   FMOD::Channel* stChannel_;

   // Channel of current soundeffect.
   FMOD::Channel* eChannel_;

   // A map of all sound effects, these are gathered from the JSON level.
   std::unordered_map<std::string, FMOD::Sound*> soundeffects_;

   // Maximum number of audio channels.
   const int MAX_CHANNELS = 512;
};

#endif
