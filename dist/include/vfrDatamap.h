//
// vfrDatamap
//   Data Value and HSV Color Mapping class
//

#ifndef _VFR_DATAMAP_H_
#define _VFR_DATAMAP_H_

#include "vfrBase.h"


class EXPORTED_CLASS vfrDatamap {
protected:
  float _dmin;
  float _dmax;
  float* _hue;
  float* _sat;
  float* _val;
  float* _alp;
  int _ncol;

  void makeDefault(const int =2);

  static void hsv2rgb(const vector3, vector3);

public:
  vfrDatamap()
    : _dmin(0.0f),_dmax(1.0f),
      _hue(NULL),_sat(NULL),_val(NULL),_alp(NULL), _ncol(0) {
    makeDefault();
  }
  vfrDatamap(const float min, const float max)
    : _hue(NULL),_sat(NULL),_val(NULL),_alp(NULL), _ncol(0) {
    setMinMax(min, max);
    makeDefault();
  }
  vfrDatamap(const int,
	     const float*, const float*, const float*, const float*);
  vfrDatamap(const vfrDatamap& org)
    : _dmin(0.0f),_dmax(1.0f),
      _hue(NULL),_sat(NULL),_val(NULL),_alp(NULL), _ncol(0) {*this = org;}
  virtual ~vfrDatamap();

  void operator=(const vfrDatamap&);

  void  setMinMax(const float, const float);
  float getMin() const {return _dmin;}
  float getMax() const {return _dmax;}

  Bool  setHue(const int, const float);
  const float* getHue() const {return _hue;}

  Bool  setSat(const int, const float);
  const float* getSat() const {return _sat;}

  Bool  setVal(const int, const float);
  const float* getVal() const {return _val;}

  Bool  setAlp(const int, const float);
  const float* getAlp() const {return _alp;}

  int   getNumCol() const {return _ncol;}
  void  datamap(const float, vector4 &) const;
};

#endif /* _VFR_DATAMAP_H_ */
