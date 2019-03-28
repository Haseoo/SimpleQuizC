#ifndef QUESTIONS_H_INCLUDED
#define QUESTIONS_H_INCLUDED

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct category {
    char filePath[50]; 
    char categoryName[50]; 
    size_t numberOfQuestions;
    size_t numberOfUsedQuestions; 
};


struct questionsList {
    size_t category; 
    size_t question; 
    struct questionsList *next; 

};
extern void endStringProperly (char *string);
size_t countNumberOfCategories(const char *fileName);
void setUsedQuestionToZero (struct category *categoriesList, const size_t NUMBER_OF_CATEGORIES);
size_t countMaxQuestion (struct category *categoriesList, const size_t NUMBER_OF_CATEGORIES);
int initCategoriesList (struct category *categoriesList, const char *fileName, const size_t NUMBER_OF_CATEGORIES);
void addToList(struct questionsList ** ront, size_t categoryNumber, size_t questionNumber);
void deleteList(struct questionsList **front);
size_t getQuestionNumber(struct category *categoriesList, struct questionsList *front, size_t categoryNumber);
void getRandomQuestion(struct questionsList **front, struct category *categoriesList, const size_t NUMBER_OF_CATEGORIES);

#endif // QUESTIONS_H_INCLUDED
