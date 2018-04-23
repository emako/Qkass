#include "ass2text.h"
#include "ui_ass2text.h"

Ass2text::Ass2text(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ass2text)
{
    ui->setupUi(this);
    this->setupUi();

}

Ass2text::~Ass2text()
{
    delete ui;
}

void Ass2text::setupUi(void)
{
    ui->textEdit->setWordWrapMode(QTextOption::WordWrap);
    this->setWindowTitle(QString(APP_TOOLS_ASS2TEXT));
    this->setAcceptDrops(true);
    this->setWindowIcon(QIcon(":res/text.ico"));
    m_clipboard = QApplication::clipboard();
}

void Ass2text::on_hideBt_clicked()
{
    this->hide();
}

void Ass2text::on_copyBt_clicked()
{
    QString text = ui->textEdit->toPlainText();
    if(!text.isEmpty())
    {
        m_clipboard->setText(text);
    }
}

void Ass2text::setText(QString text)
{
    ui->textEdit->setText(text);
}

QString Ass2text::stripTag(QString text)
{
    text = text.replace(QRegExp(ASS_TAG_BRACE_REGEXP), QT_EMPTY);
    text = text.replace(ASS_TAG_HARD_EOL, QT_NOR_EOL);
    text = text.replace(ASS_TAG_SOFT_EOL, QT_NOR_EOL);
    text = text.replace(ASS_TAG_BLANK, QT_BLANK);
    return text;
}

void Ass2text::ass2Text(QString text)
{
    if(text.isEmpty())
    {
        return;
    }

    QStringList ass_text_out_list;
    QStringList ass_text_in_list = text.split(QT_NOR_EOL);
    for(QString ass_text : ass_text_in_list)
    {
        if(ass_text.startsWith(ASS_TAG_EVENT_FORMAT_DIALOGUE) || ass_text.startsWith(ASS_TAG_EVENT_FORMAT_COMMENT))
        {
            QStringList events = ass_text.split(ASS_TAG_COMMA);
            if(events.length() < (int)AssEvents::MaxEvent)
            {
                /*メモリー溢れ回避*/
                return;
            }

            ass_text = stripTag(ass_text);
            ass_text = getAssText(events);
            ass_text_out_list.append(ass_text);
        }
    }
    ui->textEdit->setText(ass_text_out_list.join(QT_NOR_EOL));
}


void Ass2text::dragEnterEvent(QDragEnterEvent* event)
{
    if(event->mimeData()->hasText())
    {
        event->acceptProposedAction();
    }
}


QString Ass2text::getPathExt(QString path)
{
    QStringList exts = path.split(QT_EXT_SPLITE);
    QString ext = QString(QT_EMPTY);
    if(exts.length() != QT_INDEX_0)
    {
        ext = exts.at(exts.length() - QT_INDEX_1);
    }
    if(!ext.isEmpty())
    {
        /* 点が付いてる対応方式 */
        ext = QT_EXT_SPLITE + ext;
    }
    return ext;
}

void Ass2text::reloadInput(QString path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"Can't open the file!"<<endl;
    }
    QStringList lines;
    QTextStream in(&file);
    while(!in.atEnd())
    {
        QString line = in.readLine();
        lines.append(line);
    }
    ass2Text(lines.join(QT_NOR_EOL));
}

void Ass2text::dropEvent(QDropEvent* event)
{
    const QMimeData* mime = event->mimeData();
    if(mime->hasUrls())
    {
        QString droppath;
        for(QUrl url : event->mimeData()->urls())
        {
            droppath = url.toString();
            droppath = droppath.mid(QString(QT_FILE_PROTOCOL).length());
            qDebug()<<droppath;
            QString ext = getPathExt(droppath).toLower();
            if(ext == ASS_EXT_TYPE_A2 || ext == ASS_EXT_TYPE_B2 || ext == ASS_EXT_TYPE_C2)
            {
                reloadInput(droppath);
            }
            break;
        }
    }
    event->acceptProposedAction();
}

void Ass2text::dragMoveEvent(QDragMoveEvent *event)
{
    event->pos();/*警告対応*/
    QT_PASS;
}


QString Ass2text::getAssText(QStringList events)
{
    QString text = QT_EMPTY;
    if(events.length() < (int)AssEvents::MaxEvent)
    {
        /*一般的な字幕テキストではない*/
        /*メモリー溢れ回避、何もしない*/
        QT_PASS;
    }
    else if(events.length() == (int)AssEvents::MaxEvent)
    {
        /*一般的な字幕テキスト*/
        text = events.at((int)AssEvents::Text);
    }
    else if(events.length() > (int)AssEvents::MaxEvent)
    {
        /*字幕テキストにカンマが付いてる場合*/
        QStringList texts;
        for(int i = (int)AssEvents::Text; i < events.length(); i++)
        {
            texts.append(events.at(i));
        }
        text = texts.join(ASS_TAG_COMMA);
    }
    return text;
}
