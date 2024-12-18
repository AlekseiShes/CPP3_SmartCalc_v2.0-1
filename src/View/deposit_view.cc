#include "deposit_view.h"

#include "ui_deposit.h"

namespace s21 {

DepositView::DepositView(QWidget *parent)
    : QDialog(parent), deposit_ui(new Ui::DepositView) {
  deposit_ui->setupUi(this);
  connect(deposit_ui->pushButton_closedeposit, SIGNAL(clicked()), this,
          SLOT(on_pushButton_closedeposit_clicked()));
};

DepositView::~DepositView() { delete deposit_ui; };

void DepositView::on_pushButton_closedeposit_clicked() { QWidget::close(); };

void DepositView::on_pushButton_calcDeposit_clicked() {
  if (deposit_ui->radioButton_simple->isChecked()) {
    this->calculateDeposit(true);
  } else if (deposit_ui->radioButton_capital->isChecked()) {
    this->calculateDeposit(false);
  } else {
    QMessageBox msgBox;
    msgBox.setText("Attention! Select deposit type.");
    msgBox.exec();
  }
};

int double_size_deposit(double res) {
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

double DepositView::initInput() {
  double percentPeriod = 0;
  if (deposit_ui->percentBet->text() == "")
    deposit_ui->percentBet->setText("0");
  if (deposit_ui->refill->text() == "") deposit_ui->refill->setText("0");
  if (deposit_ui->write_off->text() == "") deposit_ui->write_off->setText("0");
  if (deposit_ui->comboBox_periodization->currentIndex() == 0)
    percentPeriod = 12;
  if (deposit_ui->comboBox_periodization->currentIndex() == 1)
    percentPeriod = 4;
  if (deposit_ui->comboBox_periodization->currentIndex() == 2)
    percentPeriod = 1;
  if (deposit_ui->comboBox_periodization->currentIndex() == 3)
    percentPeriod = 0;
  return percentPeriod;
};

void DepositView::calculateDeposit(bool Simple) {
  QMessageBox msgBox;
  double Pr = initInput();
  try {
    std::map<std::string, std::vector<double>> res;
    if (Simple) {
      res = this->controller_->depositCalculatorAction(
          deposit_ui->deposit_summ->text(), deposit_ui->period->text(),
          deposit_ui->percent->text(), deposit_ui->percentBet->text(),
          deposit_ui->refill->text(), deposit_ui->write_off->text(), Pr, true);
    } else {
      res = this->controller_->depositCalculatorAction(
          deposit_ui->deposit_summ->text(), deposit_ui->period->text(),
          deposit_ui->percent->text(), deposit_ui->percentBet->text(),
          deposit_ui->refill->text(), deposit_ui->write_off->text(), Pr, false);
    }
    deposit_ui->label_profit->setText(
        QString::number((res.find("Profit")->second)[0], 'f',
                        double_size_deposit((res.find("Profit")->second)[0])));
    deposit_ui->label_tax_2->setText(QString::number(
        (res.find("TotalTax")->second)[0], 'f',
        double_size_deposit((res.find("TotalTax")->second)[0])));
    deposit_ui->label_total->setText(QString::number(
        (res.find("TotalPayment")->second)[0], 'f',
        double_size_deposit((res.find("TotalPayment")->second)[0])));
  } catch (std::exception &err) {
    msgBox.setText("Check input data");
    msgBox.exec();
  }
};

void DepositView::init_and_exec(s21::SmartCalculatorController *controller) {
  this->controller_ = controller;
  // this->setWindowTitle("Deposit Calculator");
  // this->setModal(true);
  this->exec();
};
};  // namespace s21
