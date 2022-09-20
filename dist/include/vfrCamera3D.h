//
// vfrCamera3D
//   3D Camera Object Class
//
#ifndef _VFR_CAMERA_3D_H_
#define _VFR_CAMERA_3D_H_

#include "vfrCamera.h"

class EXPORTED_CLASS vfrCamera3D : public vfrCamera {
public:
  vfrCamera3D(const Bool =FALSE);
  virtual ~vfrCamera3D();

  void setProjection(const ProjectType t) {_projMode = t; notice();}
  ProjectType getProjection() const {return _projMode;}

  Bool sweepZoom(const Point2, const Point2);
  void setHpr(const float h, const float p, const float r);
  void setHprDeg(const float h, const float p, const float r) {
    setHpr(CES::Deg2Rad(h), CES::Deg2Rad(p), CES::Deg2Rad(r));
  }

protected:
  ProjectType _projMode;

  void projection(const GLdouble asp);
};

#endif /* _VFR_CAMERA_3D_H_ */
