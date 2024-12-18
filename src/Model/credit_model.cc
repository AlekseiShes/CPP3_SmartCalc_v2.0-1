#include "credit_model.h"

#include <iostream>
namespace s21 {
// CommonFunctions start
std::vector<double> CreditCalculations::CommonFunctions::getInputData(
    const std::string &credit_sum, const std::string &period,
    const std::string &percent) {
  if (credit_sum == "" || period == "" || percent == "" ||
      !this->strIsNumber(credit_sum) || !this->strIsNumber(period) ||
      !this->strIsNumber(percent))
    throw std::invalid_argument("Enter data");
  std::string credit_sum_tmp = credit_sum;
  std::string period_tmp = period;
  std::string percent_tmp = percent;
  double credit_sum_num = 0;
  double period_num = 0;
  double percent_num = 0;
  credit_sum_num = std::stod(credit_sum_tmp);
  period_num = std::stod(period_tmp);
  percent_num = std::stod(percent_tmp);
  if (credit_sum_num <= 0 || period_num <= 0 || percent_num <= 0 ||
      !std::isnormal(credit_sum_num) || !std::isnormal(period_num) ||
      !std::isnormal(percent_num))
    throw std::invalid_argument("Check input data");
  std::vector<double> input_data{};
  input_data.push_back(credit_sum_num);
  input_data.push_back(period_num);
  input_data.push_back(percent_num);
  return input_data;
};

bool CreditCalculations::CommonFunctions::strIsNumber(const std::string &str) {
  int flag = 0;
  int count = 0;
  bool check = true;
  for (size_t i = 0; i != str.length(); ++i) {
    if (!std::isdigit(str[i]) && str[i] != '.') ++flag;
    if (str[i] == '.') ++count;
  }
  if (flag > 0 || count > 1) check = false;
  return check;
};
// CommonFunctions end

// Annuity start
std::map<std::string, std::vector<double>>
CreditCalculations::Annuity::calculate(const std::string &credit_sum,
                                       const std::string &period,
                                       const std::string &percent) {
  try {
    std::vector<double> data = this->getInputData(credit_sum, period, percent);
    std::map<std::string, std::vector<double>> result =
        this->annuityCredit(data);
    return result;
  } catch (std::exception &err) {
    throw err;
  }
};

std::map<std::string, std::vector<double>>
CreditCalculations::Annuity::annuityCredit(const std::vector<double> &data) {
  if (data.empty() && data.size() < 3) throw std::logic_error("Enter data");
  double S = data[0];
  double T = data[1];
  double P = data[2] / 100.0 / 12.0;
  double monthly_payment = S * (P * pow(1 + P, T)) / (pow(1 + P, T) - 1);
  monthly_payment = round(monthly_payment * 100.0) / 100.0;
  double overpayment = monthly_payment * T - S;
  overpayment = round(overpayment * 100.0) / 100.0;
  double full_payment = overpayment + S;
  full_payment = round(full_payment * 100) / 100;
  std::vector<double> payments_per_mounth{};
  for (int i = 0; i < (int)T; i++)
    payments_per_mounth.push_back(monthly_payment);
  std::map<std::string, std::vector<double>> res = {
      {"TotalPayment", {full_payment}},
      {"CreditOverpayment", {overpayment}},
      {"MonthPayments", payments_per_mounth}};
  return res;
};
// Annuity end

// Differential start
std::map<std::string, std::vector<double>>
CreditCalculations::Differential::calculate(const std::string &credit_sum,
                                            const std::string &period,
                                            const std::string &percent) {
  try {
    std::vector<double> data = this->getInputData(credit_sum, period, percent);
    std::map<std::string, std::vector<double>> result =
        this->differentialCredit(data);
    return result;
  } catch (std::exception &err) {
    throw err;
  }
};

std::map<std::string, std::vector<double>>
CreditCalculations::Differential::differentialCredit(
    const std::vector<double> &data) {
  if (data.empty() && data.size() < 3) throw std::logic_error("Enter data");
  double S = data[0];
  double T = data[1];
  double P = data[2] / 100.0 / 12.0;
  double D = S / T;
  double overpayment = 0;
  std::vector<double> payments_per_mounth{};
  for (int i = 1; i <= T; i++) {
    double tmp = D + (S - D * (i - 1)) * P;
    tmp = round(tmp * 100) / 100;
    payments_per_mounth.push_back(tmp);
    overpayment += tmp - D;
  };
  double full_payment = overpayment + S;
  full_payment = round(full_payment * 100) / 100;
  std::map<std::string, std::vector<double>> res = {
      {"TotalPayment", {full_payment}},
      {"CreditOverpayment", {overpayment}},
      {"MonthPayments", payments_per_mounth}};
  return res;
};
// Differential end
};  // namespace s21
