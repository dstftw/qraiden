QT *= core testlib
QT -= gui
CONFIG *= console testcase app_bundle
TEMPLATE = app

TARGET = tst_qraiden
SOURCES += tst_qraiden.cpp

INCLUDEPATH += -I $$PWD/../../include/
LIBS += -L$$OUT_PWD/../../lib -lqraiden
macx {
    qosdnotification.path = Contents/MacOS
    qosdnotification.files = $$OUT_PWD/../../lib/libqraiden.1.dylib
    QMAKE_BUNDLE_DATA += qraiden
}
