#ifndef STRAITX_LOG_HPP
#define STRAITX_LOG_HPP

#include "platform/result.hpp"
#include "core/print.hpp"
#include "platform/compiler.hpp"


#ifdef SX_DEBUG

#define DLog(source,error) StraitX::Print("[%]: %: %\n",error==Result::Success? "Info " : "Result",source, error.Name())
#define DLogTrace(...) do{StraitX::Print("[Trace]: "); StraitX::Println(__VA_ARGS__); }while(0)
#define DLogInfo(...)  do{StraitX::Print("[Info ]: "); StraitX::Println(__VA_ARGS__); }while(0)
#define DLogWarn(...)  do{StraitX::Print("[Warn ]: "); StraitX::Println(__VA_ARGS__); }while(0)
#define DLogError(...) do{StraitX::Print("[Error]: "); StraitX::Println(__VA_ARGS__); }while(0)
#define DLogSeparator() StraitX::Println("===============================================================")

#else

#define DLog(source,error)  ((void)0)
#define DLogTrace(...)      ((void)0)
#define DLogInfo(...)       ((void)0)
#define DLogWarn(...)       ((void)0)
#define DLogError(...)      ((void)0)
#define DLogSeparator()     ((void)0)

#endif

#define Log(source,error) StraitX::Print("[%]: %: %\n",error==Result::Success? "Info " : "Result",source, error.Name())
#define LogTrace(...) do{StraitX::Print("[Trace]: "); StraitX::Println(__VA_ARGS__); }while(0)
#define LogInfo(...)  do{StraitX::Print("[Info ]: "); StraitX::Println(__VA_ARGS__); }while(0)
#define LogWarn(...)  do{StraitX::Print("[Warn ]: "); StraitX::Println(__VA_ARGS__); }while(0)
#define LogError(...) do{StraitX::Print("[Error]: "); StraitX::Println(__VA_ARGS__); }while(0)
#define LogSeparator() StraitX::Println("===============================================================")

#endif // STRAITX_LOG_HPP