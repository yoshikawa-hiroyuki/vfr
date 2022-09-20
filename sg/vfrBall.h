//
// vfrBall
//   Sphere Ball Object Class
//
#ifndef _VFR_BALL_H_
#define _VFR_BALL_H_

#include "vfrNode.h"


class EXPORTED_CLASS vfrBall : public vfrNode {
public:
  vfrBall(const std::string& =std::string(VFR_NONAME), const Bool =FALSE);
  vfrBall(const float,
	  const std::string& =std::string(VFR_NONAME), const Bool =FALSE);
  virtual ~vfrBall();

  void setRadius(const float);
  void setSubdiv(const int);
  float getRadius() const {return _radius;}

protected:
  float _radius;
  int _subdiv;

  void makeBall(const float);
  void renderSolid();
  void renderWire();
  void drawFace(const vector3, const vector3, const vector3,
		const int phase);
  void subdivFace(const vector3, const vector3, const vector3,
		  const int level, const int phase);
  void generateBbox();
  void generateBbox(const vector3) {generateBbox();}

  static const vector3 _surface[6];
  static const int     _sindex[8][3];

  static void getUVC(const int phase, const vector3, vector2);
};

#endif /* _VFR_BALL_H_ */
