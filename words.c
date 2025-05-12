#include <stdlib.h>
#include "words.h"

const char* get_random_word() {
    static const char* words[] = {
        "computer", "program", "keyboard", "monitor", "mouse",
        "screen", "memory", "processor", "network", "system",
        "software", "hardware", "internet", "browser", "server",
        "database", "algorithm", "function", "variable", "string"
    };
    return words[rand() % 20];
}
