//
// vfrEvent
//   Basic Event Class
//
#ifndef _VFR_EVENT_H_
#define _VFR_EVENT_H_

#include "vfrBase.h"
typedef unsigned int  VFRkeyCode;


class EXPORTED_CLASS vfrEvent {
public:
  void regist(class vfrAction* act) {_action = act;}
  class vfrAction* getAction() {return _action;}
  void execAction();

  class vfrScreen* getScreen() {return _currentScreen;}
  void setScreen(class vfrScreen* s) {_currentScreen = s;}
  virtual Point2& getMPoint() {return _point;}
  virtual void setMPoint(const Point2& p) {_point = p;}
  virtual Point2& getMMove() {return _move;}
  virtual void setMMove(const Point2& p) {_move = p;}
  virtual VFRkeyCode& getKey() {return _keyCode;}
  virtual void setKey(const VFRkeyCode& kc) {_keyCode = kc;}
  virtual int& getWheelRot() {return _wheelRot;}
  virtual void setWheelRot(const int wr) {_wheelRot = wr;}
  virtual Bool isKeyEvent() const {return FALSE;}
  virtual Bool isClickEvent() const {return FALSE;}
  virtual Bool isDragEvent() const {return FALSE;}
  virtual Bool isDragStartEvent() const {return FALSE;}
  virtual Bool isDragEndEvent() const {return FALSE;}
  virtual Bool isDblClickEvent() const {return FALSE;}
  virtual Bool isWheelEvent() const {return FALSE;}
  virtual Bool withLeftButton() const {return FALSE;}
  virtual Bool withRightButton() const {return FALSE;}
  virtual Bool withMiddleButton() const {return FALSE;}
  virtual Bool withShiftKey() const {return FALSE;}
  virtual Bool withControlKey() const {return FALSE;}
  
protected:
  Point2 _point;
  Point2 _move;
  VFRkeyCode _keyCode;
  int _wheelRot;
  class vfrScreen* _currentScreen;
  class vfrAction* _action;
    
  vfrEvent() {reset();}
  virtual ~vfrEvent() {}
  
  virtual void reset() {
    _keyCode = 0;
    _wheelRot = 0;
    _point.x = _point.y = 0;
    _move.x = _move.y = 0;
    _currentScreen = NULL;
    _action = NULL;
  }
  
  friend class vfrDispatch;
};

#endif /* _VFR_EVENT_H_ */
