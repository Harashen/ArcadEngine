#ifndef _SAMPLE_H_
#define _SAMPLE_H_

#include <string>
#include <vector>
#include <SDL/SDL_mixer.h>
#include "audio.h"

using namespace std;


//! Sample state values.
enum {
        SAMPLE_STOPPED = 0,
        SAMPLE_PAUSED,
        SAMPLE_PLAYING,
};


/*!
	CSample is the class that manages the playback
	of sample audio files.
	\author Javier Bermejo García
*/
class CSample {
	friend class CAudio;		//!< CAudio friend class.

	static vector<CSample *> SampleList; //!< Array of CSample instances.
	static SDL_mutex        *Mutex;		 //!< Mutex object.

	Mix_Chunk *Chunk;			//!< Sample buffer.
	string     Filepath;		//!< Sample filepath.

public:
	Uint8  State;				//!< Current state.
	Sint32 Channel;				//!< Audio output channel.

private:
	/*!
		Callback function that is called when a sample playback finishes.
		\param channel the sample channel
	*/
	static void FinishedCB(Sint32 channel);

public:
	/*!
		CSample constructor.
	*/
	CSample(void);

	/*!
		CSample destructor.
	*/
	~CSample(void);

	/*!
		Load a sample from a file.
		\param filename the sample filename.
		\return true if success, false otherwise.
	*/
	bool Load(const char *filename);

	/*!
		Load a sample from a file.
		\param filename the sample filename.
		\return true if success, false otherwise.
	*/
	bool Load(string filename);

	/*!
		Unload the sample.
	*/
	void Unload(void);

	/*!
		Start the sample playback.
		\param Loops the number of times to repeat the playback (-1 = infinite).
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
