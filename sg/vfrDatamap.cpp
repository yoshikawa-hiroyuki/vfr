//
// vfrDatamap
//   Data Value and HSV Color Mapping Class
//

#include "vfrDatamap.h"

vfrDatamap::vfrDatamap(const int nc,
  const float* h, const float* s, const float* v, const float* a)
 : _dmin(0.0f),_dmax(1.0f), _hue(NULL),_sat(NULL),_val(NULL),_alp(NULL)
{
  if ( nc > 1 )
    _ncol = nc;
  else
    _ncol = 2;

  if ( !h || !s || !v || !a ) {
    makeDefault(_ncol);
    return;
  }
  _hue = (float*)ReAllocate(_hue, _ncol*sizeof(float));
  _sat = (float*)ReAllocate(_sat, _ncol*sizeof(float));
  _val = (float*)ReAllocate(_val, _ncol*sizeof(float));
  _alp = (float*)ReAllocate(_alp, _ncol*sizeof(float));
  if ( !_hue || !_sat || !_val || !_alp ) {
    _ncol = 0;
    return;
  }

  memcpy(_hue, h, sizeof(float)*_ncol);
  memcpy(_sat, s, sizeof(float)*_ncol);
  memcpy(_val, v, sizeof(float)*_ncol);
  memcpy(_alp, a, sizeof(float)*_ncol);
}


vfrDatamap::~vfrDatamap() {
  if ( _hue ) DeAllocate(_hue);
  if ( _sat ) DeAllocate(_sat);
  if ( _val ) DeAllocate(_val);
  if ( _alp ) DeAllocate(_alp);
}

void
vfrDatamap::makeDefault(const int nc) {
  if ( nc > 1 )
    _ncol = nc;
  else
    _ncol = 2;

  _hue = (float*)ReAllocate(_hue, _ncol*sizeof(float));
  _sat = (float*)ReAllocate(_sat, _ncol*sizeof(float));
  _val = (float*)ReAllocate(_val, _ncol*sizeof(float));
  _alp = (float*)ReAllocate(_alp, _ncol*sizeof(float));
  if ( !_hue || !_sat || !_val || !_alp ) {
    _ncol = 0;
    return;
  }

  _hue[0] = 0.666667f; _hue[_ncol-1] = 0.0f;
  _sat[0] = 1.0f;      _sat[_ncol-1] = 1.0f;
  _val[0] = 1.0f;      _val[_ncol-1] = 1.0f;
  _alp[0] = 1.0f;      _alp[_ncol-1] = 1.0f;

  float dh = (_hue[_ncol-1] - _hue[0]) / (float)(_ncol -1);
  float ds = (_sat[_ncol-1] - _sat[0]) / (float)(_ncol -1);
  float dv = (_val[_ncol-1] - _val[0]) / (float)(_ncol -1);
  float da = (_alp[_ncol-1] - _alp[0]) / (float)(_ncol -1);
  for ( register int i = 1; i < _ncol -1; i++ ) {
    _hue[i] = _hue[0] + (float)i * dh;
    _sat[i] = _sat[0] + (float)i * ds;
    _val[i] = _val[0] + (float)i * dv;
    _alp[i] = _alp[0] + (float)i * da;
  }
}

void
vfrDatamap::operator=(const vfrDatamap& org) {
  int nc = org.getNumCol();
  if ( nc < 2 ) return;
  _ncol = nc;

  _hue = (float*)ReAllocate(_hue, _ncol*sizeof(float));
  _sat = (float*)ReAllocate(_sat, _ncol*sizeof(float));
  _val = (float*)ReAllocate(_val, _ncol*sizeof(float));
  _alp = (float*)ReAllocate(_alp, _ncol*sizeof(float));
  if ( !_hue || !_sat || !_val || !_alp ) {
    _ncol = 0;
    return;
  }

  memcpy(_hue, org.getHue(), sizeof(float)*_ncol);
  memcpy(_sat, org.getSat(), sizeof(float)*_ncol);
  memcpy(_val, org.getVal(), sizeof(float)*_ncol);
  memcpy(_alp, org.getAlp(), sizeof(float)*_ncol);

  _dmin = org.getMin();
  _dmax = org.getMax();
}

void
vfrDatamap::setMinMax(const float min, const float max) {
  if ( min > max ) {
    _dmin = max;
    _dmax = min;
  } else {
    _dmin = min;
    _dmax = max;
  }
}

Bool
vfrDatamap::setHue(const int idx, const float h) {
  if ( 0.0f > h || 1.0f < h )
    return FALSE;
  if ( idx < 0 || idx >= _ncol )
    return FALSE;

  _hue[idx] = h;
  return TRUE;
};

