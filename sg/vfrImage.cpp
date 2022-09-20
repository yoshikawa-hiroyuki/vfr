//
// vfrImage
//  Texture Image Class
//
#include "vfrImage.h"


vfrImage::vfrImage(const int x, const int y, const int z, const Bool adjustFlg)
 : _nx(0),_ny(0),_nz(0),_format(0),_d(NULL)
{
  if ( allocateDatas(x, y, z, adjustFlg) ) {
    memset(_d, 0, sizeof(char)*_nx*_ny*_nz);
  }
}

Bool
vfrImage::allocateDatas(const int x, const int y, const int z,
  const Bool adjustFlg)
{
  // Cleanup
  if ( _d ) {
    DeAllocate(_d);
    _d = NULL;
  }
  _nx = _ny = _nz = _format = 0;

  if ( x < 1 || y < 1 || z < 1 )
    return FALSE;

  // Adjust size
  if ( adjustFlg ) {
    Point2 sz; vfrImage::GetCropSize(x, y, sz);
    _nx = sz.x;
    _ny = sz.y;
  } else {
    _nx = x;
    _ny = y;
  }
  _nz = z;
  switch ( _nz ) {
  case 2: _format = GL_LUMINANCE_ALPHA; break;
  case 3: _format = GL_RGB; break;
  case 4: _format = GL_RGBA; break;
  case 1: default:
    _format = GL_LUMINANCE; break;
  }

  // Allocate datas
  _d = (unsigned char*)Allocate(_nx*_ny*_nz);
  if ( ! _d ) {
    _nx = _ny = _nz = _format = 0;
    return FALSE;
  }

  return TRUE;
}

vfrImage::vfrImage(const vfrImage& org)
  : _nx(0),_ny(0),_nz(0),_format(0),_d(NULL) {
  *this = org;
}

vfrImage::~vfrImage() {
  if ( _d ) {
    DeAllocate(_d);
  }
}

void vfrImage::operator=(const vfrImage &org) {
  _nx = org._nx;
  _ny = org._ny;
  _nz = org._nz;
  _format = org._format;
  if ( org._d ) {
    if ( _d ) DeAllocate(_d);
    _d = (unsigned char*)Allocate(_nx*_ny*_nz);
    memcpy(_d, org._d, _nx*_ny*_nz);
  }
}

int vfrImage::setFormat(const int fmt) {
  int xfmt = _format;
  _format = fmt;
  return xfmt;
}

// STATIC
void vfrImage::GetWrapSize(const int x, const int y, Point2& sz) {
  register int s;
  if ( x < y ) {
    for ( s = 1; s < x && s <= VFR_IMG_TILE_MAX; s *= 2 );
    sz.x = s;
    for ( s = 1; sz.x*s < y && s <= VFR_IMG_ASPR_MAX; s *= 2 );
    sz.y = sz.x * s;
  } else { 
    for ( s = 1; s < y && s <= VFR_IMG_TILE_MAX; s *= 2 );
    sz.y = s;
    for ( s = 1; sz.y*s < x && s <= VFR_IMG_ASPR_MAX; s *= 2 );
    sz.x = sz.y * s;
  }
}

// STATIC
void vfrImage::GetCropSize(const int x, const int y, Point2& sz) {
  register int s;
  if ( x < y ) {
    for ( s = 1; s <= x && s <= VFR_IMG_TILE_MAX; s *= 2 );
    sz.x = s / 2;
    for ( s = 1; sz.x*s <= y && s <= VFR_IMG_ASPR_MAX; s *= 2 );
    sz.y = sz.x * s / 2;
  } else {
    for ( s = 1; s <= y && s <= VFR_IMG_TILE_MAX; s *= 2 );
    sz.y = s / 2;
    for ( s = 1; sz.y*s <= x && s <= VFR_IMG_ASPR_MAX; s *= 2 );
    sz.x = sz.y * s / 2;
  }
}

// EOF
