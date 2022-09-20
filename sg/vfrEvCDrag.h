//
// vfrEvCDragStart
//   Cntl+Drag Event Class
//
#ifndef _VFR_EVENT_CDRAG_H_
#define _VFR_EVENT_CDRAG_H_

#include "vfrEvent.h"

class EXPORTED_CLASS vfrEvCDragStart : public vfrEvent {
  public:
    vfrEvCDragStart() {}
    virtual ~vfrEvCDragStart() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragStartEvent() const {return TRUE;}
    Bool withLeftButton() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvCDrag : public vfrEvent {
  public:
    vfrEvCDrag() {}
    virtual ~vfrEvCDrag() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEvent() const {return TRUE;}
    Bool withLeftButton() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvCDragEnd : public vfrEvent {
  public:
    vfrEvCDragEnd() {}
    virtual ~vfrEvCDragEnd() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEndEvent() const {return TRUE;}
    Bool withLeftButton() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};


class EXPORTED_CLASS vfrEvRCDragStart : public vfrEvent {
  public:
    vfrEvRCDragStart() {}
    virtual ~vfrEvRCDragStart() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragStartEvent() const {return TRUE;}
    Bool withRightButton() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvRCDrag : public vfrEvent {
  public:
    vfrEvRCDrag() {}
    virtual ~vfrEvRCDrag() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEvent() const {return TRUE;}
    Bool withRightButton() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvRCDragEnd : public vfrEvent {
  public:
    vfrEvRCDragEnd() {}
    virtual ~vfrEvRCDragEnd() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEndEvent() const {return TRUE;}
    Bool withRightButton() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};


class EXPORTED_CLASS vfrEvMCDragStart : public vfrEvent {
  public:
    vfrEvMCDragStart() {}
    virtual ~vfrEvMCDragStart() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragStartEvent() const {return TRUE;}
    Bool withMiddleButton() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvMCDrag : public vfrEvent {
  public:
    vfrEvMCDrag() {}
    virtual ~vfrEvMCDrag() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEvent() const {return TRUE;}
    Bool withMiddleButton() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvMCDragEnd : public vfrEvent {
  public:
    vfrEvMCDragEnd() {}
    virtual ~vfrEvMCDragEnd() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEndEvent() const {return TRUE;}
    Bool withMiddleButton() const {return TRUE;}
    Bool withControlKey() const {return TRUE;}
};

#endif /* _VFR_EVENT_CDRAG_H_ */
