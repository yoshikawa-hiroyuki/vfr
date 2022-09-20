//
// vfrTexture
//   Texture Data class
//

#include "vfrTexture.h"
#include "vfrNode.h"

static GLfloat _pX0[] = {1.0f, 0.0f, 0.0f, 0.0f};
static GLfloat _pY0[] = {0.0f, 1.0f, 0.0f, 0.0f};
static GLfloat _pZ0[] = {0.0f, 0.0f, 1.0f, 0.0f};


vfrTexture::vfrTexture(const CES::Vec3<float>* bb, const Bool sm)
  : vfrBase(sm),_image(),_mapType(XYP),_clipType(REPEAT),
    _uvc(NULL),_num_uvc(0),_hasBbox(FALSE),_texUpdated(FALSE)
{
  if ( bb ) setBbox(bb);
}

vfrTexture::vfrTexture(const vfrImage& img,
		       const CES::Vec3<float>* bb, const Bool sm)
  : vfrBase(sm),_image(img),_mapType(XYP),_clipType(REPEAT),
    _uvc(NULL),_num_uvc(0),_hasBbox(FALSE),_texUpdated(FALSE)
{
  if ( bb ) setBbox(bb);
}

vfrTexture::vfrTexture(const vfrTexture& org,
		       const CES::Vec3<float>* bb, const Bool sm)
  : vfrBase(sm),_image(),_mapType(XYP),_clipType(REPEAT),
    _uvc(NULL),_num_uvc(0),_hasBbox(FALSE),_texUpdated(FALSE)
{
  *this = org;
  if ( bb ) setBbox(bb);
}

vfrTexture::~vfrTexture() {
  if ( _uvc ) {
    DeAllocate(_uvc);
    _uvc = NULL;
    _num_uvc = 0;
  }

  clearDispList();
}

void
vfrTexture::operator=(const vfrTexture& org) {
  _image = org._image;
  _matrix = org._matrix;
  _mapType = org._mapType;
  _clipType = org._clipType;

  if ( org._uvc && org._num_uvc > 0 ) {
    if ( alcUVCs(org._num_uvc) )
      memcpy(_uvc, org._uvc, sizeof(vector2)*_num_uvc);
  }
  else if ( _uvc ) {
    DeAllocate(_uvc);
    _uvc = NULL;
    _num_uvc = 0;
  }

  if ( org._hasBbox ) {
    setBbox(org._bbox);
  } else {
    _bbox[0][0] = _bbox[0][1] = _bbox[0][2] = 0.0f;
    _bbox[1][0] = _bbox[1][1] = _bbox[1][2] = 0.0f;
  }
}

Bool
vfrTexture::alcUVCs(const int n) {
  if ( _num_uvc < n ) {
    _uvc = (vector2*)ReAllocate(_uvc, sizeof(vector2)*n);
    if ( ! _uvc ) {
      _num_uvc = 0;
      return FALSE;
    }
  }
  for ( register int i = _num_uvc; i < n; i++ ) {
    _uvc[i][0] = 0.0f;
    _uvc[i][1] = 0.0f;
  }
  _num_uvc = n;
  return TRUE;
}

Bool
vfrTexture::setUVCs(const int n, const vector2* pv) {
  if ( _num_uvc < n ) {
    if ( ! alcUVCs(n) )
      return FALSE;
  }
  for ( register int i = 0; i < n; i++ ) {
    _uvc[i][0] = pv[i][0];
    _uvc[i][1] = pv[i][1];
  }
  _num_uvc = n;
  notice();
  return TRUE;
}

Bool
vfrTexture::setUVC(const int n, const vector2 v) {
  if ( n >= _num_uvc )
    return FALSE;
  _uvc[n][0] = v[0];
  _uvc[n][1] = v[1];
  notice();
  return TRUE;
}

void
vfrTexture::clearDispList() {
  _texObj.DelObj();
}

void
vfrTexture::notice() {
  _texUpdated = FALSE;
  vfrBase::notice();
}

void
vfrTexture::apply() const {
  const CES::Vec3<float>* bbox = getBbox();
  if ( ! bbox ) return;
  const unsigned char* dp = _image.getConstData();
  if ( ! dp ) return;

  float spanS = 1.0f, spanT = 1.0f;
  vector3 tV = {0.0f, 0.0f, 0.0f};

  // set texture parameters
  _texObj.Enable();
  if ( ! _texObj.GenObj() ) return;
  if ( ! _texObj.Bind() ) return;
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  // load texture image if need
  if ( ! _texUpdated ) {
    glTexImage2D(_texObj.target, 0, GL_RGB,
		 _image.getWidth(), _image.getHeight(),
		 0, _image.getFormat(), GL_UNSIGNED_BYTE, dp);
    glTexParameteri(_texObj.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(_texObj.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    _texUpdated = TRUE;
  }

  // set texgen params
  if ( _mapType == UVC ) {
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
  }
  else {
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    switch ( _mapType ) {
    case XYP:
      glTexGenfv(GL_S, GL_OBJECT_PLANE, _pX0);
      glTexGenfv(GL_T, GL_OBJECT_PLANE, _pY0);
      spanS = bbox[1][0]-bbox[0][0]; spanT = bbox[1][1]-bbox[0][1];
      tV[0] = -bbox[0][0]; tV[1] = -bbox[0][1];
      break;
    case YZP:
      glTexGenfv(GL_S, GL_OBJECT_PLANE, _pY0);
      glTexGenfv(GL_T, GL_OBJECT_PLANE, _pZ0);
      spanS = bbox[1][1]-bbox[0][1]; spanT = bbox[1][2]-bbox[0][2];
      tV[0] = -bbox[0][1]; tV[1] = -bbox[0][2];
      break;
    case ZXP:
      glTexGenfv(GL_S, GL_OBJECT_PLANE, _pZ0);
      glTexGenfv(GL_T, GL_OBJECT_PLANE, _pX0);
      spanS = bbox[1][2]-bbox[0][2]; spanT = bbox[1][0]-bbox[0][0];
      tV[0] = -bbox[0][2]; tV[1] = -bbox[0][0];
      break;
    }
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
  }

  // set texture transformation
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();

  if ( spanS > EPSF && spanT > EPSF )
    glScalef(1.f/spanS, 1.f/spanT, 1.0f);
  glTranslatef(tV[0], tV[1], tV[2]);

  glMultMatrixf(_matrix.m_v);
  glMatrixMode(GL_MODELVIEW);
}

void
vfrTexture::unApply() const {
  if ( _mapType != UVC ) {
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
  }

  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);

  _texObj.Disable();
}

void
vfrTexture::setBbox(const CES::Vec3<float>* bb) {
  if ( bb ) {
    _bbox[0] = bb[0];
    _bbox[1] = bb[1];
    _hasBbox = TRUE;
  } else {
    _bbox[0][0] = _bbox[0][1] = _bbox[0][2] = 0.0f;
    _bbox[1][0] = _bbox[1][1] = _bbox[1][2] = 0.0f;
    _hasBbox = FALSE;
  }
  notice();
}

const CES::Vec3<float>*
vfrTexture::getBbox() const {
  if ( _hasBbox )
    return _bbox;
  for ( register int i = 0; i < _nRef; i++ )
    if ( _Ref[i] )
      return ((vfrNode*)(_Ref[i]))->getBbox();
  return NULL;
}

// EOF
