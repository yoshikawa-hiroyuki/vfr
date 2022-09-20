//
// vfrDefaultActions
//   Default Actions for vfrDrawArea
//
#include "vfrDefaultActions.h"
#include "vfrKeyCode.h"

vfrNode* vfrDefaultActions::_SelectedNode = NULL;


void
vfrKeyInAction::execute(vfrEvent& e) {
  if ( ! e.isKeyEvent() )
    return;

  VFRkeyCode kc = e.getKey();
  if ( kc == VFRKC_RETURN ) {
    vfrScreen* screen = (vfrScreen*)e.getScreen();
    if ( ! screen )
      return;
    vfrNode* selectedNode = vfrDefaultActions::GetSelectedNode();
    if ( selectedNode ) {
      selectedNode->identity();
    } else {
      vfrCamera *camera = screen->getCamera();
      if ( ! camera )
        return;
      camera->identity();
    }
    return;
  }
  if ( kc == VFRKC_ESCAPE )
    exit(0);
}
vfrKeyInAction&
vfrKeyInAction::instance() {
  static vfrKeyInAction thevfrKeyInAction;
  return thevfrKeyInAction;
}

void
vfrWheelAction::execute(vfrEvent& e) {
  if ( ! e.isWheelEvent() )
    return;

  int wr = e.getWheelRot();
  if ( wr ) {
    vfrScreen* screen = (vfrScreen*)e.getScreen();
    if ( ! screen )
      return;
    vfrNode* selectedNode = vfrDefaultActions::GetSelectedNode();
    if ( selectedNode ) {
      float scale = 1.f;
      if ( wr < 0 ) scale = -wr * 1.05f;
      else scale = wr * 0.95238f;
      selectedNode->scale(scale);
    } else {
      vfrCamera *camera = screen->getCamera();
      if ( ! camera )
        return;
      camera->dragTransZ(0.f, wr * 2.0f);
    }
    return;
  }
}
vfrWheelAction&
vfrWheelAction::instance() {
  static vfrWheelAction thevfrWheelAction;
  return thevfrWheelAction;
}

void
vfrClickSelectAction::execute(vfrEvent &e) {
  if ( ! e.isClickEvent() )
    return;

  Point2 cp = e.getMPoint();
  vfrScreen* screen = (vfrScreen*)e.getScreen();
  if ( ! screen )
    return;
  cp = screen->getRelativePoint(cp);
  unsigned int* r = screen->clickSelect(cp.x, cp.y);
  vfrNode* selectedNode = vfrDefaultActions::GetSelectedNode();
  if ( selectedNode ) {
    selectedNode->setBboxShowMode(FALSE);
    vfrDefaultActions::SetSelectedNode(NULL);
  }
  if ( r[0] > 0 ) {
    vfrNode* co = screen->getNode(r[1]);
    if ( co ) {
      screen->clickFeedback(cp.x, cp.y, r[1]);
      co->setBboxShowMode(TRUE);
      vfrDefaultActions::SetSelectedNode(co);

      // Change Actions
      vfrDrawArea* da = screen->getParent();
      if ( ! da ) return;
      vfrDispatch& dpr = vfrDispatch::instance(*da);
      if ( _bType & VFRDA_MBLEFT ) {
        vfrEvent& Sdrag = vfrEvSDrag::instance(dpr);
        Sdrag.regist(&vfrTransNodeAction::instance());
        vfrEvent& Cdrag = vfrEvCDrag::instance(dpr);
        Cdrag.regist(&vfrRotNodeAction::instance());
        vfrEvent& SCdrag = vfrEvSCDrag::instance(dpr);
        SCdrag.regist(&vfrScaleNodeAction::instance());
      } 
      if ( _bType & VFRDA_MBRIGHT ) {
        vfrEvent& Sdrag = vfrEvRSDrag::instance(dpr);
        Sdrag.regist(&vfrTransNodeAction::instance());
        vfrEvent& Cdrag = vfrEvRCDrag::instance(dpr);
        Cdrag.regist(&vfrRotNodeAction::instance());
        vfrEvent& SCdrag = vfrEvRSCDrag::instance(dpr);
        SCdrag.regist(&vfrScaleNodeAction::instance());
      } 
      if ( _bType & VFRDA_MBMIDDLE ) {
        vfrEvent& Sdrag = vfrEvMSDrag::instance(dpr);
        Sdrag.regist(&vfrTransNodeAction::instance());
        vfrEvent& Cdrag = vfrEvMCDrag::instance(dpr);
        Cdrag.regist(&vfrRotNodeAction::instance());
        vfrEvent& SCdrag = vfrEvMSCDrag::instance(dpr);
        SCdrag.regist(&vfrScaleNodeAction::instance());
      }

      return;
    }
  }
  vfrDrawArea* da = screen->getParent();
  if ( ! da ) return;
  vfrDispatch& dpr = vfrDispatch::instance(*da);
  if ( _bType & VFRDA_MBLEFT ) {
    vfrEvent& Sdrag = vfrEvSDrag::instance(dpr);
    Sdrag.regist(&vfrTransCameraAction::instance());
    vfrEvent& Cdrag = vfrEvCDrag::instance(dpr);
    Cdrag.regist(&vfrRotCameraAction::instance());
    vfrEvent& SCdrag = vfrEvSCDrag::instance(dpr);
    SCdrag.regist(&vfrScaleCameraAction::instance());
  } 
  if ( _bType & VFRDA_MBRIGHT ) {
    vfrEvent& Sdrag = vfrEvRSDrag::instance(dpr);
    Sdrag.regist(&vfrTransCameraAction::instance());
    vfrEvent& Cdrag = vfrEvRCDrag::instance(dpr);
    Cdrag.regist(&vfrRotCameraAction::instance());
    vfrEvent& SCdrag = vfrEvRSCDrag::instance(dpr);
    SCdrag.regist(&vfrScaleCameraAction::instance());
  } 
  if ( _bType & VFRDA_MBMIDDLE ) {
    vfrEvent& Sdrag = vfrEvMSDrag::instance(dpr);
    Sdrag.regist(&vfrTransCameraAction::instance());
    vfrEvent& Cdrag = vfrEvMCDrag::instance(dpr);
    Cdrag.regist(&vfrRotCameraAction::instance());
    vfrEvent& SCdrag = vfrEvMSCDrag::instance(dpr);
    SCdrag.regist(&vfrScaleCameraAction::instance());
  }
}
vfrClickSelectAction&
vfrClickSelectAction::instance() {
  static vfrClickSelectAction thevfrClickSelectAction;
  return thevfrClickSelectAction;
}

