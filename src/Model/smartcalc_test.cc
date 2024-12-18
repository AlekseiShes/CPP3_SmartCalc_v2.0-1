#include <gtest/gtest.h>

#include <iostream>
#include <iterator>
#include <map>

#include "credit_model.h"
#include "deposit_model.h"
#include "smartcalc_model.h"

using namespace s21;
class CalcTest : public ::testing::Test {
 protected:
  void SetUp() override {}
  s21::SmartCalculatorModel model = s21::SmartCalculatorModel();
  s21::CreditCalculations credit = s21::CreditCalculations();
  s21::DepositCalculations deposit = s21::DepositCalculations();

  s21::SmartCalculatorModel::Calculations calc =
      s21::SmartCalculatorModel::Calculations();

  s21::CreditCalculations::Annuity calc_credit_annuity =
      s21::CreditCalculations::Annuity();
  s21::CreditCalculations::Differential calc_credit_diff =
      s21::CreditCalculations::Differential();
  std::string credit_sum;
  std::string period;
  std::string percent;

  s21::DepositCalculations::SimplePercent simple_deposit_calc =
      s21::DepositCalculations::SimplePercent();
  s21::DepositCalculations::CapitalPercent capital_deposit_calc =
      s21::DepositCalculations::CapitalPercent();
  std::string nalog;
  std::string refill_capacity;
  std::string write_off_capacity;
  double percents_period;
};

TEST_F(CalcTest, CalculationTest_1) {
  EXPECT_ANY_THROW(calc.calculating("123+321("));
}

TEST_F(CalcTest, CalculationTest_2) {
  EXPECT_ANY_THROW(calc.calculating(
      "acos(1)+sin(2)-tan(3)/cos(4)*asin(5)+atan(6)-3^2*ln(7)+log(8)/"
      "(-1)*(12331)("
      "3mod2("));
}

TEST_F(CalcTest, CalculationTest_3) {
  EXPECT_NO_THROW(calc.calculating("cos(1)+sin(2)"));
}

TEST_F(CalcTest, CalculationTest_4) { EXPECT_ANY_THROW(calc.calculating("")); }

TEST_F(CalcTest, CalculationTest_5) {
  EXPECT_ANY_THROW(
      calc.calculating("(1+2+3+4+5)+((1+1))()+(12.23.3+123.3.4.5.6)"));
}

TEST_F(CalcTest, CalculationTest_6) {
  EXPECT_NO_THROW(calc.calculating("+123"));
}

TEST_F(CalcTest, CalculationTest_7) {
  EXPECT_ANY_THROW(calc.calculating("caas(1)"));
}

TEST_F(CalcTest, CalculationTest_8) {
  EXPECT_NO_THROW(calc.calculating("2E-2"));
}

TEST_F(CalcTest, CalculationTest_9) {
  EXPECT_NO_THROW(calc.calculating("2E+13"));
};

TEST_F(CalcTest, CalculationTest_9_0) {
  EXPECT_ANY_THROW(calc.calculating("2E+"));
};

TEST_F(CalcTest, CalculationTest_10) {
  EXPECT_ANY_THROW(calc.calculating("E--3"));
}

TEST_F(CalcTest, CalculationTest_11) {
  EXPECT_ANY_THROW(calc.calculating("123+321()"));
}

TEST_F(CalcTest, CalculationTest_12) {
  EXPECT_NO_THROW(calc.calculating(
      "(123+321*1-2-acos(0.2)+atan(3))*2-sqrt(4)+log(10)-asin(0.3)"));
}

TEST_F(CalcTest, CalculationTest_13) {
  double number = calc.calculating("-0.2");
  EXPECT_EQ(number, -0.2);
}

TEST_F(CalcTest, CalculationTest_14) {
  double number = calc.calculating("5mod2");
  EXPECT_EQ(number, 1);
}

TEST_F(CalcTest, CalculationTest_15) {
  double number = calc.calculating("2+3*3-1");
  double test = 2 + 3 * 3 - 1;
  // std::cout << "number " << number << std::endl;
  EXPECT_EQ(number, test);
}

TEST_F(CalcTest, CalculationTest_16) {
  double number = calc.calculating("lg(-32;-0.5)");
  double test = -0.2;
  // std::cout << "number " << number << std::endl;
  EXPECT_EQ(fabs(number - test) < PRECISION, true);
}

TEST_F(CalcTest, CalculationTest_16_0) {
  double number = calc.calculating("sin(-lg(-32;-0.5)*PI)");
  double test = sin(0.2 * M_PI);
  // std::cout << "number " << number << std::endl;
  EXPECT_EQ(fabs(number - test) < PRECISION, true);
}

