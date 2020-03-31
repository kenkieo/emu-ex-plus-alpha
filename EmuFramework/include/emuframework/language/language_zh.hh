//防止被重复引用
#pragma once;
#include <string>
#include <map>

#ifndef LANGUAGE_ZH
#define LANGUAGE_ZH
#ifdef __cplusplus
extern "C" {
#endif

const std::map<std::string, std::string> language_zh = {
    {"Load Game", "加载游戏"}
};


#ifdef __cplusplus
}
#endif
#endif
