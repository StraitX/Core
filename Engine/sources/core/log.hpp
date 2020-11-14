#ifndef STRAITX_LOG_HPP
#define STRAITX_LOG_HPP

#include "platform/error.hpp"
#include "platform/io.hpp"
#include "platform/compiler.hpp"


#ifdef SX_DEBUG

#define DLog(source,error) Output::Printf("[%]: %: %\n",error==Error::Success? "Info " : "Error",source, ErrorNames[(int)error])
#define DLogTrace(msg) Output::Print("[Trace]: " msg)
#define DLogInfo(msg)  Output::Print("[Info ]: " msg)
#define DLogWarn(msg)  Output::Print("[Warn ]: " msg)
#define DLogError(msg) Output::Print("[Error]: " msg)
#define DLogSeparator() Output::Print("===============================================================")

#else

#define DLog(source,error)
#define DLogTrace(msg)
#define DLogInfo(msg)
#define DLogWarn(msg)
#define DLogError(msg)
#define DLogSeparator()

#endif

#define Log(source,error) Output::Printf("[%]: %: %\n",error==Error::Success? "Info " : "Error",source, ErrorNames[(int)error])
#define LogTrace(msg) Output::Print("[Trace]: " msg)
#define LogInfo(msg)  Output::Print("[Info ]: " msg)
#define LogWarn(msg)  Output::Print("[Warn ]: " msg)
#define LogError(msg) Output::Print("[Error]: " msg)
#define LogSeparator() Output::Print("===============================================================")

#endif // STRAITX_LOG_HPP