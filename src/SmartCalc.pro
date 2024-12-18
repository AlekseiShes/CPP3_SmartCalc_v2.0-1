QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Controller/smartcalc_controller.cc \
    View/smartcalc_view.cc \
    View/credit_view.cc \
    View/deposit_view.cc \
    View/qcustomplot.cpp \
    Model/smartcalc_model.cc \
    Model/smartcalc_model_func.cc \
    Model/credit_model.cc \
    Model/deposit_model.cc \
    main.cc

HEADERS += \
    Controller/smartcalc_controller.h \
    View/smartcalc_view.h \
    View/credit_view.h \
    View/deposit_view.h \
    View/qcustomplot.h \
    Model/smartcalc_model.h \
    Model/credit_model.h \
    Model/deposit_model.h 

FORMS += \
    View/credit.ui \
    View/deposit.ui \
    View/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
