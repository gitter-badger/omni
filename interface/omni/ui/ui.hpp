#ifndef OMNI_UI_UI_HPP
#define OMNI_UI_UI_HPP

#ifdef omniui_BUILD_LIB
#define OMNI_UI_API __declspec (dllexport)
#else
#define OMNI_UI_API __declspec (dllimport)
#endif

#endif