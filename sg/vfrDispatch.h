//
// vfrDispatch
//   Event Dispatcher Class
//
#ifndef _VFR_DISPATCH_H_
#define _VFR_DISPATCH_H_

#include "vfrEvKeyIn.h"
#include "vfrEvClick.h"
#include "vfrEvDrag.h"
#include "vfrEvSClick.h"
#include "vfrEvSDrag.h"
#include "vfrEvCClick.h"
#include "vfrEvCDrag.h"
#include "vfrEvSCClick.h"
#include "vfrEvSCDrag.h"
#include "vfrEvWheel.h"

class EXPORTED_CLASS vfrDispatch {
public:
  static vfrDispatch& instance(class vfrDrawArea&);

  vfrEvent& KeyIn() {return evKeyIn;}

  vfrEvent& Click() {return evClick;}
  vfrEvent& DragStart() {return evDragStart;}
  vfrEvent& DragEnd() {return evDragEnd;}
  vfrEvent& Drag() {return evDrag;}
  vfrEvent& SClick() {return evSClick;}
  vfrEvent& SDragStart() {return evSDragStart;}
  vfrEvent& SDragEnd() {return evSDragEnd;}
  vfrEvent& SDrag() {return evSDrag;}
  vfrEvent& CClick() {return evCClick;}
  vfrEvent& CDragStart() {return evCDragStart;}
  vfrEvent& CDragEnd() {return evCDragEnd;}
  vfrEvent& CDrag() {return evCDrag;}
  vfrEvent& SCClick() {return evSCClick;}
  vfrEvent& SCDragStart() {return evSCDragStart;}
  vfrEvent& SCDragEnd() {return evSCDragEnd;}
  vfrEvent& SCDrag() {return evSCDrag;}

  vfrEvent& RClick() {return evRClick;}
  vfrEvent& RDragStart() {return evRDragStart;}
  vfrEvent& RDragEnd() {return evRDragEnd;}
  vfrEvent& RDrag() {return evRDrag;}
  vfrEvent& RSClick() {return evRSClick;}
  vfrEvent& RSDragStart() {return evRSDragStart;}
  vfrEvent& RSDragEnd() {return evRSDragEnd;}
  vfrEvent& RSDrag() {return evRSDrag;}
  vfrEvent& RCClick() {return evRCClick;}
  vfrEvent& RCDragStart() {return evRCDragStart;}
  vfrEvent& RCDragEnd() {return evRCDragEnd;}
  vfrEvent& RCDrag() {return evRCDrag;}
  vfrEvent& RSCClick() {return evRSCClick;}
  vfrEvent& RSCDragStart() {return evRSCDragStart;}
  vfrEvent& RSCDragEnd() {return evRSCDragEnd;}
  vfrEvent& RSCDrag() {return evRSCDrag;}

  vfrEvent& MClick() {return evMClick;}
  vfrEvent& MDragStart() {return evMDragStart;}
  vfrEvent& MDragEnd() {return evMDragEnd;}
  vfrEvent& MDrag() {return evMDrag;}
  vfrEvent& MSClick() {return evMSClick;}
  vfrEvent& MSDragStart() {return evMSDragStart;}
  vfrEvent& MSDragEnd() {return evMSDragEnd;}
  vfrEvent& MSDrag() {return evMSDrag;}
  vfrEvent& MCClick() {return evMCClick;}
  vfrEvent& MCDragStart() {return evMCDragStart;}
  vfrEvent& MCDragEnd() {return evMCDragEnd;}
  vfrEvent& MCDrag() {return evMCDrag;}
  vfrEvent& MSCClick() {return evMSCClick;}
  vfrEvent& MSCDragStart() {return evMSCDragStart;}
  vfrEvent& MSCDragEnd() {return evMSCDragEnd;}
  vfrEvent& MSCDrag() {return evMSCDrag;}

  vfrEvent& Wheel() {return evWheel;}

protected:
  vfrDispatch();
  virtual ~vfrDispatch();

  class vfrEvKeyIn       evKeyIn;

  class vfrEvClick       evClick;
  class vfrEvDragStart   evDragStart;
  class vfrEvDragEnd     evDragEnd;
  class vfrEvDrag        evDrag;
  class vfrEvSClick      evSClick;
  class vfrEvSDragStart  evSDragStart;
  class vfrEvSDragEnd    evSDragEnd;
  class vfrEvSDrag       evSDrag;
  class vfrEvCClick      evCClick;
  class vfrEvCDragStart  evCDragStart;
  class vfrEvCDragEnd    evCDragEnd;
  class vfrEvCDrag       evCDrag;
  class vfrEvSCClick     evSCClick;
  class vfrEvSCDragStart evSCDragStart;
  class vfrEvSCDragEnd   evSCDragEnd;
  class vfrEvSCDrag      evSCDrag;

  class vfrEvRClick       evRClick;
  class vfrEvRDragStart   evRDragStart;
  class vfrEvRDragEnd     evRDragEnd;
  class vfrEvRDrag        evRDrag;
  class vfrEvRSClick      evRSClick;
  class vfrEvRSDragStart  evRSDragStart;
  class vfrEvRSDragEnd    evRSDragEnd;
  class vfrEvRSDrag       evRSDrag;
  class vfrEvRCClick      evRCClick;
  class vfrEvRCDragStart  evRCDragStart;
  class vfrEvRCDragEnd    evRCDragEnd;
  class vfrEvRCDrag       evRCDrag;
  class vfrEvRSCClick     evRSCClick;
  class vfrEvRSCDragStart evRSCDragStart;
  class vfrEvRSCDragEnd   evRSCDragEnd;
  class vfrEvRSCDrag      evRSCDrag;

  class vfrEvMClick       evMClick;
  class vfrEvMDragStart   evMDragStart;
  class vfrEvMDragEnd     evMDragEnd;
  class vfrEvMDrag        evMDrag;
  class vfrEvMSClick      evMSClick;
  class vfrEvMSDragStart  evMSDragStart;
  class vfrEvMSDragEnd    evMSDragEnd;
  class vfrEvMSDrag       evMSDrag;
  class vfrEvMCClick      evMCClick;
  class vfrEvMCDragStart  evMCDragStart;
  class vfrEvMCDragEnd    evMCDragEnd;
  class vfrEvMCDrag       evMCDrag;
  class vfrEvMSCClick     evMSCClick;
  class vfrEvMSCDragStart evMSCDragStart;
  class vfrEvMSCDragEnd   evMSCDragEnd;
  class vfrEvMSCDrag      evMSCDrag;

  class vfrEvWheel        evWheel;

  friend class vfrEvent;
  friend class vfrDrawArea;
  friend class vfrDrawAreaSDL;
  friend class vfrDrawAreaGTK;
  friend class vfrDrawAreaMTF;
  friend class vfrDrawAreaMFC;
};

#endif /* _VFR_DISPATCH_H_ */
