//
// vfrEvSCDrag
//    Shift+Cntl+Drag Event Class
//
#ifndef _VFR_EVENT_SCDRAG_H_
#define _VFR_EVENT_SCDRAG_H_

#include "vfrEvent.h"

class EXPORTED_CLASS vfrEvSCDragStart : public vfrEvent {
  public:
    vfrEvSCDragStart() {}
    virtual ~vfrEvSCDragStart() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragStartEvent() const {return TRUE;}
    Bool withLeftButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvSCDrag : public vfrEvent {
  public:
    vfrEvSCDrag() {}
    virtual ~vfrEvSCDrag() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEvent() const {return TRUE;}
    Bool withLeftButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvSCDragEnd : public vfrEvent {
  public:
    vfrEvSCDragEnd() {}
    virtual ~vfrEvSCDragEnd() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEndEvent() const {return TRUE;}
    Bool withLeftButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};


class EXPORTED_CLASS vfrEvRSCDragStart : public vfrEvent {
  public:
    vfrEvRSCDragStart() {}
    virtual ~vfrEvRSCDragStart() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragStartEvent() const {return TRUE;}
    Bool withRightButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvRSCDrag : public vfrEvent {
  public:
    vfrEvRSCDrag() {}
    virtual ~vfrEvRSCDrag() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEvent() const {return TRUE;}
    Bool withRightButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvRSCDragEnd : public vfrEvent {
  public:
    vfrEvRSCDragEnd() {}
    virtual ~vfrEvRSCDragEnd() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEndEvent() const {return TRUE;}
    Bool withRightButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};


class EXPORTED_CLASS vfrEvMSCDragStart : public vfrEvent {
  public:
    vfrEvMSCDragStart() {}
    virtual ~vfrEvMSCDragStart() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragStartEvent() const {return TRUE;}
    Bool withMiddleButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvMSCDrag : public vfrEvent {
  public:
    vfrEvMSCDrag() {}
    virtual ~vfrEvMSCDrag() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEvent() const {return TRUE;}
    Bool withMiddleButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvMSCDragEnd : public vfrEvent {
  public:
    vfrEvMSCDragEnd() {}
    virtual ~vfrEvMSCDragEnd() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEndEvent() const {return TRUE;}
    Bool withMiddleButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};

#endif /* _VFR_EVENT_SCDRAG_H_ */
