#ifndef AUDIO_MANAGER_H_
#define AUDIO_MANAGER_H_

#include <stdio.h>
#include <stdlib.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include <exception>
#include <iostream>
#include <vector>

namespace audio_manager {

    // Audio manager exception type
    class AudioManagerException : public std::exception
    {
    private:
        std::string message_;
    public:
        AudioManagerException(std::string message) : message_(message) {};
        ~AudioManagerException() throw () {};
        virtual const char* what() const throw() { return message_.c_str(); };
    };

    // A simple audio manager implemented with OpenAl
    class AudioManager {
    public:
        AudioManager(void);
        ~AudioManager();
        /* Initialize the audio system. device_name is the name of
         * the audio device to be used. If the default device should
         * be used, set device_name to NULL. */
        void Init(const char* device_name);
        // Shut down the audio system
        void ShutDown(void);
        /* Load a wav audio file and add its contents to a buffer.
         * The buffer can then be played multiple times with
         * PlaySound. AddSound returns the index of the file in
         * the list of buffers. This index should be passed to
         * PlaySound to play the respective file */
        int AddSound(const char* filename);
        // Play buffer with specific index
        void PlaySound(int index);
        // Check if the buffer with the given index is being played
        bool SoundIsPlaying(int index);
        // Check if any buffer is being played
        bool AnySoundIsPlaying(void);
        // List all audio devices available to standard output
        void ListAudioDevices(void);
        // Set spatial position of listener
        void SetListenerPosition(double x, double y, double z);
        /* Set spatial position of a sound. Same index as used for
         * PlaySound */
        void SetSoundPosition(int index, double x, double y, double z);
        // Set whether sound should be looped
        void SetLoop(int index, bool loop);

    private:
        // Audio context used by OpenAl
        ALCcontext* context_;
        // All the buffers we can play
        std::vector<ALuint> buffer_;
        // One source for each buffer
        std::vector<ALuint> source_;

        // Keep track if we already initialized the audio manager
        int initialized_;

        // Auxiliary method to handle OpenAl errors
        void CheckForErrors(const char* msg);
    };

} // namespace audio_manager;

#endif // AUDIO_MANAGER_H_
