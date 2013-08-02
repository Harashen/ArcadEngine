#include "sample.h"
#include "utils.h"

/* Sample list */
vector<CSample *> CSample::SampleList;
SDL_mutex        *CSample::Mutex = NULL;


CSample::CSample(void)
{
	/* Initialize mutex */
	if (!Mutex)
		Mutex = SDL_CreateMutex();

	/* Initialize variables */
	Channel = -1;
	Chunk   = NULL;
	State   = SAMPLE_STOPPED;

	/* Add to list */
	SampleList.push_back(this);
}

CSample::~CSample(void)
{
	vector<CSample *>::iterator it;

	/* Unload */
	Unload();

	/* Remove from list */
	foreach (SampleList, it) {
		CSample *Sample = *it;

		/* Sample found */
		if (Sample == this) {
			SampleList.erase(it);
			break;
		}
	}
}

void CSample::FinishedCB(Sint32 channel)
{
	vector<CSample *>::iterator it;

	/* Lock mutex */
	SDL_LockMutex(Mutex);

	/* Find sample */
	foreach (SampleList, it) {
		CSample *Sample = *it;

		/* Sample found */
		if (Sample->Channel == channel) {
			Sample->State   = SAMPLE_STOPPED;
			Sample->Channel = -1;
			break;
		}
	}

	/* Unlock mutex */
	SDL_UnlockMutex(Mutex);
}

bool CSample::Load(const char *filename)
{
	/* Unload */
	Unload();

	/* Set filepath */
	Filepath = filename;

	/* Load sample */
	Chunk = Mix_LoadWAV(filename);

	return !!Chunk;
}

bool CSample::Load(string filename)
{
	const char *c = filename.c_str();

	/* Load sample */
	return Load(c);
}

void CSample::Unload(void)
{
	/* Not loaded */
	if (!Chunk)
		return;

	/* Lock mutex */
	SDL_LockMutex(Mutex);

	/* Stop sample */
	Stop();

	/* Free chunk */
	Mix_FreeChunk(Chunk);

	/* Reset variables */
	Chunk = NULL;
	Filepath.clear();

	/* Unlock mutex */
	SDL_UnlockMutex(Mutex);
}

bool CSample::Play(Sint32 loops)
{
	/* No sample */
	if (!Chunk)
		return false;

	/* Sample is already playing */
	if (State != SAMPLE_STOPPED)
		return false;

	/* Play sample */
	Channel = Mix_PlayChannel(-1, Chunk, loops);
	if (Channel < 0)
		return false;

	/* Set playing state */
	State = SAMPLE_PLAYING;

	return true;
}

void CSample::Stop(void)
{
	/* Sample is not playing */
	if (State != SAMPLE_PLAYING)
		return;

	/* Stop sample */
	Mix_HaltChannel(Channel);

	/* Set stopped state */
	State = SAMPLE_STOPPED;

	/* Unset channel */
	Channel = -1;
}

void CSample::Pause(void)
{
	/* Sample is not playing */
	if (State != SAMPLE_PLAYING)
		return;

	/* Pause sample */
	Mix_Pause(Channel);

	/* Set paused state */
	State = SAMPLE_PAUSED;
}

void CSample::Resume(void)
{
	/* Sample is not paused */
	if (State != SAMPLE_PAUSED)
		return;

	/* Resume sample */
	Mix_Resume(Channel);

	/* Set playing state */
	State = SAMPLE_PLAYING;
}
