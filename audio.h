#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <SDL/SDL.h>

//! Sample buffer size
#define AUDIO_BUFSIZE 4096

/*!
	CAudio is the class that manages the audio output
	of the engine (frequency, channels, buffers...).
	\author Javier Bermejo García
*/
class CAudio {
	static Uint32 Instances;	//!< CAudio instances counter.

	Sint32 Channels;		//!< Audio output channels.
	Sint32 Frequency;		//!< Audio output frequency.
	Uint16 Format;			//!< Audio output format.

public:
	/*!
		CAudio constructor.
	*/
	CAudio(void);

	/*!
		CAudio destructor.
	*/
	~CAudio(void);

	/*!
		Initialize the audio subsystem.
		\return true if success, false otherwise.
	*/
	bool Init(void);

	/*!
		Free the audio subsystem.
	*/
	virtual void Free(void);

	/*!
		Configure the audio output.
		\param frequency frequency of the audio output.
		\param channels number of channels of the audio output.
		\return true if success, false otherwise.
	*/
	bool Configure(Sint32 channels = 0, Sint32 frequency = 0);

	/*!
		Set the active audio object.
	*/
	void SetActive(void);
};

//! Active audio object.
extern CAudio *pAudio;

#endif
