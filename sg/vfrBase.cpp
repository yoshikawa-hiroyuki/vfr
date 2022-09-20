//
// vfrBase
//     Base object class
//
#include "vfrBase.h"


vfrBase::vfrBase(const Bool suicide)
 : _matrix(), _nRef(0), _Ref(NULL), _doSuicide(suicide)
{
}

vfrBase::vfrBase(const vfrBase& org, const Bool suicide)
 : _matrix(), _nRef(0), _Ref(NULL), _doSuicide(suicide)
{
  *this = org;
}

vfrBase::~vfrBase() {
  resetRef();
}

void
vfrBase::resetRef() {
  for ( register int i = 0; i < _nRef; i++ )
    _Ref[i]->rumor(this);
  if ( _Ref ) {
    DeAllocate(_Ref);
    _Ref = NULL;
  }
  _nRef = 0;
}

void
vfrBase::addRef(vfrBase *refptr) {
  if ( ! refptr )
    return;
  if ( refptr == this )
    return;
  if ( _nRef < 0 ) {
    _nRef = 0;
    _Ref = NULL;
  }
  _Ref = (vfrBase**)ReAllocate(_Ref, sizeof(vfrBase*) * (_nRef+1));
  if ( _Ref == NULL ) {
    _nRef = 0;
    return;
  }
  _Ref[_nRef++] = refptr;
}

Bool
vfrBase::remRef(vfrBase *dPtr) {
  if ( ! dPtr )
    return FALSE;
  for ( register int i = 0; i < _nRef; i++ ) {
    if ( _Ref[i] == dPtr ) {
      for ( register int j = i; j < _nRef-1; j++ )
        _Ref[j] = _Ref[j+1];
      _Ref[_nRef-1] = NULL;
      _nRef--;
      if ( _nRef < 1 ) {
        if ( _Ref ) {
          DeAllocate(_Ref);
          _Ref = NULL;
        }
        clearDispList();
#ifdef _VFR_APOTHEOSIS
        if ( _doSuicide )
          delete this;
#endif //_VFR_APOTHEOSIS
      }
      return TRUE;
    }
  }
  return FALSE;
}

void
vfrBase::notice() {
  for ( register int i = 0; i < _nRef; i++ )
    if ( _Ref[i] )
      _Ref[i]->notice();
}

void
vfrBase::chkNotice() {
  for ( register int i = 0; i < _nRef; i++ )
    if ( _Ref[i] )
      _Ref[i]->chkNotice();
}

//EOF
