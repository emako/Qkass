#include "ass2lrc.h"
#include "ui_ass2lrc.h"

Ass2lrc::Ass2lrc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ass2lrc)
{
    ui->setupUi(this);
    this->setupUi();
}

Ass2lrc::~Ass2lrc()
{
    delete ui;
}

void Ass2lrc::setupUi(void)
{
    this->setWindowTitle(QString(APP_TOOLS_ASS2LRC));
    this->setAcceptDrops(true);
    this->setWindowIcon(QIcon(":res/trill.ico"));
    ui->inputEdit->setReadOnly(true);
    ui->authorEdit->setText(QString(APP_TOOLS_ASS2LRC));
}

QString Ass2lrc::createLrcTag()
{
    return QString(LRC_TAG_TI).arg(ui->tiEdit->text()) + QT_OTR_EOL
         + QString(LRC_TAG_AR).arg(ui->arEdit->text()) + QT_OTR_EOL
         + QString(LRC_TAG_AL).arg(ui->alEdit->text()) + QT_OTR_EOL
         + QString(LRC_TAG_OFFSET).arg(QString::number(ui->offsetSpinBox->value())) + QT_OTR_EOL
         + QString(LRC_TAG_AUTHOR).arg(ui->authorEdit->text()) + QT_OTR_EOL;
}

void Ass2lrc::on_creatBt_clicked()
{
    for(QString input : inputs)
    {
        /*字幕テキストを導入する*/
        QFile infile(input);
        infile.open(QIODevice::ReadOnly);
        QTextStream in(&infile);
        QString text = in.readAll();
        /*字幕テキストを処理する*/
        QString tag = createLrcTag();
        text = QString(QT_STR_ARG_2).arg(tag).arg(ass2lrc(text));
        QString output = Common::getPathFileName(input) + QT_EXT_SPLITE + LRC_EXT;
        /*歌詞テキストを保存する*/
        QFile outfile(output);
        outfile.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream out(&outfile);
        out << text;
        outfile.close();
    }
}

QString Ass2lrc::ass2lrc(QString text)
{
    if(text.isEmpty())
    {
        return QT_EMPTY;
    }

    text = Common::toNorEol(text);
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
                return QT_EMPTY;
            }
            ass_text = Ass2text::getAssText(events);
            ass_text = stripTag(ass_text);
            QString start = events.at((int)AssEvents::Start);
            ass_text = QString(QT_STR_LRC_TEXT).arg(start).arg(ass_text);
            ass_text_out_list.append(ass_text);
        }
    }
    return ass_text_out_list.join(QT_OTR_EOL);
}

QString Ass2lrc::stripTag(QString text)
{
    text = text.replace(QRegExp(ASS_TAG_BRACE_REGEXP), QT_EMPTY);
    text = text.replace(ASS_TAG_HARD_EOL, QT_BLANK);
    text = text.replace(ASS_TAG_SOFT_EOL, QT_BLANK);
    text = text.replace(ASS_TAG_BLANK, QT_BLANK);
    return text;
}

void Ass2lrc::dragEnterEvent(QDragEnterEvent* event)
{
    if(event->mimeData()->hasText())
    {
        event->acceptProposedAction();
    }
}

void Ass2lrc::dropEvent(QDropEvent* event)
{
    const QMimeData* mime = event->mimeData();
    if(mime->hasUrls())
    {
        inputs.clear();
        for(QUrl url : event->mimeData()->urls())
        {
            QString inputpath = url.toString();
            inputpath.remove(QT_FILE_PROTOCOL);
            QString ext = Common::getPathExtOnly(inputpath).toLower();
            if(ext == ASS_EXT_TYPE_A1 || ext == ASS_EXT_TYPE_B1 || ext == ASS_EXT_TYPE_C1)
            {
                inputs << inputpath;
            }
        }
        ui->inputEdit->setText(inputs.join(QT_PATH_SPLITE));
    }
    event->acceptProposedAction();
}

void Ass2lrc::on_inputBt_clicked()
{
    QStringList paths = QFileDialog::getOpenFileNames(this, tr("Select SubStationAlpha files"), "/", tr("*.ass *.ssa *.txt"));
    if(paths.isEmpty())
    {
        return;
    }
    inputs = paths;
    ui->inputEdit->setText(inputs.join(QT_PATH_SPLITE));
}

void Ass2lrc::on_hideBt_clicked()
{
    this->hide();
}