void
vfrStartRBoxAction::execute(vfrEvent& e) {
  Point2 cp = e.getMPoint();
  vfrScreen* screen = (vfrScreen*)e.getScreen();
  if ( ! screen )
    return;
  screen->startRubberBox(cp);
}
vfrStartRBoxAction&
vfrStartRBoxAction::instance() {
  static vfrStartRBoxAction thevfrStartRBoxAction;
  return thevfrStartRBoxAction;
}

void
vfrEndRBoxAction::execute(vfrEvent& e) {
  Point2 cp0, cp1;
  int w, h;
  vfrScreen* screen = (vfrScreen*)e.getScreen();
  if ( ! screen )
    return;
  screen->clearRubberBox(&cp0, &cp1);
  cp0 = screen->getRelativePoint(cp0);
  cp1 = screen->getRelativePoint(cp1);
  w = cp1.x - cp0.x;
  h = cp1.y - cp0.y;
  if ( w < 0 ) {
    cp0.x += w;
    w *= -1;
  }
  if ( h < 0 ) {
    cp0.y += h;
    h *= -1;
  }
  unsigned int* r = screen->sweepSelect(cp0.x, cp0.y, w, h);
  vfrNode* selectedNode = vfrDefaultActions::GetSelectedNode();
  if ( selectedNode ) {
    selectedNode->setBboxShowMode(FALSE);
    vfrDefaultActions::SetSelectedNode(NULL);
  }
  if ( r[0] > 0 ) {
    vfrNode* co = screen->getNode(r[1]);
    if ( co ) {
      screen->sweepFeedback(cp0.x, cp0.y, w, h, r[1]);
      co->setBboxShowMode(TRUE);
      vfrDefaultActions::SetSelectedNode(co);

      // Change Actions
      vfrDrawArea* da = screen->getParent();
      if ( ! da ) return;
      vfrDispatch& dpr = vfrDispatch::instance(*da);
      if ( _bType & VFRDA_MBLEFT ) {
        vfrEvent& Sdrag = vfrEvSDrag::instance(dpr);
        Sdrag.regist(&vfrTransNodeAction::instance());
        vfrEvent& Cdrag = vfrEvCDrag::instance(dpr);
        Cdrag.regist(&vfrRotNodeAction::instance());
        vfrEvent& SCdrag = vfrEvSCDrag::instance(dpr);
        SCdrag.regist(&vfrScaleNodeAction::instance());
      } 
      if ( _bType & VFRDA_MBRIGHT ) {
        vfrEvent& Sdrag = vfrEvRSDrag::instance(dpr);
        Sdrag.regist(&vfrTransNodeAction::instance());
        vfrEvent& Cdrag = vfrEvRCDrag::instance(dpr);
        Cdrag.regist(&vfrRotNodeAction::instance());
        vfrEvent& SCdrag = vfrEvRSCDrag::instance(dpr);
        SCdrag.regist(&vfrScaleNodeAction::instance());
      } 
      if ( _bType & VFRDA_MBMIDDLE ) {
        vfrEvent& Sdrag = vfrEvMSDrag::instance(dpr);
        Sdrag.regist(&vfrTransNodeAction::instance());
        vfrEvent& Cdrag = vfrEvMCDrag::instance(dpr);
        Cdrag.regist(&vfrRotNodeAction::instance());
        vfrEvent& SCdrag = vfrEvMSCDrag::instance(dpr);
        SCdrag.regist(&vfrScaleNodeAction::instance());
      }

      return;
    }
  }
  vfrDrawArea* da = screen->getParent();
  if ( ! da ) return;
  vfrDispatch& dpr = vfrDispatch::instance(*da);
  if ( _bType & VFRDA_MBLEFT ) {
    vfrEvent& Sdrag = vfrEvSDrag::instance(dpr);
    Sdrag.regist(&vfrTransCameraAction::instance());
    vfrEvent& Cdrag = vfrEvCDrag::instance(dpr);
    Cdrag.regist(&vfrRotCameraAction::instance());
    vfrEvent& SCdrag = vfrEvSCDrag::instance(dpr);
    SCdrag.regist(&vfrScaleCameraAction::instance());
  } 
  if ( _bType & VFRDA_MBRIGHT ) {
    vfrEvent& Sdrag = vfrEvRSDrag::instance(dpr);
    Sdrag.regist(&vfrTransCameraAction::instance());
    vfrEvent& Cdrag = vfrEvRCDrag::instance(dpr);
    Cdrag.regist(&vfrRotCameraAction::instance());
    vfrEvent& SCdrag = vfrEvRSCDrag::instance(dpr);
    SCdrag.regist(&vfrScaleCameraAction::instance());
  } 
  if ( _bType & VFRDA_MBMIDDLE ) {
    vfrEvent& Sdrag = vfrEvMSDrag::instance(dpr);
    Sdrag.regist(&vfrTransCameraAction::instance());
    vfrEvent& Cdrag = vfrEvMCDrag::instance(dpr);
    Cdrag.regist(&vfrRotCameraAction::instance());
    vfrEvent& SCdrag = vfrEvMSCDrag::instance(dpr);
    SCdrag.regist(&vfrScaleCameraAction::instance());
  }
}
vfrEndRBoxAction&
vfrEndRBoxAction::instance() {
  static vfrEndRBoxAction thevfrEndRBoxAction;
  return thevfrEndRBoxAction;
}

