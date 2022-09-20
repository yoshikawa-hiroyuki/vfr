//
// vfrNode
//     Common Node base class
//
#ifndef _VFR_NODE_H_
#define _VFR_NODE_H_

#include <string>
#include "vfrBase.h"
#include "vfrMaterial.h"
#include "vfrDatamap.h"
#include "vfrTexture.h"

#include "utilOglObj.h"

// Anonymous //
#ifndef VFR_NONAME
#define VFR_NONAME "%%noname%%"
#endif

// Feedback Size //
#define VFR_FEEDBACK_SIZE_FACTOR  8

// Element List Delimiter //
#define VFR_END_OF_ELEM  -1

// Material Stack //
#include <stack>
typedef std::stack<vfrMaterial*> vfrMaterialStack;

using namespace VFR;


class EXPORTED_CLASS vfrNode : public vfrBase {
public:
  enum DLFlagType {
    DLF_NONE=0, DLF_POINT =(1L<<0), DLF_WIRE =(1L<<1), DLF_SOLID =(1L<<2)
  };

  virtual ~vfrNode();

  // Naming methods
  void setName(const std::string& name);
  const std::string getName() const {return _name;}
  unsigned int getID() const {return _currentSeq;}

  // Rendering methods
  virtual void render(vfrMaterialStack* mstk =NULL);
  virtual void render(const Bool transp, vfrMaterialStack* mstk =NULL);
  virtual void renderFeedBack(const unsigned int);
  virtual Bool generateNormals() {return FALSE;}
  virtual GLsizei getFeedbackSize() const;
  virtual void notice();
  virtual void rumor(vfrBase*);

  virtual void renderSolid() {}
  virtual void renderWire() {}
  virtual void renderPoint();

  // Display-list control
  Bool beginDispList(const DLFlagType);
  void endDispList(const DLFlagType);
  virtual void invalidateDispList();
  virtual void clearDispList();

  // Transparency control methods
  Bool isTransparency() const {return _transparent;}
  void setTransparency(const Bool);
  void setAlpha(const float);
  void setAlpha(const Bool, const float);

  // Traverse parameters
  Bool alcPools(const int, const int, const int, const int);

  Bool alcVerts(const int);
  Bool setVerts(const int, const vector3*);
  Bool setVert(const int, const vector3, const Bool =TRUE);

  Bool alcNormals(const int);
  Bool setNormals(const int, const vector3*);
  Bool setNormal(const int, const vector3);

  Bool alcIndices(const int);
  Bool setIndices(const int, const int*);
  Bool setIndice(const int, const int);

  Bool alcColors(const int);
  Bool setColors3(const int, const vector3*);
  Bool setColors4(const int, const vector4*);
  Bool setColor3(const int, const vector3);
  Bool setColor3(const int, const float, const float, const float);
  Bool setColor4(const int, const vector4);
  Bool setColor4(const int,
		 const float, const float, const float, const float);

  void setNormalMode(const AppearType);
  AppearType getNormalMode() const {return _normalMode;}
  void setColorMode(const AppearType);
  AppearType getColorMode() const {return _colorMode;}
  void setFeedbackMode(const FeedbackType);
  FeedbackType getFeedbackMode() const {return _feedbackMode;}
  void setPickMode(const PickType);
  PickType getPickMode() const {return _pickable;}

  int getNumVerts() const {return nVerts;}
  int getNumIndices() const {return nIndices;}
  int getNumNormals() const {return nNormals;}
  int getNumColors() const {return nColors;}

  vector3* getVerts() {return _verts;}
  int* getIndices() {return _indices;}
  vector3* getNormals() {return _normals;}
  vector4* getColors() {return _colors;}

  // Bounding box control methods
  virtual void renderBbox();
  const CES::Vec3<float>* getBbox() const {return _bbox;}
  void getMatrixBbox(CES::Vec3<float>*) const;
  void setBboxShowMode(const Bool);
  Bool getBboxShowMode() const {return _showBbox;}
  void setBboxColor(const vector4);
  void setBboxColor(const float, const float, const float);
  void setBboxColor(const float, const float, const float, const float);
  void getBboxColor(vector4) const;
  void setBboxWidth(const float);
  float getBboxWidth() const {return _bboxWidth;}
  virtual void generateBbox();
  virtual void generateBbox(const CES::Vec3<float>&);

