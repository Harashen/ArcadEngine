#include "sample.hpp"
#include "utils.hpp"

/* Sample list */
vector<Sample *> Sample::mSampleList;
SDL_mutex        *Sample::mpMutex = NULL;


Sample::Sample(void)
{
    /* Initialize mutex */
    if (!mpMutex) mpMutex = SDL_CreateMutex();

    /* Initialize variables */
    mChannel = -1;
    mpChunk  = NULL;
    mState   = SAMPLE_STOPPED;

    /* Add to list */
    mSampleList.push_back(this);
}

Sample::~Sample(void)
{
    vector<Sample *>::iterator it;

    /* Unload */
    Unload();

    /* Remove from list */
    foreach (mSampleList, it) {
        Sample *sample = *it;

        /* Sample found */
        if (sample == this) {
            mSampleList.erase(it);
            break;
        }
    }
}

void Sample::FinishedCB(Sint32 channel)
{
    vector<Sample *>::iterator it;

    /* Lock mutex */
    SDL_LockMutex(mpMutex);

    /* Find sample */
    foreach (mSampleList, it) {
        Sample *sample = *it;

        /* Sample found */
        if (sample->mChannel == channel) {
            sample->mState   = SAMPLE_STOPPED;
            sample->mChannel = -1;
            break;
        }
    }

    /* Unlock mutex */
    SDL_UnlockMutex(mpMutex);
}

bool Sample::Load(const char *filename)
{
    /* Unload */
    Unload();

    /* Set filepath */
    mFilepath = filename;

    /* Load sample */
    mpChunk = Mix_LoadWAV(filename);

    return !!mpChunk;
}

bool Sample::Load(string filename)
{
    const char *c = filename.c_str();

    /* Load sample */
    return Load(c);
}

void Sample::Unload(void)
{
    /* Not loaded */
    if (!mpChunk)	return;

    /* Lock mutex */
    SDL_LockMutex(mpMutex);

    /* Stop sample */
    Stop();

    /* Free chunk */
    Mix_FreeChunk(mpChunk);

    /* Reset variables */
    mpChunk = NULL;
    mFilepath.clear();

    /* Unlock mutex */
    SDL_UnlockMutex(mpMutex);
}

bool Sample::Play(Sint32 loops)
{
    /* No sample */
    if (!mpChunk) return false;

    /* Sample is already playing */
    if (mState != SAMPLE_STOPPED) return false;

    /* Play sample */
    mChannel = Mix_PlayChannel(-1, mpChunk, loops);
    if (mChannel < 0) return false;

    /* Set playing state */
    mState = SAMPLE_PLAYING;

    return true;
}

void Sample::Stop(void)
{
    /* Sample is not playing */
    if (mState != SAMPLE_PLAYING) return;

    /* Stop sample */
    Mix_HaltChannel(mChannel);

    /* Set stopped state */
    mState = SAMPLE_STOPPED;

    /* Unset channel */
    mChannel = -1;
}

void Sample::Pause(void)
{
    /* Sample is not playing */
    if (mState != SAMPLE_PLAYING) return;

    /* Pause sample */
    Mix_Pause(mChannel);

    /* Set paused state */
    mState = SAMPLE_PAUSED;
}

void Sample::Resume(void)
{
    /* Sample is not paused */
    if (mState != SAMPLE_PAUSED) return;

    /* Resume sample */
    Mix_Resume(mChannel);

    /* Set playing state */
    mState = SAMPLE_PLAYING;
}
