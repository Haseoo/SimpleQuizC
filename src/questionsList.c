#include "headers/questions.h"

extern void endStringProperly (char *string) {
    char *spot = strstr(string, "\n");
    if (spot) {
        *spot = 0;
    }
}

size_t countNumberOfCategories(const char *filePath) {
    size_t i = 0;
    FILE *file = fopen(filePath, "r");
    if (file) {
        char tmp [200];
        memset(tmp, 0, sizeof(char) * 200);
        while (strncmp(tmp, "END_OF_LIST", 11) != 0 && !feof(file)){
            fgets(tmp, sizeof(tmp)/sizeof(tmp[0]),file);
            i++;
        }
        i -= 2;
        fclose(file);
    }
    else {
        fprintf(stderr,"Couldn't open file with list of categories: %s\n",filePath);
    }
    return i;
}

static int countNumberOfQuestions (const char *filePath) {
    int i = 0;
    FILE *file = fopen(filePath,"r");
    if (file) {
        char tmp[100];
        memset(tmp, 0, sizeof(char) * 100);
        while (strncmp(tmp, "END_OF_LIST", 11) != 0 && !feof(file)) {
            fgets(tmp, sizeof(tmp)/sizeof(tmp[0]),file);
            if (strncmp(tmp, "BEGIN_OF_QUESTION", 17) == 0)
                i++;
        }
        fclose(file);
    }
    else {
        fprintf(stderr,"Couldn't open file with list of questions: %s \n",filePath);
    }
    return i;
}


void setUsedQuestionToZero (struct category *categoriesList, const size_t NUMBER_OF_CATEGORIES) {
    for (size_t i = 0; i < NUMBER_OF_CATEGORIES; i++) {
        categoriesList[i].numberOfUsedQuestions = 0;
    }
}


int initCategoriesList (struct category *categoriesList, const char *fileName, const size_t NUMBER_OF_CATEGORIES) {
    FILE *file = fopen(fileName,"r");
    if (file && categoriesList) {
        memset(categoriesList, 0, sizeof(struct category) * NUMBER_OF_CATEGORIES);
        char textLine [100];
        fgets(textLine, sizeof(textLine)/sizeof(textLine[0]),file);
        for (size_t i = 0; i < NUMBER_OF_CATEGORIES; i++) {
            fgets(textLine, sizeof(textLine)/sizeof(textLine[0]),file);
            endStringProperly (textLine);
            char *minusKeySpot = strrchr(textLine, '-');
            if (minusKeySpot) {
                strncpy(categoriesList[i].filePath, textLine, minusKeySpot - textLine);
                strncpy(categoriesList[i].categoryName, minusKeySpot + 1, 50);
            }
            else {
                fprintf(stderr, "Niepoprawne kodowanie w pliku %s", fileName);
                return -1;
            }
            size_t numberOfQuestions = countNumberOfQuestions(categoriesList[i].filePath);
            if (numberOfQuestions != 0) {
                categoriesList[i].numberOfQuestions = numberOfQuestions;
                categoriesList[i].numberOfUsedQuestions = 0;
            }
            else {
                return -1;
            }
        }

        fclose(file);
    }
    else {
        fprintf(stderr,"Couldn't open file with list of categories: %s \n",fileName);
        return -1;
    }
    return 0;
}


size_t countMaxQuestion (struct category *categoriesList, const size_t NUMBER_OF_CATEGORIES) {
    size_t numberOfQuestions = 0;
    if (categoriesList) {
        for (size_t i = 0; i < NUMBER_OF_CATEGORIES; i++) {
            numberOfQuestions = numberOfQuestions +  categoriesList[i].numberOfQuestions;
        }
    }
    return numberOfQuestions;
}



void addToList(struct questionsList **front, size_t categoryNumber, size_t questionNumber) {
    struct questionsList *newElement = (struct questionsList*)malloc(sizeof(struct questionsList));
    if (newElement) {
        newElement->category = categoryNumber;
        newElement->question = questionNumber;
        if (*front == NULL) {
            newElement->next = NULL;
            *front = newElement;
        }
        else {
            newElement->next = *front;
            *front = newElement;
        }
    }
    else {
            fprintf(stderr,"Could't add question to list- couldn't create new element\n");
        }
}


void deleteList(struct questionsList **front) {
    while (*front != NULL) {
        struct questionsList *tmp = (*front)->next;
        free (*front);
        *front = tmp;
    }
}


static int searchQuestion(struct questionsList *front, size_t categoryNumber, size_t questonNumber) {
    if (front == NULL)
        return 1;
    while (front != NULL) {
        if (front->category == categoryNumber && front->question == questonNumber)
            return 0;
        front = front->next;
    }
    return 1;
}


static size_t getCategoryNumber(struct category *categoriesList, const size_t NUMBER_OF_CATEGORIES) {
    size_t randomCategory = 0;
    while (1) {
        randomCategory = rand() % NUMBER_OF_CATEGORIES;
        if (categoriesList[randomCategory].numberOfQuestions > categoriesList[randomCategory].numberOfUsedQuestions) {
            categoriesList[randomCategory].numberOfUsedQuestions += 1;
            break;
        }
    }
    return randomCategory;
}


size_t getQuestionNumber(struct category *categoriesList, struct questionsList *front, size_t categoryNumber) {
    size_t randomQuestion = 0;
    while (1) {
        randomQuestion = rand() % categoriesList[categoryNumber].numberOfQuestions;
        if (searchQuestion(front,categoryNumber,randomQuestion )) {
            break;
        }
    }
    return randomQuestion;
}


void getRandomQuestion(struct questionsList **front, struct category *categoriesList, const size_t NUMBER_OF_CATEGORIES) {
    size_t categoryNumber = getCategoryNumber(categoriesList, NUMBER_OF_CATEGORIES);
    size_t questionNumber = getQuestionNumber(categoriesList, *front, categoryNumber);
    addToList(front, categoryNumber, questionNumber);
}
