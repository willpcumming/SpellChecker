# SpellChecker
Below is a listed description of our spellchecker program

hashWord- takes a word as input and calculates its hash value based on the first character of the word. It is used to determind the index in our hash table "wordMap" where the word will be stored to be searched

makeWordNode- Allocates memory for each new WordNode which are structs containing the word and the location of the next word in the linked list

insertWord- inserts a word into the hash table ensuring the words are inserted in lexicographical order within their linked lists. It uses makeWordNode to create the new node then finds the correct position for the word

loadDict- Opens and reads a dictionary file specified as one of the args. It inserts both the original word case specific and its uppercase version into the hashtable using insertWord. This is used later to compare with text files for spellchecking

traverseDir- this function is how we navigate the directories starting from the given path in args. Each .txt file it encounters calls processFile to perform the main function of the code, spellchecking. 

reportSpellingError- this prints an error message indicating a spelling error in a file. It provides the file path, line number, column number, and mispelled word. 

matchesCapitalizationRules- this function is a boolean meant to help the code adhere to the capitalization rules given in the assignment pdf

findWord- Searches for a specific word in the hashtable, it considers capitalization using the afformentioned boolean function to check if the word matches a word from the dictionary and returns a pointer to the WordNode in the hash table if found otherwise it returns NULL.

isAlphaorHyphen- determins whether a character is alphabetical or a special character. 

cleanAndCheckWord- trims the trailing non-alphabetic characters then checks if the word is spelled correctly using findWord and reports any errors with reportSpellingError

extractWords- Reads a given txt file extracts and checks the words spelling. It also handles hyphenated words, tracks lie and column numbers for error reporting, and handles comma spacing errors

processFile- This file initiates the spell check process for a single file it then prints a message indicating the file being processed and calls extractWords to perform the spell checking

main- this function initializes the hashtable, loads the dictionary and processes each file or directory specified in the args.
