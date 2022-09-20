//-------------------------------------------------------
// vfrImageReaderSDL
//   read image file by using SDL_image library
//-------------------------------------------------------

#ifndef _VFR_IMAGE_READER_SDL_H_
#define _VFR_IMAGE_READER_SDL_H_

#include "vfrImage.h"


class EXPORTED_CLASS vfrImageReaderSDL {
public:
  static vfrImage* ReadImage(const char*, const Bool =TRUE);
};

#endif // _VFR_IMAGE_READER_SDL_H_
