//
// vfrVectors
//   Vector-set Object Class
//
#ifndef _VFR_VECTORS_H_
#define _VFR_VECTORS_H_

#include "vfrNode.h"


class EXPORTED_CLASS vfrVectors : public vfrNode {
public:
  enum VecPosType {NORMAL, CENTER, TIP};

  vfrVectors(const std::string& =std::string(VFR_NONAME), const Bool =FALSE);
  virtual ~vfrVectors();

  void setHeadMode(const Bool);
  Bool getHeadMode() const {return _head;}
  void setHeadScale(const float);
  float getHeadScale() const {return _headScale;}
  void setHeadWidth(const float);
  float getHeadWidth() const {return _headWidth;}
  void setScaleFac(const float);
  float getScaleFac() const {return _scaleFac;}
  void setPosType(const VecPosType);
  VecPosType getPosType() const {return _pType;}
  void setShowZero(const Bool);
  Bool getShowZero() const {return _showZero;}
  RenderType getRenderMode() const;

protected:
  Bool  _head;
  float _headScale;
  float _headWidth;
  float _scaleFac;
  VecPosType _pType;
  Bool  _showZero;

  void renderWire();
  void renderFeedBack(const unsigned int);
};

#endif /* _VFR_VECTORS_H_ */
