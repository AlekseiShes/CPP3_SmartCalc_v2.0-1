#ifndef CREDIT_MODEL_V2_H
#define CREDIT_MODEL_V2_H

#include "smartcalc_model.h"

namespace s21 {

class CreditCalculations {
 private:
  class CommonFunctions {
   public:
    std::vector<double> getInputData(const std::string &credit_sum,
                                     const std::string &period,
                                     const std::string &percent);
    bool strIsNumber(const std::string &str);
  };

 public:
  class Annuity : public CommonFunctions {
   public:
    std::map<std::string, std::vector<double>> calculate(
        const std::string &credit_sum, const std::string &period,
        const std::string &percent);

   private:
    std::map<std::string, std::vector<double>> annuityCredit(
        const std::vector<double> &data);
  };

  class Differential : public CommonFunctions {
   public:
    std::map<std::string, std::vector<double>> calculate(
        const std::string &credit_sum, const std::string &period,
        const std::string &percent);

   private:
    std::map<std::string, std::vector<double>> differentialCredit(
        const std::vector<double> &data);
  };
  s21::CreditCalculations::Annuity annuity_calculation;
  s21::CreditCalculations::Differential differential_calculation;
};
};  // namespace s21

#endif
