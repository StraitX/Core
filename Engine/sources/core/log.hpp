#ifndef STRAITX_LOG_HPP
#define STRAITX_LOG_HPP

#include "platform/error.hpp"
#include "platform/io.hpp"
#include "platform/compiler.hpp"


#ifdef SX_DEBUG

#define DLog(source,error) StraitX::Output::Printf("[%]: %: %\n",error==Error::Success? "Info " : "Error",source, ErrorNames[(int)error])
#define DLogTrace(msg) StraitX::Output::Printf("[Trace]: %\n", msg)
#define DLogInfo(msg)  StraitX::Output::Printf("[Info ]: %\n", msg)
#define DLogWarn(msg)  StraitX::Output::Printf("[Warn ]: %\n", msg)
#define DLogError(msg) StraitX::Output::Printf("[Error]: %\n", msg)
#define DLogSeparator() StraitX::Output::Print("===============================================================")

#else

#define DLog(source,error)
#define DLogTrace(msg)
#define DLogInfo(msg)
#define DLogWarn(msg)
#define DLogError(msg)
#define DLogSeparator()

#endif

#define Log(source,error) StraitX::Output::Printf("[%]: %: %\n",error==Error::Success? "Info " : "Error",source, ErrorNames[(int)error])
#define LogTrace(msg) StraitX::Output::Printf("[Trace]: %\n", msg)
#define LogInfo(msg)  StraitX::Output::Printf("[Info ]: %\n", msg)
#define LogWarn(msg)  StraitX::Output::Printf("[Warn ]: %\n", msg)
#define LogError(msg) StraitX::Output::Printf("[Error]: %\n", msg)
#define LogSeparator() StraitX::Output::Print("===============================================================")

#endif // STRAITX_LOG_HPP