//
// vfrEvSDragStart
//   Shift+Drag Event Class
//
#ifndef _VFR_EVENT_SDRAG_H_
#define _VFR_EVENT_SDRAG_H_

#include "vfrEvent.h"

class EXPORTED_CLASS vfrEvSDragStart : public vfrEvent {
  public:
    vfrEvSDragStart() {}
    virtual ~vfrEvSDragStart() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragStartEvent() const {return TRUE;}
    Bool withLeftButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvSDrag : public vfrEvent {
  public:
    vfrEvSDrag() {}
    virtual ~vfrEvSDrag() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEvent() const {return TRUE;}
    Bool withLeftButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvSDragEnd : public vfrEvent {
  public:
    vfrEvSDragEnd() {}
    virtual ~vfrEvSDragEnd() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEndEvent() const {return TRUE;}
    Bool withLeftButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
};


class EXPORTED_CLASS vfrEvRSDragStart : public vfrEvent {
  public:
    vfrEvRSDragStart() {}
    virtual ~vfrEvRSDragStart() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragStartEvent() const {return TRUE;}
    Bool withRightButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvRSDrag : public vfrEvent {
  public:
    vfrEvRSDrag() {}
    virtual ~vfrEvRSDrag() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEvent() const {return TRUE;}
    Bool withRightButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvRSDragEnd : public vfrEvent {
  public:
    vfrEvRSDragEnd() {}
    virtual ~vfrEvRSDragEnd() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEndEvent() const {return TRUE;}
    Bool withRightButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
};


class EXPORTED_CLASS vfrEvMSDragStart : public vfrEvent {
  public:
    vfrEvMSDragStart() {}
    virtual ~vfrEvMSDragStart() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragStartEvent() const {return TRUE;}
    Bool withMiddleButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvMSDrag : public vfrEvent {
  public:
    vfrEvMSDrag() {}
    virtual ~vfrEvMSDrag() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEvent() const {return TRUE;}
    Bool withMiddleButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
};

class EXPORTED_CLASS vfrEvMSDragEnd : public vfrEvent {
  public:
    vfrEvMSDragEnd() {}
    virtual ~vfrEvMSDragEnd() {}
    static vfrEvent& instance(class vfrDispatch&);

    Bool isDragEndEvent() const {return TRUE;}
    Bool withMiddleButton() const {return TRUE;}
    Bool withShiftKey() const {return TRUE;}
};

#endif /* _VFR_EVENT_SDRAG_H_ */
