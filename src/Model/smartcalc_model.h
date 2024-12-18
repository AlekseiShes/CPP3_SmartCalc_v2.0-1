#ifndef SMARTCALC_V2_H
#define SMARTCALC_V2_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace s21 {

#define OK 0
#define Err 1
#define PRECISION 1E-7
#define DECIMAL_SEPARATOR '.'   // 3.141592
#define ARGUMENT_SEPARATOR ';'  // lg(-32;-0.2)
#define ARG_MAX 100
#define NULL____ nullptr

std::string print_vect(std::vector<double> &v);

typedef enum priority_t {
  COM = -1,
  BRA = -1,
  NUM = 0,
  LOW = 1,
  MED = 2,
  HGH = 3,
  UHG = 4,
  UUH = 5
} priority_t;

typedef enum {
  LOPERAND = -1,  // +/-
  NONTYPED = 0,   // (;)
  ROPERAND = 1,   // !
  OPERAND2 = 2,   // +-/*...
  NUMERICL = 3,
  CONSTANT = 4,
  FUNCTION = 5  // lg(;)
} type_t;

typedef enum { J_NO = -1, JMAY = 0, JYES = 1 } join_t;

class Func_Model {
 private:
  priority_t priority_;  // Приоритет операции
  type_t type_;
  join_t left_;
  join_t right_;  // Тип операции
  int (*function_)(double *res,
                   std::vector<double> val);  // Указатель на функцию
  size_t argc_;  // Количество аргументов

 public:
  size_t cnt_args = 0;
  // Конструктор по умолчанию
  Func_Model(priority_t priority = NUM, type_t type = NONTYPED,
             join_t left = JMAY, join_t right = JMAY,
             int (*function)(double *, std::vector<double>) = nullptr,
             size_t argc = 0)
      : priority_(priority),
        type_(type),
        left_(left),
        right_(right),
        function_(function),
        argc_(argc) {}

  // Методы доступа
  priority_t getPriority() const { return priority_; }
  type_t getType() const { return type_; }
  join_t getLeft() const { return left_; }
  join_t getRight() const { return right_; }
  int (*getFunction() const)(double *, std::vector<double>) {
    return function_;
  }
  size_t getArgc() const { return argc_; }
};

std::map<std::string, Func_Model> Init_Func();

class SmartCalculatorModel {
 public:
  class Calculations {
   public:
    double calculating(const std::string &input);
    void clear();

   private:
    std::string expression;
    std::vector<std::pair<std::string, Func_Model>> lexems;
    std::vector<std::pair<std::string, Func_Model>> polish_2;
    double result;
    const std::map<std::string, Func_Model> func = Init_Func();

    void checkBrackets();
    void Create_Lexems();
    void Check_Lexems();
    const std::string getNumber(size_t &i, const std::string &expression);
    const std::pair<std::string, Func_Model> extract_Numeric(
        const size_t &i, const std::string &expression);
    const std::pair<std::string, Func_Model> extract_Function(
        const size_t &i, const std::string &expression);
    void polishNotation();
    void polishCalculating();
  };
  s21::SmartCalculatorModel::Calculations calculator;
};
};  // namespace s21
#endif
