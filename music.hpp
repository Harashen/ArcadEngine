#ifndef _MUSIC_HPP_
#define _MUSIC_HPP_

#include <string>
#include <vector>

#include <SDL/SDL_mixer.h>

#include "audio.hpp"

using namespace std;


//! Music state values.
enum {
    MUSIC_STOPPED = 0,
    MUSIC_PAUSED,
    MUSIC_PLAYING
};


/*!
    Music is the class that manages the playback
    of music audio files.
    \author Javier Bermejo García
*/
class Music {
    friend class Audio;			//!< Audio friend class.

    static vector<Music *> mMusicList;	//!< Array of Music instances.
    static SDL_mutex      *mpMutex;		//!< Mutex object.

    Mix_Music *mpMusic;			//!< Music buffer.

    string mFilepath;			//!< Music filepath.

public:
    Uint8 mState;				//!< Current state.

private:
    /*!
        Callback function that is called when the playback finishes.
    */
    static void FinishedCB(void);

public:
    /*!
        Music constructor.
    */
    Music(void);

    /*!
        Music destructor.
    */
    ~Music(void);

    /*!
        Load music from a file.
        \param filename the music filename.
        \return true if success, false otherwise.
    */
    bool Load(const char *filename);

    /*!
        Load music from a file.
        \param filename the music filename.
        \return true if success, false otherwise.
    */
    bool Load(string filename);

    /*!
        Unload the music.
    */
    void Unload(void);

    /*!
        Start the music playback.
        \param loops the number of times to repeat the playback (-1 = infinite).
        \return true if success, false otherwise.
    */
    bool Play(Sint32 loops = -1);

    /*!
        Stop the music playback.
    */
    void Stop(void);

    /*!
        Pause the music playback.
    */
    void Pause(void);

    /*!
        Resume the music playback.
    */
    void Resume(void);
};

#endif
