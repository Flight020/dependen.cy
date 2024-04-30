
#include "dependencyParser.h"
#include "stdio.h"


int main()
{
  //Parse .cy file
  sDependencyModel_t model = parseDependencyFile("../dependen.cy");
  printf("Name: %s \n", model.name);
  printf("Version: %d.%d.%d \n", model.version.Major, model.version.Minor, model.version.Bugfix);
  for(int i = 0; i < model.requirementCount; i++)
  {
    printf("Dependency:\n Name: %s\n Version: %d.%d.%d\nSource: %s\n", model.requirements[i].name, model.requirements[i].version.Major, model.requirements[i].version.Minor, model.requirements[i].version.Bugfix, model.requirements[i].source);
  }

  
  //Add dependencies to tree
  //
  //Parse .cy files of tree leaves
  //
  //Add these dependencies to the tree
  //
  //If everything is added, i.e. no unresolved dependencies left, build the tree bottom up
}
