#ifndef STRAITX_ASSERT_HPP
#define STRAITX_ASSERT_HPP 

#include "platform/compiler.hpp"

extern void AssertFail(const char *assertion, const char *filename, unsigned int line, const char * function);
extern void CoreAssertFail(const char *assertion,const char *message, const char *function);

#ifdef SX_DEBUG
    #define Assert(expression) (static_cast<bool>(expression)? (void)0 : AssertFail(#expression,SX_THIS_FILE,SX_THIS_LINE,SX_THIS_FUNCTION))
#else
    #define Assert(expression) ((void)0)
#endif


#ifdef SX_DEBUG
    #define CoreAssert(expression,message) (static_cast<bool>(expression)? (void)0 : CoreAssertFail(#expression,message,SX_THIS_FUNCTION))
#else
    #define CoreAssert(expression,message) ((void)0)
#endif

#ifdef SX_DEBUG
    #define FunctionAssert(function, ret_val) Assert(function == ret_val)
#else
    #define FunctionAssert(function, ret_val) (void)function
#endif

#ifdef SX_DEBUG
    #define CoreFunctionAssert(function, ret_val, message) CoreAssert(function == ret_val, message)
#else
    #define CoreFunctionAssert(function, ret_val, message) (void)function
#endif

#endif // STRAITX_ASSERT_HPP