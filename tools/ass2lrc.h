#ifndef ASS2LRC_H
#define ASS2LRC_H

#include "com/tag.h"
#include "com/common.h"
#include "ass2text.h"

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QPixmap>
#include <QFont>
#include <QProcess>
#include <QTextStream>
#include <QIODevice>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTime>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QMimeData>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDebug>

namespace Ui {
    class Ass2lrc;
}

class Ass2lrc : public QWidget
{
    Q_OBJECT

public:
    explicit Ass2lrc(QWidget *parent = 0);
    ~Ass2lrc();
    void changeDirFormatToDos(QString & path);
    void removeEffectText(QString &text);
    QString ass2lrc(QString text);
    QString createLrcTag();
    QString stripTag(QString text);

    void setupUi(void);
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

private slots:
    void on_creatBt_clicked();
    void on_inputBt_clicked();
    void on_hideBt_clicked();

private:
    Ui::Ass2lrc *ui;
    QStringList inputs;
};

#endif // ASS2LRC_H
