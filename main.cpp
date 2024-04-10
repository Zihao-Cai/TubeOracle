#include "TubeOracle.h"
#include <QtWidgets/QApplication>

//项目入口函数
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*主类w构造完之前打开启动动画
    

    */
    TubeOracle w;
    /*主类w构造过程中启动动画作为启动标志，w构造完成后结束启动动画，然后打开主控界面w
    

    */
    w.show();
   
    return a.exec();
}
