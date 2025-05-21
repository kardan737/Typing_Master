#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "words.h"

char loaded_words[MAX_NOUNS][MAX_WORD_LENGTH];
int noun_count = 0;

char loaded_adjectives[MAX_ADJECTIVES][MAX_WORD_LENGTH];
int adjective_count = 0;


static void load_strings_from_file_into_array(const char* filename, char array[][MAX_WORD_LENGTH], int max_count, int* count) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        *count = 0;
        return;
    }

    *count = 0; 
    char line[MAX_WORD_LENGTH];
    while (fgets(line, sizeof(line), f) && *count < max_count) {
        size_t len = strlen(line);
        if (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
            line[len-1] = '\0';
            len--;
        }
        if (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
            line[len-1] = '\0';
        }
        
        if (strlen(line) > 0) {
            strncpy(array[*count], line, MAX_WORD_LENGTH - 1);
            array[*count][MAX_WORD_LENGTH - 1] = '\0';
            (*count)++;
        }
    }

    fclose(f);
}

void load_words_from_file(const char* filename) {
    load_strings_from_file_into_array(filename, loaded_words, MAX_NOUNS, &noun_count);
    
    if (noun_count == 0) {
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
    }
}

void load_adjectives_from_file(const char* filename) {
    load_strings_from_file_into_array(filename, loaded_adjectives, MAX_ADJECTIVES, &adjective_count);

    if (adjective_count == 0) {
         const char* fallback_adjectives[] = {
            "digital", "analog", "electronic", "wireless", "wired"
        };
        adjective_count = sizeof(fallback_adjectives) / sizeof(fallback_adjectives[0]);
        for (int i = 0; i < adjective_count; i++) {
            strncpy(loaded_adjectives[i], fallback_adjectives[i], MAX_WORD_LENGTH - 1);
            loaded_adjectives[i][MAX_WORD_LENGTH - 1] = '\0';
        }
    }
}

void cleanup_words() {
}

const char* get_random_word() {
    if (noun_count == 0) {
        return "error";
    }
    return loaded_words[rand() % noun_count];
}
