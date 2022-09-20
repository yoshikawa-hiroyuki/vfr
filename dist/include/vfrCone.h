//
// vfrCone
//   Circular Cone Object Class
//
#ifndef _VFR_CONE_H_
#define _VFR_CONE_H_

#include "vfrNode.h"


class EXPORTED_CLASS vfrCone : public vfrNode {
public:
  vfrCone(const std::string& =std::string(VFR_NONAME), const Bool =FALSE);
  vfrCone(const float r, const float h,
	  const std::string& =std::string(VFR_NONAME), const Bool =FALSE);
  virtual ~vfrCone();

  void setRadius(const float);
  void setHeight(const float);
  void setSubdiv(const int);
  void setShowBottom(const Bool);
  Bool showBottom() const {return _showBottom;}
  float getRadius() const {return _radius;}
  float getHeight() const {return _height;}

protected:
  float _radius;
  float _height;
  int   _subdiv;
  Bool  _showBottom;

  void makeCone(const float r, const float h);
  void renderSolid();
  void renderWire();
  void drawFace(const vector3, const vector3, const vector3,
		const int phase);
  void subdivFace(const vector3, const vector3, const vector3,
		  const int level, const int phase);
  void generateBbox();
  void generateBbox(const vector3) {generateBbox();}

  static const vector3 _surface[6];
  static const vector3 _snormal[2];
  static const int     _sindex[8][3];

  static void getUVC(const int phase, const vector3, vector2);
};

#endif /* _VFR_CONE_H_ */
