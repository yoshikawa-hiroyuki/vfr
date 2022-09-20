//
// vfrCamera2D
//   2D Camera Object Class
//
#ifndef _VFR_CAMERA_2D_H_
#define _VFR_CAMERA_2D_H_

#include "vfrCamera.h"

class EXPORTED_CLASS vfrCamera2D : public vfrCamera {
public:
  vfrCamera2D(const Bool =FALSE);
  virtual ~vfrCamera2D();

  void dragRot(const float, const float) {}
  void dragTrans(const float, const float);
  void dragTransZ(const float, const float);

  void rotx(const float) {}
  void roty(const float) {}
  void rotz(const float) {}
  void rotation(const float, const vector3) {}
  Bool sweepZoom(const Point2, const Point2);

protected:
  void projection(const GLdouble);
};

#endif /* _VFR_CAMERA_2D_H_ */
