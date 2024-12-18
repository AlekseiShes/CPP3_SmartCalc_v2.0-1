#ifndef SMARTCALC_VIEW_V2_H
#define SMARTCALC_VIEW_V2_H

#include <QButtonGroup>
#include <QByteArray>
#include <QDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <QVector>
#include <QWidget>
#include <cctype>
#include <cmath>
#include <list>

#include "../Controller/smartcalc_controller.h"
#include "credit_view.h"
#include "deposit_view.h"
#include "qcustomplot.h"
#include "ui_credit.h"
#include "ui_deposit.h"
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SmartCalculatorView;
};  // namespace Ui
QT_END_NAMESPACE

namespace s21 {
class SmartCalculatorView : public QMainWindow {
  Q_OBJECT
 public:
  SmartCalculatorView(QWidget* parent = nullptr);
  ~SmartCalculatorView();
  SmartCalculatorController* controller;
  void init_and_show(SmartCalculatorController* controller);

 private slots:
  QString check_result_show(QButtonGroup* group, int sender);
  QString check_before_eq();
  void button_pressed(int sender);
  void function_pressed(int sender);
  void constant_pressed(int sender);
  void on_pushButton_AC_clicked();
  void on_pushButton_eq_clicked();
  void on_pushButton_dot_clicked();
  void on_pushButton_back_clicked();
  void on_pushButton_close_clicked();
  void on_pushButton_graph_clicked();
  void on_pushButton_credit_clicked();
  void on_pushButton_deposit_clicked();

 private:
  const int MAX_INP_SIZE = 255;
  void doGraph(int points);
  int setGraphData();
  QButtonGroup* button_group = new QButtonGroup(this);
  QButtonGroup* function_group = new QButtonGroup(this);
  QButtonGroup* constant_group = new QButtonGroup(this);
  Ui::SmartCalculatorView* calc_ui;
  QVector<double> x;
  QVector<double> y;
  double xBegin, xEnd, yBegin, yEnd, h;
};
};  // namespace s21
#endif
