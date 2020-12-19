#ifndef STRAITX_LOG_HPP
#define STRAITX_LOG_HPP

#include "platform/error.hpp"
#include "platform/io.hpp"
#include "platform/compiler.hpp"


#ifdef SX_DEBUG

#define DLog(source,error) StraitX::Output::Printf("[%]: %: %\n",error==Error::Success? "Info " : "Error",source, ErrorNames[(int)error])
#define DLogTrace(...) {StraitX::Output::Printf("[Trace]: "); StraitX::Output::Printf(__VA_ARGS__); StraitX::Output::Printf("\n");}
#define DLogInfo(...)  {StraitX::Output::Printf("[Info ]: "); StraitX::Output::Printf(__VA_ARGS__); StraitX::Output::Printf("\n");}
#define DLogWarn(...)  {StraitX::Output::Printf("[Warn ]: "); StraitX::Output::Printf(__VA_ARGS__); StraitX::Output::Printf("\n");}
#define DLogError(...) {StraitX::Output::Printf("[Error]: "); StraitX::Output::Printf(__VA_ARGS__); StraitX::Output::Printf("\n");}
#define DLogSeparator() StraitX::Output::Print("===============================================================")

#else

#define DLog(source,error)  ((void)0)
#define DLogTrace(msg)      ((void)0)
#define DLogInfo(msg)       ((void)0)
#define DLogWarn(msg)       ((void)0)
#define DLogError(msg)      ((void)0)
#define DLogSeparator()     ((void)0)

#endif

#define Log(source,error) StraitX::Output::Printf("[%]: %: %\n",error==Error::Success? "Info " : "Error",source, ErrorNames[(int)error])
#define LogTrace(...) {StraitX::Output::Printf("[Trace]: "); StraitX::Output::Printf(__VA_ARGS__); StraitX::Output::Printf("\n");}
#define LogInfo(...)  {StraitX::Output::Printf("[Info ]: "); StraitX::Output::Printf(__VA_ARGS__); StraitX::Output::Printf("\n");}
#define LogWarn(...)  {StraitX::Output::Printf("[Warn ]: "); StraitX::Output::Printf(__VA_ARGS__); StraitX::Output::Printf("\n");}
#define LogError(...) {StraitX::Output::Printf("[Error]: "); StraitX::Output::Printf(__VA_ARGS__); StraitX::Output::Printf("\n");}
#define LogSeparator() StraitX::Output::Print("===============================================================")

#endif // STRAITX_LOG_HPP