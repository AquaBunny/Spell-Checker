/*******************************************************************************
  Daniel Bachler
  CS120
  4/15/16
  spellcheck.c
  Desc:
	A spellchecker function with several helper functions 
	spell_check causes a stack dump, all other functions work
*******************************************************************************/
#include <string.h> /*                                     */
#include <stdio.h>  /* FILE                                */

/* FILE_OK, FILE_ERR_OPEN, WORD_OK, WORD_BAD, LONGEST_WORD */
#include "spellcheck.h"


void wordCutter(char *word) {
	int i = 0;
	while(word[i] != '\n' && word[i] != 0) {
		++i;
	}
	word[i] = 0;
}

/*converts given string into all uppercase*/
char *mystrupr(char *string) {
	int i, length = strlen(string);
	for(i = 0; i < length; ++i) {
		if(string[i] >= 'a' && string[i] <= 'z') {
			string[i] -= 32;
		}
	}
	return string;
}

/*Gives info about a given dictionary*/
int info(const char *dictionary, struct DICTIONARY_INFO *dinfo) {
	int count = 0, shortest = LONGEST_WORD, longest = 0, length;
	char line[LONGEST_WORD];
	FILE *dictFile;
	dictFile = fopen(dictionary, "rt");
	if(dictFile != NULL) {
		do {
			if(fgets(line, LONGEST_WORD, dictFile) != NULL) {
				wordCutter(line);
				length = strlen(line);
				if(length < shortest)
					shortest = length;
				if(length > longest) 
					longest = length;
				if(length > 1)
					count++;
				if(feof(dictFile))
					break;
			} else {
				break;
			}
		} while(line != NULL);
		dinfo->shortest = shortest;
		dinfo->longest = longest;
		dinfo->count = count;
		fclose(dictFile);
		return FILE_OK;
	}
	return FILE_ERR_OPEN;
}

/*Checks if a given word is in the dictionary*/
int spell_check(const char *dictionary, const char *word) {
	FILE *dictFile;
	char line[LONGEST_WORD];
	char word2[LONGEST_WORD];
	dictFile = fopen(dictionary, "rt");
	stpcpy(word2, word);
	wordCutter(word2);
	mystrupr(word2);
	if(dictFile != NULL) {
		while (fgets(line, LONGEST_WORD, dictFile) != NULL) {
			wordCutter(line);
			mystrupr(line);
			if(strcmp(line, word2) == 0) {
				return WORD_OK;
			}
			if(feof(dictFile))
				break;
		} 
		fclose(dictFile);
		return WORD_BAD;
	}
	
	return FILE_ERR_OPEN;
}

/*Counts the amount of words in the dictionary with a length from 0 to 
count*/
int word_lengths(const char *dictionary, int lengths[], int count) {
	FILE *dictFile;
	char line[LONGEST_WORD];
	int length;
	dictFile = fopen(dictionary, "rt");
	
	lengths[0] = 0;
	if(dictFile != NULL) {	
		while(fgets(line, LONGEST_WORD, dictFile) != NULL) {
			wordCutter(line);
			length = strlen(line);
			if(length <= count)
				lengths[length]++;
		} 
		fclose(dictFile);
		return FILE_OK;
	}
	return FILE_ERR_OPEN;
}

/*counts the number of words that start with the given char*/
int words_starting_with(const char *dictionary, char letter) {
	FILE *dictFile;
	char line[LONGEST_WORD];
	int imABoolean = 1, count = 0;
	
	dictFile = fopen(dictionary, "rt");
	if(dictFile != NULL) {
		if(letter >= 'a' && letter <= 'z') {
			letter -= 32;
		}
		while(imABoolean) {
			if(fgets(line, LONGEST_WORD, dictFile)) {
				mystrupr(line);
				if(line[0] == letter)
					++count;
			} else {
				imABoolean = 0;
			}
		}
		fclose(dictFile);
		return count;
	}	
	return FILE_ERR_OPEN;
}