#include "smartcalc_model.h"

namespace s21 {

std::string print_vect(std::vector<double> &v) {
  std::string str = "{";
  for (size_t i = 0; i < v.size(); ++i) {
    str += "`" + std::to_string(v[i]) + "`";
    if (i < v.size() - 1) str += "; ";
  }
  return str + "}";
}

double SmartCalculatorModel::Calculations::calculating(
    const std::string &input) {
  try {
    this->clear();
    this->expression =
        input != "" ? input : throw std::logic_error("Empty expression!");
    checkBrackets();
    this->Create_Lexems();
    this->Check_Lexems();
    this->polishNotation();
    this->polishCalculating();
    this->clear();
    return this->result;
  } catch (const std::exception &err) {
    this->clear();
    throw;
  }
};

void SmartCalculatorModel::Calculations::clear() {
  this->lexems.clear();
  this->polish_2.clear();
  this->expression.clear();
};

void SmartCalculatorModel::Calculations::checkBrackets() {
  int bracket_number = 0;
  size_t new_open = 0;
  for (size_t i = 0; i < expression.size(); ++i) {
    if (expression[i] == '(') {
      if (bracket_number == 0) new_open = i;
      ++bracket_number;
    } else if (expression[i] == ')') {
      --bracket_number;
    }
    if (bracket_number < 0)
      throw std::invalid_argument(
          "For the closing bracket on the " + std::to_string(i + 1) +
          " position there is not enough opening bracket!");
  }
  if (bracket_number > 0)
    throw std::invalid_argument(
        "Starting from the " + std::to_string(new_open + 1) + " position," +
        " there are more open brackets than closed ones.");
};

const std::pair<std::string, Func_Model>
SmartCalculatorModel::Calculations::extract_Function(
    const size_t &i, const std::string &expression) {
  // Перебираем все ключи из карты
  for (auto it = func.begin(); it != func.end(); ++it) {
    const std::string &key = it->first;
    // Проверяем, входит ли ключ начиная с позиции i
    if (expression.size() > i + key.size() - 1 &&
        expression.substr(i, key.length()) == key)
      return {key, it->second};  // Возвращаем ключ //i += key.size();
  }
  throw std::invalid_argument("Unknown symbol");
  return {"", Func_Model()};  // Если совпадений нет
}

void SmartCalculatorModel::Calculations::Create_Lexems() {
  expression.erase(std::remove_if(expression.begin(), expression.end(),
                                  [](unsigned char c) {
                                    return !std::isprint(c) || c == ' ';
                                  }),
                   expression.end());
  std::pair<std::string, Func_Model> lex;
  for (size_t i = 0; i < expression.size();) {
    if (std::isdigit(expression[i]))
      lex = extract_Numeric(i, expression);
    else if ((lex = extract_Function(i, expression)).first == "")
      throw std::invalid_argument("Create_Lexems: Unknown symbol");
    this->lexems.push_back(lex);
    i += lex.first.size();
  }
};

const std::string SmartCalculatorModel::Calculations::getNumber(
    size_t &i, const std::string &expression) {
  std::string number{};
  int dot_count = 0;
  while ((i < expression.size()) &&
         (std::isdigit(expression[i]) || expression[i] == '.')) {
    number += expression[i];
    dot_count += (expression[i] == '.');
    ++i;
  }
  if (dot_count > 1) throw std::invalid_argument("Check dots count");
  return number;
};

const std::pair<std::string, Func_Model>
SmartCalculatorModel::Calculations::extract_Numeric(
    const size_t &i, const std::string &expression) {
  size_t k = i;
  std::string number = getNumber(k, expression);
  if (k < expression.size() && (expression[k] == 'e' || expression[k] == 'E')) {
    number += "e";
    ++k;
    if (!(k < expression.size()))
      throw std::invalid_argument("Empty after exponent");
    if (expression[k] == '+' || expression[k] == '-')
      number.push_back(expression[k++]);
    if (!((k < expression.size()) && std::isdigit(expression[k])))
      throw std::invalid_argument("Empty after exponent");
    number += getNumber(k, expression);
  }
  return {number, Func_Model(UHG, NUMERICL, J_NO, J_NO, nullptr, 0)};
};

void SmartCalculatorModel::Calculations::Check_Lexems() {
  for (int i = 0; i < (int)lexems.size(); /*++i*/) {
    if (i == 0 && lexems[i].second.getLeft() == JYES)
      throw std::invalid_argument(
          "Elemen '" + lexems[i].first +
          "' can't be at the beginning of an expression");
    if (i < 1 || lexems[i - 1].second.getRight() == JYES) {
      if (lexems[i].first == "-") {
        lexems[i] = extract_Function(0, "unar-");
        i++;
        continue;
      } else if (lexems[i].first == "+") {
        lexems.erase(lexems.begin() + i);
        //--i;
        continue;
      }
    }
    if (i > 0 && lexems[i - 1].second.getType() == type_t::FUNCTION &&
        lexems[i - 1].second.getArgc() > 1 && lexems[i].first != "(")
      throw std::invalid_argument("After a multi-argument functuion '" +
                                  lexems[i - 1].first +
                                  "' an open bracket is required.");
    if (i > 0 &&
        1 < abs(lexems[i - 1].second.getRight() + lexems[i].second.getLeft()))
      throw std::invalid_argument("Element '" + lexems[i].first +
                                  "' can't be after '" + lexems[i - 1].first +
                                  "' an expression");
    if (i == (int)lexems.size() - 1 && lexems[i].second.getRight() == JYES)
      throw std::invalid_argument("Element '" + lexems[i].first +
                                  "' can't be at the end of an expression");
    ++i;
  }
}

std::pair<std::string, Func_Model> move_erase(
    std::vector<std::pair<std::string, Func_Model>> &oper,
    std::vector<std::pair<std::string, Func_Model>>::iterator &it) {
  std::pair<std::string, Func_Model> res = {"", Func_Model()};
  if (!oper.empty() && (it != oper.end())) {
    res = {it->first, it->second};
    oper.erase(it);
  }
  return res;
}

void SmartCalculatorModel::Calculations::polishNotation() {
  std::vector<std::pair<std::string, Func_Model>> oper{};
  std::vector<std::pair<std::string, Func_Model>>::iterator it;
  for (size_t i = 0; i < lexems.size(); ++i) {
    if (std::isdigit(lexems[i].first[0]) ||
        lexems[i].second.getType() == CONSTANT) {
      polish_2.push_back(lexems[i]);
      while (!oper.empty() &&
             ((it = std::prev(oper.end()))->second.getType() == FUNCTION))
        polish_2.push_back(move_erase(oper, it));
    } else if (lexems[i].first == "(")
      oper.push_back(lexems[i]);
    else if (lexems[i].first == ")" || lexems[i].first == ";") {
      while (oper.size() && (it = std::prev((oper.end())))->first != "(")
        polish_2.push_back(move_erase(oper, it));
      if (it != oper.begin() && std::prev(it)->second.getType() == FUNCTION &&
          (++std::prev(it)->second.cnt_args) > std::prev(it)->second.getArgc())
        throw std::invalid_argument("Too many arg in :" + std::prev(it)->first);
      if (lexems[i].first == ")") {
        it = oper.erase(it);
        while (!oper.empty() &&
               (it = std::prev(oper.end()))->second.getType() == FUNCTION)
          polish_2.push_back(move_erase(oper, it));
      }
    } else {
      while (oper.size() && [&]() -> bool {
        if ((it = std::prev(oper.end()))->second.getType() == FUNCTION &&
            lexems[i].second.getType() == FUNCTION)
          return it->second.getPriority() > lexems[i].second.getPriority();
        else
          return it->second.getPriority() >= lexems[i].second.getPriority();
      }())
        polish_2.push_back(move_erase(oper, it));
      oper.push_back(lexems[i]);
    }
  }
  while (!oper.empty() && (it = std::prev(oper.end())) != oper.end())
    polish_2.push_back(move_erase(oper, it));
  lexems.clear();
};

double getunsignnum_uptoStop(char **p) {
  double num = 0;
  for (; p && *p && (**p <= '9') && (**p >= '0'); (*p)++)
    num = num * 10 + (**p - '0');
  return num;
}

double myconv(std::string expr, char dec) {
  int sign = 0;
  if (expr[0] == '-') sign = -1;
  if (expr[0] == '+') sign = 1;
  char *p = &expr[(int)abs(sign)];
  double num = getunsignnum_uptoStop(&p);
  if (p && (*p == dec)) {
    p++;
    char *s = p;
    double frac = getunsignnum_uptoStop(&p);
    num += frac * pow(0.1, p - s);
  }
  return num * (sign == 0 ? 1 : sign);
};

double myconv_sci(std::string expr, char dec) {
  size_t pos = expr.find('e');
  if (pos != std::string::npos)
    return myconv(expr.substr(0, pos), dec) *
           pow(10, myconv(expr.substr(pos + 1), dec));
  else
    return myconv(expr, dec);
}

void SmartCalculatorModel::Calculations::polishCalculating() {
  std::vector<double> tmp{}, val{};
  for (size_t i = 0; i < polish_2.size(); ++i)
    if (std::isdigit(polish_2[i].first[0])) {
      tmp.push_back(myconv_sci(polish_2[i].first, '.'));
    } else {
      Func_Model fm = polish_2[i].second;
      if (fm.getType() == FUNCTION && fm.getArgc() > 1 &&
          fm.cnt_args < (fm.getArgc() == ARG_MAX ? 1 : fm.getArgc()))
        throw std::invalid_argument("The function `" + polish_2[i].first +
                                    "` does not have enought argumets.");
      fm.cnt_args = (fm.getType() == FUNCTION && fm.getArgc() > 1)
                        ? fm.cnt_args
                        : fm.getArgc();
      if (tmp.size() < fm.cnt_args)
        throw std::invalid_argument("Argument count is more then possible!");
      val.clear();
      size_t k = 0;
      for (; k < (fm.cnt_args > 0 ? fm.cnt_args - 1 : 0) && k < tmp.size(); k++)
        val.push_back(tmp[tmp.size() - 1 - k]);
      if (fm.cnt_args > 0) {
        val.push_back(tmp[tmp.size() - 1 - k]);
        std::reverse(val.begin(), val.end());  // invert order
      } else
        tmp.push_back(0);  // for CONSTANT value
      double res = 0;
      if (!fm.getFunction() || fm.getFunction()(&res, val) != OK)
        throw std::invalid_argument(
            "The `" + polish_2[i].first +
            "` cann't calc with val=" + print_vect(val));
      tmp[tmp.size() - 1 - k] = res;
      for (; k > 0; --k) tmp.pop_back();
    }
  if (tmp.size() != 1) {
    tmp.clear();
    val.clear();
    polish_2.clear();
    throw std::invalid_argument("polishCalculating: Check expression");
  } else {
    this->result = tmp[0];
  }
  tmp.clear();
  val.clear();
  polish_2.clear();
};

};  // namespace s21
