#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

/* High quality mode */
#define HIGH_QUALITY

/* Define constants */
#ifdef HIGH_QUALITY
#define GL_TEXTURE_MIN		GL_LINEAR 
#define GL_TEXTURE_MAX		GL_LINEAR
#else
#define GL_TEXTURE_MIN		GL_NEAREST
#define GL_TEXTURE_MAX		GL_NEAREST
#endif

#endif
