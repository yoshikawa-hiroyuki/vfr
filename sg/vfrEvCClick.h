//
// vfrEvCClick
//   Cntl+Click Event Class
//
#ifndef _VFR_EVENT_CCLICK_H_
#define _VFR_EVENT_CCLICK_H_

#include "vfrEvent.h"

class EXPORTED_CLASS vfrEvCClick : public vfrEvent {
  public:
    vfrEvCClick() {}
    virtual ~vfrEvCClick() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isClickEvent() const {return TRUE;}
    Bool withLeftButton() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvRCClick : public vfrEvent {
  public:
    vfrEvRCClick() {}
    virtual ~vfrEvRCClick() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isClickEvent() const {return TRUE;}
    Bool withRightButton() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvMCClick : public vfrEvent {
  public:
    vfrEvMCClick() {}
    virtual ~vfrEvMCClick() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isClickEvent() const {return TRUE;}
    Bool withMiddleButton() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};

#endif /* _VFR_EVENT_CCLICK_H_ */
