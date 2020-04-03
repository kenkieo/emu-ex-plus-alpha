#include "imagine/language/language.hh"
#include <string>

char *get_local_language(char *key) {
    if (language_zh.count(key) > 0) {
        return const_cast<char *>(language_zh[key].c_str());
    } else {
        return key;
    }
}