#include "smartcalc_controller.h"

namespace s21 {

double SmartCalculatorController::calculatorAction(
    const std::string &str_action) {
  try {
    double res = model->calculator.calculating(str_action);
    return res;
  } catch (std::exception &e) {
    throw;
  }
};

std::map<std::string, std::vector<double>>
SmartCalculatorController::creditCalculatorAction(const QString &credit_sum,
                                                  const QString &period,
                                                  const QString &percent,
                                                  bool isAnnuityType) {
  try {
    if (isAnnuityType)
      return credit->annuity_calculation.calculate(credit_sum.toStdString(),
                                                   period.toStdString(),
                                                   percent.toStdString());

    return credit->differential_calculation.calculate(
        credit_sum.toStdString(), period.toStdString(), percent.toStdString());

  } catch (std::exception &err) {
    throw err;
  }
};

std::map<std::string, std::vector<double>>
SmartCalculatorController::depositCalculatorAction(
    const QString &deposit_sum, const QString &period, const QString &percent,
    const QString &tax, const QString &refill_capacity,
    const QString &write_off_capacity, double &percents_period, bool isSimple) {
  try {
    if (isSimple)
      return deposit->simple_calculation.calculate(
          deposit_sum.toStdString(), period.toStdString(),
          percent.toStdString(), tax.toStdString(),
          refill_capacity.toStdString(), write_off_capacity.toStdString(),
          percents_period);

    return deposit->capital_calculation.calculate(
        deposit_sum.toStdString(), period.toStdString(), percent.toStdString(),
        tax.toStdString(), refill_capacity.toStdString(),
        write_off_capacity.toStdString(), percents_period);

  } catch (std::exception &err) {
    throw err;
  }
};
};  // namespace s21
