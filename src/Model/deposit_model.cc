#include "deposit_model.h"

namespace s21 {
// CommonFunctions start
std::vector<double> DepositCalculations::CommonFunctions::getInputData(
    const std::string &deposit_sum, const std::string &period,
    const std::string &percent, const std::string &tax,
    const std::string &refill_capacity, const std::string &write_off_capacity,
    double &percents_period) {
  std::vector<std::string> input_strings = getInputStrings(
      deposit_sum, period, percent, tax, refill_capacity, write_off_capacity);
  std::vector<double> input_data{};
  if (!checkInputStr(input_strings))
    throw std::invalid_argument("Check input data");
  else {
    for (size_t i = 0; i != input_strings.size(); ++i)
      input_data.push_back(std::stod(input_strings[i]));
  }
  if (!checkInputNum(input_data))
    throw std::invalid_argument("Incorrect data");
  else {
    input_data.push_back(percents_period);
  }
  this->checkLogic(input_data);
  return input_data;
};

const std::vector<std::string>
DepositCalculations::CommonFunctions::getInputStrings(
    const std::string &deposit_sum, const std::string &period,
    const std::string &percent, const std::string &tax,
    const std::string &refill_capacity, const std::string &write_off_capacity) {
  std::vector<std::string> tmp{};
  tmp.push_back(deposit_sum);
  tmp.push_back(period);
  tmp.push_back(percent);
  tmp.push_back(tax);
  tmp.push_back(refill_capacity);
  tmp.push_back(write_off_capacity);
  return tmp;
};

bool DepositCalculations::CommonFunctions::checkInputStr(
    const std::vector<std::string> &strings) {
  int flag = 0;
  bool check = true;
  for (size_t i = 0; i != strings.size(); ++i)
    if (strings[i] == "" || !this->strIsNumber(strings[i])) ++flag;
  if (flag > 0) check = false;
  return check;
};

bool DepositCalculations::CommonFunctions::strIsNumber(const std::string &str) {
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

bool DepositCalculations::CommonFunctions::checkInputNum(
    const std::vector<double> &numbers) {
  int flag = 0;
  bool check = true;
  for (size_t i = 0; i != 3; ++i)  // 3 - check sum, period, percent
    if (!std::isnormal(numbers[i])) ++flag;
  for (size_t i = 3; i != 6; ++i)  // check tax, refill, write-off
    if (numbers[i] < 0) ++flag;
  if (flag > 0) check = false;
  return check;
};

void DepositCalculations::CommonFunctions::checkLogic(
    const std::vector<double> &data) {
  if (data[6] == 4 && data[1] < 3)
    throw std::logic_error("Period is less than 1 quarter");
  if (data[6] == 1 && data[1] < 12)
    throw std::logic_error("Period is less than 1 year");
  if (data[5] > data[0] * data[2] / 100.0 / 12) {
    throw std::logic_error("Write-off can't be more than income");
  }
};
// CommonFunctions end

// SimplePercent start
std::map<std::string, std::vector<double>>
DepositCalculations::SimplePercent::calculate(
    const std::string &deposit_sum, const std::string &period,
    const std::string &percent, const std::string &tax,
    const std::string &refill_capacity, const std::string &write_off_capacity,
    double &percents_period) {
  try {
    std::vector<double> data =
        this->getInputData(deposit_sum, period, percent, tax, refill_capacity,
                           write_off_capacity, percents_period);
    std::map<std::string, std::vector<double>> result =
        this->simpleDeposit(data);
    return result;
  } catch (std::exception &err) {
    throw err;
  }
};

std::map<std::string, std::vector<double>>
DepositCalculations::SimplePercent::simpleDeposit(
    const std::vector<double> &data) {
  if (data.empty() && data.size() < 3) throw std::logic_error("Enter data");
  double S = data[0];
  double T = data[1];
  double P = data[2] / 100.0;
  double N = data[3] / 100.0;
  double R = data[4];
  double W = data[5];
  double Pr = data[6];
  double profit = 0;
  double tax_sum = 0;
  double full_payment = 0;
  if (R == 0 && W == 0) {
    profit = S * P * T / 12.0;
    tax_sum = round(profit * N * 100) / 100;
    full_payment = round((S + profit - tax_sum) * 100) / 100;
  } else {
    std::vector<double> out = this->calcSimpleDeposit(S, T, P, N, R, W, Pr);
    profit = out[0];
    tax_sum = out[1];
    full_payment = out[2];
  }
  std::map<std::string, std::vector<double>> res = {
      {"Profit", {profit}},
      {"TotalTax", {tax_sum}},
      {"TotalPayment", {full_payment}}};
  return res;
};

const std::vector<double> DepositCalculations::SimplePercent::calcSimpleDeposit(
    double S, double T, double P, double N, double R, double W, double Pr) {
  std::vector<double> res{};
  double profit = 0;
  if (Pr == 12) {
    for (int i = 1; i <= T; i++) {
      S += (R - W);
      profit += S * P / 12.0;
    }
  } else if (Pr == 4) {
    for (int i = 1; i <= int(T / 3); i++) {
      S += (R - W);
      profit += S * P / 4.0;
    }
  } else if (Pr == 1) {
    for (int i = 1; i <= int(T / 12); i++) {
      S += (R - W);
      profit += S * P;
    }
  } else if (Pr == 0) {
    S += (R - W);
    profit = S * P;
  }
  profit = round(profit * 100) / 100;
  double tax_sum = round(profit * N * 100) / 100;
  double full_payment = round((S + profit - tax_sum) * 100) / 100;
  res.push_back(profit);
  res.push_back(tax_sum);
  res.push_back(full_payment);
  return res;
};
// SimplePercent end

// CapitalPercent start
std::map<std::string, std::vector<double>>
DepositCalculations::CapitalPercent::calculate(
    const std::string &deposit_sum, const std::string &period,
    const std::string &percent, const std::string &tax,
    const std::string &refill_capacity, const std::string &write_off_capacity,
    double &percents_period) {
  try {
    std::vector<double> data =
        this->getInputData(deposit_sum, period, percent, tax, refill_capacity,
                           write_off_capacity, percents_period);
    std::map<std::string, std::vector<double>> result =
        this->сapitalizedDeposit(data);
    return result;
  } catch (std::exception &err) {
    throw err;
  }
};

std::map<std::string, std::vector<double>>
DepositCalculations::CapitalPercent::сapitalizedDeposit(
    const std::vector<double> &data) {
  if (data.empty() && data.size() < 3) throw std::logic_error("Enter data");
  double S = data[0];          // deposit_sum
  double T = data[1];          // period
  double P = data[2] / 100.0;  // percent
  double N = data[3] / 100.0;  // tax
  double R = data[4];          // refill_capacity
  double W = data[5];          // write_off_capacity
  double Pr = data[6];         // percents_period
  std::vector<double> out = this->calcCapitalizedDeposit(S, T, P, N, R, W, Pr);
  double profit = out[0];
  double tax_sum = out[1];
  double full_payment = out[2];
  std::map<std::string, std::vector<double>> res = {
      {"Profit", {profit}},
      {"TotalTax", {tax_sum}},
      {"TotalPayment", {full_payment}}};
  return res;
};

const std::vector<double>
DepositCalculations::CapitalPercent::calcCapitalizedDeposit(double S, double T,
                                                            double P, double N,
                                                            double R, double W,
                                                            double Pr) {
  std::vector<double> res{};
  double profit = 0;
  double tmp = 0;
  if (Pr == 12) {
    for (int i = 1; i <= T; i++) {
      S += (R - W);
      tmp = S * pow((1 + P / 12), 1) - S;
      profit += tmp;
      S += tmp;
    }
  } else if (Pr == 4) {
    for (int i = 1; i <= int(T / 3); i++) {
      S += (R - W);
      tmp = S * pow((1 + P / 4), 1) - S;
      profit += tmp;
      S += tmp;
    }
  } else if (Pr == 1) {
    for (int i = 1; i <= int(T / 12); i++) {
      S += (R - W);
      tmp = S * P;
      profit += tmp;
      S += tmp;
    }
  } else if (Pr == 0) {
    S += (R - W);
    profit = S * P;
    S += profit;
  }
  profit = round(profit * 100) / 100;
  double tax_sum = round(profit * N * 100) / 100;
  double full_payment = round((S - tax_sum) * 100) / 100;
  res.push_back(profit);
  res.push_back(tax_sum);
  res.push_back(full_payment);
  return res;
};
// CapitalPercent end
};  // namespace s21
