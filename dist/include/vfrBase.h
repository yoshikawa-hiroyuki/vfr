//
// vfrBase
//     Base object class
//
#ifndef _VFR_BASE_H_
#define _VFR_BASE_H_

#ifdef WINDOWS
#include "stdafx.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#include "utilMath.h"
#include "vfrDefs.h"

// Object Apotheosis //
#define _VFR_APOTHEOSIS

// Clamp value
#define VFR_CLAMP(x,y,z)  (((x)>(z))?(x):(((y)<(z))?(y):(z)))

// Matrix type
typedef CES::Mat4<float> vfrMatrix;


using namespace VFR;

class EXPORTED_CLASS vfrBase {

public:
  virtual ~vfrBase();
  void operator=(const vfrBase& org) {_matrix = org._matrix;}

  // Geometric Transformation
  virtual void identity() {_matrix.Identity(); notice();}
  virtual void rotx(const float r) {
    _matrix.RotX(r); vfrBase::notice();
  }
  virtual void roty(const float r) {
    _matrix.RotY(r); vfrBase::notice();
  }
  virtual void rotz(const float r) {
    _matrix.RotZ(r); vfrBase::notice();
  }
  virtual void rotation(const float a, const vector3 v) {
    _matrix.Rotation(a, v); vfrBase::notice();
  }
  virtual void trans(const vector3 v) {
    _matrix.Translate(v); vfrBase::notice();
  }
  virtual void trans(const float x, const float y, const float z) {
    _matrix.Translate(CES::Vec3<float>(x, y, z)); vfrBase::notice();
  }
  virtual void scale(const vector3 v) {
    _matrix.Scale(v); vfrBase::notice();
  }
  virtual void scale(const float s) {
    _matrix.Scale(CES::Vec3<float>(s, s, s)); vfrBase::notice();
  }
  virtual void scale(const float x, const float y, const float z) {
    _matrix.Scale(CES::Vec3<float>(x, y, z)); vfrBase::notice();
  }
  virtual void mult(const vfrMatrix& M) {
    _matrix = _matrix * M; vfrBase::notice();
  }
  virtual void mult(const matrix4 m) {
    _matrix = _matrix * vfrMatrix(m); vfrBase::notice();
  }
  virtual void mult(const matrix44 m) {
    const float* mm = (const float*)m;
    _matrix = _matrix * vfrMatrix(mm); vfrBase::notice();
  }
  const vfrMatrix& getMatrix() const {return _matrix;}
  void setMatrix(const vfrMatrix& M) {_matrix = M; vfrBase::notice();}

  void applyMatrix() const {glPushMatrix(); glMultMatrixf(_matrix.m_v);}
  void unApplyMatrix() const {glPopMatrix();}

  // Reference pointers control
  void addRef(vfrBase*);
  Bool remRef(vfrBase*);

  // Apotheosis mode control
  void setSuicide(const Bool sm) {_doSuicide = sm;}
  Bool getSuicide() const {return _doSuicide;}

  // Base interface
  virtual void rumor(vfrBase*) {}
  virtual void notice();
  virtual void chkNotice();
  virtual void clearDispList() {}

protected:
  vfrBase(const Bool =FALSE);
  vfrBase(const vfrBase&, const Bool =FALSE);

  vfrMatrix _matrix;
  int       _nRef;
  vfrBase** _Ref;
  Bool      _doSuicide;

private:
  void resetRef();
};

#endif /* _VFR_BASE_H_ */
