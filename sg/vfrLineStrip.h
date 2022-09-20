//
// vfrLineStrip
//   Line Strip Class
//
#ifndef _VFR_LINE_STRIP_H_
#define _VFR_LINE_STRIP_H_

#include "vfrNode.h"

class EXPORTED_CLASS vfrLineStrip : public vfrNode {
public:
  vfrLineStrip(const std::string& =std::string(VFR_NONAME),
	       const Bool =FALSE);
  virtual ~vfrLineStrip();
  void renderFeedBack(const unsigned int);
  void setLoopMode(Bool lm) {
    if ( _loopMode == lm ) return;
    _loopMode = lm; notice();
  }
  Bool getLoopMode() const {return _loopMode;}
  RenderType getRenderMode() const;

protected:
  void renderWire();
  void reset();
  Bool _loopMode;
};

#endif /* _VFR_LINE_STRIP_H_ */
