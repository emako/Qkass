#ifndef COMMON_H
#define COMMON_H

#include "tag.h"
#include "config.h"

#include <QString>
#include <QStringList>
#include <QLocale>
#include <QTranslator>
#include <QSettings>

class Common
{
public:
    Common();
    static QString loadTransConfig(void);
    static QString loadTrans(void);
    static QString loadTransFile(QLocale::Language language);
    static QString getPathExt(QString path);
    static QString getPathExtOnly(QString path);
    static QString getPathFileName(QString path);
    static QString getPathOutput(QString path, QString name);
    static QString toNorEol(QString text);

};


#endif // COMMON_H
