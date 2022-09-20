//
// vfrEvents
//   Event Classes
//

#include "vfrEvent.h"
#include "vfrAction.h"
#include "vfrDispatch.h"
#include "vfrEvKeyIn.h"
#include "vfrEvClick.h"
#include "vfrEvCClick.h"
#include "vfrEvSClick.h"
#include "vfrEvSCClick.h"
#include "vfrEvDrag.h"
#include "vfrEvCDrag.h"
#include "vfrEvSDrag.h"
#include "vfrEvSCDrag.h"
#include "vfrEvWheel.h"


//================== class vfrEvent ==================//
void
vfrEvent::execAction() {
  if ( _action )
    _action->execute(*this);
}

//================== class vfrEvKeyIn ==================//
vfrEvent&
vfrEvKeyIn::instance(vfrDispatch& d) {
  return d.KeyIn();
}

//================== class vfrEvClick ==================//
vfrEvent&
vfrEvClick::instance(vfrDispatch& d) {
  return d.Click();
}

//================== class vfrEvRClick ==================//
vfrEvent&
vfrEvRClick::instance(vfrDispatch& d) {
  return d.RClick();
}

//================== class vfrEvMClick ==================//
vfrEvent&
vfrEvMClick::instance(vfrDispatch& d) {
  return d.MClick();
}

//================== class vfrEvCClick ==================//
vfrEvent&
vfrEvCClick::instance(vfrDispatch& d) {
  return d.CClick();
}

//================== class vfrEvRCClick ==================//
vfrEvent&
vfrEvRCClick::instance(vfrDispatch& d) {
  return d.RCClick();
}

//================== class vfrEvMCClick ==================//
vfrEvent&
vfrEvMCClick::instance(vfrDispatch& d) {
  return d.MCClick();
}

//================== class vfrEvSClick ==================//
vfrEvent&
vfrEvSClick::instance(vfrDispatch& d) {
  return d.SClick();
}

//================== class vfrEvRSClick ==================//
vfrEvent&
vfrEvRSClick::instance(vfrDispatch& d) {
  return d.RSClick();
}

//================== class vfrEvMSClick ==================//
vfrEvent&
vfrEvMSClick::instance(vfrDispatch& d) {
  return d.MSClick();
}

//================== class vfrEvSCClick ==================//
vfrEvent&
vfrEvSCClick::instance(vfrDispatch& d) {
  return d.SCClick();
}

//================== class vfrEvRSCClick ==================//
vfrEvent&
vfrEvRSCClick::instance(vfrDispatch& d) {
  return d.RSCClick();
}

//================== class vfrEvMSCClick ==================//
vfrEvent&
vfrEvMSCClick::instance(vfrDispatch& d) {
  return d.MSCClick();
}

//================== class vfrEvDragStart ==================//
vfrEvent&
vfrEvDragStart::instance(vfrDispatch& d) {
  return d.DragStart();
}

//================== class vfrEvDrag ==================//
vfrEvent&
vfrEvDrag::instance(vfrDispatch& d) {
  return d.Drag();
}

//================== class vfrEvDragEnd ==================//
vfrEvent&
vfrEvDragEnd::instance(vfrDispatch& d) {
  return d.DragEnd();
}

//================== class vfrEvRDragStart ==================//
vfrEvent&
vfrEvRDragStart::instance(vfrDispatch& d) {
  return d.RDragStart();
}

//================== class vfrEvRDrag ==================//
vfrEvent&
vfrEvRDrag::instance(vfrDispatch& d) {
  return d.RDrag();
}

//================== class vfrEvRDragEnd ==================//
vfrEvent&
vfrEvRDragEnd::instance(vfrDispatch& d) {
  return d.RDragEnd();
}

//================== class vfrEvMDragStart ==================//
vfrEvent&
vfrEvMDragStart::instance(vfrDispatch& d) {
  return d.MDragStart();
}

//================== class vfrEvMDrag ==================//
vfrEvent&
vfrEvMDrag::instance(vfrDispatch& d) {
  return d.MDrag();
}

//================== class vfrEvMDragEnd ==================//
vfrEvent&
vfrEvMDragEnd::instance(vfrDispatch& d) {
  return d.MDragEnd();
}

//================== class vfrEvCDragStart ==================//
vfrEvent&
vfrEvCDragStart::instance(vfrDispatch& d) {
  return d.CDragStart();
}

//================== class vfrEvCDrag ==================//
vfrEvent&
vfrEvCDrag::instance(vfrDispatch& d) {
  return d.CDrag();
}

