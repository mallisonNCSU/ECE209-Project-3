/*
Author: Michelle Allison
Description: This program autocompletes a word in the process of being written by predicting what the word will be using two things: the letters of the word already written, and the word immediately before the word being written, if there is one.

I prefer the Virtual Box for grading.
*/

#include "autocomplete.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct word_list {
	char word[100];
	struct word_list * next;
	struct word_list * prior;
}word_List;
word_List * head = NULL; //head of word list
int numDict = 0; //number of dictionary entries
int numContext = 0; //number of context entries
int memUsed = 0; //amount of memory used

// Add the words in file specified by filename to the
// auto-complete engine. This may be called multiple
// times with different filenames.  The words in the new
// file are appended to the existing list.  Duplicates are not 
// allowed.
// Argument: filename - file with words that need to be added
//                      to the table
// Return value: 0: success
//               1: bad file name
int AutoComplete_LoadDictionary(const char *filename)
{
	char w[100] = {0}; // scanf will use this to take a word from the dictionary document and give it to AutoComplete_AddWord
	FILE* file = fopen(filename,"r");
	if(file==NULL) {
		return 1;
	}
	while((fscanf(file,"%s",w)) != EOF) {
		AutoComplete_AddWord(w);	
	}
	return 0;
}

// Add a word to the autocomplete dictionary.  If the word is already 
// present, it simply does nothing.
void AutoComplete_AddWord(const char *word)
{
	static int count = 0; //used for checking if it's the first added word or not
	word_List * tmp = head; //used to check if the word is already added to the list
	while(tmp != NULL) {
		if((strcmp(tmp->word,word))==0) {
			return;
		}
		tmp = tmp->next;
	}
	word_List * ptr = malloc(sizeof(word_List)); //space for new word!
	memUsed += sizeof(word_List);
	numDict++;
	if(count == 0) { //if this is the first entry in the word list, put it in head's spot
		head = ptr;
		strcpy(head->word, word);
		head->next = NULL;
		head->prior = NULL;
		count++;
	} else { //if this isn't the first entry, put new word in front and assign head to it
		ptr->next = head;
		strcpy(ptr->word,word);
		ptr->prior = NULL;
		head = ptr;
	}
	return;
}

// Add context for a word to the autocomplete dictionary.  If the word
// is not already present, then add it. If the word is present, add 
// prior to its context. If prior is already present, do not add it a 
// second time.
void AutoComplete_AddContext(const char *word, const char * prior)
{
	AutoComplete_AddWord(word); //add word if it isn't already added

	word_List * tmp = head; //tmp is used for traversing the list of added words
	word_List * tmpp; //tmpp is used for going to the end of the context list and adding prior (I realize now I should've just added it to the front like I did with AddWord but it works) as well as checking if the prior word is already there
	while(tmp != NULL) { // find word parameter in the list so it can add prior to it
		if(strcmp(word,tmp->word) == 0) {
			tmpp = tmp->prior;
			while(tmpp != NULL) { //check if prior is already there; if it is, leave
				if((strcmp(tmpp->word,word))==0) {
					return;
				}
				tmpp = tmpp->prior;
			}
			word_List * ptr	= malloc(sizeof(word_List));
			memUsed += sizeof(word_List);
			strcpy(ptr->word,prior); //set up prior word's entry in list
			ptr->next = NULL;
			tmpp = tmp;
			while((tmpp->prior) != NULL) { //go to end of context list and add prior word
				tmpp = tmpp->prior;
			}
			tmpp->prior = ptr;
			numContext++;
			break;

		}
		tmp = tmp->next;
	}

	return;
}


// Return an integer specifying the total amount of memory
// used by the auto-completer.
// Arguments: None
// Return value: integer that specifies in bytes the amount
//               of memory used, including global/static variables.
int AutoComplete_TotalMemoryUsed()
{
	return memUsed;
}

// Return an integer specifying the total number of words
// added to the auto-completer.
int AutoComplete_NumberOfDictionaryWords()
{
	return numDict;
}

// Return an integer specifying the total number of words
// used for semantic context.
int AutoComplete_NumberOfContextWords()
{
	return numContext;
}

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
int AutoComplete_Suggestion(const char *partial_word, const char *prior, const char *completions[], int size_of_array)
{
	word_List * p; // used for checking if the prior parameter matches any prior words in the found word's list
	int cmp = 0; //comparison value--if it's 0, that means it's a match!
	int matches = 0; //number of matches in completions array, to be returned
	word_List * tmp = head; //used for traversing word list
	while((tmp != NULL) && (matches < size_of_array)) {
		cmp = strncmp(partial_word,tmp->word,strlen(partial_word)); //check to see if the partial word matches the beginning of a word on the list
		if(cmp == 0) { //if it matches, check to see if any prior words for the matches word fit the parameter given
			p = tmp->prior;
			if((p == NULL) || ((strlen(prior) < 1))) { //if there is no prior word, or if there was no parameter for prior given
				completions[matches] = tmp->word;
				matches++;
			} else {
				while(p != NULL){ //go along prior word list and check each one
					if((strcmp(p->word,prior)) == 0) {
						completions[matches] = tmp->word; //there's a match! add the suggested word to the array
						matches++;
						}
					p = p->prior;
				}
			}
		}
		tmp = tmp->next;
	}

	return matches;
}









