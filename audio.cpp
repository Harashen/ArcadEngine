#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "audio.hpp"
#include "music.hpp"
#include "sample.hpp"

/* Audio handler */
Audio *gpAudio = NULL;

/* Audio instances */
Uint32 Audio::mInstances = 0;


Audio::Audio(void)
{
    /* New instance */
    mInstances++;
}

Audio::~Audio(void)
{
    /* Free */
    Free();

    /* Quit SDL audio */
    if (!--mInstances) {
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }
}

bool Audio::Init(void)
{
    /* Audio already initialized */
    if (SDL_WasInit(SDL_INIT_AUDIO)) {
        return true;    
    }

    /* Initialize SDL audio */
    return !SDL_InitSubSystem(SDL_INIT_AUDIO);
}

void Audio::Free(void)
{
    Sint32 ret;

    /* Free mixer */
    ret = Mix_QuerySpec(&mFrequency, &mFormat, &mChannels);

    if (ret) Mix_CloseAudio();
}

bool Audio::Configure(Sint32 channels, Sint32 frequency)
{
    Sint32 ret;

    /* Free mixer */
    Free();
    
    /* Set attributes */
    mChannels  = (channels)  ? channels  : MIX_DEFAULT_CHANNELS;
    mFrequency = (frequency) ? frequency : MIX_DEFAULT_FREQUENCY;
    mFormat    = MIX_DEFAULT_FORMAT;

    /* Initialize SDL mixer */
    ret = Mix_OpenAudio(mFrequency, mFormat, mChannels, AUDIO_BUFSIZE);

    if (ret < 0) return false;

    /* Set callbacks */
    Mix_HookMusicFinished(Music::FinishedCB);
    Mix_ChannelFinished(Sample::FinishedCB);

    return true;
}

void Audio::SetActive(void)
{
    /* Set active audio */
    gpAudio = this;
}
