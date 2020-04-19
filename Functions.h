#if !defined FUNCTIONSANDSTRUCTURES_H
#define FUNCTIONSANDSTRUCTURES_H
#include <stdio.h>
#include <crtdbg.h>
#include <stdlib.h> 
#include <malloc.h>
#include <ctype.h>
#include <conio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#define e 2.71828182845904523536
#define pi 3.14159265358979323846264338327950288419716939937510
#define true 1 
#define false 0 

typedef int bool;

typedef enum {
  ADD = 4,
  SUB,
  MUL,
  DIV,
  POWER_SIGN,
  OPENING_BRACKET,
  CLOSING_BRACKET,
  UNAR_SUB,
  SQRT,
  SIN,
  COS,
  TG,
  CTG,
  ARCSIN,
  ARCCOS,
  ARCTG,
  ARCCTG,
  LN,
  FLOOR,
  CEIL,
  UNKNOWN_OPERATION,
  NUMBER,
  OPERATION //любая операция
} operation_t;

typedef struct {
  operation_t top; //пик стека
  int size;  //размер стека в ШТУКАХ
  operation_t* array; //массив стека 
} stackP_t;

typedef struct {
  double top; //пик стека
  int size;  //размер стека в ШТУКАХ
  double* array; //массив стека 
} stackC_t;

typedef enum {
  ERROR_OK,
  ERROR_MEMORY,                   //ошибка памяти
  ERROR_BRACKETS,                 //много скобок
  ERROR_UNKNOWN_OPERATION,        //получили символ который ни является вариацией числа, ни каким либо оператором
  ERROR_ILLEGAL_EXPRESSION,        //получили неправильную запись числа (12.75.75)
  ERROR_TOO_MANY_OPERATIONS,      //получили слишком много операторов (2+2+)
  ERROR_FILE,                     //ошибка открытия файла
  ERROR_DOMAIN_OF_FUNCTION,        //ввели число для функции которая нарушает её область определения
  ERROR_TOO_MANY_ARGUMENTS     //слишком много аргументов на входе (argc)
} error_t;

stackC_t* CreateStackCalc(error_t* error);
error_t StackPushCalc(stackC_t* stack, double number);
void StackPopCalc(stackC_t *stack);
double StackTopCalc(stackC_t *stack);
int StackIsEmptyCalc(stackC_t *stack);
void DeleteStackC(stackC_t **stack);
stackC_t* OperationResult(stackC_t* stack, operation_t operation, error_t* error);
double CalculatePolska(const char* str, error_t* error);
operation_t OperationInOriginal(unsigned char const* str, int position);
int LengthOfOperation(operation_t operation);
operation_t OperationInPolska(char const* str, int position);

int IsCommentary(unsigned char* str);
int IsEmpty(unsigned char* str);
unsigned char* ReadString(FILE* stream, int* g_ctrlz, int* g_length, error_t* error);
bool PrintError(error_t error);
char* PrintOperation(char* str, int position, operation_t operation);

stackP_t* CreateStackP(error_t* error);
void DeleteStackP(stackP_t **stack);
error_t StackPushP(stackP_t* stack, operation_t operation);
void StackPopP(stackP_t *stack);
operation_t StackTopP(stackP_t *stack);
int StackIsEmptyP(stackP_t *stack);
int Priority(operation_t operation);
char* TranslationToPolska(unsigned char const* str, error_t* error);
int IsNumberWrittenCorrect(bool* isNumberBeforePoint, bool* isScientificExponent, bool* isSecondScientificNumber, bool* isPoint, bool* isNumberAfterPoint, bool* isSignAfterExponent);
bool IsErrorInNumber(unsigned char character, bool isNumberBeforePoint, bool isScientificExponent, bool isPoint, bool isNumberAfterPoint, bool isSignAfterExponent, bool isConstantWritten);
bool IsBinar(operation_t operation);
#endif