//================== class vfrEvCDragEnd ==================//
vfrEvent&
vfrEvCDragEnd::instance(vfrDispatch& d) {
  return d.CDragEnd();
}

//================== class vfrEvRCDragStart ==================//
vfrEvent&
vfrEvRCDragStart::instance(vfrDispatch& d) {
  return d.RCDragStart();
}

//================== class vfrEvRCDrag ==================//
vfrEvent&
vfrEvRCDrag::instance(vfrDispatch& d) {
  return d.RCDrag();
}

//================== class vfrEvRCDragEnd ==================//
vfrEvent&
vfrEvRCDragEnd::instance(vfrDispatch& d) {
  return d.RCDragEnd();
}

//================== class vfrEvMCDragStart ==================//
vfrEvent&
vfrEvMCDragStart::instance(vfrDispatch& d) {
  return d.MCDragStart();
}

//================== class vfrEvMCDrag ==================//
vfrEvent&
vfrEvMCDrag::instance(vfrDispatch& d) {
  return d.MCDrag();
}

//================== class vfrEvMCDragEnd ==================//
vfrEvent&
vfrEvMCDragEnd::instance(vfrDispatch& d) {
  return d.MCDragEnd();
}

//================== class vfrEvSDragStart ==================//
vfrEvent&
vfrEvSDragStart::instance(vfrDispatch& d) {
  return d.SDragStart();
}

//================== class vfrEvSDrag ==================//
vfrEvent&
vfrEvSDrag::instance(vfrDispatch& d) {
  return d.SDrag();
}

//================== class vfrEvSDragEnd ==================//
vfrEvent&
vfrEvSDragEnd::instance(vfrDispatch& d) {
  return d.SDragEnd();
}

//================== class vfrEvRSDragStart ==================//
vfrEvent&
vfrEvRSDragStart::instance(vfrDispatch& d) {
  return d.RSDragStart();
}

//================== class vfrEvRSDrag ==================//
vfrEvent&
vfrEvRSDrag::instance(vfrDispatch& d) {
  return d.RSDrag();
}

//================== class vfrEvRSDragEnd ==================//
vfrEvent&
vfrEvRSDragEnd::instance(vfrDispatch& d) {
  return d.RSDragEnd();
}

//================== class vfrEvMSDragStart ==================//
vfrEvent&
vfrEvMSDragStart::instance(vfrDispatch& d) {
  return d.MSDragStart();
}

//================== class vfrEvMSDrag ==================//
vfrEvent&
vfrEvMSDrag::instance(vfrDispatch& d) {
  return d.MSDrag();
}

//================== class vfrEvMSDragEnd ==================//
vfrEvent&
vfrEvMSDragEnd::instance(vfrDispatch& d) {
  return d.MSDragEnd();
}

//================== class vfrEvSCDragStart ==================//
vfrEvent&
vfrEvSCDragStart::instance(vfrDispatch& d) {
  return d.SCDragStart();
}

//================== class vfrEvSCDrag ==================//
vfrEvent&
vfrEvSCDrag::instance(vfrDispatch& d) {
  return d.SCDrag();
}

//================== class vfrEvSCDragEnd ==================//
vfrEvent&
vfrEvSCDragEnd::instance(vfrDispatch& d) {
  return d.SCDragEnd();
}

//================== class vfrEvRSCDragStart ==================//
vfrEvent&
vfrEvRSCDragStart::instance(vfrDispatch& d) {
  return d.RSCDragStart();
}

//================== class vfrEvRSCDrag ==================//
vfrEvent&
vfrEvRSCDrag::instance(vfrDispatch& d) {
  return d.RSCDrag();
}

//================== class vfrEvRSCDragEnd ==================//
vfrEvent&
vfrEvRSCDragEnd::instance(vfrDispatch& d) {
  return d.RSCDragEnd();
}

//================== class vfrEvMSCDragStart ==================//
vfrEvent&
vfrEvMSCDragStart::instance(vfrDispatch& d) {
  return d.MSCDragStart();
}

//================== class vfrEvMSCDrag ==================//
vfrEvent&
vfrEvMSCDrag::instance(vfrDispatch& d) {
  return d.MSCDrag();
}

//================== class vfrEvMSCDragEnd ==================//
vfrEvent&
vfrEvMSCDragEnd::instance(vfrDispatch& d) {
  return d.MSCDragEnd();
}

//================== class vfrEvWheel ==================//
vfrEvent&
vfrEvWheel::instance(vfrDispatch& d) {
  return d.Wheel();
}

// EOF
