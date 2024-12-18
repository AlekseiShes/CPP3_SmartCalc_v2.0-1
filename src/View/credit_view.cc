#include "credit_view.h"

#include "ui_credit.h"

namespace s21 {

CreditView::CreditView(QWidget *parent)
    : QDialog(parent), credit_ui(new Ui::CreditView) {
  credit_ui->setupUi(this);
  credit_ui->MonthPayments->setColumnCount(1);
  credit_ui->MonthPayments->setHorizontalHeaderLabels(QStringList()
                                                      << "Платёж");
  credit_ui->MonthPayments->horizontalHeader()->setSectionResizeMode(
      QHeaderView::ResizeToContents);
  credit_ui->MonthPayments->setEditTriggers(QAbstractItemView::NoEditTriggers);

  connect(credit_ui->pushButton_closecredit, SIGNAL(clicked()), this,
          SLOT(on_pushButton_closecredit_clicked()));
};

CreditView::~CreditView() { delete credit_ui; };

void CreditView::on_pushButton_closecredit_clicked() { QWidget::close(); };

void CreditView::on_pushButton_calcCredit_clicked() {
  if (credit_ui->radioButton_annuit->isChecked()) {
    this->calculateCredit(true);
  } else if (credit_ui->radioButton_diff->isChecked()) {
    this->calculateCredit(false);
  } else {
    QMessageBox msgBox;
    msgBox.setText("Attention! Select calculation type.");
    msgBox.exec();
  }
};

int double_size_credit(double res) {
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

void CreditView::calculateCredit(bool Annuity) {
  QMessageBox msgBox;
  credit_ui->MonthPayments->clearContents();
  try {
    std::map<std::string, std::vector<double>> res;
    if (Annuity) {
      res = this->controller_->creditCalculatorAction(
          credit_ui->credit_sum->text(), credit_ui->period->text(),
          credit_ui->percent->text(), true);
    } else {
      res = this->controller_->creditCalculatorAction(
          credit_ui->credit_sum->text(), credit_ui->period->text(),
          credit_ui->percent->text(), false);
    }
    auto tmp = (res.find("MonthPayments"))->second;
    credit_ui->MonthPayments->setRowCount(tmp.size());
    int i = 0;
    for (auto it = tmp.begin(); it != tmp.end(); it++) {
      QTableWidgetItem *tmp = new QTableWidgetItem(
          QString::number(*it, 'f', double_size_credit(*it)));
      credit_ui->MonthPayments->setItem(i, 0, tmp);
      i++;
    }
    credit_ui->label_overpayment->setText(QString::number(
        (res.find("CreditOverpayment")->second)[0], 'f',
        double_size_credit((res.find("CreditOverpayment")->second)[0])));
    credit_ui->label_total_2->setText(QString::number(
        (res.find("TotalPayment")->second)[0], 'f',
        double_size_credit((res.find("TotalPayment")->second)[0])));
  } catch (std::exception &err) {
    msgBox.setText("Check input data");
    msgBox.exec();
  }
};

void CreditView::init_and_exec(s21::SmartCalculatorController *controller) {
  this->controller_ = controller;
  // this->setWindowTitle("Credit Calculator");
  // this->setModal(true);
  this->exec();
};
};  // namespace s21
