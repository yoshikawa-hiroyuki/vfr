//
// vfrEvClick
//   Click Event Class
//
#ifndef _VFR_EVENT_CLICK_H_
#define _VFR_EVENT_CLICK_H_

#include "vfrEvent.h"

class EXPORTED_CLASS vfrEvClick : public vfrEvent {
 public:
  vfrEvClick() : m_dblClk(FALSE) {}
  virtual ~vfrEvClick() {}
  static vfrEvent& instance(class vfrDispatch&);

  Bool isClickEvent() const {return TRUE;}
  Bool withLeftButton() const {return TRUE;}
  Bool isDblClickEvent() const {return m_dblClk;}

  void setDblClickMode(const Bool dblClk) {m_dblClk = dblClk;}

 protected:
  Bool m_dblClk;
};

class EXPORTED_CLASS vfrEvRClick : public vfrEvent {
 public:
  vfrEvRClick() : m_dblClk(FALSE) {}
  virtual ~vfrEvRClick() {}
  static vfrEvent& instance(class vfrDispatch&);

  Bool isClickEvent() const {return TRUE;}
  Bool withRightButton() const {return TRUE;}
  Bool isDblClickEvent() const {return m_dblClk;}

  void setDblClickMode(const Bool dblClk) {m_dblClk = dblClk;}

 protected:
  Bool m_dblClk;
};

class EXPORTED_CLASS vfrEvMClick : public vfrEvent {
 public:
  vfrEvMClick() : m_dblClk(FALSE) {}
  virtual ~vfrEvMClick() {}
  static vfrEvent& instance(class vfrDispatch&);

  Bool isClickEvent() const {return TRUE;}
  Bool withMiddleButton() const {return TRUE;}
  Bool isDblClickEvent() const {return m_dblClk;}

  void setDblClickMode(const Bool dblClk) {m_dblClk = dblClk;}

 protected:
  Bool m_dblClk;
};

#endif /* _VFR_EVENT_CLICK_H_ */
