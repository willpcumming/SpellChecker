

Test Cases

Test 1: Basic Spellchecking (test1.txt)
Description: This test verifies basic spellchecking capabilities, including handling of capital letters and punctuation.
Expected Behavior: spchk should correctly identify all words as spelled correctly, given a comprehensive dictionary. The test includes variations in capitalization to ensure case-insensitivity except for proper handling of initial capitals.

Test 2: Proper Nouns and Capitalization (test2.txt)
Description: Tests proper noun recognition and sensitivity to initial capitalization.
Expected Behavior: Proper nouns like "MacDonald" should be recognized in their exact form or in all uppercase. However, "macdonald" should not be considered correct unless explicitly included in the dictionary.

Test 3: Punctuation Handling (test3.txt)
Description: Focuses on spchk's ability to handle punctuation correctly, including quotes, parentheses, and commas.
Expected Behavior: spchk should ignore punctuation at the beginning and end of words, allowing for correct identification of words. Commas without a following space should result in an error.

Test 4: Case Sensitivity (test4.txt)
Description: Evaluates the spellchecker's handling of case sensitivity and variations in capitalization.
Expected Behavior: Words should match their dictionary entries in a case-insensitive manner, except when a word is capitalized differently in the dictionary.

Test 7: Quotation Marks (test7.txt)
Description: Tests the handling of words enclosed in quotation marks.
Expected Behavior: Words within quotes should be recognized correctly, with the quotation marks themselves being ignored.

Test 8: Special Characters (test8.txt)
Description: This test checks how spchk deals with words that contain special characters not present in the basic ASCII character set.
Expected Behavior: Given that spchk operates on ASCII text, it should ignore diacritical marks, treating "cafe" and "café" as distinct words unless both forms are present in the dictionary.

Test 9: Technical Terms (test9.txt)
Description: Evaluates the spellchecker's performance with technical terms and neologisms.
Expected Behavior: Technical terms should be checked against the dictionary, and any unrecognized terms should be reported as spelling errors.

Test 10: Common Misspellings (test10.txt)
Description: Contains commonly misspelled words to test the spellchecker's effectiveness.
Expected Behavior: Each misspelled word should be identified as incorrect, assuming the correct spellings are present in the dictionary.

Test 14: Acronyms and Abbreviations (test14.txt)
Description: Tests the recognition of acronyms and abbreviations, which are often written in all caps.
Expected Behavior: Acronyms and abbreviations should be recognized based on their presence in the dictionary, regardless of capitalization.

Test 15: Complex Sentences (test15.txt)
Description: A complex sentence with multiple punctuation marks and a high-level vocabulary to test the spellchecker's robustness.
Expected Behavior: spchk should accurately identify any misspelled words while correctly handling punctuation and capitalization.
