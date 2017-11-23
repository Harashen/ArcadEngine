#include "music.hpp"
#include "utils.hpp"

/* Music list */
vector<Music *> Music::mMusicList;
SDL_mutex      *Music::mpMutex = NULL;


Music::Music(void)
{
    /* Initialize mutex */
    if (!mpMutex) {
        mpMutex = SDL_CreateMutex();
    }

    /* Initialize variables */
    mpMusic = NULL;
    mState = MUSIC_STOPPED;

    /* Add to list */
    mMusicList.push_back(this);
}

Music::~Music(void)
{
    vector<Music *>::iterator it;

    /* Unload */
    Unload();

    /* Remove from list */
    foreach (mMusicList, it) {
        Music *song = *it;

        /* Song found */
        if (song == this) {
            mMusicList.erase(it);
            break;
        }
    }
}

void Music::FinishedCB(void)
{
    vector<Music *>::iterator it;

    /* Lock mutex */
    SDL_LockMutex(mpMutex);

    /* Find song */
    foreach (mMusicList, it) {
        Music *song = *it;

        /* Song found */
        if (song->mState == MUSIC_PLAYING) {
            song->mState = MUSIC_STOPPED;
            break;
        }
    }

    /* Unlock mutex */
    SDL_UnlockMutex(mpMutex);
}

bool Music::Load(const char *filename)
{
    /* Unload */
    Unload();

    /* Set filepath */
    mFilepath = filename;

    /* Load music */
    mpMusic = Mix_LoadMUS(filename);

    return !!mpMusic;
}

bool Music::Load(string filename)
{
    const char *c = filename.c_str();

    /* Load music */
    return Load(c);
}

void Music::Unload(void)
{
    /* Not loaded */
    if (!mpMusic) return;

    /* Lock mutex */
    SDL_LockMutex(mpMutex);

    /* Stop music */
    Stop();

    /* Free music */
    Mix_FreeMusic(mpMusic);

    /* Reset variables */
    mpMusic = NULL;
    mFilepath.clear();

    /* Unlock mutex */
    SDL_UnlockMutex(mpMutex);
}

bool Music::Play(Sint32 loops)
{
    Sint32 ret;

    /* No music */
    if (!mpMusic) return false;

    /* Check for music */
    ret = Mix_PlayingMusic();
    if (ret) return false;

    /* Play music */
    ret = Mix_PlayMusic(mpMusic, loops);
    if (ret) return false;

    /* Set playing state */
    mState = MUSIC_PLAYING;

    return true;
}

void Music::Stop(void)
{
    /* Music not playing */
    if (mState != MUSIC_PLAYING) return;

    /* Stop music */
    Mix_HaltMusic();

    /* Set stopped state */
    mState = MUSIC_STOPPED;
}

void Music::Pause(void)
{
    /* Music not playing */
    if (mState != MUSIC_PLAYING) return;

    /* Pause music */
    Mix_PauseMusic();

    /* Set paused state */
    mState = MUSIC_PAUSED;
}

void Music::Resume(void)
{
    /* Music not paused */
    if (mState != MUSIC_PAUSED)	return;

    /* Resume music */
    Mix_ResumeMusic();

    /* Set playing state */
    mState = MUSIC_PLAYING;
}
