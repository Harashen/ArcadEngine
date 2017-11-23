#ifndef _AUDIO_HPP_
#define _AUDIO_HPP_

#include <SDL/SDL.h>

//! Sample buffer size
#define AUDIO_BUFSIZE 4096

/*!
    Audio is the class that manages the audio output
    of the engine (frequency, channels, buffers...).
    \author Javier Bermejo García
*/
class Audio {
    static Uint32 mInstances;	//!< Audio instances counter.

    Sint32 mChannels;		//!< Audio output channels.
    Sint32 mFrequency;		//!< Audio output frequency.
    Uint16 mFormat;			//!< Audio output format.

public:
    /*!
        Audio constructor.
    */
    Audio(void);

    /*!
        Audio destructor.
    */
    virtual ~Audio(void);

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
extern Audio *gpAudio;

#endif
