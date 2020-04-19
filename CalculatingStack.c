// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "Functions.h"

stackC_t* CreateStackCalc(error_t* error) {
  stackC_t* stack = NULL;
  stack = (stackC_t*)malloc(sizeof(stackC_t));
  if (stack == NULL) {
    *error = ERROR_MEMORY;
    return stack;
  }
  stack->array = malloc(sizeof(double));
  if (stack->array == NULL) {
    *error = ERROR_MEMORY;
    return stack;
  }
  stack->size = 0;
  return stack;
}

void DeleteStackC(stackC_t **stack) {
  free((*stack)->array);
  free(*stack);
  *stack = NULL;
}

error_t StackPushCalc(stackC_t* stack, double number) {
  double* stackRealloc = NULL;
  stackRealloc = (double*)realloc(stack->array, (stack->size + 1) * sizeof(double));
  if (stackRealloc == NULL)
    return ERROR_MEMORY;
  stack->array = stackRealloc;
  stack->array[stack->size] = number;
  stack->top = number;
  stack->size++;
  return ERROR_OK;
}
void StackPopCalc(stackC_t *stack) {
  stack->size--;
  if (!StackIsEmptyCalc(stack))
    stack->top = stack->array[stack->size - 1];
}
double StackTopCalc(stackC_t *stack) {
  return stack->top;
}
int StackIsEmptyCalc(stackC_t *stack) {
  if (stack->size == 0) {
    return 1;
  }
  return 0;
}

