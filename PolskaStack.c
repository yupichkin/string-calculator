// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "Functions.h"

stackP_t* CreateStackP(error_t* error) {
  stackP_t* stack = NULL;
  stack = (stackP_t*)malloc(sizeof(stackP_t));
  if (stack) {
    stack->array = (operation_t*)malloc(sizeof(operation_t));
    stack->size = 0;
    if (!stack->array) {
      *error = ERROR_MEMORY;
    }
  }
  else {
    *error = ERROR_MEMORY;
  }
  return stack;
}

void DeleteStackP(stackP_t **stack) {
  free((*stack)->array);
  free(*stack);
  *stack = NULL;
}

error_t StackPushP(stackP_t* stack, operation_t operation) {
  operation_t* stackArrayRealloc = NULL;
  stackArrayRealloc = (operation_t*)realloc(stack->array, (stack->size + 1) * sizeof(operation_t));
  if (stackArrayRealloc == NULL) {
    return ERROR_MEMORY;
  }
  stack->array = stackArrayRealloc;
  stack->array[stack->size] = operation;
  stack->top = stack->array[stack->size];
  stack->size++;
  return ERROR_OK;
}
void StackPopP(stackP_t *stack) {
  stack->size--;
  if (!StackIsEmptyP(stack)) {
    stack->top = stack->array[stack->size - 1];
  }
}
operation_t StackTopP(stackP_t *stack) {
  return stack->top;
}
int StackIsEmptyP(stackP_t *stack) {
  if (stack->size == 0) {
    return 1;
  }
  return 0;
}

