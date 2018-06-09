#include <QApplication>
#include <QFile>
#include "Demos/SignalsAndSlot.h"
#include "Demos/ThreadSigAndSlots.h"
#include "Demos/BezierCurve.h"
#include "Demos/GifCreater.h"
#include "Demos/GifBuilderWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile nFile("./styles/DarkStyle.css");
    if (nFile.open(QFile::ReadOnly))
    {
        QString styleSheetString = nFile.readAll().data();
        a.setStyleSheet(styleSheetString);
    }

//    SignalsAndSlotDemo w;
//    w.resize(300, 150);
//    w.show();

//    ThreadSigAndSlotsWidget threadW;
//    threadW.resize(300, 150);
//    threadW.show();

    GifBuilderWidget w;
    w.resize(1000, 800);
    w.show();

    return a.exec();
}
