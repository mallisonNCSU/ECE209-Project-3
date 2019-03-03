// Add the words in file specified by filename to the
// auto-complete engine. This may be called multiple
// times with different filenames.  The words in the new
// file are appended to the existing list.  Duplicates are not 
// allowed.
// Argument: filename - file with words that need to be added
//                      to the table
// Return value: 0: success
//               1: bad file name
int AutoComplete_LoadDictionary(const char *filename);

// Add a word to the autocomplete dictionary.  If the word is already 
// present, it simply does nothing.
void AutoComplete_AddWord(const char *word);

// Add context for a word to the autocomplete dictionary.  If the word
// is not already present, then add it. If the word is present, add 
// prior to its context. If prior is already present, do not add it a 
// second time.
void AutoComplete_AddContext(const char *word, const char * prior);


// Return an integer specifying the total amount of memory
// used by the auto-completer.
// Arguments: None
// Return value: integer that specifies in bytes the amount
//               of memory used, including global/static variables.
int AutoComplete_TotalMemoryUsed();

// Return an integer specifying the total number of words
// added to the auto-completer.
int AutoComplete_NumberOfDictionaryWords();

// Return an integer specifying the total number of words
// used for semantic context.
int AutoComplete_NumberOfContextWords();

// For a given partial word, return no more than size possible
// completions.
// Arguments: prompt - the beginning of a word to complete
//            prior -  a string with the word that came before it; it 
//                     may be NULL, and if so, no context is needed 
//                     for a match. Otherwise, the prior must be part 
//                     of the context of all returned words.
//            completions - an array to store the completions inside
//            size_of_array - size of the completions array
//
// Return value: number of completions returned in the array; 0 means
//               nothing was returned
//
int AutoComplete_Suggestion(const char *partial_word, const char *prior, const char *completions[], int size_of_array);
