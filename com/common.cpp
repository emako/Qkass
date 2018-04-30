#include "common.h"

Common::Common()
{
    QT_PASS;
}

QString Common::loadTransConfig(void)
{
    QSettings *config = new QSettings(CONFIG_FILE, QSettings::IniFormat);
    config->beginGroup(CONFIG_PREFIX_COMMON);
    QString language = loadTransFile((QLocale::Language)config->value(CONFIG_KEY_LANGUAGE).toInt());
    delete config;
    return language;
}

QString Common::loadTrans(void)
{
    QLocale local;
    return loadTransFile(local.language());
}

QString Common::loadTransFile(QLocale::Language language)
{
    QString trans;
    switch(language)
    {
    case QLocale::China:
    case QLocale::Chinese:
        trans = "trans/chinese.qm";
        break;
    case QLocale::Japan:
    case QLocale::Japanese:
        trans = "trans/japanese.qm";
        break;
    case QLocale::English:
    default:
        trans = "trans/default.qm";
        break;
    }
    return trans;
}

QString Common::getPathExt(QString path)
{
    QStringList exts = path.split(QT_EXT_SPLITE);
    QString ext = QString(QT_EMPTY);
    if(exts.length() != QT_INDEX_0)
    {
        ext = exts.at(exts.length() - QT_INDEX_1);
    }
    if(!ext.isEmpty())
    {
        /* ピリオド付き */
        ext = QT_EXT_SPLITE + ext;
    }
    return ext;
}

QString Common::getPathExtOnly(QString path)
{
    QStringList exts = path.split(QT_EXT_SPLITE);
    QString ext = QString(QT_EMPTY);
    if(exts.length() != QT_INDEX_0)
    {
        ext = exts.at(exts.length() - QT_INDEX_1);
    }
    return ext;
}

QString Common::getPathFileName(QString path)
{
    QStringList exts = path.split(QT_EXT_SPLITE);
    QStringList filenames;
    QString filename;
    /*拡張子を切り捨てる*/
    for(int i = 0; i < exts.length() - QT_INDEX_1; i++)
    {
        filenames.append(exts.at(i));
    }
    filename = filenames.join(QT_EXT_SPLITE);
    /*拡張子なし*/
    if(filename.isEmpty() || exts.length() == QT_INDEX_0)
    {
        filename = path;
    }
    return filename;
}

QString Common::getPathOutput(QString path, QString name)
{
    QString ext = getPathExt(path);
    int index_ext = path.length() - ext.length();
    QString file_name = path.left(index_ext);
    file_name += name;
    return file_name + ext;
}

QString Common::toNorEol(QString text)
{
    /* CRLF -> LF */
    text = text.replace(QT_OTR_EOL, QT_NOR_EOL);
    /* CR   -> LF */
    text = text.replace(QT_MAC_EOL, QT_NOR_EOL);
    return text;
}
