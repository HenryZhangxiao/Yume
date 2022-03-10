#include "audio_manager.h"

/* Based on the example in http://ffainelli.github.io/openal-example/ */

namespace audio_manager {


    AudioManager::AudioManager(void) {

        initialized_ = 0;

    }


    AudioManager::~AudioManager() {

        if (initialized_) {
            ShutDown();
        }
    }


    void AudioManager::Init(const char* device_name) {

        if (!initialized_) {
            ALCdevice* device = NULL;

            /* Initialize audio device */
            if (device_name) {
                device = alcOpenDevice(device_name);
            }
            else {
                /* If no device name specified, use default device */
                const char* name = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
                device = alcOpenDevice(name);
            }

            if (!device) {
                throw(AudioManagerException(std::string("Unable to open device")));
            }

            //Name of device: alcGetString(device, ALC_DEVICE_SPECIFIER));

            /* Create audio context */
            context_ = alcCreateContext(device, NULL);
            if (!alcMakeContextCurrent(context_)) {
                throw(AudioManagerException(std::string("Failed to create default context")));
            }
            CheckForErrors("Failed to create default context");

            /* Initialize the Alut library */
            alutInitWithoutContext(NULL, NULL);

            /* Remember that we initialized the audio system */
            initialized_ = 1;
        }
    }


    void AudioManager::ShutDown(void) {

        if (initialized_) {

            ALCdevice* device;

            for (int i = 0; i < buffer_.size(); i++) {
                alDeleteSources(1, &source_[i]);
                alDeleteBuffers(1, &buffer_[i]);
            }
            device = alcGetContextsDevice(context_);
            alcMakeContextCurrent(NULL);
            alcDestroyContext(context_);
            alcCloseDevice(device);

            alutExit();

            initialized_ = 0;
        }
    }


    int AudioManager::AddSound(const char* filename) {

        ALuint buffer;
        ALuint source;

        /* Load data from wav file with Alut library */
        buffer = alutCreateBufferFromFile(filename);
        if (!buffer) {
            throw(AudioManagerException(std::string("Failed to load wav file")));
        }
        CheckForErrors("Failed to load wav file");

        /* Keep track of buffers created */
        buffer_.push_back(buffer);

        /* Create a source for this buffer */
        alGenSources((ALuint)1, &source);
        CheckForErrors("Failed to generate source");

        /* Keep track of sources created */
        source_.push_back(source);

        /* Return index of last added buffer */
        return buffer_.size() - 1;
    }


    void AudioManager::PlaySound(int index) {

        /* Associate buffer to source */
        alSourcei(source_[index], AL_BUFFER, buffer_[index]);
        CheckForErrors("Failed to bind buffer");

        /* Play source */
        alSourcePlay(source_[index]);
        CheckForErrors("Failed to play source");
    }


    bool AudioManager::SoundIsPlaying(int index) {

        ALint source_state;

        alGetSourcei(source_[index], AL_SOURCE_STATE, &source_state);
        CheckForErrors("Failed to get source state");

        return (source_state == AL_PLAYING);
    }


    bool AudioManager::AnySoundIsPlaying(void) {

        for (int i = 0; i < source_.size(); i++) {
            if (SoundIsPlaying(i)) {
                return true;
            }
        }
        return false;
    }


    void AudioManager::ListAudioDevices(void) {

        std::cout << "Audio devices:" << std::endl;
        const ALCchar* devices = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
        while (((*devices) != 0) && ((*(devices + 1)) != 0)) {
            std::cout << (*devices);
            devices++;
        }
    }


    void AudioManager::CheckForErrors(const char* msg) {

        ALCenum error;

        error = alGetError();

        if (error != AL_NO_ERROR) {
            throw(AudioManagerException(std::string(msg)));
        }
    }


    void AudioManager::SetListenerPosition(double x, double y, double z) {

        alListener3f(AL_POSITION, x, y, z);
        CheckForErrors("Failed to set listener position");
    }


    void AudioManager::SetSoundPosition(int index, double x, double y, double z) {

        alSource3f(source_[index], AL_POSITION, x, y, z);
        CheckForErrors("Failed to set sound position");
    }


    void AudioManager::SetLoop(int index, bool loop) {

        if (loop) {
            alSourcei(source_[index], AL_LOOPING, AL_TRUE);
        }
        else {
            alSourcei(source_[index], AL_LOOPING, AL_FALSE);
        }
        CheckForErrors("Failed to set sound looping flag");
    }


} // namespace audio_manager;
