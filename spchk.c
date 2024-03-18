#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <fcntl.h>

#define HASH_SIZE 127

typedef struct WordNode {
    char* word;
    struct WordNode* next;
} WordNode;

WordNode* wordMap[HASH_SIZE];

unsigned int hashWord(const char* word) {
    return (unsigned int)word[0] % HASH_SIZE;
}

WordNode* makeWordNode(char* word) {
    WordNode* node = malloc(sizeof(WordNode));
    node->word = strdup(word);
    node->next = NULL;
    return node;
}

void insertWord(char* word) {
    unsigned int idx = hashWord(word);
    WordNode* node = makeWordNode(word);
    if (!wordMap[idx] || strcmp(word, wordMap[idx]->word) < 0) {
        node->next = wordMap[idx];
        wordMap[idx] = node;
    } else {
        WordNode* curr = wordMap[idx];
        while (curr->next && strcmp(word, curr->next->word) > 0) {
            curr = curr->next;
        }
        node->next = curr->next;
        curr->next = node;
    }
}

void loadDict(const char* path) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("Error opening dictionary");
        return;
    }
    char buf[100], word[100];
    int bytesRead, wIdx = 0;
    while ((bytesRead = read(fd, buf, sizeof(buf) - 1)) > 0) {
        for (int i = 0; i < bytesRead; ++i) {
            if (!isspace(buf[i]) && buf[i] != '\n') {
                word[wIdx++] = buf[i];
            } else if (wIdx > 0) {
                word[wIdx] = '\0';
                // Insert the original word with its current capitalization
                insertWord(word);
                // Convert word to uppercase and insert
                for (int j = 0; word[j] != '\0'; j++) {
                    word[j] = toupper(word[j]);
                }
                insertWord(word);
                wIdx = 0;
            }
        }
    }
    if (bytesRead == -1) perror("Error reading dictionary");
    close(fd);
}

void processFile(const char* path);

void traverseDir(const char* basePath) {
    DIR *dir = opendir(basePath);
    if (!dir) return;

    struct dirent *dp;
    while ((dp = readdir(dir)) != NULL) {
        if (dp->d_name[0] == '.') continue;

        char path[1000];
        snprintf(path, sizeof(path), "%s/%s", basePath, dp->d_name);

        struct stat statPath;
        stat(path, &statPath);

        if (S_ISDIR(statPath.st_mode)) {
            traverseDir(path);
        } else if (strstr(dp->d_name, ".txt")) {
            processFile(path);
        }
    }
    closedir(dir);
}

void reportSpellingError(const char* word, const char* path, int line, int col, bool isCommaBefore) {
    if (isCommaBefore) {
        printf("Spelling error in '%s' at line %d, column %d: ',%s' no space between word and comma.\n",
               path, line, col, word);
    } else {
        printf("Spelling error in '%s' at line %d, column %d: '%s' inccorect spelling.\n",
               path, line, col, word);
    }
}

bool matchesCapitalizationRules(const char* dictWord, const char* inputWord) {
    // Exact match
    if (strcmp(dictWord, inputWord) == 0) return true;

    // Convert dictionary word to uppercase for comparison
    char upperDictWord[strlen(dictWord) + 1];
    for (int i = 0; dictWord[i] != '\0'; i++) {
        upperDictWord[i] = toupper(dictWord[i]);
    }
    upperDictWord[strlen(dictWord)] = '\0';

    // Compare uppercase dictionary word with input word
    if (strcmp(upperDictWord, inputWord) == 0) return true;
   
    // Check if the first letter of the dictionary word is uppercase
    if (isupper(dictWord[0])) {
        // Compare the entire dictionary word (with the first letter capitalized) with the input word
        if (strcasecmp(dictWord, inputWord) == 0) {
            return true;
        }
    }

    // Initial capital or dictionary word has internal capitals
    for (int i = 0; dictWord[i] != '\0'; i++) {
        if (islower(dictWord[i])) {
            if (strcasecmp(dictWord, inputWord) == 0) return true;
            break;
        }
    }

    return false;
}

