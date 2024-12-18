#include "smartcalc_model.h"

namespace s21 {

int s21_add_(double *res, std::vector<double> val) {
  int status = res && val.size() > 1 ? OK : Err;
  if (status == OK) *res = val[0] + val[1];
  return status;
}
int s21_sub_(double *res, std::vector<double> val) {
  int status = (res && val.size() > 1) ? OK : Err;
  if (status == OK) *res = val[0] - val[1];
  return status;
}
int s21_mul_(double *res, std::vector<double> val) {
  int status = (res && val.size() > 1) ? OK : Err;
  if (status == OK) *res = val[0] * val[1];
  return status;
}
int s21_div_(double *res, std::vector<double> val) {
  int status = (res && val.size() > 1 && fabs(val[1]) > PRECISION ? OK : Err);
  if (status == OK) *res = val[0] / val[1];
  return status;
}
int s21_pow_(double *res, std::vector<double> val) {
  int status = (res && val.size() > 1) ? OK : Err;
  if (status == OK) {
    *res = 0;
    double a = val[0], b = val[1];
    if (a > 0)
      *res = pow(a, b);
    else if (a < 0) {
      *res = 1;
      double f = b - (int)b; /* +-0.3333 */
      if (fabs(f) > PRECISION) {
        f = fabs(1 / f); /* +-3.000 */
        status = ((f - (int)f) > PRECISION || ((int)f) % 2 == 0) ? Err : status;
        if (status == OK) *res = -pow(-a, b);
      } else {
        b = (int)b;
        *res = pow(a, b);
      }
    }
  }
  return status;
}
int s21_mod_(double *res, std::vector<double> val) {
  int status = (res && val.size() > 1 && fabs(val[1]) > PRECISION ? OK : Err);
  if (status == OK) {
    *res = fmod(val[0], val[1]);
    // status = s21_div_(res, val);
    // if (status== OK) *res = (*res - (int)(*res)) * val[1];
  }
  return status;
}
int s21_sin_(double *res, std::vector<double> val) {
  int status = (res && val.size() > 0) ? OK : Err;
  if (status == OK) *res = sin(val[0]);
  return status;
}
int s21_cos_(double *res, std::vector<double> val) {
  int status = (res && val.size() > 0) ? OK : Err;
  if (status == OK) *res = cos(val[0]);
  return status;
}
int s21_tan_(double *res, std::vector<double> val) {
  int status = (res && val.size() > 0) ? OK : Err;
  if (status == OK) *res = tan(val[0]);
  return status;
}
int s21_asin(double *res, std::vector<double> val) {
  int status =
      (res && val.size() > 0 && (-1 <= val[0]) && (val[0] <= 1)) ? OK : Err;
  if (status == OK) *res = asin(val[0]);
  return status;
}
int s21_acos(double *res, std::vector<double> val) {
  int status =
      (res && val.size() > 0 && (-1 <= val[0]) && (val[0] <= 1)) ? OK : Err;
  if (status == OK) *res = acos(val[0]);
  return status;
}
int s21_atan(double *res, std::vector<double> val) {
  int status = (res && val.size() > 0) ? OK : Err;
  if (status == OK) *res = atan(val[0]);
  return status;
}
int s21_ln__(double *res, std::vector<double> val) {
  int status = res && val.size() > 0 && val[0] > PRECISION ? OK : Err;
  if (status == OK) *res = log(val[0]);
  return status;
}
int s21_log_(double *res, std::vector<double> val) {
  int status = (res && val.size() > 0 && val[0] > PRECISION) ? OK : Err;
  if (status == OK) *res = log10(val[0]);
  return status;
}

int ostatok2_IsEq_k(double ch, unsigned int k) {
  ch = fabs(ch) * 10000000;
  unsigned long ch_int = ch;
  double dif = fabs(ch - ch_int);
  if (dif > 0.5) ch_int++;
  return (dif < 1) && ((ch_int % (2 * 10000000)) == (k * 10000000));
}

int s21_lg__(double *res, std::vector<double> val) {
  int status = (res && val.size() > 1) ? OK : Err;
  if (status == OK) {
    std::vector<double> mas = {val[1], val[0]};
    if (mas[1] >= 0) {
      status = (mas[0] >= 0) ? OK : Err;
      std::vector<double> temp = {mas[0]};
      status = status == OK ? s21_ln__(&(mas[0]), temp) : status;
      temp.clear();
      if (status == OK && fabs(mas[0]) < PRECISION) {
        *res = 0;
      } else {
        std::vector<double> temp = {mas[1]};
        status = status == OK ? s21_ln__(&(mas[1]), temp) : status;
        temp.clear();
        status = status == OK ? s21_div_(res, mas) : status;
      }
    } else {
      if (mas[0] >= 0) {
        val[0] *= (-1);
        double temp = 0;
        status = s21_lg__(&temp, val);
        status = (status == OK) && ostatok2_IsEq_k(temp, 0) ? OK : Err;
        if (status == OK) *res = temp;
      } else {
        val[0] *= (-1);
        val[1] *= (-1);
        double temp = 0;
        status = s21_lg__(&temp, val);
        double ch = fabs(temp);
        status = (status == OK) ? ((ch < PRECISION) ? Err : OK) : status;
        if ((ch < 1) && (ch >= PRECISION)) ch = 1 / ch;
        status = (status == OK) && ostatok2_IsEq_k(ch, 1) ? OK : Err;
        if (status == OK) *res = temp;
      }
    }
  }
  return status;
}

int s21_sqrt(double *res, std::vector<double> val) {
  int status = (res && val.size() > 0 && val[0] >= 0) ? OK : Err;
  if (status == OK) *res = sqrt(val[0]);
  return status;
}
int s21_PI__(double *res, std::vector<double> val) {
  int status = (res && val.size() < SIZE_MAX - 1) ? OK : Err;
  if (status == OK) *res = M_PI;
  return status;
}
int s21_EXP_(double *res, std::vector<double> val) {
  int status = (res && val.size() < SIZE_MAX - 1) ? OK : Err;
  if (status == OK) *res = exp(1);
  return status;
}
int s21_avrg(double *res, std::vector<double> val) {
  int status = (res && val.size() > 0) ? OK : Err;
  if (status == OK) {
    *res = 0;
    for (size_t i = 0; i < val.size(); i++) *res += val[i];
    *res /= val.size();
  }
  return status;
}

int s21_fact(double *res, std::vector<double> val) {
  int status = (res && val.size() > 0) ? OK : Err;
  if (status == OK) {
    if (val[0] > 0) {
      *res = val[0] * tgamma(val[0]);
    } else {
      long double z = sin(M_PI * val[0]) * tgamma(1 - val[0]);
      if (fabsl(z) > 1E-16) {
        *res = M_PI / z;
      } else {
        status = Err;
      }
      // status = Err;
    }
  }
  return status;
}

int s21_umin(double *res, std::vector<double> val) {
  int status = (res && val.size() > 0) ? OK : Err;
  if (status == OK) *res = -val[0];
  return status;
}

std::map<std::string, Func_Model> Init_Func() {
  return {// name {prior, type, left, right, ref2fun, argc}
          {"EXP", Func_Model(UHG, CONSTANT, J_NO, J_NO, s21_EXP_, 0)},
          {"PI", Func_Model(UHG, CONSTANT, J_NO, J_NO, s21_PI__, 0)}

          ,
          {"(", Func_Model(BRA, NONTYPED, J_NO, JYES, nullptr, 0)},
          {";", Func_Model(COM, NONTYPED, JYES, JYES, nullptr, 0)},
          {")", Func_Model(BRA, NONTYPED, JYES, J_NO, nullptr, 0)}

          ,
          {"+", Func_Model(LOW, OPERAND2, JMAY, JYES, s21_add_, 2)},
          {"-", Func_Model(LOW, OPERAND2, JMAY, JYES, s21_sub_, 2)},
          {"unar-", Func_Model(UHG, FUNCTION, J_NO, JYES, s21_umin, 1)}

          ,
          {"*", Func_Model(MED, OPERAND2, JYES, JYES, s21_mul_, 2)},
          {"/", Func_Model(MED, OPERAND2, JYES, JYES, s21_div_, 2)},
          {"mod", Func_Model(MED, OPERAND2, JYES, JYES, s21_mod_, 2)},
          {"^", Func_Model(HGH, OPERAND2, JYES, JYES, s21_pow_, 2)}

          ,
          {"cos", Func_Model(UUH, FUNCTION, J_NO, JYES, s21_cos_, 1)},
          {"sin", Func_Model(UUH, FUNCTION, J_NO, JYES, s21_sin_, 1)},
          {"tan", Func_Model(UUH, FUNCTION, J_NO, JYES, s21_tan_, 1)},
          {"acos", Func_Model(UUH, FUNCTION, J_NO, JYES, s21_acos, 1)},
          {"asin", Func_Model(UUH, FUNCTION, J_NO, JYES, s21_asin, 1)},
          {"atan", Func_Model(UUH, FUNCTION, J_NO, JYES, s21_atan, 1)},
          {"sqrt", Func_Model(UUH, FUNCTION, J_NO, JYES, s21_sqrt, 1)},
          {"ln", Func_Model(UUH, FUNCTION, J_NO, JYES, s21_ln__, 1)},
          {"log", Func_Model(UUH, FUNCTION, J_NO, JYES, s21_log_, 1)},
          {"lg", Func_Model(UUH, FUNCTION, J_NO, JYES, s21_lg__, 2)},
          {"fact", Func_Model(UUH, ROPERAND, J_NO, JYES, s21_fact, 1)},
          {"avrg", Func_Model(UUH, FUNCTION, J_NO, JYES, s21_avrg, ARG_MAX)}

          ,
          {"!", Func_Model(UUH, ROPERAND, JYES, J_NO, s21_fact, 1)}};
}

};  // namespace s21

/*
std::string print_vect(std::vector<std::string> &v){
  std::string str = "{";
  for (size_t i = 0; i < v.size(); ++i){
    str += "`"+v[i]+"`";
    if (i < v.size()-1) str += "; ";
  }
  return str+"}";
}
*/

/*
std::string print_Func(const Func_Model &f){
  std::string str = "{";
  str += std::to_string(f.getPriority()) + "; ";
  str += std::to_string(f.getType()) + "; ";
  str += std::to_string(f.getLeft()) + "; ";
  str += std::to_string(f.getRight()) + "; ";
  str += std::to_string(f.getArgc()) + "}";
  return str;
}
*/

/*
std::string print_pair(std::vector<std::pair<std::string, Func_Model>> &m){
  std::string str = "{\n";
  for (const auto& [key, value] : m) {
    str += key + " : " + print_Func(value) + '\n';
  }
  return str+"}";
}
*/

/*
std::string print_map(const std::map<std::string, Func_Model> &m){
  std::string str = "{\n";
  for (const auto& [key, value] : m) {
    str += key + " : " + print_Func(value) + '\n';
  }
  return str+"}";
}
*/