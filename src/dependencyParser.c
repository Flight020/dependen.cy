/*----------------------------------------------*/
/*---------Includes-----------------------------*/
/*----------------------------------------------*/
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

#include "dependencyParser.h"

/*----------------------------------------------*/
/*---------Defines and Constants----------------*/
/*----------------------------------------------*/
const char cNameEntry[] = "name = ";
const char cVersionEntry[] = "version = ";
const char cRequireEntry[] = "require = ";
/*----------------------------------------------*/
/*---------Types--------------------------------*/
/*----------------------------------------------*/
typedef enum 
{
  empty,
  name,
  version,
  config,
  req,
  invalid
} eParseType_t;

typedef struct sParseResult
{
  eParseType_t resultType; 
  bool moreFollows;
  char name[256];
  sVersion_t version;
  sRequirement_t requirement;
} sParseResult_t;

/*----------------------------------------------*/
/*---------Static Data--------------------------*/
/*----------------------------------------------*/
static char lineBuffer[256] = {0};

/*----------------------------------------------*/
/*---------Static Functions---------------------*/
/*----------------------------------------------*/
static sParseResult_t processLine(char* line, bool consecutive);
static sRequirement_t parseRequirement(char*);
static char* parseVersion(char* character, sVersion_t* version);
static bool isLetter(char*);
static bool isNumber(char*);
/*----------------------------------------------*/
/*---------Interface functions------------------*/
/*----------------------------------------------*/
sDependencyModel_t parseDependencyFile(char* filename)
{
  int lineCounter = 0;
  sDependencyModel_t fdep = {0};
  bool parsedName = false;
  bool parsedVersion = false;

  sParseResult_t result = {0};

  //Try to open the depency file
  FILE* depFile = fopen(filename, "r");
  if(depFile == NULL)
  {
    printf("Could not open dependency file %s", filename);
    return fdep;
  }

	//Loop through it line by line
	char* Line;
  while(1)
  {
    lineCounter++;
    fgets(&lineBuffer[0], sizeof(lineBuffer), depFile);
		if(feof(depFile))
		{
			printf("Enf of %s reached\n", filename);
			return fdep;
		}
    result = processLine(&lineBuffer[0], result.moreFollows);
    if(result.resultType == invalid)
    {
      printf("Invalid Line: %d!\n", lineCounter);
      return fdep;
    }

    if(result.resultType == name)
    {
      if(parsedName)
      {
        printf("Got multiple names!\n");
        return fdep;
      }
      strncpy(fdep.name, result.name, sizeof(fdep.name));
      parsedName = true;
      continue;
    }

    if(result.resultType == version)
    {
      if(parsedVersion)
      {
        printf("Got multiple versions!\n");
        return fdep;
      }
      fdep.version = result.version;
      parsedVersion = true;
      continue;
    }

    if(result.resultType == req)
    {
      if(fdep.requirementCount > 255)
      {
        printf("Too many requirements!\n");
      }
      fdep.requirements[fdep.requirementCount] = result.requirement;
      fdep.requirementCount++;
      continue;
    }
  }
  return fdep;
}

/*----------------------------------------------*/
/*---------Static functions---------------------*/
/*----------------------------------------------*/
static sParseResult_t processLine(char* line, bool consecutive)
{
  sParseResult_t parseResult = {0};
  char* parsedCharacter = line;
  while(*parsedCharacter == 0x20 || *parsedCharacter == 0x09)
  {
    parsedCharacter++;
  }

  //Test for line ending
  if(*parsedCharacter == 0x0A || *parsedCharacter == 0x0D || *parsedCharacter == '#')
  {
    parseResult.resultType = empty;
    parseResult.moreFollows = consecutive; //If consecutive, next is too
    return parseResult;
  }

  if(consecutive)
  {
    printf("Consecutive Req!\n");
    sRequirement_t requirement = parseRequirement(parsedCharacter);
    if(strlen(&requirement.name[0]) > 0)
    {
      parseResult.requirement = requirement;
      parseResult.resultType = req;
      parseResult.moreFollows = requirement.moreFollows;
    }
    else
    {
      printf("Invalid Consecutive Req!\n");
      parseResult.resultType = invalid;
    }
    return parseResult;
  }

  //Test for valid entry types
  int parseOffset = 0;
  if( (*parsedCharacter == 'n') /*name*/ || 
      (*parsedCharacter == 'v') /*version*/ || 
      (*parsedCharacter == 'r') /*requirement*/)
  {
    printf("Entry candidate found!\n");
    //Test for name entry
    if(strstr(parsedCharacter, &cNameEntry[0]) == parsedCharacter)
    {
      printf("Name entry found!\n");
      parseResult.resultType = name;
      parsedCharacter += strlen(cNameEntry);

      if(*parsedCharacter != '"')
      {
        printf("Name does not start with \"");
        parseResult.resultType = invalid;
        return parseResult;
      }

      parsedCharacter++;

      //Test if the requirements string name part start with a valid letter
      if(!isLetter(parsedCharacter))
      {
        printf("Invalid Character in Name: %c!\n", *parsedCharacter);
        parseResult.resultType = invalid;
        return parseResult;
      }
      
      //Parse the name string of the requirement
      int parseCounter = 0;
      do
      {
        parseResult.name[parseCounter] = *parsedCharacter;
        parseCounter++;
        parsedCharacter++;
        if(parseCounter > sizeof(parseResult.name))
        {
          printf("Name too long!\n");
          parseResult.resultType = invalid;
          return parseResult;
        }
      } while (isLetter(parsedCharacter) || *parsedCharacter == '_');
      parseResult.name[parseCounter] = 0; //Terminate
      if(*parsedCharacter != '"')
      {
        printf("Invalid Ending!\n");
        parseResult.resultType = invalid;
        return parseResult;
      }
    }
    //Test for version entry
    else if(strstr(parsedCharacter, cVersionEntry) == parsedCharacter)
    {
      parsedCharacter += strlen(cVersionEntry);
      //Parse version and test if successfull and that the delimitter is expected
      if(!parseVersion(parsedCharacter, &parseResult.version) || (*parsedCharacter != '"'))
      {
        parseResult.resultType = invalid;
        return parseResult;
      }
      parseResult.resultType = version;
      
    }
    else if(strstr(parsedCharacter, cRequireEntry) == parsedCharacter)
    {
      parseResult.resultType = req;
      parseOffset = strlen(cRequireEntry);
      sRequirement_t requirement = parseRequirement(parsedCharacter + parseOffset);
      if(strlen(requirement.name) > 0)
      {
        parseResult.resultType = req;
        parseResult.requirement = requirement;
        parseResult.moreFollows = requirement.moreFollows;
      }
      else
      {
        parseResult.resultType = invalid;
      }
      return parseResult;
    }
    else
    {
      parseResult.resultType = invalid;
      return parseResult;
    }

  }
  return parseResult;
}

