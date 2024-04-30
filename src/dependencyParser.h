#ifndef DEPENDENCY_PARSER_H
#define DEPENDENCY_PARSER_H

#include "stdbool.h"

typedef struct sVersion
{
    int Major;
    int Minor;
    int Bugfix;
}sVersion_t;

typedef struct sRequirement
{
    char name[256];
    sVersion_t version;
    char source[256];
    bool moreFollows;
}sRequirement_t;

typedef struct sDependencyModel
{
    char name[265];
    sVersion_t version;
    sRequirement_t requirements[256];
    unsigned int requirementCount;
} sDependencyModel_t;



sDependencyModel_t parseDependencyFile(char* filename);


#endif