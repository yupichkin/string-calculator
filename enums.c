// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "Functions.h"

int Priority(operation_t operation) {
  switch (operation) {
  case OPENING_BRACKET: { //ОТКРЫВАЮЩАЯ СКОБКА
    return 0;
    break;
  }
  case ADD:  //ПЛЮС
  case SUB: { //МИНУС
    return 1;
    break;
  }
  case MUL:  //УМНОЖЕНИЕ
  case DIV: { //ДЕЛЕНИЕ
    return 3;
    break;
  }
  case POWER_SIGN: { //ВОЗВЕДЕНИЕ В СТЕПЕНЬ
    return 4;
    break;
  }
  case UNAR_SUB:  //УНАРНЫЙ МИНУС
  case SQRT:  //КОРЕНЬ
  case SIN: //СИНУС
  case COS:  //КОСИНУС
  case TG:  //ТАНГЕНС
  case CTG:  //КОТАНГЕНС
  case ARCSIN:  //АРКСИНУС
  case ARCCOS:  //АРККОСИНУС
  case ARCTG:  //АРКТАНГЕНС
  case ARCCTG:  //АРККОТАНГЕНС
  case LN:  //НАТУРАЛЬНЫЙ ЛОГАРИФМ
  case FLOOR:  //ОКРУГЛЕНИЕ ВНИЗ
  case CEIL: { //ОКРУГЛЕНИЕ ВВЕРХ
    return 5;
    break;
  }
  case CLOSING_BRACKET: { //ЗАКРЫВАЮЩАЯ СКОБКА
    return 6;
    break;
  }
  default: {
    return -1;
    break;
  }
  }
}

bool IsBinar(operation_t operation) {
  switch (operation) {
  case ADD: {
    return 1;
    break;
  }
  case SUB: {
    return 1;
    break;
  }
  case MUL: {
    return 1;
    break;
  }
  case DIV: {
    return 1;
    break;
  }
  case POWER_SIGN: {
    return 1;
    break;
  }
  default:
    return 0;
    break;
  }
}

operation_t OperationInOriginal(unsigned char const* str, int position) {
  switch (str[position]) {
  case '+': { //ПЛЮС
    return ADD;
    break;
  }
  case '-': { //МИНУС
    return UNAR_SUB; //затем проверяем последний прочитанный элемент. Если же число или закрывающая скобка, то меняем на обычный
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
  case '(': {
    return OPENING_BRACKET;
    break;
  }
  case ')': {
    return CLOSING_BRACKET;
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
  default: {
    break;
  }
  }
  return UNKNOWN_OPERATION;
}

char* PrintOperation(char* str, int position, operation_t operation) {
  switch (operation) {
  case ADD: {
    str[position] = '+';
    break;
  }
  case SUB: {
    str[position] = '-';
    break;
  }
  case UNAR_SUB: {
    str[position] = '_';
    break;
  }
  case MUL: {
    str[position] = '*';
    break;
  }
  case DIV: {
    str[position] = '/';
    break;
  }
  case POWER_SIGN: {
    str[position] = '^';
    break;
  }
  case COS: {
    str[position] = 'c';
    str[position + 1] = 'o';
    str[position + 2] = 's';
    break;
  }
  case CTG: {
    str[position] = 'c';
    str[position + 1] = 't';
    str[position + 2] = 'g';
    break;
  }
  case CEIL: {
    str[position] = 'c';
    str[position + 1] = 'e';
    str[position + 2] = 'i';
    str[position + 3] = 'l';
    break;
  }
  case SIN: {
    str[position] = 's';
    str[position + 1] = 'i';
    str[position + 2] = 'n';
    break;
  }
  case SQRT: {
    str[position] = 's';
    str[position + 1] = 'q';
    str[position + 2] = 'r';
    str[position + 3] = 't';
    break;
  }
  case TG: {
    str[position] = 't';
    str[position + 1] = 'g';
    break;
  }
  case ARCCOS: {
    str[position] = 'a';
    str[position + 1] = 'r';
    str[position + 2] = 'c';
    str[position + 3] = 'c';
    str[position + 4] = 'o';
    str[position + 5] = 's';
    break;
  }
  case ARCSIN: {
    str[position] = 'a';
    str[position + 1] = 'r';
    str[position + 2] = 'c';
    str[position + 3] = 's';
    str[position + 4] = 'i';
    str[position + 5] = 'n';
    break;
  }
  case ARCCTG: {
    str[position] = 'a';
    str[position + 1] = 'r';
    str[position + 2] = 'c';
    str[position + 3] = 'c';
    str[position + 4] = 't';
    str[position + 5] = 'g';
    break;
  }
  case ARCTG: {
    str[position] = 'a';
    str[position + 1] = 'r';
    str[position + 2] = 'c';
    str[position + 3] = 't';
    str[position + 4] = 'g';
    break;
  }
  case LN: {
    str[position] = 'l';
    str[position + 1] = 'n';
    break;
  }
  case FLOOR: {
    str[position] = 'f';
    str[position + 1] = 'l';
    str[position + 2] = 'o';
    str[position + 3] = 'o';
    str[position + 4] = 'r';
    break;
  }
  default: {
    return str;
    break;
  }
  }
  return str;
}

int LengthOfOperation(operation_t operation) {
  switch (operation) {
  case ADD: 
  case SUB:
  case UNAR_SUB:
  case MUL: 
  case DIV:
  case POWER_SIGN: 
  case OPENING_BRACKET:
  case CLOSING_BRACKET: {
    return 1;
    break;
  }
  case TG: 
  case LN: {
    return 2;
    break;
  }
  case COS: 
  case CTG:
  case SIN: {
    return 3;
    break;
  }
  case CEIL:
  case SQRT: {
    return 4;
    break;
  }
  case ARCTG:
  case FLOOR: {
    return 5;
    break;
  }
  case ARCCOS: 
  case ARCSIN: 
  case ARCCTG: {
    return 6;
    break;
  }
  default: {
    return 0;
    break;
  }
  }
}
