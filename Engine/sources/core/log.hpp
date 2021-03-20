#ifndef STRAITX_LOG_HPP
#define STRAITX_LOG_HPP

#include "platform/result.hpp"
#include "platform/io.hpp"
#include "platform/compiler.hpp"


#ifdef SX_DEBUG

#define DLog(source,error) StraitX::Output::Printf("[%]: %: %\n",error==Result::Success? "Info " : "Result",source, error.Name())
#define DLogTrace(...) {StraitX::Output::Printf("[Trace]: "); StraitX::Output::Printf(__VA_ARGS__); StraitX::Output::Printf("\n");}
#define DLogInfo(...)  {StraitX::Output::Printf("[Info ]: "); StraitX::Output::Printf(__VA_ARGS__); StraitX::Output::Printf("\n");}
#define DLogWarn(...)  {StraitX::Output::Printf("[Warn ]: "); StraitX::Output::Printf(__VA_ARGS__); StraitX::Output::Printf("\n");}
#define DLogError(...) {StraitX::Output::Printf("[Error]: "); StraitX::Output::Printf(__VA_ARGS__); StraitX::Output::Printf("\n");}
#define DLogSeparator() StraitX::Output::Print("===============================================================")

#else

#define DLog(source,error)  ((void)0)
#define DLogTrace(...)      ((void)0)
#define DLogInfo(...)       ((void)0)
#define DLogWarn(...)       ((void)0)
#define DLogError(...)      ((void)0)
#define DLogSeparator()     ((void)0)

#endif

#define Log(source,error) StraitX::Output::Printf("[%]: %: %\n",error==Result::Success? "Info " : "Result",source, error.Name())
#define LogTrace(...) {StraitX::Output::Printf("[Trace]: "); StraitX::Output::Printf(__VA_ARGS__); StraitX::Output::Printf("\n");}
#define LogInfo(...)  {StraitX::Output::Printf("[Info ]: "); StraitX::Output::Printf(__VA_ARGS__); StraitX::Output::Printf("\n");}
#define LogWarn(...)  {StraitX::Output::Printf("[Warn ]: "); StraitX::Output::Printf(__VA_ARGS__); StraitX::Output::Printf("\n");}
#define LogError(...) {StraitX::Output::Printf("[Error]: "); StraitX::Output::Printf(__VA_ARGS__); StraitX::Output::Printf("\n");}
#define LogSeparator() StraitX::Output::Print("===============================================================")

#endif // STRAITX_LOG_HPP