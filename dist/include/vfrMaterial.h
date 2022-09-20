//
// vtMaterial
//   Material data class
//
#ifndef _VFR_MATERIAL_H_
#define _VFR_MATERIAL_H_

#ifdef WINDOWS
#include "stdafx.h"
#endif

#include "vfrBase.h"

class vfrNode;


class EXPORTED_CLASS vfrMaterial {
public:
  vfrMaterial();
  vfrMaterial(const vfrMaterial&);
  virtual ~vfrMaterial();

  void operator=(const vfrMaterial&);
  void reset();

  void apply() const;
  void unApply() const;

  void setLineStipple(const StippleType);
  StippleType getLineStipple() const {return lineStipple;}
  void setLineWidth(const float);
  GLfloat getLineWidth() const {return lineWidth;}

  void setPointSize(const float);
  GLfloat getPointSize() const {return pointSize;}
  void setPointSymbol(const SymbolType);
  SymbolType getPointSymbol() const {return pointSymbol;}

  void setRenderMode(const RenderType rm) {renderMode = rm; notice();}
  RenderType getRenderMode() const {return renderMode;}

  void setFaceMode(const PolyFaceType fm) {faceMode = fm; notice();}
  GLenum getFaceMode() const;

  void setSpecular(const vector4);
  void setSpecular(const float, const float, const float, const float);
  void getSpecular(vector4&) const;

  void setAmbient(const vector4);
  void setAmbient(const float, const float, const float, const float);
  void getAmbient(vector4&) const;

  void setShininess(const vector4);
  void setShininess(const float);
  float getShininess() const;

  void setEmission(const vector4);
  void setEmission(const float, const float, const float, const float);
  void getEmission(vector4 &) const;

  void setAuxPointColor(const Bool, const vector4);
  void setAuxPointColor(const Bool, const float, const float, const float);
  Bool useAuxPointColor(vector4&) const;

  void setAuxLineColor(const Bool, const vector4);
  void setAuxLineColor(const Bool, const float, const float, const float);
  Bool useAuxLineColor(vector4&) const;

  void setParent(vfrNode*);

protected:
  GLfloat      specular[4];
  GLfloat      ambient[4];
  GLfloat      shininess[1];
  GLfloat      emission[4];
  StippleType  lineStipple;
  GLfloat      lineWidth;
  GLfloat      pointSize;
  SymbolType   pointSymbol;
  RenderType   renderMode;
  PolyFaceType faceMode;
  vector4      auxPointColor;
  vector4      auxLineColor;
  Bool         useAuxPC;
  Bool         useAuxLC;
 
  vfrNode*    _parent;

  void notice();
};

#endif /* _VFR_MATERIAL_H_ */
