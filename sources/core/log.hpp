#ifndef STRAITX_LOG_HPP
#define STRAITX_LOG_HPP

#include "core/result.hpp"
#include "core/print.hpp"
#include "core/env/compiler.hpp"

#ifdef SX_DEBUG

#define DLog(source,error) Print("[%]: %: %\n",error==Result::Success? "Info " : "Result",source, error.Name())
#define DLogTrace(...) do{Print("[Trace]: "); Println(__VA_ARGS__); }while(0)
#define DLogInfo(...)  do{Print("[Info ]: "); Println(__VA_ARGS__); }while(0)
#define DLogWarn(...)  do{Print("[Warn ]: "); Println(__VA_ARGS__); }while(0)
#define DLogError(...) do{Print("[Error]: "); Println(__VA_ARGS__); }while(0)
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
#define LogTrace(...) do{Print("[Trace]: "); Println(__VA_ARGS__); }while(0)
#define LogInfo(...)  do{Print("[Info ]: "); Println(__VA_ARGS__); }while(0)
#define LogWarn(...)  do{Print("[Warn ]: "); Println(__VA_ARGS__); }while(0)
#define LogError(...) do{Print("[Error]: "); Println(__VA_ARGS__); }while(0)
#define LogSeparator() Println("===============================================================")

#endif // STRAITX_LOG_HPP