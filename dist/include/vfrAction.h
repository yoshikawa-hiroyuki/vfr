//
// vfrAction
//   Basic Action Class
//
#ifndef _VFR_ACTION_H_
#define _VFR_ACTION_H_

#include "vfrBase.h"


class EXPORTED_CLASS vfrAction {
public:
  virtual ~vfrAction() {}
  virtual void execute(class vfrEvent &);

protected:
  vfrAction() {}
};

#endif /* _VFR_ACTION_H_ */
