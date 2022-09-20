//
// vfrLight
//   Light Object Class
//
#ifndef _VFR_LIGHT_H_
#define _VFR_LIGHT_H_

#include "vfrNode.h"

#define  VFR_LIGHT_MAX  4


class EXPORTED_CLASS vfrLight : public vfrNode {
public:
  virtual ~vfrLight();

  void render(vfrMaterialStack* =NULL);
  void render(const Bool, vfrMaterialStack* =NULL) {render();}

  void reset();

  void setOnOff(const Bool);
  Bool isOn() const {return _on;}

  void setLightType(const LightType);
  LightType getLightType() const {return _type;}

  void setShowMode(const Bool);
  Bool isShow() const {return _showLight;}

  int getLightNum() const {return _order;}

  void setAmbient(const vector4);
  void setAmbient(const float, const float, const float, const float);
  void getAmbient(vector4 v) const {
    if ( ! v ) return;
    v[0] = _ambient[0];
    v[1] = _ambient[1];
    v[2] = _ambient[2];
    v[3] = _ambient[3];
  }
  void setDiffuse(const vector4);
  void setDiffuse(const float, const float, const float, const float);
  void getDiffuse(vector4 v) const {
    if ( ! v ) return;
    v[0] = _diffuse[0];
    v[1] = _diffuse[1];
    v[2] = _diffuse[2];
    v[3] = _diffuse[3];
  }
  void setSpecular(const vector4);
  void setSpecular(const float, const float, const float, const float);
  void getSpecular(vector4 v) const {
    if ( ! v ) return;
    v[0] = _specular[0];
    v[1] = _specular[1];
    v[2] = _specular[2];
    v[3] = _specular[3];
  }
  void setSpotCutoff(const float);
  float getSpotCutoff() const {return _spotCutoff[0];}

protected:
  vfrLight();
  vfrLight(const int);

  void makeLight(const int);
  Bool setLightNum(const int);
  void generateBbox();
  void generateBbox(const CES::Vec3<float>&) {generateBbox();}
  void drawLight();

  int _order;
  LightType _type;
  Bool _on;
  Bool _showLight;

  vector4 _ambient;
  vector4 _diffuse;
  vector4 _specular;
  float _spotCutoff[1];

  friend class vfrScene;
};

#endif /* _VFR_LIGHT_H_ */
