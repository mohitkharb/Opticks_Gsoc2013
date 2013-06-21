
#ifndef ERROR_H
#define ERROR_H

#include "parser.h"

//error class
class CmCError
{
 public:

  CmCToken *token_;
  int      errorCode_;
  
  CmCError(CmCToken *token, int errorCode) {
    token_ = token;
    errorCode_ = errorCode;
  }
  ~CmCError( void ) { return; }
};

#endif
