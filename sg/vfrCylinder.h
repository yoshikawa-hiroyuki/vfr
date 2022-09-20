//
// vfrCylinder
//   Circular Cylinder Object Class
//
#ifndef _VFR_CYLINDER_H_
#define _VFR_CYLINDER_H_

#include "vfrNode.h"

class EXPORTED_CLASS vfrCylinder : public vfrNode {
public:
  vfrCylinder(const std::string& =std::string(VFR_NONAME), const Bool =FALSE);
  vfrCylinder(const float r, const float h,
	      const std::string& =std::string(VFR_NONAME), const Bool =FALSE);
  virtual ~vfrCylinder();

  void setRadius(const float);
  void setHeight(const float);
  void setSubdiv(const int);
  void setShowBottom(const Bool);
  void setShowTop(const Bool);
  Bool showBottom() const {return _showBottom;}
  Bool showTop()    const {return _showTop;}
  float getRadius() const {return _radius;}
  float getHeight() const {return _height;}

protected:
  float _radius;
  float _height;
  int   _subdiv;
  Bool  _showBottom;
  Bool  _showTop;
  
  void makeCylinder(const float r, const float h);
  void renderSolid();
  void renderWire();
  void drawTri(const vector3, const vector3, const vector3);
  void drawQuad(const vector3, const vector3, const vector3, const vector3,
		const int phase);
  void subdivTri(const vector3, const vector3, const vector3, const int);
  void subdivQuad(const vector3, const vector3, const vector3, const vector3,
		  const int level, const int phase);
  void generateBbox();
  void generateBbox(const vector3) {generateBbox();}
  
  static const vector3 _surface[10];
  static const vector3 _snormal[2];
  static const int     _tindex[8][3];
  static const int     _qindex[4][4];

  static void getUVC(const int phase, const vector3, vector2);
};

#endif /* _VFR_CYLINDER_H_ */