void
vfrDrawRBoxAction::execute(vfrEvent& e) {
  Point2 cp = e.getMPoint();
  vfrScreen* screen = (vfrScreen*)e.getScreen();
  if ( ! screen )
    return;
  if ( ! screen->isOnScreen(cp) )
    return;
  screen->clearRubberBox();
  screen->drawRubberBox(cp);
}
vfrDrawRBoxAction&
vfrDrawRBoxAction::instance() {
  static vfrDrawRBoxAction thevfrDrawRBoxAction;
  return thevfrDrawRBoxAction;
}

void
vfrRotCameraAction::execute(vfrEvent& e) {
  Point2 mv = e.getMMove();
  vfrScreen* screen = (vfrScreen*)e.getScreen();
  if ( ! screen )
    return;
  vfrCamera* camera = screen->getCamera();
  if ( ! camera )
    return;
  camera->dragRot(mv.x, mv.y);
}
vfrRotCameraAction&
vfrRotCameraAction::instance() {
  static vfrRotCameraAction thevfrRotCameraAction;
  return thevfrRotCameraAction;
}

void
vfrTransCameraAction::execute(vfrEvent& e) {
  Point2 mv = e.getMMove();
  vfrScreen* screen = (vfrScreen*)e.getScreen();
  if ( ! screen )
    return;
  vfrCamera* camera = screen->getCamera();
  if ( ! camera )
    return;
  camera->dragTrans(mv.x, mv.y);
}
vfrTransCameraAction&
vfrTransCameraAction::instance() {
  static vfrTransCameraAction thevfrTransCameraAction;
  return thevfrTransCameraAction;
}

