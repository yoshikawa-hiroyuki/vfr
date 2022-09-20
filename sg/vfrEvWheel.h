//
// vfrEvWheel
//   Wheel Event Class
//
#ifndef _VFR_EVENT_WHEEL_H_
#define _VFR_EVENT_WHEEL_H_

#include "vfrEvent.h"

class EXPORTED_CLASS vfrEvWheel : public vfrEvent {
 public:
  vfrEvWheel() : _shifted(FALSE), _controlled(FALSE) {}
  virtual ~vfrEvWheel() {}
  static vfrEvent& instance(class vfrDispatch&);

  Bool isWheelEvent() const {return TRUE;}
  Bool withShiftKey() const {return _shifted;}
  Bool withControlKey() const {return _controlled;}

  void setKeyShifted(const Bool ks) {_shifted = ks;}
  void setKeyControlled(const Bool kc) {_controlled = kc;}

 protected:
  Bool _shifted;
  Bool _controlled;

  virtual void reset() {
    _shifted = _controlled = FALSE;
    vfrEvent::reset();
  }
};

#endif /* _VFR_EVENT_WHEEL_H_ */
