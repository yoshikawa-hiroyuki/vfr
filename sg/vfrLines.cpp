//
// vfrLines
//   Line Segments Class
//
#include "vfrLines.h"


vfrLines::vfrLines(const std::string& nm, const Bool ssm)
  : vfrNode(nm, ssm) {
  reset();
}

vfrLines::~vfrLines() {
}


void
vfrLines::reset() {
}

RenderType
vfrLines::getRenderMode() const {
  const vfrMaterial* mp = getMaterial();
  if ( ! mp )
    return RT_WIRE;
  RenderType prt = mp->getRenderMode();
  if ( prt == RT_NONE || prt == RT_POINT )
    return prt;
  return RT_WIRE;
}

void
vfrLines::renderFeedBack(const unsigned int oid) {
  if ( _currentSeq != oid )
    return;

  applyMatrix();

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
    register int index = 0;
    register int findex = 0;
    while ( index < nVerts -1 ) {
      glPassThrough((GLfloat)findex);
      glBegin(GL_LINES);
      glVertex3fv(_verts[index++]);
      glVertex3fv(_verts[index++]);
      glEnd();
      findex++;
    }
    break;
    }
  case FB_FACE:
  default:
    break;    // do nothing.
  }

  unApplyMatrix();
}

void
vfrLines::renderWire() {
  // display-list check
  if ( beginDispList(DLF_WIRE) ) return;

  Bool auxMode = useAuxLineColor();
  register int index = 0;
  register int findex = 0;
  while ( index < nVerts -1 ) {
    if ( ! auxMode && _colorMode == AT_PER_FACE )
      if ( findex < nColors )
        glColor4fv(_colors[findex]);
    glBegin(GL_LINES);
    if ( ! auxMode && _colorMode == AT_PER_VERTEX )
      if ( index < nColors )
        glColor4fv(_colors[index]);
    glVertex3fv(_verts[index++]);
    if ( ! auxMode && _colorMode == AT_PER_VERTEX )
      if ( index < nColors )
        glColor4fv(_colors[index]);
    glVertex3fv(_verts[index++]);
    glEnd();
    findex++;
  } // end of while(index)

  // end display-list definition
  endDispList(DLF_WIRE);
}

// EOF
