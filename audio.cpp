#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "audio.h"
#include "music.h"
#include "sample.h"

/* Audio handler */
CAudio *pAudio = NULL;

/* Audio instances */
Uint32 CAudio::Instances = 0;


CAudio::CAudio(void)
{
	/* New instance */
	Instances++;
}

CAudio::~CAudio(void)
{
	/* Free */
	Free();

	/* Quit SDL audio */
	if (!--Instances)
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

bool CAudio::Init(void)
{
	/* Audio already initialized */
	if (SDL_WasInit(SDL_INIT_AUDIO))
		return true;

	/* Initialize SDL audio */
	return !SDL_InitSubSystem(SDL_INIT_AUDIO);
}

void CAudio::Free(void)
{
	Sint32 ret;

	/* Free mixer */
	ret = Mix_QuerySpec(&Frequency, &Format, &Channels);
	if (ret)
		Mix_CloseAudio();
}

bool CAudio::Configure(Sint32 channels, Sint32 frequency)
{
	Sint32 ret;

	/* Free mixer */
	Free();
	
	/* Set attributes */
	this->Channels  = (channels)  ? channels  : MIX_DEFAULT_CHANNELS;
	this->Frequency = (frequency) ? frequency : MIX_DEFAULT_FREQUENCY;
	this->Format    = MIX_DEFAULT_FORMAT;

	/* Initialize SDL mixer */
	ret = Mix_OpenAudio(Frequency, Format, Channels, AUDIO_BUFSIZE);
	if (ret < 0)
		return false;

	/* Set callbacks */
	Mix_HookMusicFinished(CMusic::FinishedCB);
	Mix_ChannelFinished(CSample::FinishedCB);

	return true;
}

void CAudio::SetActive(void)
{
	/* Set active audio */
	pAudio = this;
}
