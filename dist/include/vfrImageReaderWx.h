//-------------------------------------------------------
// vfrImageReaderWx
//   read image file by using wxWidgets image feature
//-------------------------------------------------------

#ifndef _VFR_IMAGE_READER_WX_H_
#define _VFR_IMAGE_READER_WX_H_

#include "vfrImage.h"


class EXPORTED_CLASS vfrImageReaderWx {
public:
  static vfrImage* ReadImage(const char*, const Bool =TRUE);

protected:
  static bool s_handlerInstalled;
};

#endif // _VFR_IMAGE_READER_WX_H_
