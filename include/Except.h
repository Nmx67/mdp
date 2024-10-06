#pragma once



// #include<string>
#include <stdexcept>
#include<string>

#include "Trace.h"


struct DbgInfo
{
   const char* file;
   int line;
   const char* function;
};

inline
std::string toString(DbgInfo dbgInfo)
{
   std::string str;
   if (nullptr != dbgInfo.file) {

      str += (strrchr(dbgInfo.file, '\\') ? strrchr(dbgInfo.file, '\\') + 1 : dbgInfo.file);
      str += ':';
   }
   if (0 != dbgInfo.line) {
      str += std::to_string(dbgInfo.line);
      str += ' ';
   }

   if (nullptr != dbgInfo.function) {
      str += dbgInfo.function;
      str += ' ';
   }

   return str;
}

template <typename BaseException>
struct MdpException : public BaseException
{
public:
   MdpException(DbgInfo info, const char* msg)
      : BaseException(msg)
   {}

   MdpException(const char* msg) 
      : BaseException(msg)
   {}
};


/* TODO: support proper customization */
using MessageInvalid = MdpException<std::invalid_argument>;
using MessageFormatInvalid = MdpException<std::invalid_argument>;
using BrokerMessageFormatInvalid = MdpException<std::invalid_argument>;
using WorkerMessageFormatInvalid = MdpException<std::invalid_argument>;
using ClientMessageFormatInvalid = MdpException<std::invalid_argument>;

using BrokerHeartbeatExpired = MdpException<std::runtime_error>;
using BrokerDisconnected = MdpException<std::runtime_error>;

using FlowError = MdpException<std::runtime_error>;

using RecvFailed = MdpException<std::runtime_error>;
using SendFailed = MdpException<std::runtime_error>;

using WorkerDuplicate = MdpException<std::runtime_error>;
using ServiceUnsupported = MdpException<std::runtime_error>;

using IdentityInvalid = MdpException<std::invalid_argument>;

using RuntimeError = MdpException<std::runtime_error>;



#define TO_STRING_IMPL(x) #x
#define TO_STRING(x) TO_STRING_IMPL(x)



#define ENSURE_IMPL(cond, ExceptionType) \
   do { \
      if(!(cond)) { \
         TRACE(TraceLevel::Error, "throw ", TO_STRING(cond), ", ", TO_STRING(ExceptionType)); \
         throw  (ExceptionType)  ( \
            DbgInfo{__FILE__, __LINE__, __FUNCTION__}, \
            TO_STRING(ExceptionType) " : " TO_STRING(cond) \
         ); \
      } \
   } while(false)


#define VAR_ENSURE_IMPL(cond, ExceptionType, ...) \
   do { \
      if(!(cond)) { \
         TRACE(TraceLevel::Error, "throw ", TO_STRING(cond), ", ", TO_STRING(ExceptionType), ", ", __VA_ARGS__); \
         throw  (ExceptionType)  ( \
            DbgInfo{__FILE__, __LINE__, __FUNCTION__}, \
            TO_STRING(ExceptionType) " : " TO_STRING(cond) \
         ); \
      } \
   } while(false)

#define ENSURE(cond, ExceptionType) ENSURE_IMPL(cond, ExceptionType)
#define vENSURE(cond, ExceptionType, ...) VAR_ENSURE_IMPL(cond, ExceptionType, __VA_ARGS__)