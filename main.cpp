#include "global_settings.h"

#include <QCoreApplication>
#include <QDebug>

//输出当前设置
void PrintCurrentSettings() {
    qDebug() << "current setting1: " << GlobalSettings::Instance()->GetSetting1();
    qDebug() << "current setting2: " << GlobalSettings::Instance()->GetSetting2();
}

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);
    PrintCurrentSettings();
    //改变设置
    GlobalSettings::Instance()->SetSetting1(11);
    GlobalSettings::Instance()->SetSetting2("22");
    qDebug() << endl << "Change settings" << endl;
    PrintCurrentSettings();
    return a.exec();
}
