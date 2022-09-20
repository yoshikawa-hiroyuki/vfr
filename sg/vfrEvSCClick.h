//
// vfrEvSCClick
//    Shift+Cntl+Click Event Class
//
#ifndef _VFR_EVENT_SCCLICK_H_
#define _VFR_EVENT_SCCLICK_H_

#include "vfrEvent.h"

class EXPORTED_CLASS vfrEvSCClick : public vfrEvent {
  public:
    vfrEvSCClick() {}
    virtual ~vfrEvSCClick() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isClickEvent() const {return TRUE;}
    Bool withLeftButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvRSCClick : public vfrEvent {
  public:
    vfrEvRSCClick() {}
    virtual ~vfrEvRSCClick() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isClickEvent() const {return TRUE;}
    Bool withRightButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvMSCClick : public vfrEvent {
  public:
    vfrEvMSCClick() {}
    virtual ~vfrEvMSCClick() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isClickEvent() const {return TRUE;}
    Bool withMiddleButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};

#endif /* _VFR_EVENT_SCCLICK_H_ */
