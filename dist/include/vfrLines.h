//
// vfrLines
//   Line Segments Class
//
#ifndef _VFR_LINES_H_
#define _VFR_LINES_H_

#include "vfrNode.h"


class EXPORTED_CLASS vfrLines : public vfrNode {
public:
  vfrLines(const std::string& =std::string(VFR_NONAME), const Bool =FALSE);
  virtual ~vfrLines();
  void renderFeedBack(const unsigned int);
  RenderType getRenderMode() const;

protected:
  void renderWire();
  void reset();
};

#endif /* _VFR_LINES_H_ */
