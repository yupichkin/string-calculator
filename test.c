// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "Functions.h"

int main(int argc, char* argv[]) {
  int ctrlz = 0;
  int length;
  double result;
  unsigned char* charArray = NULL;
  char* polskaArray = NULL;
  FILE* readFile = stdin;
  error_t error = ERROR_OK;
  if (argc == 1) {
    readFile = stdin;
  }
  if (argc == 2) {
    freopen_s(&readFile, argv[1], "r", stdin);
    if (readFile == NULL) {
      error = ERROR_FILE;
      PrintError(error);
      return 0;
    }
  }
  if (argc > 2) {
    error = ERROR_TOO_MANY_ARGUMENTS;
    PrintError(error);
    return 0;
  }
  do {
    error = ERROR_OK;
    charArray = ReadString(readFile, &ctrlz, &length, &error);
    if (error != ERROR_OK) {
      PrintError(error);
      continue;
    }
    if ((ctrlz == 1) && (length == 0)) { //конец ввода в начале строки 
      free(charArray);
      break;
    }
    if (IsCommentary(charArray) || IsEmpty(charArray)) {
      printf("%s\n", charArray);
    }
    else { //expressions.txt
      polskaArray = TranslationToPolska(charArray, &error);
      if (error == ERROR_OK) {
        result = CalculatePolska(polskaArray, &error);
        free(polskaArray);
        if (error == ERROR_OK) {
         printf("%s == %g\n", charArray, result);
        }
        else {
          printf("%s == ", charArray);
          PrintError(error);
        }
      }
      else {
        printf("%s == ", charArray);
        PrintError(error);
      }
    }
    free(charArray);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
    _CrtDumpMemoryLeaks();
  } while (ctrlz != 1);
  //_getch();
  return 0;
}


