// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "Functions.h"

int IsCommentary(unsigned char* str) {
  int i = 0;
  while (isspace(str[i])) {
    i++;
  }
  if (str[i] && str[i+1]) {
    if ((str[i] == '/') && (str[i+1] == '/')) {
      return 1;
    }
  }
  return 0;
}
int IsEmpty(unsigned char* str) {
  int i = 0;
  while (str[i] != 0) {
    if (isspace(str[i]) == 0) {
      return 0;
    }
    i++;
  }
  return 1;
}
unsigned char* ReadString(FILE* stream, int* g_ctrlz, int* g_length, error_t* error) {
  int i = 0;
  //setlocale(LC_CTYPE, "Russian");
  int symbol = fgetc(stream);
  unsigned char* str = NULL;
  unsigned char* str_realloc = NULL;
  if (symbol == (int)EOF || symbol == 26) {
    *g_ctrlz = 1;
    *g_length = 0;
    return NULL;
  }
  while (symbol != (int)('\n')) {
    if ((symbol == EOF) || (ferror(stream) != 0) || (symbol == 26)) {
      *g_ctrlz = 1;
      break;
    }
    str_realloc = (unsigned char*)realloc(str, (i + 1) * sizeof(char));
    if (str_realloc != NULL) {
      str = str_realloc;
      str[i] = (unsigned char)symbol;
      i++;
      symbol = fgetc(stream);
    }
    else {
      *error = ERROR_MEMORY;
      free(str);
      while (symbol != (int)('\n') && (symbol != EOF) && (ferror(stream) == 0) && (symbol != 26)) {
        symbol = fgetc(stream);
      }
      if ((symbol == EOF) || (ferror(stream) != 0) || (symbol == 26)) {
        *g_ctrlz = 1;
      }
      return NULL;
    }

  }
  str_realloc = (unsigned char*)realloc(str, (i + 1) * sizeof(char));
  if (str_realloc == NULL) {
    *error = ERROR_MEMORY;
    free(str);
    return NULL;
  }
  str = str_realloc;
  str[i] = '\0';
  *g_length = i;
  return str;
}

bool PrintError(error_t error) {
  switch (error) {
  case ERROR_OK: {
    return 0;
    break;
  }
  case ERROR_MEMORY: {
    printf("ERROR: memory\n");
    break;
  }
  case ERROR_BRACKETS: {
    printf("ERROR: brackets\n");
    break;
  }
  case ERROR_UNKNOWN_OPERATION: {
    printf("ERROR: unknown operation\n");
    break;
  }
  case ERROR_ILLEGAL_EXPRESSION: {
    printf("ERROR: illegal expression\n");
    break;
  }
  case ERROR_TOO_MANY_OPERATIONS: {
    printf("ERROR: too much operations\n");
    break;
  }
  case ERROR_FILE: {
    printf("ERROR: cannot open file\n");
    break;
  }
  case ERROR_DOMAIN_OF_FUNCTION: {
    printf("ERROR: domain of function\n");
    break;
  }
  case ERROR_TOO_MANY_ARGUMENTS: {
    printf("ERROR: too many arguments\n");
    break;
  }

  default: {

    break;
  }
  }
  return 1;
}