TEST_F(CalcTest, CalculationTest_16_1) {
  double number = calc.calculating("sin(-lg(-1*3!!/22.5;-0.5)*PI)");
  double test = sin(0.2 * M_PI);
  // std::cout << "number " << number << std::endl;
  EXPECT_EQ(fabs(number - test) < PRECISION, true);
}

TEST_F(CalcTest, CalculationTest_16_1_0) {
  double number = calc.calculating("sin(-lg(-(3!!)/22.5;-0.5)*PI)");
  double test = sin(0.2 * M_PI);
  // std::cout << "number " << number << std::endl;
  EXPECT_EQ(fabs(number - test) < PRECISION, true);
}

TEST_F(CalcTest, CalculationTest_16_2) {
  double number = calc.calculating("avrg(1; 2; 3; 4; 5)");
  double test = 3;
  // std::cout << "number " << number << std::endl;
  EXPECT_EQ(fabs(number - test) < PRECISION, true);
}

TEST_F(CalcTest, CalculationTest_16_2_2) {
  double number = calc.calculating("avrg(1)");
  double test = 1;
  // std::cout << "number " << number << std::endl;
  EXPECT_EQ(fabs(number - test) < PRECISION, true);
}

TEST_F(CalcTest, CalculationTest_16_3) {
  double number = calc.calculating("--1");
  double test = 1;
  // std::cout << "number " << number << std::endl;
  EXPECT_EQ(fabs(number - test) < PRECISION, true);
}

TEST_F(CalcTest, CalculationTest_16_4) {
  EXPECT_ANY_THROW(calc.calculating("--"));
}

TEST_F(CalcTest, CalculationTest_16_5) {
  double number = calc.calculating("avrg(--1; -+-+2; ----3; 4+0*--2; 5)");
  double test = 3;
  // std::cout << "number " << number << std::endl;
  EXPECT_EQ(fabs(number - test) < PRECISION, true);
}

TEST_F(CalcTest, CalculationTest_16_7) {
  double number = calc.calculating("2.1+sin(3)-tan(2)*1^3");
  double test = 2.1 + sin(3) - tan(2) * pow(1, 3);
  // std::cout << "number " << number << std::endl;
  EXPECT_EQ(number, test);
}

TEST_F(CalcTest, CalculationTest_16_8) {
  double number = calc.calculating("2.1+sin(3)-tan(2)*1^3");
  double test = 2.1 + sin(3) - tan(2) * pow(1, 3);
  // std::cout << "number " << number << std::endl;
  EXPECT_EQ(number, test);
}

TEST_F(CalcTest, CalculationTest_16_9) {
  double number = calc.calculating("cos(PI)*ln(EXP)");
  double test = cos(M_PI) * log(M_E);
  ASSERT_LE(fabs(fabs(number) - fabs(test)), 1E-7);
}

TEST_F(CalcTest, CalculationTest_17) {
  double number = calc.calculating("cossin0");
  double test = cos(sin(0));
  ASSERT_LE(fabs(fabs(number) - fabs(test)), 1E-7);
}

TEST_F(CalcTest, CalculationTest_18) {
  // double number = calc.calculating("cos0;sin(0)");
  // double test = cos(sin(0));
  EXPECT_ANY_THROW(calc.calculating("cos0;sin(0)"));
  // ASSERT_LE(fabs(fabs(number) - fabs(test)), 1E-7);
}

TEST_F(CalcTest, CalculationTest_18_1) {
  double number = calc.calculating("-  PI");
  // std::cout << "number " << number << std::endl;
  ASSERT_LE(fabs(fabs(number - (-M_PI))), 1E-7);
}

TEST_F(CalcTest, CalculationTest_18_2) {
  double number = calc.calculating("sin(3!)");
  // std::cout << "number " << number << std::endl;
  ASSERT_LE(fabs(fabs(number - (sin(6)))), 1E-7);
}

TEST_F(CalcTest, CalculationTest_19) {
  EXPECT_ANY_THROW(calc.calculating("2Ea"));
}

TEST_F(CalcTest, CalculationTest_20) {
  EXPECT_ANY_THROW(calc.calculating("1..2"));
}

TEST_F(CalcTest, CalculationTest_21) {
  EXPECT_NO_THROW(calc.calculating("-2+sin2"));
  ASSERT_LE(fabs(calc.calculating("-2+sin2") - (sin(2) - 2)), 1E-7);
}

TEST_F(CalcTest, CalculationTest_22) {
  EXPECT_ANY_THROW(calc.calculating("1+"));
}

TEST_F(CalcTest, CalculationTest_23) {
  EXPECT_ANY_THROW(calc.calculating("+"));
}

TEST_F(CalcTest, CalculationTest_24) {
  EXPECT_ANY_THROW(calc.calculating("`"));
}