Bool
vfrDatamap::setSat(const int idx, const float s) {
  if ( 0.0f > s || 1.0f < s )
    return FALSE;
  if ( idx < 0 || idx >= _ncol )
    return FALSE;

  _sat[idx] = s;
  return TRUE;
};

Bool
vfrDatamap::setVal(const int idx, const float v) {
  if ( 0.0f > v || 1.0f < v )
    return FALSE;
  if ( idx < 0 || idx >= _ncol )
    return FALSE;

  _val[idx] = v;
  return TRUE;
};

Bool
vfrDatamap::setAlp(const int idx, const float a) {
  if ( 0.0f > a || 1.0f < a )
    return FALSE;
  if ( idx < 0 || idx >= _ncol )
    return FALSE;

  _alp[idx] = a;
  return TRUE;
};

// STATIC
void
vfrDatamap::hsv2rgb(const vector3 hsv, vector3 rgb) {
  float f, p, q, t;
  float h;
  int   i;

  h = hsv[0];
  if ( hsv[2] == 0.0f ) {
    rgb[0] = rgb[1] = rgb[2] = 0.0f;
  } else if ( hsv[1] == 0.0f ) {
    rgb[0] = rgb[1] = rgb[2] = hsv[2];
  } else {
    h *= 6.0f;
    if ( h >= 6.0f )
      h = 0.0f;

    i = (int)h;
    f = h - (float)i;
    p = hsv[2]*(1.0f - hsv[1]);
    q = hsv[2]*(1.0f - hsv[1]*f);
    t = hsv[2]*(1.0f - hsv[1]*(1.0f - f));

    switch ( i ) {
    case 0:
      rgb[0] = hsv[2];
      rgb[1] = t;
      rgb[2] = p;
      break;
    case 1:
      rgb[0] = q;
      rgb[1] = hsv[2];
      rgb[2] = p;
      break;
    case 2:
      rgb[0] = p;
      rgb[1] = hsv[2];
      rgb[2] = t;
      break;
    case 3:
      rgb[0] = p;
      rgb[1] = q;
      rgb[2] = hsv[2];
      break;
    case 4:
      rgb[0] = t;
      rgb[1] = p;
      rgb[2] = hsv[2];
      break;
    case 5:
      rgb[0] = hsv[2];
      rgb[1] = p;
      rgb[2] = q;
      break;
    }
  }
}

void
vfrDatamap::datamap(const float data, vector4& rgba) const {
  float hsv[3], a;

  if ( _dmin > data ) {
    hsv[0] = _hue[0];
    hsv[1] = _sat[0];
    hsv[2] = _val[0];
    a      = _alp[0];
  } else if ( data > _dmax ) {
    hsv[0] = _hue[_ncol-1];
    hsv[1] = _sat[_ncol-1];
    hsv[2] = _val[_ncol-1];
    a      = _alp[_ncol-1];
  } else {
    if ( _dmax - _dmin < 1e-20 ) {
      hsv[0] = (_hue[_ncol-1] - _hue[0])*0.5f + _hue[0];
      hsv[1] = (_sat[_ncol-1] - _sat[0])*0.5f + _sat[0];
      hsv[2] = (_val[_ncol-1] - _val[0])*0.5f + _val[0];
      a      = (_alp[_ncol-1] - _alp[0])*0.5f + _alp[0];
    } else {
      float v = (data - _dmin) / (_dmax - _dmin);
      float dv = 1.0f / (float)(_ncol-1);
      register int i;
      for ( i = 1; i < _ncol; i++ )
        if ( v <= (float)i*dv )
          break;
      if ( dv < 1e-20 ) {
        hsv[0] = _hue[i];
        hsv[1] = _sat[i];
        hsv[2] = _val[i];
        a      = _alp[i];
      } else {
        hsv[0] = ((_hue[i]-_hue[i-1])*v +dv*i*_hue[i-1] -dv*(i-1)*_hue[i])/dv;
        hsv[1] = ((_sat[i]-_sat[i-1])*v +dv*i*_sat[i-1] -dv*(i-1)*_sat[i])/dv;
        hsv[2] = ((_val[i]-_val[i-1])*v +dv*i*_val[i-1] -dv*(i-1)*_val[i])/dv;
        a      = ((_alp[i]-_alp[i-1])*v +dv*i*_alp[i-1] -dv*(i-1)*_alp[i])/dv;
      }
    }
  }
  hsv2rgb(hsv, rgba);
  rgba[3] = a;
}

// EOF
