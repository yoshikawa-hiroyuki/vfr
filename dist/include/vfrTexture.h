//
// vfrTexture
//   Texture Data class
//
#ifndef _VFR_TEXTURE_H_
#define _VFR_TEXTURE_H_

#ifdef WINDOWS
#include "stdafx.h"
#endif

#include "vfrBase.h"
#include "vfrImage.h"

#include "utilMath.h"
#include "utilOglObj.h"

class vfrNode;


class EXPORTED_CLASS vfrTexture : public vfrBase {
public:
  enum MapType  {NOMAP=0, UVC, XYP, YZP, ZXP};
  enum ClipType {REPEAT, CLAMP};

  vfrTexture(const CES::Vec3<float>* =NULL, const Bool =FALSE);
  vfrTexture(const vfrImage&,
	     const CES::Vec3<float>* =NULL, const Bool =FALSE);
  vfrTexture(const vfrTexture&,
	     const CES::Vec3<float>* =NULL, const Bool =FALSE);
  virtual ~vfrTexture();
  void operator=(const vfrTexture&);

  virtual void apply() const;
  virtual void unApply() const;

  vfrImage& getImage() {return _image;}
  void setImage(const vfrImage& I) {_image=I; notice();}

  MapType getMapType() const {return _mapType;}
  void setMapType(const MapType mt) {
    if (_mapType!=mt) {_mapType=mt; notice();}
  }

  ClipType getClipType() const {return _clipType;}
  void setClipType(const ClipType ct) {
    if (_clipType!=ct) {_clipType=ct; notice();}
  }

  Bool alcUVCs(const int);
  Bool setUVCs(const int, const vector2*);
  Bool setUVC(const int, const vector2);
  vector2* getUVCs() {return _uvc;}
  Bool getUVC(const int n, vector2& v) const {
    if ( 0 <= n && n < _num_uvc ) {
      v[0] = _uvc[n][0];
      v[1] = _uvc[n][1];
      return TRUE;
    }
    v[0] = v[1] = 0.0f;
    return FALSE;
  }
  int getNumUVCs() const {return _num_uvc;}

  void setBbox(const CES::Vec3<float>*);
  const CES::Vec3<float>* getBbox() const;

  void addRef(vfrNode* node) {this->vfrBase::addRef((vfrBase*)node);}
  void clearDispList();
  virtual void notice();


protected:
  class vfrImage   _image;
  MapType          _mapType;
  ClipType         _clipType;
  vector2*         _uvc;
  int              _num_uvc;
  CES::Vec3<float> _bbox[2];
  Bool             _hasBbox;
  mutable CES::TexObj2D
                   _texObj;
  mutable Bool     _texUpdated;
};

#endif /* _VFR_TEXTURE_H_ */