int IsNumberWrittenCorrect(bool* isNumberBeforePoint, bool* isScientificExponent, bool* isSecondScientificNumber, bool* isPoint, bool* isNumberAfterPoint, bool* isSignAfterExponent) {
  if (*isNumberBeforePoint == false && *isScientificExponent == false
    && *isSecondScientificNumber == false && *isPoint == false
    && *isNumberAfterPoint == false) {//если ещё не успели считать никакое число
    return 2; //если последнее что обработали было не число или ещё не успели считать число
  }
  if ((*isPoint && (*isNumberBeforePoint || *isNumberAfterPoint)) || *isNumberBeforePoint) {// если число написано корректно перед экспонентой (12.75 | 1. | .1 | 12)
    *isPoint = false;
    *isNumberAfterPoint = false;
    *isNumberBeforePoint = false;
    if (*isScientificExponent) {
      *isSignAfterExponent = false;
    }
    if (*isSecondScientificNumber == *isScientificExponent) {  //если записана научная форма корректно или её вообще нет (12.75 | 12.75E-4)
      *isSecondScientificNumber = false;
      *isScientificExponent = false;
      return 1; //если число записано корректно
    }
  }
  return 0; //если число записано некорректно
}
bool IsErrorInNumber(unsigned char character, bool isNumberBeforePoint, bool isScientificExponent, bool isPoint, bool isNumberAfterPoint, bool isSignAfterExponent, bool isConstantWritten) {
  if (isConstantWritten && character == '.') {// точка после e
    return true;
  }
  if ((character == 'e' || character == 'E') && isPoint && (!isNumberBeforePoint && !isNumberAfterPoint)) {// .e+4 нет никакого числа до е
    return true;
  }
  if ((character == 'e' || character == 'E') && isScientificExponent) {// 2 экспоненты в одном числе 12.75e+04e
    return true;
  }
  if (character == '.' && (isPoint || isScientificExponent || isNumberAfterPoint)) {//2 точки в числе (12.75.) или точка после экспоненты
    return true;
  }
  if (character == 'p' && (isNumberBeforePoint || isScientificExponent || isPoint)) {//константа после числа
    return true;
  }
  if (isSignAfterExponent && (character == '+' || character == '-')) {//подряд 2 плюса (1e++1)
    return true;
  }
  return false;
}
bool IsBeginningOfNumber(char* str, int position) {
  //if
  return true;
}
char* TranslationToPolska(unsigned char const* str, error_t* error) {
  int i;                                   //счётчик прочитанной строки 
  int k = 0;                               //счётчик польской строки
  int commaCount = 0;                      //счётчик пробелов между операндами
  int length = (int)strlen((char*)str);
  int isNumberWritedCorrect;
  char* strPolska = NULL;
  char* strPolskaRealloc = NULL;
  operation_t operation;
  operation_t lastOperation = 0;              //последняя обработанная операция(NUMBER если число)
  bool isBracketsEven = false;
  bool isConstantWritten = false;
  bool isNumberBeforePoint = false;   //по умолчанию любое число берётся как число перед точкой
  bool isNumberAfterPoint = false;        //написано ли число после точки
  bool isScientificExponent = false;      //написана ли экспонента для научной записи
  bool isSignAfterExponent = false;        //написано ли знак плюса или минуса после е
  bool isSecondScientificNumber = false;  //написано ли число после экспоненты
  bool isPoint = false;                   //написана ли точка
  stackP_t* stack = NULL;

  stack = CreateStackP(error);
  strPolska = (char*)malloc(length + 1); //делаем +1 для конца строки и +1 для пробела после последнего считанного числа

  if (strPolska == NULL || *error == ERROR_MEMORY) {
    *error = ERROR_MEMORY;
    DeleteStackP(&stack);
    if (strPolska != NULL) {
      free(strPolska);
    }
    return NULL;
  }
  //.1e1-1.e4
  for (i = 0; i < length; i++) {
    if ((str[i] >= '0' && str[i] <= '9') || str[i] == '.' || str[i] == 'e' || str[i] == 'E' \
      || (str[i] == 'p' && str[i + 1] == 'i') || ((str[i] == '+' || str[i] == '-') && (isScientificExponent && !isSecondScientificNumber))) {
      if (IsErrorInNumber(str[i], isNumberBeforePoint, isScientificExponent, isPoint, isNumberAfterPoint, isSignAfterExponent, isConstantWritten)) {
        *error = ERROR_ILLEGAL_EXPRESSION;
        DeleteStackP(&stack);
        free(strPolska);
        return NULL;
      }
      if ((lastOperation == NUMBER || isConstantWritten) || lastOperation == CLOSING_BRACKET) { //два числа подряд или число сразу после закрывающей скобки
        *error = ERROR_ILLEGAL_EXPRESSION;
        DeleteStackP(&stack);
        free(strPolska);
        return NULL;
      }
      if (str[i] >= '0' && str[i] <= '9') {
        while (str[i] >= '0' && str[i] <= '9') {
          strPolska[k] = str[i];
          k += 1;
          i += 1;
        }
        i += -1; //так как остановка цикла не на числе
        if (!isScientificExponent && !isPoint) {
          isNumberBeforePoint = true;
        }
        if (isScientificExponent) {
          isSecondScientificNumber = true;
        }
        if (isPoint) {
          isNumberAfterPoint = true;
        }
      }
      else {
        if (str[i] == '.') {
          isPoint = true;
        }
        if ((str[i] == 'e' || str[i] == 'E') && (isNumberBeforePoint || isNumberAfterPoint)) {
          isScientificExponent = true;
        }
        else {
          if (str[i] == 'e') {
            isConstantWritten = true;
          }
        }
        if (str[i] == '+' || str[i] == '-') {
          isSignAfterExponent = true;
        }
        if (str[i] == 'p' && str[i+1] == 'i') {
          strPolska[k] = str[i];
          k += 1;
          i += 1;
          isConstantWritten = true;
        }
        strPolska[k] = str[i];
        k += 1;
      }
    }
    else {
      isNumberWritedCorrect = IsNumberWrittenCorrect(&isNumberBeforePoint, &isScientificExponent, \
        &isSecondScientificNumber, &isPoint, &isNumberAfterPoint, &isSignAfterExponent);
      if (isNumberWritedCorrect == 1 || isConstantWritten) {
        if (isConstantWritten) {
          isConstantWritten = false;
        }
        lastOperation = NUMBER;
        commaCount++;
        strPolskaRealloc = (char*)realloc(strPolska, length + 1 + commaCount);
        if (strPolskaRealloc == NULL) {
          *error = ERROR_MEMORY;
          DeleteStackP(&stack);
          free(strPolska);
          return NULL;
        }
        strPolska = strPolskaRealloc;
        strPolska[k] = ' ';
        k += 1;
      }
      if (isNumberWritedCorrect == 0) {
        *error = ERROR_ILLEGAL_EXPRESSION;
        DeleteStackP(&stack);
        free(strPolska);
        return NULL;
      }
      if (isspace(str[i])) {
        while (isspace(str[i])) { // пропускаем все последующие подряд идущие пробелы если попался пробел
          i += 1;
        }
        i -= 1;
        continue; //чтобы вернуться назад если вдруг попалось число
      }

      operation = OperationInOriginal(str, i);
      if (operation == UNKNOWN_OPERATION) {
        *error = ERROR_UNKNOWN_OPERATION;
        DeleteStackP(&stack);
        free(strPolska);
        return NULL;
      }
      i += (LengthOfOperation(operation) - 1); //т.к в цикле мы и так прибавляем на 1

      if (IsBinar(operation)) { //если подряд 2 бинарные операции или бинарная операция после открывающей скобки
        if (lastOperation != NUMBER && lastOperation != CLOSING_BRACKET) {
          *error = ERROR_ILLEGAL_EXPRESSION;
          DeleteStackP(&stack);
          free(strPolska);
          return NULL;
        }
      }

      if (operation == UNAR_SUB && (lastOperation == NUMBER || lastOperation == CLOSING_BRACKET)) { //различаем унарный и бинарный минусы
        operation = SUB;
      }
      if (operation == OPENING_BRACKET) {
        isBracketsEven++;
        if (lastOperation == CLOSING_BRACKET || lastOperation == NUMBER) { //если встретились открывающая и закрывающая скобка ((4)(4))
          *error = ERROR_ILLEGAL_EXPRESSION;
          DeleteStackP(&stack);
          free(strPolska);
          return NULL;
        }
        *error = StackPushP(stack, operation);
        if (*error == ERROR_MEMORY) {
          DeleteStackP(&stack);
          free(strPolska);
          return NULL;
        }
        lastOperation = operation;
        continue;
      }
      if (operation == CLOSING_BRACKET) { //проверяем скобку
        if (isBracketsEven == 0 || (lastOperation != NUMBER && lastOperation != CLOSING_BRACKET)) { //если встретились пустые скобки  ( ) или до закрывающей скобки было не число
          *error = ERROR_BRACKETS;
          DeleteStackP(&stack);
          free(strPolska);
          return NULL;
        }
        while (!StackIsEmptyP(stack) && stack->array[stack->size - 1] != OPENING_BRACKET) {
          PrintOperation(strPolska, k, StackTopP(stack));
          k += LengthOfOperation(StackTopP(stack));
          StackPopP(stack);
        }
        isBracketsEven--;
        StackPopP(stack);
        lastOperation = CLOSING_BRACKET;
        continue;
      }
      if (StackIsEmptyP(stack)) {
        *error = StackPushP(stack, operation);
        if (*error == ERROR_MEMORY) {
          DeleteStackP(&stack);
          free(strPolska);
          return NULL;
        }
      } 
      else {
        if (operation == POWER_SIGN && StackTopP(stack) == UNAR_SUB) { // унарный минус со степенью (-4^2 = -16)
          *error = StackPushP(stack, operation);
          if (*error == ERROR_MEMORY) {
            DeleteStackP(&stack);
            free(strPolska);
            return NULL;
          }
          lastOperation = operation;
          continue;
        }
        if (Priority(stack->array[stack->size - 1]) < Priority(operation) \
          || (Priority(operation) == 5)) { //унарные операции закидываются в стек в любом случае
          *error = StackPushP(stack, operation);
          if (*error == ERROR_MEMORY) {
            DeleteStackP(&stack);
            free(strPolska);
            return NULL;
          }
        }
        else {
          while (!StackIsEmptyP(stack) && (Priority(stack->array[stack->size - 1]) >= Priority(operation))) {
            if (operation == POWER_SIGN && stack->array[stack->size - 1] == POWER_SIGN)   //правоассоциативность возведения в степень (2^3^4 = 2^81)
              break;
            PrintOperation(strPolska, k, StackTopP(stack));
            k += LengthOfOperation(StackTopP(stack));
            StackPopP(stack);
          }
          *error = StackPushP(stack, operation);
          if (*error == ERROR_MEMORY) {
            DeleteStackP(&stack);
            free(strPolska);
            return NULL;
          }
        }
      }
      lastOperation = operation;
    }
  }
  isNumberWritedCorrect = IsNumberWrittenCorrect(&isNumberBeforePoint, &isScientificExponent, \
    &isSecondScientificNumber, &isPoint, &isNumberAfterPoint, &isSignAfterExponent);
  if (isNumberWritedCorrect == 1 || isConstantWritten) { //проверка последнего прочитанного числа из-за того что элементы з
    lastOperation = NUMBER;  
  }
  if (isNumberWritedCorrect == 0) { 
    *error = ERROR_ILLEGAL_EXPRESSION;
    DeleteStackP(&stack);
    free(strPolska);
    return NULL;
  }
  if (isBracketsEven != 0) {
    *error = ERROR_BRACKETS;
    DeleteStackP(&stack);
    free(strPolska);
    return NULL;
  }
  if (lastOperation != NUMBER && lastOperation != CLOSING_BRACKET) { // если выражение закончилось не закрывающей скобкой и не числом, а операцией
    *error = ERROR_ILLEGAL_EXPRESSION;
    DeleteStackP(&stack);
    free(strPolska);
    return NULL;
  }
  if (!StackIsEmptyP(stack)) {
    commaCount++;
    strPolskaRealloc = (char*)realloc(strPolska, length + 1 + commaCount);
    if (strPolskaRealloc == NULL) {
      *error = ERROR_MEMORY;
      DeleteStackP(&stack);
      free(strPolska);
      return NULL;
    }
    strPolska = strPolskaRealloc;
    strPolska[k] = ' ';
    k += 1;
    while (!StackIsEmptyP(stack)) {
      PrintOperation(strPolska, k, StackTopP(stack));
      k += LengthOfOperation(StackTopP(stack));
      StackPopP(stack);
    }
  }
  DeleteStackP(&stack);
  strPolska[k] = '\0';
  return strPolska;
}
