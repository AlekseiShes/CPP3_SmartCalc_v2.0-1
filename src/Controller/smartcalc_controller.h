#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QString>
#include <vector>

#include "../Model/credit_model.h"
#include "../Model/deposit_model.h"
#include "../Model/smartcalc_model.h"

namespace s21 {
class SmartCalculatorController {
 public:
  SmartCalculatorController(SmartCalculatorModel *model) {
    this->model = model;
  }

  s21::SmartCalculatorModel *model;
  s21::CreditCalculations *credit;
  s21::DepositCalculations *deposit;

  double calculatorAction(const std::string &str_action);

  std::map<std::string, std::vector<double>> creditCalculatorAction(
      const QString &credit_sum, const QString &period, const QString &percent,
      bool isAnnuityType = true);

  std::map<std::string, std::vector<double>> depositCalculatorAction(
      const QString &deposit_sum, const QString &period, const QString &percent,
      const QString &tax, const QString &refill_capacity,
      const QString &write_off_capacity, double &percents_period,
      bool isSimple = true);
};
};  // namespace s21
#endif