  // Node tree scanning methods
  virtual vfrNode* getNode(const unsigned int id); 
  virtual vfrNode* getNode(const std::string& name);
  virtual Bool accumMatrix(const unsigned int, vfrMatrix&) const;

  // Material methods
  const vfrMaterial* getMaterial() const;
  virtual GLenum getFaceMode() const;
  virtual RenderType getRenderMode() const;
  Bool useAuxPointColor(vector4&) const;
  Bool useAuxPointColor() const;
  Bool useAuxLineColor(vector4&) const;
  Bool useAuxLineColor() const;
  vfrMaterial* alcMaterial();
  vfrMaterial* alcMaterial(const vfrMaterial&);
  vfrMaterial* getPrivateMaterial() {return _material;}
  void setPrivateMaterial(vfrMaterial* pm);

  // Texture data access methods
  vfrTexture* getTexture() {return _texture;}
  void setTexture(vfrTexture*);

  // Posture control methods
  void setHpr(const float, const float, const float);
  void setHprDeg(const float h, const float p, const float r) {
    setHpr(CES::Deg2Rad(h), CES::Deg2Rad(p), CES::Deg2Rad(r));
  }

  // Static methods
  static unsigned int getNodeSequence();
  static void setImmediateMode(const Bool);
  static Bool isImmediateMode();

protected:
  // Instanciate not permitted
  vfrNode(const std::string& =std::string(VFR_NONAME), const Bool =FALSE);

  // Bbox utils
  void         checkBbox();
  void         drawBbox();

  std::string  _name;
  int nVerts,   nPoolVerts;
  int nNormals, nPoolNormals;
  int nIndices, nPoolIndices;
  int nColors,  nPoolColors;
  vector3*     _verts;
  vector3*     _normals;
  int*         _indices;
  vector4*     _colors;
  Bool         _transparent;
  AppearType   _normalMode;
  AppearType   _colorMode;
  FeedbackType _feedbackMode;
  vfrMaterial* _material;
  vfrTexture*  _texture;
  Bool         _showBbox;
  PickType     _pickable;
  CES::Vec3<float> _bbox[2];
  vector4      _bboxColor;
  float        _bboxWidth;
  unsigned int _currentSeq;

  // Display-list IDs
  CES::DispList _dlPoint, _dlWire, _dlSolid;
  long          _dlUpdated;
  Bool          _dlNodeType;
  static Bool   _useDispList;

  // Nodes' uniq ID
  static unsigned int _sequence;

private:
  void reset();
};


// inline methods

inline Bool vfrNode::beginDispList(const DLFlagType targ) {
  // display-list check
  if ( _dlNodeType && _useDispList ) {
    CES::DispList* pdl = NULL;
    switch ( targ ) {
    case DLF_POINT: pdl = &_dlPoint; break;
    case DLF_WIRE:  pdl = &_dlWire;  break;
    case DLF_SOLID: pdl = &_dlSolid; break;
    default: return FALSE;
    }
    // call display-list and return, if updated
    if ( _dlUpdated & targ ) {
      if ( pdl->CallList() )
	return TRUE; // no need to exec drawing code
    }

    // start display-list definition
    pdl->NewList(GL_COMPILE_AND_EXECUTE);
  }
  return FALSE; // need to exec drawing code
}

inline void vfrNode::endDispList(const DLFlagType targ) {
  if ( _dlNodeType && _useDispList ) {
    switch ( targ ) {
    case DLF_POINT: _dlPoint.EndList(); break;
    case DLF_WIRE:  _dlWire.EndList();  break;
    case DLF_SOLID: _dlSolid.EndList(); break;
    default: return;
    }
    _dlUpdated |= targ;
  }
}

inline void vfrNode::invalidateDispList() {
  if ( _dlNodeType ) _dlUpdated = DLF_NONE;
}

#endif /* _VFR_NODE_H_ */