WordNode* findWord(const char* word) {
    unsigned int idx = hashWord(word);
    for (WordNode* curr = wordMap[idx]; curr != NULL; curr = curr->next) {
        if (matchesCapitalizationRules(curr->word, word)) {
            return curr; // Word found with acceptable capitalization
        }
    }
    return NULL; // Word not found or unacceptable capitalization
}

void checkWord(const char* word, const char* path, int line, int col, bool isCommaBefore) {
    // Keep the existing logic that checks for comma conditions.
    // Use the updated findWord function to check for the word.
    if (isCommaBefore || !findWord(word)) {
        reportSpellingError(word, path, line, col, isCommaBefore);
    }
}

bool isAlphaOrHyphen(char c) {
    return isalpha(c) || c == '\'' || c == '-';
}

void cleanAndCheckWord(char* word, const char* path, int line, int col, bool isCommaBefore) {
    // No need to trim starting non-alphabetic characters anymore since we handle hyphens and commas differently now
    
    // Trim trailing non-alphabetic characters
    char* end = word + strlen(word) - 1;
    while (end > word && !isAlphaOrHyphen(*end)) end--;
    *(end + 1) = '\0';

    // Check if the word is directly preceded by a comma without a space
    if (isCommaBefore || !findWord(word)) {
        reportSpellingError(word, path, line, col, isCommaBefore);
    }
}

void extractWords(const char* path) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return;
    }

    char buf[1024], word[256];
    int bytesRead, wIdx = 0, line = 1, col = 1;
    bool inHyphenatedWord = false;
    char prevChar = ' ';
    bool isCommaBefore = false; // Initialize isCommaBefore
    while ((bytesRead = read(fd, buf, sizeof(buf) - 1)) > 0) {
        for (int i = 0; i < bytesRead; ++i) {
            if (isAlphaOrHyphen(buf[i]) || (buf[i] == '\'' && wIdx > 0)) { // Include hyphen and apostrophe in word
                if (wIdx < sizeof(word) - 1) {
                    word[wIdx++] = buf[i];
                }
                if (buf[i] == '-') inHyphenatedWord = true; // Set flag if a hyphen is encountered
            } else {
                if (wIdx > 0) {
                    word[wIdx] = '\0';
                    cleanAndCheckWord(word, path, line, col - wIdx, isCommaBefore);
                    wIdx = 0;
                }
                if (buf[i] == '\n') {
                    line++;
                    col = 0;
                    isCommaBefore = false; // Reset isCommaBefore at the beginning of each line
                } else if (buf[i] == ',') {
                    isCommaBefore = true; // Set isCommaBefore if a comma is encountered
                } else {
                    isCommaBefore = false; // Reset isCommaBefore if a non-comma character is encountered
                }
                inHyphenatedWord = false; // Reset flag at the end of a word
            }
            prevChar = buf[i]; // Update previous character
            col++;
            if (inHyphenatedWord && !isAlphaOrHyphen(buf[i])) inHyphenatedWord = false; // End of hyphenated word
        }
    }
    if (bytesRead == -1) perror("Error reading file");
    close(fd);
}

void processFile(const char* path) {
    printf("Processing: %s\n", path);
    extractWords(path);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <dict_path> <file_path>...\n", argv[0]);
        return 1;
    }
    memset(wordMap, 0, sizeof(wordMap));
    loadDict(argv[1]);

    struct stat statPath;
    for (int i = 2; i < argc; i++) {
        stat(argv[i], &statPath);
        if (S_ISDIR(statPath.st_mode)) {
            traverseDir(argv[i]);
        } else if (S_ISREG(statPath.st_mode)) {
            processFile(argv[i]);
        } else {
            printf("Skipping unsupported: %s\n", argv[i]);
        }
    }
    return 0;
}