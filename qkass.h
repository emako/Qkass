#ifndef QKASS_H
#define QKASS_H

#include "tag.h"
#include "highlighter.h"
#include "tools/asstt.h"
#include "tools/ass2text.h"

#include <QMainWindow>
#include <QDebug>
#include <QTime>
#include <QRegExp>
#include <QFontDatabase>
#include <QClipboard>
#include <QMessageBox>
#include <QFileDialog>
#include <QIODevice>
#include <QFile>
#include <QByteArray>
#include <QTextStream>
#include <QResource>
#include <QSize>
#include <QDesktopServices>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QCloseEvent>
#include <QMimeData>
#include <QUrl>
#include <QScrollBar>

static const char QKASS_GITHUB_URL[] = "https://github.com/emako/Qkass";
static const char QKASS_WEIBO_URL[]  = "https://weibo.com/egosub";
static const float QKASS_OPACITY     = 0.99;

namespace Ui {
class Qkass;
}

class Qkass : public QMainWindow
{
    Q_OBJECT

public:
    explicit Qkass(QWidget *parent = 0);
    ~Qkass();
    void initParm(void);
    void setHighlight(void);
    void setupUi(void);
    void setupUiFont(void);
    void setupToolBar(void);
    void reloadInput(QString path);
    void setEditorHighlightSynth(Highlighter *highlighter);
    QString getSavePath(QString path);
    QString getPathExt(QString path);
    QString callKass(QString rv_text);
    float calcTextLength(QString text);
    QString calcKassValue(QString start, QString end, float length);
    QString stripTag(QString text);
    QString getAssText(QStringList events);
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void openDiskFile(void);
    void saveDiskFile(void);
    void saveAsDiskFile(void);
    void pasteText(void);
    void showLisenceInfo(void);
    void slotGithub(void);
    void slotWeibo(void);
    void slotTextEditSliderVertical(int value);
    void slotTextEditSliderHorizontal(int value);
    void slotHighlight(void);
    void slotOpenAssTt(void);
    void slotOpenAss2text(void);

    void slotZoomIn(bool checked = false);
    void slotZoomOut(bool checked = false);
    void slotZoomFit(void);

    void on_startBt_clicked();
    void on_exitBt_clicked();
    void on_copyBt_clicked();
    void on_clearBt_clicked();
    void on_karaokeComboBox_activated(int index);
    void on_otherTagCheckBox_stateChanged(int arg1);


private:
    Ui::Qkass *ui;
    AssTt *m_asstt;
    Ass2text *m_ass2text;
    QClipboard *m_clipboard;
    Highlighter *m_input_highlighter;
    Highlighter *m_output_highlighter;
    QString input_path;
    int zoom_in_count;
    int zoom_out_count;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    bool isTimeLimted;
    int kCount;
};


#endif // QKASS_H
