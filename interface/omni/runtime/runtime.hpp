#ifndef OMNI_RUNTIME_RUNTIME_HPP
#define OMNI_RUNTIME_RUNTIME_HPP

#ifdef _MSC_VER
#   ifdef omni_runtime_EXPORTS
#       define OMNI_RUNTIME_API __declspec (dllexport)
#   else
#       define OMNI_RUNTIME_API __declspec (dllimport)
#   endif
#else
#   define OMNI_RUNTIME_API
#endif


#endif
