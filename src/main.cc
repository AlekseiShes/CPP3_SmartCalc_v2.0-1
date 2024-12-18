#include <QApplication>

#include "Controller/smartcalc_controller.h"
#include "Model/smartcalc_model.h"
#include "View/smartcalc_view.h"

using namespace s21;

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  s21::SmartCalculatorModel model;
  s21::SmartCalculatorController controller(&model);
  s21::SmartCalculatorView view;
  view.init_and_show(&controller);

  return a.exec();
}
