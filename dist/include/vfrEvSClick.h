//
// vfrEvSClick
//   Shift+Click Event Class
//
#ifndef _VFR_EVENT_SCLICK_H_
#define _VFR_EVENT_SCLICK_H_

#include "vfrEvent.h"

class EXPORTED_CLASS vfrEvSClick : public vfrEvent {
  public:
    vfrEvSClick() {}
    virtual ~vfrEvSClick() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isClickEvent() const {return TRUE;}
    Bool withLeftButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvRSClick : public vfrEvent {
  public:
    vfrEvRSClick() {}
    virtual ~vfrEvRSClick() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isClickEvent() const {return TRUE;}
    Bool withRightButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvMSClick : public vfrEvent {
  public:
    vfrEvMSClick() {}
    virtual ~vfrEvMSClick() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isClickEvent() const {return TRUE;}
    Bool withMiddleButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
};

#endif /* _VFR_EVENT_SCLICK_H_ */
