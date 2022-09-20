//
// vfrIndexLines
//   Indexed Line Strips Class
//
#ifndef _VFR_INDEXLINES_H_
#define _VFR_INDEXLINES_H_

#include "vfrNode.h"


class EXPORTED_CLASS vfrIndexLines : public vfrNode {
public:
  vfrIndexLines(const std::string& =std::string(VFR_NONAME),
		const Bool =FALSE);
  virtual ~vfrIndexLines();
  void setLoopMode(const Bool lm) {
    if ( _loopMode == lm ) return;
    _loopMode = lm; notice();
  }
  Bool getLoopMode() const {return _loopMode;}
  RenderType getRenderMode() const;
  void renderFeedBack(const unsigned int);

protected:
  void renderWire();
  Bool _loopMode;
};

#endif /* _VFR_INDEXLINES_H_ */
