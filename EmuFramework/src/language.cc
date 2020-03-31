#include "emuframework/language/language.hh"
#include <string>

char *get_local_language(char *key) {
    if (language.count(key) > 0) {
        return const_cast<char *>(language[key].c_str());
    } else {
        return key;
    }
}