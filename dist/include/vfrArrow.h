//
// vfrArrow
//   Arrow Object Class
//
#ifndef _VFR_ARROW_H_
#define _VFR_ARROW_H_

#include "vfrNode.h"


class EXPORTED_CLASS vfrArrow : public vfrNode {
public:
  vfrArrow(const std::string& =std::string(VFR_NONAME), const Bool =FALSE);
  vfrArrow(const CES::Vec3<float>&, const float,
	   const std::string& =std::string(VFR_NONAME), const Bool =FALSE);
  vfrArrow(const CES::Vec3<float>&,
	   const std::string& =std::string(VFR_NONAME), const Bool =FALSE);
  vfrArrow(const float,
	   const std::string& =std::string(VFR_NONAME), const Bool =FALSE);
  virtual ~vfrArrow();
  
  void setDirection(const CES::Vec3<float>&);
  CES::Vec3<float> getDirection() const {return _direction;}
  void setLength(const float l) {_magnitude = l; generateBbox(); notice();}
  float getLength() const {return _magnitude;}
  void setHeadMode(const Bool h) {_head = h; notice();}
  Bool getHeadMode() const {return _head;}
  void setHeadScale(const float);
  float getHeadScale() const {return _headScale;}
  void setHeadWidth(const float);
  float getHeadWidth() const {return _headWidth;}
  void setVector(const CES::Vec3<float>&);
  RenderType getRenderMode() const;

protected:
  float _magnitude;
  CES::Vec3<float>
        _direction;
  Bool  _head;
  float _headScale;
  float _headWidth;
  
  virtual void renderWire();
  virtual void generateBbox();
  virtual void generateBbox(const vector3 nv) {vfrNode::generateBbox(nv);}

  void makeArrow(const CES::Vec3<float>&, const float);
};

#endif    /* _VFR_ARROW_H_ */
