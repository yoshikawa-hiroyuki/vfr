//
// vfrEvDragStart
//   Drag Event Class
//
#ifndef _VFR_EVENT_DRAG_H_
#define _VFR_EVENT_DRAG_H_

#include "vfrEvent.h"

class EXPORTED_CLASS vfrEvDragStart : public vfrEvent {
  public:
    vfrEvDragStart() {}
    virtual ~vfrEvDragStart() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragStartEvent() const {return TRUE;}
    Bool withLeftButton() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvDrag : public vfrEvent {
  public:
    vfrEvDrag() {}
    virtual ~vfrEvDrag() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEvent() const {return TRUE;}
    Bool withLeftButton() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvDragEnd : public vfrEvent {
  public:
    vfrEvDragEnd() {}
    virtual ~vfrEvDragEnd() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEndEvent() const {return TRUE;}
    Bool withLeftButton() const {return TRUE;}
};


class EXPORTED_CLASS vfrEvRDragStart : public vfrEvent {
  public:
    vfrEvRDragStart() {}
    virtual ~vfrEvRDragStart() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragStartEvent() const {return TRUE;}
    Bool withRightButton() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvRDrag : public vfrEvent {
  public:
    vfrEvRDrag() {}
    virtual ~vfrEvRDrag() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEvent() const {return TRUE;}
    Bool withRightButton() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvRDragEnd : public vfrEvent {
  public:
    vfrEvRDragEnd() {}
    virtual ~vfrEvRDragEnd() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEndEvent() const {return TRUE;}
    Bool withRightButton() const {return TRUE;}
};


class EXPORTED_CLASS vfrEvMDragStart : public vfrEvent {
  public:
    vfrEvMDragStart() {}
    virtual ~vfrEvMDragStart() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragStartEvent() const {return TRUE;}
    Bool withMiddleButton() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvMDrag : public vfrEvent {
  public:
    vfrEvMDrag() {}
    virtual ~vfrEvMDrag() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEvent() const {return TRUE;}
    Bool withMiddleButton() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvMDragEnd : public vfrEvent {
  public:
    vfrEvMDragEnd() {}
    virtual ~vfrEvMDragEnd() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEndEvent() const {return TRUE;}
    Bool withMiddleButton() const {return TRUE;}
};

#endif /* _VFR_EVENT_DRAG_H_ */