TEST_F(CalcTest, CalculationTest_25) {
  EXPECT_ANY_THROW(calc.calculating("lg(32; 3; 4)"));
}

TEST_F(CalcTest, CalculationTest_26) {
  EXPECT_ANY_THROW(calc.calculating("lg(32)"));
}

TEST_F(CalcTest, CalculationTest_27) {
  EXPECT_ANY_THROW(calc.calculating("sin ( 32 ; 4)"));
}

TEST_F(CalcTest, CalculationTest_28) {
  EXPECT_ANY_THROW(calc.calculating("asin(2)"));
}

TEST_F(CalcTest, CalculationTest_29) {
  EXPECT_ANY_THROW(calc.calculating("lg32;0.5"));
}

TEST_F(CalcTest, CalculationTest_30) {
  EXPECT_ANY_THROW(calc.calculating("*1"));
  EXPECT_ANY_THROW(calc.calculating("mod2"));
  EXPECT_ANY_THROW(calc.calculating("2/"));
  EXPECT_ANY_THROW(calc.calculating("2-"));
}

TEST_F(CalcTest, CalculationTest_31) {
  EXPECT_ANY_THROW(calc.calculating("invers(3)"));
}

TEST_F(CalcTest, CalculationTest_32) {
  EXPECT_ANY_THROW(calc.calculating("sin(cos(0)"));
  EXPECT_ANY_THROW(calc.calculating("sincos(0))"));
}

TEST_F(CalcTest, CalculationTest_33) {
  EXPECT_ANY_THROW(calc.calculating("-2^0.5"));
  EXPECT_ANY_THROW(calc.calculating("fact(0)"));
}
TEST_F(CalcTest, CalculationTest_33_1) {
  ASSERT_LE(fabs(calc.calculating("(8)^(-1/3)") - (0.5)), 1E-7);
}

TEST_F(CalcTest, CalculationTest_33_2) {
  ASSERT_LE(fabs(calc.calculating("-8^(1/3)") - (-2)), 1E-7);
}

TEST_F(CalcTest, CalculationTest_34) {
  ASSERT_LE(fabs(calc.calculating("lg(-2;4)") - (2)), 1E-7);
}

TEST_F(CalcTest, CreditAnnuityTest_1) {
  credit_sum = "1.22.3";
  period = "12";
  percent = "";

  EXPECT_ANY_THROW(calc_credit_annuity.calculate(credit_sum, period, percent));
}

TEST_F(CalcTest, CreditAnnuityTest_2) {
  credit_sum = "12";
  period = "12";
  percent = "12";
  EXPECT_NO_THROW(calc_credit_annuity.calculate(credit_sum, period, percent));
}

TEST_F(CalcTest, CreditAnnuityTest_3) {
  credit_sum = "1000000";
  period = "1.2";
  percent = "12";
  EXPECT_NO_THROW(calc_credit_annuity.calculate(credit_sum, period, percent));
}

TEST_F(CalcTest, CreditAnnuityTest_5) {
  credit_sum = "123-12312";
  period = "1.2";
  percent = "12";
  EXPECT_ANY_THROW(calc_credit_annuity.calculate(credit_sum, period, percent));
}

TEST_F(CalcTest, CreditAnnuityTest_6) {
  credit_sum = "100";
  period = "2";
  percent = "10";
  EXPECT_NO_THROW(calc_credit_annuity.calculate(credit_sum, period, percent));
}

TEST_F(CalcTest, CreditDiffTest_1) {
  credit_sum = "1.22.3";
  period = "12";
  percent = "20";
  EXPECT_ANY_THROW(calc_credit_diff.calculate(credit_sum, period, percent));
}

TEST_F(CalcTest, CreditDiffTest_2) {
  credit_sum = "12";
  period = "12";
  percent = "0";
  EXPECT_ANY_THROW(calc_credit_diff.calculate(credit_sum, period, percent));
}

TEST_F(CalcTest, CreditDiffTest_3) {
  credit_sum = "";
  period = "1.2";
  percent = "12";
  EXPECT_ANY_THROW(calc_credit_diff.calculate(credit_sum, period, percent));
}

TEST_F(CalcTest, CreditDiffTest_5) {
  credit_sum = "123a12312";
  period = "1.2";
  percent = "12";
  EXPECT_ANY_THROW(calc_credit_diff.calculate(credit_sum, period, percent));
}

TEST_F(CalcTest, CreditDiffTest_6) {
  credit_sum = "100000";
  period = "12";
  percent = "20";
  EXPECT_NO_THROW(calc_credit_diff.calculate(credit_sum, period, percent));
}

TEST_F(CalcTest, DepositTest_1) {
  credit_sum = "1000000";
  period = "12";
  percent = "10";
  nalog = "13";
  refill_capacity = "0";
  write_off_capacity = "0";
  percents_period = 12;
  EXPECT_NO_THROW(simple_deposit_calc.calculate(
      credit_sum, period, percent, nalog, refill_capacity, write_off_capacity,
      percents_period));
};

