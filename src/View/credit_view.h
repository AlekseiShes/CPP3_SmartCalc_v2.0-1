#ifndef CREDIT_VIEW_H_
#define CREDIT_VIEW_H_

#include "../Controller/smartcalc_controller.h"
#include "smartcalc_view.h"
#include "ui_credit.h"
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class CreditView;
};  // namespace Ui
QT_END_NAMESPACE

namespace s21 {

class CreditView : public QDialog {
  Q_OBJECT
 public:
  explicit CreditView(QWidget *parent = nullptr);
  ~CreditView();

  void init_and_exec(s21::SmartCalculatorController *controller);

 private:
  Ui::CreditView *credit_ui;
  s21::SmartCalculatorController *controller_;

 private slots:
  void calculateCredit(bool Annuity = true);
  void on_pushButton_closecredit_clicked();
  void on_pushButton_calcCredit_clicked();
};
};  // namespace s21
#endif
