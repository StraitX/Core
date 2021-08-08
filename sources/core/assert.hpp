#ifndef STRAITX_ASSERT_HPP
#define STRAITX_ASSERT_HPP 

#include "core/env/compiler.hpp"

extern void _AssertFail(const char *assertion, const char *filename, unsigned int line, const char * function);
extern void _AssertFail(const char *assertion, const char *message, const char *filename, unsigned int line, const char * function);

extern void _CoreAssertFail(const char *assertion, const char *message, const char * function);

#ifdef SX_DEBUG
    #define _SX_ASSERT_EXPR(expr) (static_cast<bool>(expr)? (void)0 : _AssertFail(#expr,SX_THIS_FILE,SX_THIS_LINE,SX_THIS_FUNCTION))
    #define _SX_ASSERT_EXPR_MSG(expr, msg) (static_cast<bool>(expr)? (void)0 : _AssertFail(#expr, msg, SX_THIS_FILE,SX_THIS_LINE,SX_THIS_FUNCTION))
    
    #define _SX_ASSERT_SWITCH(_1, _2, name, ...) name 
    #define SX_ASSERT(...) _SX_ASSERT_SWITCH(__VA_ARGS__, _SX_ASSERT_EXPR_MSG, _SX_ASSERT_EXPR)(__VA_ARGS__)
#else
    #define SX_ASSERT(...) ((void)0)
#endif

#ifdef SX_DEBUG
    #define SX_CORE_ASSERT(expression,message) (static_cast<bool>(expression)? (void)0 : _CoreAssertFail(#expression,message,SX_THIS_FUNCTION))
#else
    #define SX_CORE_ASSERT(expression,message) ((void)0)
#endif

#ifdef SX_DEBUG
    #define SX_CALL_ASSERT(function, ret_val, ...) SX_ASSERT(function == ret_val, __VA_ARGS__)
#else
    #define SX_CALL_ASSERT(function, ret_val, ...) (void)function
#endif

#ifdef SX_DEBUG
    #define SX_CORE_CALL_ASSERT(function, ret_val, message) SX_CORE_ASSERT(function == ret_val, message)
#else
    #define SX_CORE_CALL_ASSERT(function, ret_val, message) (void)function
#endif

#endif // STRAITX_ASSERT_HPP