/// @brief Parses a string of type "name/X.Y.Z@SourcePath"
/// @param character char*, position from where to start parsing
/// @return struct of type sParseResult_t specifying result
static sRequirement_t parseRequirement(char* character)
{
  sRequirement_t requirement = {0};
  
  //Check for valid start of requirements string
  if(*character != '"')
  {
    printf("Start of requirement is not \": %c\n", *character);
    memset(&requirement, 0, sizeof(requirement));
    return requirement;
  }
  character++;
  
  //Test if the requirements string name part start with a valid letter
  if(!(isLetter(character) || isNumber(character)))
  {
    printf("Invalid character at start of Rquirement: %c\n", *character);
    memset(&requirement, 0, sizeof(requirement));
    return requirement;
  }
  
  //Parse the name string of the requirement
  int parseCounter = 0;
  do
  {
    requirement.name[parseCounter] = *character;
    parseCounter++;
    character++;
    if(parseCounter > sizeof(requirement.name))
    {
      printf("Parsed Requierement name too long!");
      memset(&requirement, 0, sizeof(requirement));
      return requirement;
    }
  } while (isLetter(character) || *character == '_' || isNumber(character));
  requirement.name[parseCounter] = 0; //Terminate

  //Test if name string ends with expected separator and the version starts with a number
  if((*character != '/') || !isNumber(character+1))
  {
    printf("Invalid name and version separator: %c\n", *character);
    memset(&requirement, 0, sizeof(requirement));
    return requirement;
  }
  
  character++;
  sVersion_t version;
  printf("Parsing Version\n");
  character = parseVersion(character, &version);
  if(character == NULL)
  {
    printf("Parse Version failed!\n");
    memset(&requirement, 0, sizeof(requirement));
    return requirement;
  }
  
  //Test if char after version is expected delimitter
  if(*character != '@')
  {
    printf("Invalid separator between version and source: %c\n", *character);
    memset(&requirement, 0, sizeof(requirement));
    return requirement;
  }
  character++;

  //Parse source string
  parseCounter = 0;
  do
  {
    requirement.source[parseCounter] = *character;
    parseCounter++;
    character++;
    if(parseCounter > sizeof(requirement.source))
    {
      memset(&requirement, 0, sizeof(requirement));
      return requirement;
    }
  } while (*character != '"');


  character++;
  if(*character == ',')
  {
    requirement.moreFollows = true;
  }
  return requirement;
}

/// @brief Returns if a char pointed to by character is a letter from the alphabet
/// @param character char*
/// @return true if letter, false if not
static bool isLetter(char* character)
{
  if(((*character >= 'A') && (*character <= 'Z')) || ((*character >= 'a') && (*character <= 'z')))
  {
    return true;
  }
  else
  {
    return false;
  }
}

/// @brief Returns if a char pointed to by character is a number between 0 and 9
/// @param character 
/// @return true if yes, false else
static bool isNumber(char* character)
{
  if((*character >= '0') && (*character <= '9'))
  {
    return true;
  }
  else
  {
    return false;
  }
}

static char* parseVersion(char* character, sVersion_t* version)
{
  //Parse Major Version
      char buffer[256] = {0};
      int parseCounter = 0;
      do
      {
        buffer[parseCounter] = *character;
        parseCounter++;
        character++;
        if(parseCounter > sizeof(buffer))
        {
          version = NULL;
          return NULL;
        }
      } while (isNumber(character));
      buffer[parseCounter] = 0; //Terminate

      //Test if Major Version string ends with expected dot
      if(*character != '.')
      {
        version = NULL;
        return NULL;
      }
      version->Major = atoi(&buffer[0]);
      memset(&buffer[0], 0, sizeof(buffer));

      character++;

      //Parse Minor Version
      parseCounter = 0;
      do
      {
        buffer[parseCounter] = *character;
        parseCounter++;
        character++;
        if(parseCounter > sizeof(buffer))
        {
          version = NULL;
          return NULL;
        }
      } while (isNumber(character));
      //Test if Minor Version string ends with expected dot
      if(*character != '.')
      {
        version = NULL;
        return NULL;
      }
      version->Minor = atoi(&buffer[0]);
      memset(&buffer[0], 0, sizeof(buffer));

      character++;

      //Parse Bugfix Version
      parseCounter = 0;
      do
      {
        buffer[parseCounter] = *character;
        parseCounter++;
        character++;
        if(parseCounter > sizeof(buffer))
        {
          version = NULL;
          return NULL;
        }
      } while (isNumber(character));
      
      version->Bugfix = atoi(&buffer[0]);
      return character;
}