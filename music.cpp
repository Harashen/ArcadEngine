#include "music.h"
#include "utils.h"

/* Music list */
vector<CMusic *> CMusic::MusicList;
SDL_mutex       *CMusic::Mutex = NULL;


CMusic::CMusic(void)
{
	/* Initialize mutex */
	if (!Mutex)
		Mutex = SDL_CreateMutex();

	/* Initialize variables */
	Music = NULL;
	State = MUSIC_STOPPED;

	/* Add to list */
	MusicList.push_back(this);
}

CMusic::~CMusic(void)
{
	vector<CMusic *>::iterator it;

	/* Unload */
	Unload();

	/* Remove from list */
	foreach (MusicList, it) {
		CMusic *Song = *it;

		/* Song found */
		if (Song == this) {
			MusicList.erase(it);
			break;
		}
	}
}

void CMusic::FinishedCB(void)
{
	vector<CMusic *>::iterator it;

	/* Lock mutex */
	SDL_LockMutex(Mutex);

	/* Find song */
	foreach (MusicList, it) {
		CMusic *Song = *it;

		/* Song found */
		if (Song->State == MUSIC_PLAYING) {
			Song->State = MUSIC_STOPPED;
			break;
		}
	}

	/* Unlock mutex */
	SDL_UnlockMutex(Mutex);
}

bool CMusic::Load(const char *filename)
{
	/* Unload */
	Unload();

	/* Set filepath */
	Filepath = filename;

	/* Load music */
	Music = Mix_LoadMUS(filename);

	return !!Music;
}

bool CMusic::Load(string filename)
{
	const char *c = filename.c_str();

	/* Load music */
	return Load(c);
}

void CMusic::Unload(void)
{
	/* Not loaded */
	if (!Music)
		return;

	/* Lock mutex */
	SDL_LockMutex(Mutex);

	/* Stop music */
	Stop();

	/* Free music */
	Mix_FreeMusic(Music);

	/* Reset variables */
	Music = NULL;
	Filepath.clear();

	/* Unlock mutex */
	SDL_UnlockMutex(Mutex);
}

bool CMusic::Play(Sint32 loops)
{
	Sint32 ret;

	/* No music */
	if (!Music)
		return false;

	/* Check for music */
	ret = Mix_PlayingMusic();
	if (ret)
		return false;

	/* Play music */
	ret = Mix_PlayMusic(Music, loops);
	if (ret)
		return false;

	/* Set playing state */
	State = MUSIC_PLAYING;

	return true;
}

void CMusic::Stop(void)
{
	/* Music not playing */
	if (State != MUSIC_PLAYING)
		return;

	/* Stop music */
	Mix_HaltMusic();

	/* Set stopped state */
	State = MUSIC_STOPPED;
}

void CMusic::Pause(void)
{
	/* Music not playing */
	if (State != MUSIC_PLAYING)
		return;

	/* Pause music */
	Mix_PauseMusic();

	/* Set paused state */
	State = MUSIC_PAUSED;
}

void CMusic::Resume(void)
{
	/* Music not paused */
	if (State != MUSIC_PAUSED)
		return;

	/* Resume music */
	Mix_ResumeMusic();

	/* Set playing state */
	State = MUSIC_PLAYING;
}
