#ifndef DEPOSIT_MODEL_V2_H
#define DEPOSIT_MODEL_V2_H

#include "smartcalc_model.h"

namespace s21 {

class DepositCalculations {
 private:
  class CommonFunctions {
   public:
    std::vector<double> getInputData(const std::string &deposit_sum,
                                     const std::string &period,
                                     const std::string &percent,
                                     const std::string &tax,
                                     const std::string &refill_capacity,
                                     const std::string &write_off_capacity,
                                     double &percents_period);
    const std::vector<std::string> getInputStrings(
        const std::string &deposit_sum, const std::string &period,
        const std::string &percent, const std::string &tax,
        const std::string &refill_capacity,
        const std::string &write_off_capacity);
    bool checkInputStr(const std::vector<std::string> &strings);
    bool strIsNumber(const std::string &str);
    bool checkInputNum(const std::vector<double> &numbers);
    void checkLogic(const std::vector<double> &data);
  };

 public:
  class SimplePercent : public CommonFunctions {
   public:
    std::map<std::string, std::vector<double>> calculate(
        const std::string &deposit_sum, const std::string &period,
        const std::string &percent, const std::string &tax,
        const std::string &refill_capacity,
        const std::string &write_off_capacity, double &percents_period);

   private:
    std::map<std::string, std::vector<double>> simpleDeposit(
        const std::vector<double> &data);
    const std::vector<double> calcSimpleDeposit(double S, double T, double P,
                                                double N, double R, double W,
                                                double Pr);
  };
  class CapitalPercent : public CommonFunctions {
   public:
    std::map<std::string, std::vector<double>> calculate(
        const std::string &deposit_sum, const std::string &period,
        const std::string &percent, const std::string &tax,
        const std::string &refill_capacity,
        const std::string &write_off_capacity, double &percents_period);

   private:
    std::map<std::string, std::vector<double>> сapitalizedDeposit(
        const std::vector<double> &data);
    const std::vector<double> calcCapitalizedDeposit(double S, double T,
                                                     double P, double N,
                                                     double R, double W,
                                                     double Pr);
  };
  s21::DepositCalculations::SimplePercent simple_calculation;
  s21::DepositCalculations::CapitalPercent capital_calculation;
};
};  // namespace s21
#endif