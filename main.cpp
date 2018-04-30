#include "qkass.h"
#include "com/common.h"
#include "com/tag.h"

#include <QApplication>
#include <QTranslator>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator qtTranslator;
    qtTranslator.load(Common::loadTrans());
    a.installTranslator(&qtTranslator);
    int ret = QT_RETCODE_DEFAULT;
    while(true)
    {
        if(QFile::exists(CONFIG_FILE))
        {
            qtTranslator.load(Common::loadTransConfig());
            a.installTranslator(&qtTranslator);
        }
        Qkass w;
        w.show();
        ret = a.exec();
        if(ret != QT_RETCODE_RESTART)
        {
            break;
        }
    }
    return ret;
}
