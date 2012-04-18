#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//Max line length from the PVCS config file for reading
#define MAX_LINE_SIZE 1024 //Argh! I feel horrible for making this a const, but I'm lazy...

//Declare functions from strings.c
char* substr();
char* str_replace();
int strpos();

//Declare functions from log.c
void logIssue();

//Match lines from the PVCS config file
int matchLine (char* line) {
  char matchString[10];
  strncpy(matchString, line, 10);
  if (strcmp(matchString, "ANCESTOR=\"") == 0) {
    return 1;
  }
  else if (strcmp(matchString, "DERIVATIVE") == 0) {
    return 2;
  }
  return 0;
}

//Hack to make a local config file when running from a citrix install
void makeDefaultConfig () {
  FILE* configFile;
  configFile = fopen("pvcsmerge.cfg", "w+");
  fputs("C:\\PROGRA~2\\WinMerge\\WinMergeU.exe /e /u /maximize /wl /wr /dl \"(leftDesc)\" /dr \"(rightDesc)\" \"(leftPath)\" \"(rightPath)\"\r\n", configFile);
  fclose(configFile);
}

int main (int argc, char* argv[]) {
  FILE* mergeFile;
  FILE* configFile;
  char lineBuffer[MAX_LINE_SIZE];
  char *fileA, *descA, *fileB, *descB;
  size_t fileStart, fileEnd, descStart, descEnd;
  char* runCommand;

  //Open merge file from pvcs, last argument
  if (argc > 1 && (mergeFile = fopen(argv[argc-1], "r")) != NULL) {
    //Loop through line by line looking for matches
    while (fgets(lineBuffer, MAX_LINE_SIZE, mergeFile) != NULL) {
      //Find the lines we want and get the data from them
      int lineMatch = matchLine(lineBuffer);
      if (lineMatch == 1) {
        fileStart = strpos(lineBuffer, '"', 0);
        fileEnd = strpos(lineBuffer, '"', fileStart + 1) - fileStart;
        fileA = substr(lineBuffer, fileStart + 1, fileEnd - 1);
        descStart = strpos(lineBuffer,'"',  fileEnd + fileStart + 1);
        descEnd = strpos(lineBuffer, '"', descStart + 1) - descStart;
        descA = substr(lineBuffer, descStart + 1, descEnd - 1);
      }
      else if (lineMatch == 2) {
        fileStart = strpos(lineBuffer, '"', 0);
        fileEnd = strpos(lineBuffer, '"', fileStart + 1) - fileStart;
        fileB = substr(lineBuffer, fileStart + 1, fileEnd - 1);
        descStart = strpos(lineBuffer, '"', fileEnd + fileStart + 1);
        descEnd = strpos(lineBuffer, '"', descStart + 1) - descStart;
        descB = substr(lineBuffer, descStart + 1, descEnd - 1);
      }
    }

    fclose(mergeFile);

    //Open the custom merge command config
    if ((configFile = fopen("pvcsmerge.cfg", "r")) != NULL) {
      //Read the line in and replace the arguments
      fgets(lineBuffer, MAX_LINE_SIZE, configFile);
      runCommand = str_replace(lineBuffer, "(leftDesc)", descA);
      runCommand = str_replace(runCommand, "(rightDesc)", descB);
      runCommand = str_replace(runCommand, "(leftPath)", fileA);
      runCommand = str_replace(runCommand, "(rightPath)", fileB);
      //Run the command
      if (system(runCommand) != 0) {
        //Error handling bits...
        logIssue("Failed to run command? Check you don't have spaces in the exe path:\n");
        logIssue("  ");
        logIssue(runCommand);
        return 1;
      }
    }
    else {
      makeDefaultConfig();

      if ((configFile = fopen("pvcsmerge.cfg", "r")) != NULL) {
        //Read the line in and replace the arguments
        fgets(lineBuffer, MAX_LINE_SIZE, configFile);
        runCommand = str_replace(lineBuffer, "(leftDesc)", descA);
        runCommand = str_replace(runCommand, "(rightDesc)", descB);
        runCommand = str_replace(runCommand, "(leftPath)", fileA);
        runCommand = str_replace(runCommand, "(rightPath)", fileB);
        //Run the command
        if (system(runCommand) != 0) {
          //Error handling bits...
          logIssue("Failed to run command? Check you don't have spaces in the exe path:\n");
          logIssue("  ");
          logIssue(runCommand);
          return 1;
        }
      }
      else {
        logIssue("Config file not found:\n");
        logIssue("  ");
        logIssue("./pvcsmerge.cfg - not readable, nor can it be generated?!\n");
        return 1;
      }
    }
  }
  else if (argc <= 1) {
    logIssue("Merge config file not specified!\n");
    return 1;
  }
  else {
    logIssue("Merge config file not found:\n");
    logIssue("  ");
    logIssue(argv[argc-1]);
    logIssue(" - not readable\n");
    return 1;
  }

  return 0;
}
