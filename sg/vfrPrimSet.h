//
// vfrPrimSet
//  Primitive-set Class
//

#ifndef _VFR_PRIMSET_H_
#define _VFR_PRIMSET_H_

#include "vfrNode.h"

namespace VFR {
  enum {VFR_PRIMSET_ROT=0x1, VFR_PRIMSET_SCALE=(0x1<<1)};
  typedef int PrimRSType;
};


class EXPORTED_CLASS vfrPrimSet : public vfrNode {
public:
  vfrPrimSet(const std::string& =std::string(VFR_NONAME), const Bool =FALSE);
  virtual ~vfrPrimSet();

  void setPrimitive(vfrNode* p);
  vfrNode* getPrimitive() {return _prim;}

  void setRotScaleMode(const PrimRSType rsm) {
    if ( _rsMode == rsm ) return;
    _rsMode = rsm; notice();
  }
  PrimRSType getRotScaleMode() const {return _rsMode;}

  virtual void rumor(vfrBase*);
  virtual void generateBbox();
  virtual void generateBbox(const CES::Vec3<float>&);

protected:
  vfrNode* _prim;
  PrimRSType _rsMode;

  void renderSolid();
  void renderWire();
  void renderPoint();

  void callPrimRender(const RenderType rt,
		      const vector3 pos, const vector3 vec =NULL);

  bool calcBboxPoints(const int idx, CES::Vec3<float>* xbb);
};

#endif /* _VFR_PRIMSET_H_ */
