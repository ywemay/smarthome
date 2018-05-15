#ifndef H_SECURITY
#define H_SECURITY

#include "Views.h"

class Security : public Views {
  public:
  void begin();
  void check();
  void viewInit();
  void viewUpdate();
};

#endif
