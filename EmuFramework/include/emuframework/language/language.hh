//防止被重复引用
#pragma once;
#include "emuframework/language/language_zh.hh"
#include <string>
#include <map>

#ifndef LANGUAGE
#define LANGUAGE
#ifdef __cplusplus
extern "C" {
#endif

char *get_local_language(char *);


#ifdef __cplusplus
}
#endif
#endif