TEST_F(CalcTest, DepositTest_2) {
  credit_sum = "1000000";
  period = "12";
  percent = "12";
  nalog = "-13";
  refill_capacity = "10000";
  write_off_capacity = "5000";
  percents_period = 12;
  EXPECT_ANY_THROW(simple_deposit_calc.calculate(
      credit_sum, period, percent, nalog, refill_capacity, write_off_capacity,
      percents_period));
};

TEST_F(CalcTest, DepositTest_3) {
  credit_sum = "1000000";
  period = "2";
  percent = "7.8";
  nalog = "13";
  refill_capacity = "10000";
  write_off_capacity = "5000";
  percents_period = 4;
  EXPECT_ANY_THROW(capital_deposit_calc.calculate(
      credit_sum, period, percent, nalog, refill_capacity, write_off_capacity,
      percents_period));
};

TEST_F(CalcTest, DepositTest_4) {
  credit_sum = "1000000";
  period = "12";
  percent = "7.8";
  nalog = "0";
  refill_capacity = "10000";
  write_off_capacity = "5000";
  percents_period = 4;
  EXPECT_NO_THROW(capital_deposit_calc.calculate(
      credit_sum, period, percent, nalog, refill_capacity, write_off_capacity,
      percents_period));
  percents_period = 12;
  EXPECT_NO_THROW(capital_deposit_calc.calculate(
      credit_sum, period, percent, nalog, refill_capacity, write_off_capacity,
      percents_period));
  percents_period = 1;
  EXPECT_NO_THROW(capital_deposit_calc.calculate(
      credit_sum, period, percent, nalog, refill_capacity, write_off_capacity,
      percents_period));
  percents_period = 0;
  EXPECT_NO_THROW(capital_deposit_calc.calculate(
      credit_sum, period, percent, nalog, refill_capacity, write_off_capacity,
      percents_period));
};

TEST_F(CalcTest, DepositTest_5) {
  credit_sum = "1000000";
  period = "0";
  percent = "7.8";
  nalog = "10";
  refill_capacity = "10000";
  write_off_capacity = "5000";
  percents_period = 4;
  EXPECT_ANY_THROW(capital_deposit_calc.calculate(
      credit_sum, period, percent, nalog, refill_capacity, write_off_capacity,
      percents_period));
};

TEST_F(CalcTest, DepositTest_6) {
  credit_sum = "1000000";
  period = "2";
  percent = "25";
  nalog = "10";
  refill_capacity = "10000";
  write_off_capacity = "5000";
  percents_period = 4;
  EXPECT_ANY_THROW(capital_deposit_calc.calculate(
      credit_sum, period, percent, nalog, refill_capacity, write_off_capacity,
      percents_period));
};

TEST_F(CalcTest, DepositTest_7) {
  credit_sum = "1000000";
  period = "2";
  percent = "25";
  nalog = "10";
  refill_capacity = "10000";
  write_off_capacity = "5000";
  percents_period = 1;
  EXPECT_ANY_THROW(capital_deposit_calc.calculate(
      credit_sum, period, percent, nalog, refill_capacity, write_off_capacity,
      percents_period));
};

TEST_F(CalcTest, DepositTest_8) {
  credit_sum = "1000000";
  period = "20";
  percent = "25";
  nalog = "10";
  refill_capacity = "5000";
  write_off_capacity = "100000";
  percents_period = 1;
  EXPECT_ANY_THROW(capital_deposit_calc.calculate(
      credit_sum, period, percent, nalog, refill_capacity, write_off_capacity,
      percents_period));
};

TEST_F(CalcTest, DepositTest_9) {
  credit_sum = "1000000";
  period = "12";
  percent = "10";
  nalog = "13";
  refill_capacity = "10000";
  write_off_capacity = "5000";
  percents_period = 12;
  EXPECT_NO_THROW(simple_deposit_calc.calculate(
      credit_sum, period, percent, nalog, refill_capacity, write_off_capacity,
      percents_period));
  percents_period = 4;
  EXPECT_NO_THROW(simple_deposit_calc.calculate(
      credit_sum, period, percent, nalog, refill_capacity, write_off_capacity,
      percents_period));
  percents_period = 0;
  EXPECT_NO_THROW(simple_deposit_calc.calculate(
      credit_sum, period, percent, nalog, refill_capacity, write_off_capacity,
      percents_period));
  percents_period = 1;
  EXPECT_NO_THROW(simple_deposit_calc.calculate(
      credit_sum, period, percent, nalog, refill_capacity, write_off_capacity,
      percents_period));
};

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}