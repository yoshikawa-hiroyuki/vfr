//
// vfrCube
//   Cube Object Class
//
#ifndef _VFR_CUBE_H_
#define _VFR_CUBE_H_

#include "vfrNode.h"


class EXPORTED_CLASS vfrCube : public vfrNode {
public:
  vfrCube(const std::string& =std::string(VFR_NONAME), const Bool =FALSE);
  vfrCube(const float w, const float h, const float d,
	  const std::string& =std::string(VFR_NONAME), const Bool =FALSE);
  virtual ~vfrCube();
  
  void setWidth(const float);
  void setHeight(const float);
  void setDepth(const float);
  float getWidth()  const {return _width;}
  float getHeight() const {return _height;}
  float getDepth()  const {return _depth;}

protected:
  float _width;
  float _height;
  float _depth;

  void renderSolid();
  void renderWire();
  void generateBbox();
  void generateBbox(const vector3) {generateBbox();}
  void makeCube(const float w, const float h, const float d);
};

#endif /* _VFR_CUBE_H_ */
