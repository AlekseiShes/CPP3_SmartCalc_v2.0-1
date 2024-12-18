#include "smartcalc_view.h"

namespace s21 {

SmartCalculatorView::SmartCalculatorView(QWidget *parent)
    : QMainWindow(parent), calc_ui(new Ui::SmartCalculatorView) {
  calc_ui->setupUi(this);
  setStyleSheet("background-color: white;");
  // setWindowTitle("Calculator");

  button_group->addButton(calc_ui->pushButton_X);
  button_group->addButton(calc_ui->pushButton_0);
  button_group->addButton(calc_ui->pushButton_01);
  button_group->addButton(calc_ui->pushButton_02);
  button_group->addButton(calc_ui->pushButton_03);
  button_group->addButton(calc_ui->pushButton_04);
  button_group->addButton(calc_ui->pushButton_05);
  button_group->addButton(calc_ui->pushButton_06);
  button_group->addButton(calc_ui->pushButton_07);
  button_group->addButton(calc_ui->pushButton_08);
  button_group->addButton(calc_ui->pushButton_09);
  button_group->addButton(calc_ui->pushButton_mult);
  button_group->addButton(calc_ui->pushButton_div);
  button_group->addButton(calc_ui->pushButton_degree);
  button_group->addButton(calc_ui->pushButton_mod);
  button_group->addButton(calc_ui->pushButton_e);
  button_group->addButton(calc_ui->pushButton_plus);
  button_group->addButton(calc_ui->pushButton_minus);
  button_group->addButton(calc_ui->pushButton_openBracket);
  button_group->addButton(calc_ui->pushButton_exclimation_point);
  button_group->addButton(calc_ui->pushButton_dot_coma);
  button_group->addButton(calc_ui->pushButton_closeBracket);

  function_group->addButton(calc_ui->pushButton_sin);
  function_group->addButton(calc_ui->pushButton_cos);
  function_group->addButton(calc_ui->pushButton_tan);
  function_group->addButton(calc_ui->pushButton_asin);
  function_group->addButton(calc_ui->pushButton_acos);
  function_group->addButton(calc_ui->pushButton_atan);
  function_group->addButton(calc_ui->pushButton_ln);
  function_group->addButton(calc_ui->pushButton_log);
  function_group->addButton(
      calc_ui->pushButton_lg);  // lg(-32;-0.5)=-0.2  // (-32) ^ (-0.2) = -0.5
  function_group->addButton(calc_ui->pushButton_sqrt);
  function_group->addButton(calc_ui->pushButton_fact);

  constant_group->addButton(calc_ui->pushButton_PI);
  constant_group->addButton(calc_ui->pushButton_EXP);

  connect(button_group, SIGNAL(idClicked(int)), this,
          SLOT(button_pressed(int)));

  connect(function_group, SIGNAL(idClicked(int)), this,
          SLOT(function_pressed(int)));

  connect(constant_group, SIGNAL(idClicked(int)), this,
          SLOT(constant_pressed(int)));

  // connect(calc_ui->pushButton_credit, SIGNAL(clicked()), this,
  //         SLOT(on_pushButton_credit_clicked()));

  // connect(calc_ui->pushButton_deposit, SIGNAL(clicked()), this,
  //         SLOT(on_pushButton_deposit_clicked()));
};

SmartCalculatorView::~SmartCalculatorView() {
  delete calc_ui;
  delete button_group;
  delete function_group;
  delete constant_group;
};

void SmartCalculatorView::button_pressed(int sender) {
  QString new_label;
  QString tmp_label;
  QString button_text = button_group->button(sender)->text();
  // if (button_text == "EXP") button_text = 'e';
  if (calc_ui->result_tmp->text().endsWith("=", Qt::CaseInsensitive)) {
    calc_ui->result_tmp->setText(calc_ui->result_show->text());
    calc_ui->result_show->setText("");
  }
  if (button_text.toDouble() || button_text == "0" || button_text == 'x') {
    tmp_label = calc_ui->result_tmp->text() + button_text;
    calc_ui->result_tmp->setText(tmp_label);
  } else {
    tmp_label = calc_ui->result_tmp->text();
    new_label = calc_ui->result_show->text() + tmp_label + button_text;
    calc_ui->result_show->setText(new_label);
    calc_ui->result_tmp->clear();
  }
};

QString SmartCalculatorView::check_result_show(QButtonGroup *group,
                                               int sender) {
  QString label;
  QString button_text = group->button(sender)->text();
  if (calc_ui->result_tmp->text().contains(QChar('='))) {
    calc_ui->result_tmp->setText(calc_ui->result_show->text());
    calc_ui->result_show->setText("");
  }
  if (calc_ui->result_show->text() == "" ||
      calc_ui->result_tmp->text() == "0") {
    label = button_text;
  } else {
    label = calc_ui->result_show->text() + button_text;
  }
  return label;
};

void SmartCalculatorView::function_pressed(int sender) {
  QString new_label = check_result_show(function_group, sender);
  calc_ui->result_show->setText(new_label + "(");
};

void SmartCalculatorView::constant_pressed(int sender) {
  QString new_label = check_result_show(constant_group, sender);
  calc_ui->result_show->setText(new_label);
};

void SmartCalculatorView::on_pushButton_AC_clicked() {
  calc_ui->result_tmp->clear();
  calc_ui->result_show->clear();
  calc_ui->lineEdit_xValue->clear();
  calc_ui->lineEdit_xMinValue->clear();
  calc_ui->lineEdit_xMaxValue->clear();
  calc_ui->lineEdit_yMinValue->clear();
  calc_ui->lineEdit_yMaxValue->clear();
  calc_ui->widget_graph->clearGraphs();
  calc_ui->widget_graph->replot();
}

int double_size(double res) {
  int size = 7;
  res -= (int)res;
  res *= pow(10, size);
  res = round(res);
  if ((int)res == 0)
    size = 0;
  else
    for (; size && (((int)(10 * (res / 10 - (int)(res / 10)))) == 0); size--)
      res /= 10;
  return size;
};

QString SmartCalculatorView::check_before_eq() {
  QPushButton *button = (QPushButton *)sender();
  QString label;
  if (calc_ui->result_tmp->text() != "" &&
      !(calc_ui->result_tmp->text().contains('='))) {
    label = calc_ui->result_show->text() + calc_ui->result_tmp->text();
    calc_ui->result_tmp->setText(label + button->text());
    calc_ui->result_show->clear();
  } else if (!(calc_ui->result_tmp->text().contains('='))) {
    if (button->text() != '=')
      label = calc_ui->result_show->text() + button->text();
    else
      label = calc_ui->result_show->text();
    calc_ui->result_tmp->setText(calc_ui->result_show->text() + button->text());
    calc_ui->result_show->clear();
  } else if (calc_ui->result_show->text() == "" &&
             (calc_ui->result_tmp->text().contains('x'))) {
    label = calc_ui->result_tmp->text();
  } else {
    label = calc_ui->result_show->text();
    calc_ui->result_show->clear();
  }
  return label;
};

void SmartCalculatorView::on_pushButton_eq_clicked() {
  QString xValue = calc_ui->lineEdit_xValue->text();
  QString new_label = check_before_eq();
  if (new_label.contains("x")) {
    if (xValue != "") {
      xValue = QString("(") + xValue + QString(")");
      new_label.replace(QString("x"), QString(xValue));
      calc_ui->result_tmp->setText(new_label);
    }
  }
  QByteArray byte_arr;
  byte_arr = new_label.toLocal8Bit();
  std::string prestr = byte_arr.constData();
  try {
    double res = this->controller->calculatorAction(prestr);
    QString strValue = QString::number(res, 'f', double_size(res));
    calc_ui->result_show->setText(strValue);
  } catch (std::exception &err) {
    QMessageBox msgBox;
    std::string t = err.what();  /// err.what();
    msgBox.setText(
        QString::fromStdString("Please check the entered data:" + t));  /// +t
    msgBox.exec();
    calc_ui->result_show->clear();
    calc_ui->result_tmp->clear();
    calc_ui->lineEdit_xValue->clear();
    return;
  }
};

void SmartCalculatorView::on_pushButton_dot_clicked() {
  QPushButton *button = (QPushButton *)sender();
  QString tmp_label;
  if (!(calc_ui->result_tmp->text().contains('.')))
    calc_ui->result_tmp->setText(calc_ui->result_tmp->text() + ".");
  if ((calc_ui->result_tmp->text().contains('='))) {
    if (!calc_ui->result_show->text().contains('.'))
      calc_ui->result_tmp->setText(calc_ui->result_show->text() + ".");
    else
      calc_ui->result_tmp->setText(calc_ui->result_show->text());
    calc_ui->result_show->clear();
  }
  if (calc_ui->result_tmp->text() == ".") {
    tmp_label = "0" + button->text();
    calc_ui->result_tmp->setText(tmp_label);
  }
};

void SmartCalculatorView::on_pushButton_back_clicked() {
  if (calc_ui->result_tmp->text() != "") {
    QString text = calc_ui->result_tmp->text();
    if (text.contains("=")) {
      text = calc_ui->result_show->text();
      calc_ui->result_show->clear();
    } else {
      text.chop(1);
    }
    if (text.isEmpty()) {
      text = "";
    }
    calc_ui->result_tmp->setText(text);
  } else if (calc_ui->result_show->text() != "") {
    QString text = calc_ui->result_show->text();
    text.chop(1);
    if (text.isEmpty()) {
      text = "";
    }
    calc_ui->result_show->setText(text);
  }
};

void SmartCalculatorView::on_pushButton_close_clicked() { QWidget::close(); };

void SmartCalculatorView::on_pushButton_graph_clicked() {
  calc_ui->widget_graph->clearGraphs();
  x.clear();
  y.clear();
  int points = setGraphData();
  doGraph(points);
};

int SmartCalculatorView::setGraphData() {
  QString xMin_text = calc_ui->lineEdit_xMinValue->text();
  (xMin_text == "") ? xBegin = -10 : xBegin = xMin_text.toDouble();
  QString xMax_text = calc_ui->lineEdit_xMaxValue->text();
  (xMax_text == "") ? xEnd = 10 : xEnd = xMax_text.toDouble();
  QString yMin_text = calc_ui->lineEdit_yMinValue->text();
  (yMin_text == "") ? yBegin = -10 : yBegin = yMin_text.toDouble();
  QString yMax_text = calc_ui->lineEdit_yMaxValue->text();
  (yMax_text == "") ? yEnd = 10 : yEnd = yMax_text.toDouble();
  calc_ui->widget_graph->xAxis->setRange(xBegin, xEnd);
  calc_ui->widget_graph->yAxis->setRange(yBegin, yEnd);
  int points = (int)calc_ui->lineEdit_xStep->text().toInt();
  QMessageBox msgBox;
  if (points < 10) {
    msgBox.setText("Not enough points. Fixed to default.");
    msgBox.exec();
    points = 500;
  } else if (points > 1000) {
    msgBox.setText("Points>1000, It will take a lot of time");
    msgBox.exec();
    if (QMessageBox::question(this, "Accept?", "",
                              QMessageBox::Yes | QMessageBox::No) ==
        QMessageBox::No) {
      msgBox.setText("Fixed to default 500");
      msgBox.exec();
      points = 500;
    }
  }
  calc_ui->lineEdit_xStep->setText(QString::number(points, 10));
  return points;
};

void SmartCalculatorView::doGraph(int points) {
  int points_in_graph = 0, total_points_cnt = 0, i_graph = 0, status = 0,
      old_status = 0;
  double X = 0.0, Y = 0.0;
  h = ((xEnd - xBegin) / points);
  QMessageBox msgBox;
  if ((xEnd > xBegin) && (yEnd > yBegin)) {
    for (int i = 0; i < points; i++) {
      X = xBegin + h * i;
      QString y_text;
      if (calc_ui->result_tmp->text() == "" &&
          calc_ui->result_show->text() == "") {
        msgBox.setText("Please, input the data.");
        msgBox.exec();
        return;
      } else if (calc_ui->result_show->text() != "") {
        y_text = calc_ui->result_show->text();
      } else if (calc_ui->result_tmp->text() != "") {
        y_text = calc_ui->result_tmp->text();
      }
      if (y_text != "") {
        if (y_text.contains("x"))
          y_text.replace(QString("x"),
                         QString("(" + QString::number(X, 'f', 7) + ")"));
        try {
          QByteArray byte_arr;
          byte_arr = y_text.toLocal8Bit();
          std::string prestr = byte_arr.constData();
          Y = controller->calculatorAction(prestr);
          status = 0;
        } catch (...) {
          status = 1;
          // msgBox.setText("Check data.");
          // msgBox.exec();
          // return;
        }
      } else {
        status = 1;
      }
      if (status == 0) {
        x.push_back(X);
        y.push_back(Y);
        total_points_cnt++;
        points_in_graph += (!((yBegin > Y) || (Y > yEnd)));
      }
      if ((status != 0 && old_status == 0) ||
          ((yBegin * 3 > Y) || (Y > yEnd * 3))) {
        calc_ui->widget_graph->addGraph();
        calc_ui->widget_graph->graph(i_graph++)->addData(x, y);
        x.clear();
        y.clear();
      }
      old_status = status;
    }
    calc_ui->widget_graph->addGraph();
    calc_ui->widget_graph->graph(i_graph++)->addData(x, y);
    calc_ui->widget_graph->replot();
    while (i_graph) calc_ui->widget_graph->graph(--i_graph)->data()->clear();
  }
  if (total_points_cnt == 0 || (xEnd < xBegin)) {
    msgBox.setText("Function not defined in this XRange");
    msgBox.exec();
  }
  if ((total_points_cnt > 0 && points_in_graph == 0) || (yEnd < yBegin)) {
    msgBox.setText("Function not defined in this YRange");
    msgBox.exec();
  }
};

void SmartCalculatorView::on_pushButton_credit_clicked() {
  // if (calc_ui->pushButton_credit->isCheckable()) {
  CreditView credit(this);
  credit.init_and_exec(this->controller);
  //   calc_ui->pushButton_credit->setCheckable(false);
  // } else {
  //   calc_ui->pushButton_credit->setCheckable(true);
  // }
};

void SmartCalculatorView::on_pushButton_deposit_clicked() {
  // if (calc_ui->pushButton_deposit->isCheckable()) {
  DepositView deposit(this);
  deposit.init_and_exec(this->controller);
  //   calc_ui->pushButton_deposit->setCheckable(false);
  // } else {
  //   calc_ui->pushButton_deposit->setCheckable(true);
  // }
};

void SmartCalculatorView::init_and_show(SmartCalculatorController *controller) {
  this->controller = controller;
  this->show();
};
};  // namespace s21
