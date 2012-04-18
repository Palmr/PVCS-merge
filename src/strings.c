#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//Substring function
char* substr (char* pInput, int pStart, int pLength) {
  pStart = pStart * sizeof(char);
  pLength = pLength * sizeof(char);

  if (pInput == 0 || strlen(pInput) == 0 || strlen(pInput) < pStart || strlen(pInput) < (pStart + pLength)) {
    return 0;
  }

  char* buf = (char*)malloc(pLength + sizeof(char));
  strncpy(buf, pInput + pStart, pLength);
  buf[pLength] = '\0';

  return buf;
}

//String replace
char* str_replace (char* pInput, char* pSearch, char* pReplacement) {
  //Allocate buffers and pointer
  char* buf = calloc(strlen(pInput) + (strlen(pReplacement) - strlen(pSearch)), sizeof(char));
  char* buf2 = calloc(strlen(pInput) + (strlen(pReplacement) - strlen(pSearch)), sizeof(char));
  char* idx;

  //Find where pSearch is in the pInput
  if (!(idx = (char*)strstr(pInput, pSearch))) {
    logIssue("str_replace search string not found in input:\n  input = ");
    logIssue(pInput);
    logIssue("\n  search");
    logIssue(pSearch);
    logIssue("\n");
    return (char*)pInput;
  }

  //Cut up pInput and place in the buffers
  strncpy(buf, pInput, idx - pInput);
  strncpy(buf2, pInput, idx - pInput + strlen(pSearch));

  //Terminate the strings in the buffers
  buf[idx - pInput] = '\0';
  buf2[idx - pInput + strlen(pSearch)] = '\0';

  //Concatenate in the values where they're needed
  if (strlen(pSearch) >= strlen(pReplacement)) {
    sprintf(buf + (idx - pInput), "%s%s", pReplacement, idx + strlen(pSearch));
  }
  else {
    sprintf(buf2 + (idx - pInput), "%s%s", pReplacement, idx + strlen(pSearch));
    strcpy(buf, buf2);
  }

  return buf;
}

//Find the index of one char in some input
int strpos (char* pInput, char pSearch, size_t pOffset) {
  int idx;
  char* position;
  char* str;

  //If it's offset, get the position from a substringed copy
  if (pOffset != 0) {
    str = substr(pInput, pOffset, strlen(pInput) - pOffset);
  }
  else {
    str = pInput;
  }

  //Find position
  position = strchr(str, pSearch);

  //Convert the pointer to an actual character position
  idx = str - position;
  if (idx < 0) {
    idx *= -1;
  }
  //If it was offset, add that back in
  idx = idx + pOffset;

  return idx;
}
