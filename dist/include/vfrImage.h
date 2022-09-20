//
// vfrImage
//  Image data class
//

#ifndef _VFR_IMAGE_H_
#define _VFR_IMAGE_H_

#include <sys/types.h>
#include "vfrBase.h"

#define VFR_IMG_TILE_MAX 8192
#define VFR_IMG_ASPR_MAX 1024


class EXPORTED_CLASS vfrImage {
public:
  vfrImage() : _nx(0),_ny(0),_nz(0),_format(0),_d(NULL) {}
  vfrImage(const int, const int, const int, const Bool =TRUE);
  vfrImage(const vfrImage&);
  virtual ~vfrImage();
  void operator=(const vfrImage&);

  int getWidth() const {return _nx;}
  int getHeight() const {return _ny;}
  int getDepth() const {return _nz;}
  int getFormat() const {return _format;}
  int setFormat(const int);
  unsigned char* getData() {return _d;}
  const unsigned char* getConstData() const {return _d;}

  Bool allocateDatas(const int, const int, const int, const Bool);

  static void GetWrapSize(const int, const int, Point2&);
  static void GetCropSize(const int, const int, Point2&);


protected:
  int _nx, _ny, _nz;
  int _format;
  unsigned char* _d;

  friend class vfrTexture;
};

#endif /* _VFR_IMAGE_H_ */
