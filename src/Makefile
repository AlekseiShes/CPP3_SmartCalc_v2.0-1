GCC = g++ -std=c++17 -g -Wall -Werror -Wextra
TEST = Model/*_test.cc
MODEL = Model/*_model.cc Model/*_model_func.cc
GCOV =--coverage
LIBA = *.a
LIBO = *.o
OS = $(shell uname)

FILECASE= Model/*.h \
	Model/*.cc \
	Controller/*.h \
	Controller/*.cc \
	View/*.h \
	View/*.cc \
	View/*.cpp 	

ifeq ($(OS), Darwin)
LIBFLAGS = -lm -lgtest -lstdc++ -lgtest_main
else
	LIBFLAGS=-lstdc++ `pkg-config --cflags --libs gtest  --libs gtest_main`
endif

all: clean tests

tests: clean
	$(GCC) $(TEST) $(MODEL) $(LIBFLAGS)  -o test
	./test

clean:
	rm -rf *.o *.a  *.out test *.dSYM  report coverage.info *.gcda *.gcno *.txt *.tgz build Archive_SmartCalc_v2.0

dvi:
	open ./readme.html

dist:
	rm -rf Archive_SmartCalc_v2.0/
	mkdir Archive_SmartCalc_v2.0/
	mkdir Archive_SmartCalc_v2.0/src
	cp -rf Makefile Controller Model View main.cc Makefile readme.html SmartCalc.pro Archive_SmartCalc_v2.0/src/
	tar cvzf Archive_SmartCalc_v2.0.tgz Archive_SmartCalc_v2.0/
	rm -rf Archive_SmartCalc_v2.0/

s21_calculator.a: clean
	$(GCC) -c $(TEST)
	ar rcs $(LIBA) $(LIBO)
	ranlib $(LIBA)

gcov_report: s21_calculator.a
	$(GCC) $(GCOV) $(MODEL) $(LIBA) -L. $(LIBA)  $(LIBFLAGS) -o test
	./test
	lcov -t "test" -c -d  ./ --no-external --output-file ./coverage.info
	genhtml ./coverage.info --output-directory ./report/
	open ./report/index.html

install:
	make clean
	mkdir build
	cd build && qmake ../SmartCalc.pro && make --no-print-directory
# rm -rf build/*.o build/*.cpp build/*.h build/Makefile

uninstall: 
	rm -rf build*

check:
	cp ../materials/linters/.clang-format ./
	clang-format -style=Google -n $(FILECASE)
	clang-format -style=Google -i $(FILECASE)
	rm .clang-format
	make tests

ifeq ($(OS), Darwin)
	leaks --atExit -- test
else
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./test
endif

open_app:
ifeq ($(OS), Darwin)
	./build/Calculator.app/Contents/MacOS/Calculator
else
	./build/SmartCalc
endif
