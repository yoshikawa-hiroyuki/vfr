//
// vfrIndexLines
//   Indexed Line Strips Class
//

#include "vfrIndexLines.h"


vfrIndexLines::vfrIndexLines(const std::string& nm, const Bool ssm)
  : vfrNode(nm, ssm) {
  setLoopMode(FALSE);
}

vfrIndexLines::~vfrIndexLines() {
}


RenderType
vfrIndexLines::getRenderMode() const {
  const vfrMaterial* mp = getMaterial();
  if ( ! mp )
    return RT_WIRE;
  RenderType prt = mp->getRenderMode();
  if ( prt == RT_NONE || prt == RT_POINT )
    return prt;
  return RT_WIRE;
}

void
vfrIndexLines::renderFeedBack(const unsigned int oid) {
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
    register int index = 0;
    register int findex = 0;
    register int fallback = 0;
    while ( index < nIndices -1 ) {
      if ( _indices[index] == VFR_END_OF_ELEM ) {
        if ( _indices[index+1] != VFR_END_OF_ELEM )
          fallback = _indices[index+1];
        index++;
        continue;
      }
      if ( _indices[index+1] == VFR_END_OF_ELEM ) {
        if ( _loopMode && fallback >= 0 ) {
          glPassThrough((GLfloat)findex);
          glBegin(GL_LINES);
          glVertex3fv(_verts[_indices[index]]);
          glVertex3fv(_verts[fallback]);
          glEnd();
          fallback = -1;
          findex++;
        }
        index++;
        continue;
      }
      if ( _indices[index] < nVerts && _indices[index+1] < nVerts ) {
        glPassThrough((GLfloat)findex);
        glBegin(GL_LINES);
        glVertex3fv(_verts[_indices[index]]);
        glVertex3fv(_verts[_indices[index+1]]);
        glEnd();
        findex++;
      }
      index++;
    }
    if ( _indices[nIndices -1] != VFR_END_OF_ELEM ) {
      if ( _loopMode && fallback >= 0 ) {
        glPassThrough((GLfloat)findex);
        glBegin(GL_LINES);
        glVertex3fv(_verts[_indices[nIndices -1]]);
        glVertex3fv(_verts[fallback]);
        glEnd();
      }
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
vfrIndexLines::renderWire() {
  // display-list check
  if ( beginDispList(DLF_WIRE) ) return;

  Bool auxMode = useAuxLineColor();
  register int index = 0;
  register int findex = 0;
  while ( index < nIndices ) {
    if ( ! auxMode && _colorMode == AT_PER_FACE )
      if ( findex < nColors )
        glColor4fv(_colors[findex]);
    if ( _loopMode )
      glBegin(GL_LINE_LOOP);
    else
      glBegin(GL_LINE_STRIP);
    while ( _indices[index] != VFR_END_OF_ELEM && index < nIndices ) {
      if ( ! auxMode && _colorMode == AT_PER_VERTEX )
        if ( _indices[index] < nColors )
          glColor4fv(_colors[_indices[index]]);

      if ( _indices[index] < nVerts )
        glVertex3fv(_verts[_indices[index]]);

      index++;
    }
    glEnd();
    index++;
    findex++;
  } // end of while(index)

  // end display-list definition
  endDispList(DLF_WIRE);
}

// EOF
