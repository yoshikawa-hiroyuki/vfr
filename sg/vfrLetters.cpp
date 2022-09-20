//
// vfrLetters
//   Basic Text Object Class
//

#include "vfrLetters.h"


vfrLetters::vfrLetters(const std::string& nm, const Bool ssm)
  : vfrNode(nm, ssm) {
  resetLetters();
}

vfrLetters::~vfrLetters() {
  resetLetters();
}

void
vfrLetters::resetLetters() {
  _textBuf = std::string("");
  setFontAlign(AL_LEFT);
  setFontScale(1.0f);
  setSpaceRate(0.1f);
}

void
vfrLetters::setLetters(const std::string& s) {
  if ( _textBuf == s ) return;
  _textBuf = s;
  generateBbox();
  notice();
}

void
vfrLetters::setLetters(char* fmt, const int value) {
  char* format, buff[64];
  char* failSafe = "%d";

  if ( ! fmt )
    format = failSafe;
  else
    format = fmt;
  if ( sprintf(buff, format, value) >= 64 )
    buff[63] = '\0';

  _textBuf = buff;
  generateBbox();
  notice();
}

void
vfrLetters::setLetters(char* fmt, const float value) {
  char* format, buff[64];
  char* failSafe = "%f";

  if ( ! fmt )
    format = failSafe;
  else
    format = fmt;
  if ( sprintf(buff, format, value) >= 64 )
    buff[63] = '\0';

  _textBuf = buff;
  generateBbox();
  notice();
}

void
vfrLetters::setFontAlign(const AlignType at) {
  if ( _alignType == at ) return;
  _alignType = at;
  generateBbox();
  notice();
}

void
vfrLetters::setFontScale(const float si) {
  if ( _fontScale == si ) return;
  _fontScale = si > EPSF ? si : EPSF;
  generateBbox();
  notice();
}

int
vfrLetters::getNumLines() const {
  register int nl = 1;
  register size_t i, textLen = _textBuf.size();
  for ( i = 0; i < textLen; i++ ) {
    if ( _textBuf[i] == '\n' )
      nl++;
  }
  return nl;
}

void
vfrLetters::generateBbox() {
  float tw = getTextWidth();
  _bbox[0][0] = 0.f;
  _bbox[1][0] = tw * _fontScale;
  if ( _alignType == AL_CENTER ) {
    _bbox[0][0] -= 0.5f * tw * _fontScale;
    _bbox[1][0] -= 0.5f * tw * _fontScale;
  } else if ( _alignType == AL_RIGHT ) {
    _bbox[0][0] -= tw * _fontScale;
    _bbox[1][0] -= tw * _fontScale;
  }

  _bbox[0][1] = -getTextHeight() * _fontScale * (getNumLines() -1);
  _bbox[1][1] = getTextHeight() * _fontScale;

  _bbox[0][2] = _bbox[1][2] = 0.f;
}

void
vfrLetters::renderSolid() {
  // display-list check
  if ( beginDispList(DLF_SOLID) ) return;

  // draw
  glDisable(GL_LIGHTING);
  glNormal3f(0.f, 0.f, 1.f);
  glPushMatrix();
  glScalef(_fontScale, _fontScale, _fontScale);

  GLfloat tw = (_bbox[1][0] - _bbox[0][0]) / _fontScale;
  if ( _alignType == AL_RIGHT )
    glTranslatef(-tw, 0.0f, 0.0f);
  else if ( _alignType == AL_CENTER )
    glTranslatef(-0.5f*tw, 0.0f, 0.0f);

  GLfloat transltd = 0.f;
  for ( register int i = 0; i < _textBuf.size(); i++ ) {
    if ( _textBuf[i] == '\n' ) {
      glTranslatef(-transltd, -getTextHeight(), 0.f);
      transltd = 0.f;
    } else
      transltd += drawLetter(_textBuf[i]);
  } // end of for(i)

  glPopMatrix();
  glEnable(GL_LIGHTING);

  // end display-list definition
  endDispList(DLF_SOLID);
}

void
vfrLetters::renderWire() {
  renderSolid();
}

// EOF
