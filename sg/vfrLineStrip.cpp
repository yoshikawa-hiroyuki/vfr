//
// vfrLineStrip
//   Line Strip Class
//
#include "vfrLineStrip.h"


vfrLineStrip::vfrLineStrip(const std::string& nm, const Bool ssm)
  : vfrNode(nm, ssm) {
  reset();
}

vfrLineStrip::~vfrLineStrip() {
}


void
vfrLineStrip::reset() {
  setLoopMode(FALSE);
}

RenderType
vfrLineStrip::getRenderMode() const {
  const vfrMaterial* mp = getMaterial();
  if ( ! mp )
    return RT_WIRE;
  RenderType prt = mp->getRenderMode();
  if ( prt == RT_NONE || prt == RT_POINT )
    return prt;
  return RT_WIRE;
}

void
vfrLineStrip::renderFeedBack(const unsigned int oid) {
  if ( _currentSeq != oid )
    return;

  applyMatrix();

  glDisable(GL_LIGHTING);

  switch ( _feedbackMode ) {
  case FB_VERTEX:
    {
    for ( register int i = 0; i < nVerts; i++ ) {
      glPassThrough((GLfloat)i);
      glBegin(GL_POINTS);
      glVertex3fv(_verts[i]);
      glEnd();
    }
    break;
    }
  case FB_EDGE:
    {
    for ( register int i = 0; i < nVerts-1; i++ ) {
      glPassThrough((GLfloat)i);
      glBegin(GL_LINES);
      glVertex3fv(_verts[i]);
      glVertex3fv(_verts[i+1]);
      glEnd();
    }
    if ( _loopMode ) {
      glPassThrough((GLfloat)(nVerts-1));
      glBegin(GL_LINES);
      glVertex3fv(_verts[nVerts-1]);
      glVertex3fv(_verts[0]);
      glEnd();
    }
    break;
    }
  case FB_FACE:
  default:
    break;    // do nothing.
  }
  glEnable(GL_LIGHTING);
  unApplyMatrix();
}

void
vfrLineStrip::renderWire() {
  // display-list check
  if ( beginDispList(DLF_WIRE) ) return;

  Bool auxMode = useAuxLineColor();
  register int index = 0;
  if ( _loopMode )
    glBegin(GL_LINE_LOOP);
  else
    glBegin(GL_LINE_STRIP);
  while ( index < nVerts ) {
    if ( ! auxMode &&
        (_colorMode == AT_PER_VERTEX || _colorMode == AT_PER_FACE) )
      if ( index < nColors )
        glColor4fv(_colors[index]);
    glVertex3fv(_verts[index++]);
  } // end of while(index)
  glEnd();

  // end display-list definition
  endDispList(DLF_WIRE);
}

// EOF
