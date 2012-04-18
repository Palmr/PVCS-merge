#include <stdio.h>

// I did plan to add other stuff here, but this sufficed
void logIssue (char* pMessage) {
  FILE* logFile = fopen("pvcsmerge-issue-log.txt", "a+");
  fputs(pMessage, logFile);
  fclose(logFile);
}
