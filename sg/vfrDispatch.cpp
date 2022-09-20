//
// vfrDispatch
//   Event Dispatcher Class
//

#include "vfrDispatch.h"
#include "vfrDrawArea.h"


vfrDispatch::vfrDispatch() {
}

vfrDispatch::~vfrDispatch() {
}

vfrDispatch&
vfrDispatch::instance(vfrDrawArea& da) {
  return da.getDispatcher();
}

// EOF
