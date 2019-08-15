TEMPLATE = lib
TARGET = QCustomPlot

include(../../common.pri)

QT += widgets printsupport


HEADERS = qcustomplot.h


SOURCES =qcustomplot.cpp


#指定生成的lib保存目录
DESTDIR=$$PROJECT_LIBRARYDIR

