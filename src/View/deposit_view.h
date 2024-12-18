#ifndef DEPOSIT_VIEW_H_
#define DEPOSIT_VIEW_H_

#include "../Controller/smartcalc_controller.h"
#include "smartcalc_view.h"
#include "ui_deposit.h"
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class DepositView;
};  // namespace Ui
QT_END_NAMESPACE

namespace s21 {

class DepositView : public QDialog {
  Q_OBJECT
 public:
  explicit DepositView(QWidget *parent = nullptr);
  ~DepositView();

  void init_and_exec(s21::SmartCalculatorController *controller);

 private:
  Ui::DepositView *deposit_ui;
  s21::SmartCalculatorController *controller_;
  double initInput();

 private slots:
  void calculateDeposit(bool Simple = true);
  void on_pushButton_closedeposit_clicked();
  void on_pushButton_calcDeposit_clicked();
};
};  // namespace s21
#endif
