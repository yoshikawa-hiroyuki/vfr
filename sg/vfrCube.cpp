//
// vfrCube
//   Cube Object Class
//

#include "vfrCube.h"

vfrCube::vfrCube(const std::string& nm, const Bool ssm)
  : vfrNode(nm, ssm) {
  makeCube(1.0f, 1.0f, 1.0f);
}

vfrCube::vfrCube(const float w, const float h, const float d,
		 const std::string& nm, const Bool ssm)
  : vfrNode(nm, ssm) {
  makeCube(w, h, d);
}

void
vfrCube::makeCube(const float w, const float h, const float d) {
  setWidth(w);
  setHeight(h);
  setDepth(d);
  vector3 v0[1];
  v0[0][0] = v0[0][1] = v0[0][2] = 0.0f;
  setVerts(1, v0);
  generateBbox();
  notice();
}

vfrCube::~vfrCube() {
}

void
vfrCube::setWidth(const float w) {
  if ( _width == w ) return;
  _width = w > EPSF ? w : EPSF;
  generateBbox();
  notice();
}    

void
vfrCube::setHeight(const float h) {
  if ( _height == h ) return;
  _height = h > EPSF ? h : EPSF;
  generateBbox();
  notice();
}

void
vfrCube::setDepth(const float d) {
  if ( _depth == d ) return;
  _depth = d > EPSF ? d : EPSF;
  generateBbox();
  notice();
}

void
vfrCube::generateBbox() {
  _bbox[0][0] = -_width*0.5f;
  _bbox[1][0] =  _width*0.5f;
  _bbox[0][1] = -_height*0.5f;
  _bbox[1][1] =  _height*0.5f;
  _bbox[0][2] = -_depth*0.5f;
  _bbox[1][2] =  _depth*0.5f;
}

void
vfrCube::renderSolid() {
  // display-list check
  if ( beginDispList(DLF_SOLID) ) return;

  Bool cylTex = FALSE;
  if ( _texture && _texture->getMapType() == vfrTexture::UVC )
    cylTex = TRUE;

  glNormal3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_POLYGON);
  if ( cylTex ) glTexCoord2f(1.0f, 0.0f);
  glVertex3f( 0.5f*_width, -0.5f*_height, -0.5f*_depth);
  if ( cylTex ) glTexCoord2f(1.0f, 1.0f);
  glVertex3f( 0.5f*_width,  0.5f*_height, -0.5f*_depth);
  if ( cylTex ) glTexCoord2f(0.0f, 1.0f);
  glVertex3f( 0.5f*_width,  0.5f*_height,  0.5f*_depth);
  if ( cylTex ) glTexCoord2f(0.0f, 0.0f);
  glVertex3f( 0.5f*_width, -0.5f*_height,  0.5f*_depth);
  glEnd();

  glNormal3f(-1.0f, 0.0f, 0.0f);
  glBegin(GL_POLYGON);
  if ( cylTex ) glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.5f*_width, -0.5f*_height, -0.5f*_depth);
  if ( cylTex ) glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-0.5f*_width, -0.5f*_height,  0.5f*_depth);
  if ( cylTex ) glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-0.5f*_width,  0.5f*_height,  0.5f*_depth);
  if ( cylTex ) glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.5f*_width,  0.5f*_height, -0.5f*_depth);
  glEnd();

  glNormal3f(0.0f, 1.0f, 0.0f);
  glBegin(GL_POLYGON);
  if ( cylTex ) glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.5f*_width,  0.5f*_height, -0.5f*_depth);
  if ( cylTex ) glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.5f*_width,  0.5f*_height,  0.5f*_depth);
  if ( cylTex ) glTexCoord2f(1.0f, 0.0f);
  glVertex3f( 0.5f*_width,  0.5f*_height,  0.5f*_depth);
  if ( cylTex ) glTexCoord2f(1.0f, 1.0f);
  glVertex3f( 0.5f*_width,  0.5f*_height, -0.5f*_depth);
  glEnd();

  glNormal3f(0.0f, -1.0f, 0.0f);
  glBegin(GL_POLYGON);
  if ( cylTex ) glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.5f*_width, -0.5f*_height, -0.5f*_depth);
  if ( cylTex ) glTexCoord2f(1.0f, 0.0f);
  glVertex3f( 0.5f*_width, -0.5f*_height, -0.5f*_depth);
  if ( cylTex ) glTexCoord2f(1.0f, 1.0f);
  glVertex3f( 0.5f*_width, -0.5f*_height,  0.5f*_depth);
  if ( cylTex ) glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.5f*_width, -0.5f*_height,  0.5f*_depth);
  glEnd();

  glNormal3f(0.0f, 0.0f, 1.0f);
  glBegin(GL_POLYGON);
  if ( cylTex ) glTexCoord2f(1.0f, 1.0f);
  glVertex3f( 0.5f*_width,  0.5f*_height,  0.5f*_depth);
  if ( cylTex ) glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-0.5f*_width,  0.5f*_height,  0.5f*_depth);
  if ( cylTex ) glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-0.5f*_width, -0.5f*_height,  0.5f*_depth);
  if ( cylTex ) glTexCoord2f(1.0f, 0.0f);
  glVertex3f( 0.5f*_width, -0.5f*_height,  0.5f*_depth);
  glEnd();

  glNormal3f(0.0f, 0.0f, -1.0f);
  glBegin(GL_POLYGON);
  if ( cylTex ) glTexCoord2f(0.0f, 0.0f);
  glVertex3f( 0.5f*_width, -0.5f*_height, -0.5f*_depth);
  if ( cylTex ) glTexCoord2f(1.0f, 0.0f);
  glVertex3f(-0.5f*_width, -0.5f*_height, -0.5f*_depth);
  if ( cylTex ) glTexCoord2f(1.0f, 1.0f);
  glVertex3f(-0.5f*_width,  0.5f*_height, -0.5f*_depth);
  if ( cylTex ) glTexCoord2f(0.0f, 1.0f);
  glVertex3f( 0.5f*_width,  0.5f*_height, -0.5f*_depth);
  glEnd();

  // end display-list definition
  endDispList(DLF_SOLID);
}

