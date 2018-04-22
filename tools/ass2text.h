#ifndef ASS2TEXT_H
#define ASS2TEXT_H

#include "tag.h"

#include <QWidget>
#include <QDebug>
#include <QClipboard>
#include <QIODevice>
#include <QFile>
#include <QByteArray>
#include <QTextStream>
#include <QDesktopServices>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

namespace Ui {
    class Ass2text;
}

class Ass2text : public QWidget
{
    Q_OBJECT

public:
    explicit Ass2text(QWidget *parent = 0);
    ~Ass2text();
    void setupUi(void);
    void setText(QString text);
    void ass2Text(QString text);
    QString stripTag(QString text);
    QString getPathExt(QString path);
    void reloadInput(QString path);
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;

private slots:
    void on_hideBt_clicked();
    void on_copyBt_clicked();

private:
    Ui::Ass2text *ui;
    QClipboard *m_clipboard;
};
#endif // ASS2TEXT_H
