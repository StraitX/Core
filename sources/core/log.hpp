#ifndef STRAITX_LOG_HPP
#define STRAITX_LOG_HPP

#include "core/result.hpp"
#include "core/print.hpp"
#include "core/env/compiler.hpp"

#ifdef SX_DEBUG

#define DLog(source,error) Print("[%]: %: %\n",error==Result::Success? "Info " : "Result",source, error.Name())
#define DLogTrace(...) [](){Print("[Trace]: "); Println(__VA_ARGS__); }()
#define DLogInfo(...)  [](){Print("[Info ]: "); Println(__VA_ARGS__); }()
#define DLogWarn(...)  [](){Print("[Warn ]: "); Println(__VA_ARGS__); }()
#define DLogError(...) [](){Print("[Error]: "); Println(__VA_ARGS__); }()
#define DLogSeparator() Println("===============================================================")

#else

#define DLog(source,error)  ((void)0)
#define DLogTrace(...)      ((void)0)
#define DLogInfo(...)       ((void)0)
#define DLogWarn(...)       ((void)0)
#define DLogError(...)      ((void)0)
#define DLogSeparator()     ((void)0)

#endif

#define Log(source,error) Print("[%]: %: %\n",error==Result::Success? "Info " : "Result",source, error.Name())
#define LogTrace(...) [](){Print("[Trace]: "); Println(__VA_ARGS__); }()
#define LogInfo(...)  [](){Print("[Info ]: "); Println(__VA_ARGS__); }()
#define LogWarn(...)  [](){Print("[Warn ]: "); Println(__VA_ARGS__); }()
#define LogError(...) [](){Print("[Error]: "); Println(__VA_ARGS__); }()
#define LogSeparator() Println("===============================================================")

#endif // STRAITX_LOG_HPP