stackC_t* OperationResult(stackC_t* stack, operation_t operation, error_t* error) {
  double lastNumber;
  double firstNumber;
  if (stack->size == 0) {
    *error = ERROR_TOO_MANY_OPERATIONS;
    return stack;
  }
  lastNumber = StackTopCalc(stack); //второе в порядке начального массива str
  StackPopCalc(stack);
  if (IsBinar(operation) && stack->size == 0) {
    *error = ERROR_TOO_MANY_OPERATIONS;
    return stack;
  }
  switch (operation) {
  case ADD: { //ПЛЮС
    firstNumber = StackTopCalc(stack);
    StackPopCalc(stack);
    *error = StackPushCalc(stack, firstNumber + lastNumber);
    break;
  }
  case SUB: { //МИНУС
    firstNumber = StackTopCalc(stack);
    StackPopCalc(stack);
    *error = StackPushCalc(stack, firstNumber - lastNumber);
    break;
  }
  case UNAR_SUB: { //УНАРНЫЙ МИНУС
    *error = StackPushCalc(stack, -lastNumber);
    break;
  }
  case MUL: { //УМНОЖЕНИЕ
    firstNumber = StackTopCalc(stack);
    StackPopCalc(stack);
    *error = StackPushCalc(stack, firstNumber * lastNumber);
    break;
  }
  case DIV: { //ДЕЛЕНИЕ
    firstNumber = StackTopCalc(stack);
    StackPopCalc(stack);
    if (lastNumber == 0.0) {
      *error = ERROR_DOMAIN_OF_FUNCTION;
      return stack;
    }
    *error = StackPushCalc(stack, firstNumber / lastNumber);
    break;
  }
  case POWER_SIGN: { //ВОЗВЕДЕНИЕ В СТЕПЕНЬ
    firstNumber = StackTopCalc(stack);
    if (firstNumber < 0 && (lastNumber - (double)((int)(lastNumber)) != 0)) {
        *error = ERROR_DOMAIN_OF_FUNCTION;
        return stack;
    }
    StackPopCalc(stack);
    *error = StackPushCalc(stack, pow(firstNumber, lastNumber));
    break;
  }
  case SQRT: { //КОРЕНЬ
    if (lastNumber < 0.0) {
      *error = ERROR_DOMAIN_OF_FUNCTION;
      return stack;
    }
    *error = StackPushCalc(stack, sqrt(lastNumber));
    break;
  }
  case SIN: { //СИНУС
    if (cos(lastNumber) == 1.0 || cos(lastNumber) == -1.0) {
      *error = StackPushCalc(stack, 0);
      break;
    }
    *error = StackPushCalc(stack, sin(lastNumber));
    break;
  }
  case COS: { //КОСИНУС
    if (sin(lastNumber) == 1.0 || sin(lastNumber) == -1.0) {
      *error = StackPushCalc(stack, 0);
      break;
    }
    *error = StackPushCalc(stack, cos(lastNumber));
    break;
  }
  case TG: { //ТАНГЕНС
    if (sin(lastNumber) == 1.0 || sin(lastNumber) == -1.0) {
      *error = ERROR_DOMAIN_OF_FUNCTION;
      return stack;
    }
    if (cos(lastNumber) == 1.0 || cos(lastNumber) == -1.0) {
      *error = StackPushCalc(stack, 0);
      break;
    }
    *error = StackPushCalc(stack, tan(lastNumber));
    break;
  }
  case CTG: { //КОТАНГЕНС
    if (cos(lastNumber) == 1.0 || cos(lastNumber) == -1.0) {
      *error = ERROR_DOMAIN_OF_FUNCTION;
      return stack;
    }
    if (sin(lastNumber) == 1.0 || sin(lastNumber) == -1.0) {
      *error = StackPushCalc(stack, 0);
      break;
    }
    *error = StackPushCalc(stack, 1/(tan(lastNumber)));
    break;
  }
  case ARCSIN: { //АРКСИНУС
    if (lastNumber < -1.0|| lastNumber > 1.0) {
      *error = ERROR_DOMAIN_OF_FUNCTION;
      return stack;
    }
    *error = StackPushCalc(stack, asin(lastNumber));
    break;
  }
  case ARCCOS: { //АРККОСИНУС
    if (lastNumber < -1.0 || lastNumber > 1.0) {
      *error = ERROR_DOMAIN_OF_FUNCTION;
      return stack;
    }
    *error = StackPushCalc(stack, acos(lastNumber));
    break;
  }
  case ARCTG: { //АРКТАНГЕНС
    *error = StackPushCalc(stack, atan(lastNumber));
    break;
  }
  case ARCCTG: { //АРККОТАНГЕНС
    *error = StackPushCalc(stack, (pi/2 - atan(lastNumber)));
    break;
  }
  case LN: { //НАТУРАЛЬНЫЙ ЛОГАРИФМ
    if (lastNumber <= 0.0) {
      *error = ERROR_DOMAIN_OF_FUNCTION;
      return stack;
    }
    if (lastNumber == e) {
      *error = StackPushCalc(stack, 1);
      break;
    }
    *error = StackPushCalc(stack, log(lastNumber));
    break;
  }
  case FLOOR: { //ОКРУГЛЕНИЕ ВНИЗ
    *error = StackPushCalc(stack, floor(lastNumber));
    break;
  }
  case CEIL: { //ОКРУГЛЕНИЕ ВВЕРХ
    *error = StackPushCalc(stack, ceil(lastNumber));
    break;
  }
  default: {
    break;
  }
  }

  return stack;
}