void
vfrCube::renderWire() {
  // display-list check
  if ( beginDispList(DLF_WIRE) ) return;

#if 0
  glBegin(GL_LINE_LOOP);
  glVertex3f( 0.5f*_width, -0.5f*_height, -0.5f*_depth);
  glVertex3f( 0.5f*_width,  0.5f*_height, -0.5f*_depth);
  glVertex3f( 0.5f*_width,  0.5f*_height,  0.5f*_depth);
  glVertex3f( 0.5f*_width, -0.5f*_height,  0.5f*_depth);
  glEnd();

  glBegin(GL_LINE_LOOP);
  glVertex3f(-0.5f*_width, -0.5f*_height, -0.5f*_depth);
  glVertex3f(-0.5f*_width, -0.5f*_height,  0.5f*_depth);
  glVertex3f(-0.5f*_width,  0.5f*_height,  0.5f*_depth);
  glVertex3f(-0.5f*_width,  0.5f*_height, -0.5f*_depth);
  glEnd();
#else
  glBegin(GL_LINE_STRIP);
  glVertex3f( 0.5f*_width, -0.5f*_height, -0.5f*_depth);
  glVertex3f( 0.5f*_width,  0.5f*_height, -0.5f*_depth);
  glVertex3f( 0.5f*_width,  0.5f*_height,  0.5f*_depth);
  glVertex3f( 0.5f*_width, -0.5f*_height,  0.5f*_depth);
  glVertex3f( 0.5f*_width, -0.5f*_height, -0.5f*_depth);
  glEnd();

  glBegin(GL_LINE_STRIP);
  glVertex3f(-0.5f*_width, -0.5f*_height, -0.5f*_depth);
  glVertex3f(-0.5f*_width, -0.5f*_height,  0.5f*_depth);
  glVertex3f(-0.5f*_width,  0.5f*_height,  0.5f*_depth);
  glVertex3f(-0.5f*_width,  0.5f*_height, -0.5f*_depth);
  glVertex3f(-0.5f*_width, -0.5f*_height, -0.5f*_depth);
  glEnd();
#endif

  glBegin(GL_LINES);
  glVertex3f( 0.5f*_width, -0.5f*_height, -0.5f*_depth);
  glVertex3f(-0.5f*_width, -0.5f*_height, -0.5f*_depth);

  glVertex3f( 0.5f*_width,  0.5f*_height, -0.5f*_depth);
  glVertex3f(-0.5f*_width,  0.5f*_height, -0.5f*_depth);

  glVertex3f( 0.5f*_width,  0.5f*_height,  0.5f*_depth);
  glVertex3f(-0.5f*_width,  0.5f*_height,  0.5f*_depth);

  glVertex3f( 0.5f*_width, -0.5f*_height,  0.5f*_depth);
  glVertex3f(-0.5f*_width, -0.5f*_height,  0.5f*_depth);
  glEnd();

  // end display-list definition
  endDispList(DLF_WIRE);
}

// EOF
