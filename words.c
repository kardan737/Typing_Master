#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "words.h"

char loaded_words[MAX_NOUNS][MAX_WORD_LENGTH];
int noun_count = 0;

char loaded_adjectives[MAX_ADJECTIVES][MAX_WORD_LENGTH];
int adjective_count = 0;

void load_words_from_file(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        const char* fallback_words[] = {
            "computer", "program", "keyboard", "monitor", "mouse",
            "screen", "memory", "processor", "network", "system",
            "software", "hardware", "internet", "browser", "server",
            "database", "algorithm", "function", "variable", "string"
        };
        noun_count = sizeof(fallback_words) / sizeof(fallback_words[0]);
        for (int i = 0; i < noun_count; i++) {
            strncpy(loaded_words[i], fallback_words[i], MAX_WORD_LENGTH - 1);
            loaded_words[i][MAX_WORD_LENGTH - 1] = '\0';
        }
        return;
    }

    char line[MAX_WORD_LENGTH];
    while (fgets(line, sizeof(line), f) && noun_count < MAX_NOUNS) {
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) > 0) {
            strncpy(loaded_words[noun_count], line, MAX_WORD_LENGTH - 1);
            loaded_words[noun_count][MAX_WORD_LENGTH - 1] = '\0';
            noun_count++;
        }
    }

    fclose(f);
}

void load_adjectives_from_file(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        const char* fallback_adjectives[] = {
            "digital", "analog", "electronic", "wireless", "wired"
        };
        adjective_count = sizeof(fallback_adjectives) / sizeof(fallback_adjectives[0]);
        for (int i = 0; i < adjective_count; i++) {
            strncpy(loaded_adjectives[i], fallback_adjectives[i], MAX_WORD_LENGTH - 1);
            loaded_adjectives[i][MAX_WORD_LENGTH - 1] = '\0';
        }
        return;
    }

    char line[MAX_WORD_LENGTH];
    while (fgets(line, sizeof(line), f) && adjective_count < MAX_ADJECTIVES) {
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) > 0) {
            strncpy(loaded_adjectives[adjective_count], line, MAX_WORD_LENGTH - 1);
            loaded_adjectives[adjective_count][MAX_WORD_LENGTH - 1] = '\0';
            adjective_count++;
        }
    }

    fclose(f);
}

const char* get_random_word() {
    if (noun_count == 0) {
        return "error";
    }
    return loaded_words[rand() % noun_count];
}
