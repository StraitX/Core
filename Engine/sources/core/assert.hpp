#ifndef STRAITX_ASSERT_HPP
#define STRAITX_ASSERT_HPP 

#include "platform/compiler.hpp"

extern void assert_fail(const char *assertion, const char *filename, unsigned int line, const char * function);


#define assert(expression) (static_cast<bool>(expression)? (void)0 : assert_fail(#expression,SX_THIS_FILE,SX_THIS_LINE,SX_THIS_FUNCTION))

#endif // STRAITX_ASSERT_HPP