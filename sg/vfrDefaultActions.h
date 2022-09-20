//
// vfrDefaultActions
//   Default Actions for vfrDrawArea
//
#ifndef _VFR_DEFAULT_ACTIONS_H_
#define _VFR_DEFAULT_ACTIONS_H_

#include "vfrAction.h"
#include "vfrDispatch.h"
#include "vfrNode.h"
#include "vfrScene.h"
#include "vfrCamera.h"
#include "vfrScreen.h"
#include "vfrDrawArea.h"

namespace VFR {
  enum {VFRDA_MBLEFT=(0x1), VFRDA_MBRIGHT=(0x1<<1), VFRDA_MBMIDDLE=(0x1<<2)};
  typedef int MButtonType;
};


class EXPORTED_CLASS vfrDefaultActions {
public:
  static void SetDefaultAction(vfrDrawArea&);
  static void SetSelectedNode(vfrNode*);
  static vfrNode* GetSelectedNode();

private:
  // Selected Node
  static vfrNode* _SelectedNode;
};


// Action for KeyInput
class EXPORTED_CLASS vfrKeyInAction : public vfrAction {
public:
  void execute(vfrEvent&);
  static vfrKeyInAction& instance();
private:
  vfrKeyInAction() {}
};

// Action for Wheel
class EXPORTED_CLASS vfrWheelAction : public vfrAction {
public:
  void execute(vfrEvent&);
  static vfrWheelAction& instance();
private:
  vfrWheelAction() {}
};

// Action for Click and Select
class EXPORTED_CLASS vfrClickSelectAction : public vfrAction {
public:
  void execute(vfrEvent&);
  void setMButtonType(const MButtonType bt) {_bType = bt;}
  MButtonType getMButtonType() const {return _bType;}
  static vfrClickSelectAction& instance();
private:
  vfrClickSelectAction() : _bType(VFRDA_MBLEFT) {}
  MButtonType _bType;
};

// Action to start drawing Rubber Box
class EXPORTED_CLASS vfrStartRBoxAction : public vfrAction {
public:
  void execute(vfrEvent&);
  static vfrStartRBoxAction& instance();
private:
  vfrStartRBoxAction() {}
};

// Action to end drawing Rubber Box
class EXPORTED_CLASS vfrEndRBoxAction : public vfrAction {
public:
  void execute(vfrEvent&);
  void setMButtonType(const MButtonType bt) {_bType = bt;}
  MButtonType getMButtonType() const {return _bType;}
  static vfrEndRBoxAction& instance();
private:
  vfrEndRBoxAction() : _bType(VFRDA_MBLEFT) {}
  MButtonType _bType;
};

// Action to redraw Rubber Box
class EXPORTED_CLASS vfrDrawRBoxAction : public vfrAction {
public:
  void execute(vfrEvent&);
  static vfrDrawRBoxAction& instance();
private:
  vfrDrawRBoxAction() {}
};

// Action to rotate Camera
class EXPORTED_CLASS vfrRotCameraAction : public vfrAction {
public:
  void execute(vfrEvent&);
  static vfrRotCameraAction& instance();
private:
  vfrRotCameraAction() {}
};

// Action to translate Camera
class EXPORTED_CLASS vfrTransCameraAction : public vfrAction {
public:
  void execute(vfrEvent&);
  static vfrTransCameraAction& instance();
private:
  vfrTransCameraAction() {}
};

// Action to scale Camera
class EXPORTED_CLASS vfrScaleCameraAction : public vfrAction {
public:
  void execute(vfrEvent&);
  static vfrScaleCameraAction& instance();
private:
  vfrScaleCameraAction() {}
};

// Actions to rotate selected Node
class EXPORTED_CLASS vfrRotNodeAction : public vfrAction {
public:
  void execute(vfrEvent&);
  static vfrRotNodeAction& instance();
private:
  vfrRotNodeAction() {}
};

// Action to translate selected Node
class EXPORTED_CLASS vfrTransNodeAction : public vfrAction {
public:
  void execute(vfrEvent&);
  static vfrTransNodeAction& instance();
private:
  vfrTransNodeAction() {}
};

// Action to scale selected Node
class EXPORTED_CLASS vfrScaleNodeAction : public vfrAction {
public:
  void execute(vfrEvent&);
  static vfrScaleNodeAction& instance();
private:
  vfrScaleNodeAction() {}
};

#endif /* _VFR_DEFAULT_ACTIONS_H_ */