void
vfrScaleCameraAction::execute(vfrEvent& e) {
  Point2 mv = e.getMMove();
  vfrScreen* screen = (vfrScreen*)e.getScreen();
  if ( ! screen )
    return;
  vfrCamera* camera = screen->getCamera();
  if ( ! camera )
    return;
  camera->dragTransZ(mv.x, mv.y);
}
vfrScaleCameraAction&
vfrScaleCameraAction::instance() {
  static vfrScaleCameraAction thevfrScaleCameraAction;
  return thevfrScaleCameraAction;
}

void
vfrTransNodeAction::execute(vfrEvent& e) {
  vfrScreen* screen = (vfrScreen*)e.getScreen();
  if ( ! screen )
    return;
  vfrNode* selectedNode = vfrDefaultActions::GetSelectedNode();
  if ( ! selectedNode )
    return;
  screen->translateNode(selectedNode->getID(), e.getMPoint(), e.getMMove());
}
vfrTransNodeAction&
vfrTransNodeAction::instance() {
  static vfrTransNodeAction thevfrTransNodeAction;
  return thevfrTransNodeAction;
}

void
vfrRotNodeAction::execute(vfrEvent& e) {
  vfrScreen* screen = (vfrScreen*)e.getScreen();
  if ( ! screen )
    return;
  vfrNode* selectedNode = vfrDefaultActions::GetSelectedNode();
  if ( ! selectedNode )
    return;
  screen->rotateNode(selectedNode->getID(), e.getMPoint(), e.getMMove());
}
vfrRotNodeAction&
vfrRotNodeAction::instance() {
  static vfrRotNodeAction thevfrRotNodeAction;
  return thevfrRotNodeAction;
}

void
vfrScaleNodeAction::execute(vfrEvent& e) {
  vfrScreen* screen = (vfrScreen*)e.getScreen();
  if ( ! screen )
    return;
  vfrNode* selectedNode = vfrDefaultActions::GetSelectedNode();
  if ( ! selectedNode )
    return;
  screen->scaleNode(selectedNode->getID(), e.getMPoint(), e.getMMove());
}
vfrScaleNodeAction&
vfrScaleNodeAction::instance() {
  static vfrScaleNodeAction thevfrScaleNodeAction;
  return thevfrScaleNodeAction;
}


//
// Set Default Actions
//
void vfrDefaultActions::SetDefaultAction(vfrDrawArea& da) {
  vfrDispatch &dispatcher = vfrDispatch::instance(da);

  // Key Input
  vfrEvent& keyin = vfrEvKeyIn::instance(dispatcher);
  keyin.regist(&vfrKeyInAction::instance());

  // Click Selection
  vfrEvent& click = vfrEvClick::instance(dispatcher);
  click.regist(&vfrClickSelectAction::instance());

  // Dragging : Rubber-Box and Selection
  vfrEvent& dragStart = vfrEvDragStart::instance(dispatcher);
  dragStart.regist(&vfrStartRBoxAction::instance());

  vfrEvent& dragEnd = vfrEvDragEnd::instance(dispatcher);
  dragEnd.regist(&vfrEndRBoxAction::instance());

  vfrEvent& drag = vfrEvDrag::instance(dispatcher);
  drag.regist(&vfrDrawRBoxAction::instance());

  // Shift+Dragging : Camera Translation
  vfrEvent& Sdrag = vfrEvSDrag::instance(dispatcher);
  Sdrag.regist(&vfrTransCameraAction::instance());

  // Cntl+Dragging : Camera Rotation
  vfrEvent& Cdrag = vfrEvCDrag::instance(dispatcher);
  Cdrag.regist(&vfrRotCameraAction::instance());

  // Shift+Cntl+Dragging : Camera Scaling
  vfrEvent& SCdrag = vfrEvSCDrag::instance(dispatcher);
  SCdrag.regist(&vfrScaleCameraAction::instance());

  // Wheel
  vfrEvent& wheel = vfrEvWheel::instance(dispatcher);
  wheel.regist(&vfrWheelAction::instance());
}

void vfrDefaultActions::SetSelectedNode(vfrNode* o) {
  if ( o == _SelectedNode )
    return;
  _SelectedNode = o;
}

vfrNode* vfrDefaultActions::GetSelectedNode() {
  return _SelectedNode;
}

// EOF
