#pragma once

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

enum class TraceLevel
{
    Error,
    Warning,
    Info,
    Debug
};

using LogLevel = TraceLevel;

namespace impl_ {

template <typename T>
void traceSerializeImpl(std::ostream &os, const T &value)
{
    os << value;
}



// recursive
inline void traceImpl(std::ostream&) {} // recurse last
template <typename T, typename ...T_n>
void traceImpl(std::ostream &os, const T &value, const T_n &...tail)
{
    traceSerializeImpl(os, value);
    traceImpl(os, tail...);
}

inline
TraceLevel currTraceLevel()
{
   static char* env_trc_lvl = ::getenv("TRACE_LEVEL"); // can be changed runtime
   if (env_trc_lvl) {
      return static_cast<TraceLevel>(::atoi(env_trc_lvl));
   } else {
      return TraceLevel::Debug;
   }
}

} /* impl_ */


inline
void trace(TraceLevel traceLevel, const std::ostringstream &oss)
{
    if(int(impl_::currTraceLevel()) < int(traceLevel)) return;

    std::ostream &dst =
        TraceLevel::Error == traceLevel || TraceLevel::Warning == traceLevel
        ? std::cerr : std::cout;
        
    std::thread::id this_id = std::this_thread::get_id();

    dst << '[' << ::getpid() << '|' << this_id << "] " << oss.str() << '\n';
}

template <typename ...T_n>
void trace(TraceLevel traceLevel, const T_n &...tail)
{
    if(int(traceLevel) > int(impl_::currTraceLevel())) return;

    std::ostringstream ss;

    impl_::traceImpl(ss, tail...);
    trace(traceLevel, ss);
}

#define ENABLE_TRACE
#ifndef ENABLE_TRACE

#define TRACE(...)

#else

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define TRACE(traceLevel, ...) \
   do \
   { \
      trace(traceLevel, __FILENAME__, ':', __LINE__, ' ', __FUNCTION__, ' ', __VA_ARGS__); \
   } while(false)

#endif /* ENABLE_TRACE */


#ifndef ENABLE_LOG

#define LOG(...)

#else

#define LOG(logLevel, ...) \
   do \
   { \
      trace(logLevel, __VA_ARGS__); \
   } while(false)

#endif /* ENABLE_TRACE */