double CalculatePolska(const char* str, error_t* error) {
  int i = 0;
  int j;
  int length = (int)strlen((char*)str);
  unsigned char* strNumber = NULL;
  char* endptr = NULL;
  double result = 0;
  double number = 0;
  bool isScientificWriting = false;
  stackC_t* stack = NULL;
  operation_t operation;
  stack = CreateStackCalc(error);
  if (*error == ERROR_MEMORY) {
    DeleteStackC(&stack);
    return 0;
  }

  for (i = 0; i < length; i++) {
    if (isspace(str[i])) {
      continue;
    }
    if (OperationInPolska(str, i) != UNKNOWN_OPERATION) {
      operation = OperationInPolska(str, i);
      stack = OperationResult(stack, operation, error);
      if (*error != ERROR_OK) {
        DeleteStackC(&stack);
        return 0;
      }
      i += (LengthOfOperation(operation) - 1);

    }
    else {
      if (str[i] == 'p' && str[i + 1] == 'i') {
        StackPushCalc(stack, pi);
        i += 1;
        continue;
      }
      if (str[i] == 'e') {
        StackPushCalc(stack, e);
        continue;
      }
      j = i;
      //1e1-e1-1
      //e-.1e
      while (j < length) {
        if (isspace(str[j])) {
          break;
        }
        if ((OperationInPolska(str, j) == ADD || OperationInPolska(str, j) == SUB) && isScientificWriting) {
          j += 1;
          isScientificWriting = false;
          continue;
        }
        if (OperationInPolska(str, j) != UNKNOWN_OPERATION) {
          break;
        }
        if (str[j] == 'e' || str[j] == 'E') {
          isScientificWriting = true;
        }
        j += 1;
      }
      strNumber = (unsigned char*)malloc((j - i) * sizeof(char));
      if (strNumber == NULL) {
        *error = ERROR_MEMORY;
        DeleteStackC(&stack);
        return 0;
      }
      memcpy(strNumber, &str[i], j - i);
      number = strtod((char*)strNumber, &endptr);
      if (((unsigned char*)endptr - strNumber) == 0) { //если в строке 0.0 
        *error = ERROR_ILLEGAL_EXPRESSION;
        free(endptr);
        free(strNumber);
        DeleteStackC(&stack);
        return 0;
      }
      free(strNumber);
      StackPushCalc(stack, number);
      i += (j - i - 1);
      number = 0;
    }
  }
  if (stack->size != 1) {
    *error = ERROR_TOO_MANY_OPERATIONS;
    DeleteStackC(&stack);
    return 0;
  }
  result = StackTopCalc(stack);
  DeleteStackC(&stack);
  return result;
}


operation_t OperationInPolska(char const* str, int position) {
  switch (str[position]) {
  case '+': { //ПЛЮС
    return ADD;
    break;
  }
  case '-': { //МИНУС
    return SUB; 
    break;
  }
  case '_': { //МИНУС
    return UNAR_SUB; //УНАРНЫЙ МИНУС
    break;
  }
  case '*': { //УМНОЖЕНИЕ
    return MUL;
    break;
  }
  case '/': { //ДЕЛЕНИЕ
    return DIV;
    break;
  }
  case '^': { //ВОЗВЕДЕНИЕ В СТЕПЕНЬ
    return POWER_SIGN;
    break;
  }
  case 'c': {
    if (str[position + 1] == 'o' && str[position + 2] == 's') {
      return COS;
    }
    if (str[position + 1] == 't' && str[position + 2] == 'g') {
      return CTG;
    }
    if (str[position + 1] == 'e' && str[position + 2] == 'i' && str[position + 3] == 'l') {
      return CEIL;
    }
    break;
  }
  case 's': {
    if (str[position + 1] == 'i' && str[position + 2] == 'n') {
      return SIN;
    }
    if (str[position + 1] == 'q' && str[position + 2] == 'r' && str[position + 3] == 't') {
      return SQRT;
    }
    return UNKNOWN_OPERATION;
    break;
  }
  case 't': {
    if (str[position + 1] == 'g') {
      return TG;
    }
    break;
  }
  case 'a': {
    if (str[position + 1] == 'r' && str[position + 2] == 'c') {
      if (str[position + 3] == 'c' && str[position + 4] == 'o' && str[position + 5] == 's') {
        return ARCCOS;
      }
      if (str[position + 3] == 's' && str[position + 4] == 'i' && str[position + 5] == 'n') {
        return ARCSIN;
      }
      if (str[position + 3] == 'c' && str[position + 4] == 't' && str[position + 5] == 'g') {
        return ARCCTG;
      }
      if (str[position + 3] == 't' && str[position + 4] == 'g') {
        return ARCTG;
      }
    }
    break;
  }
  case 'l': {
    if (str[position + 1] == 'n') {
      return LN;
    }
    break;
  }
  case 'f': {
    if (str[position + 1] == 'l' && str[position + 2] == 'o' && str[position + 3] == 'o' && str[position + 4] == 'r') {
      return FLOOR;
    }
    break;
  }
  default:
    return UNKNOWN_OPERATION;
    break;
  }
  return UNKNOWN_OPERATION;
}
