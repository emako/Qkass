#include "asstt.h"
#include "ui_asstt.h"

AssTt::AssTt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AssTt)
{
    ui->setupUi(this);
    this->setupUi();
}

AssTt::~AssTt()
{
    delete ui;
}

void AssTt::setupUi(void)
{
    ui->textEdit->setWordWrapMode(QTextOption::WordWrap);
    this->setWindowTitle(QString(APP_TOOLS_ASSTT));
    this->setAcceptDrops(true);
    this->setWindowIcon(QIcon(":res/tt.ico"));
}

void AssTt::setEndTime(QString text)
{
    prev_calc_text = text;
    QStringList events = text.split(ASS_TAG_COMMA);
    if(events.length() < (int)AssEvents::MaxEvent)
    {
        return;
    }

    /* Length OK. */
    QString start = events.at((int)AssEvents::Start);
    QString end = events.at((int)AssEvents::End);
    QTime startTime = QTime::fromString(start, ASS_TAG_TIMECODE_FORMAT);
    QTime endTime = QTime::fromString(end, ASS_TAG_TIMECODE_FORMAT);
    int elapsed_msecs = startTime.msecsTo(endTime);
    ui->endTimeSpinBox->setValue(elapsed_msecs);
}

QString AssTt::getPrevEffectText(void)
{
    return prev_calc_text;
}

void AssTt::on_hideBt_clicked()
{
    this->hide();
}

QString AssTt::tt(int start_time, int end_time, int fx_time, QString tag1, QString tag2)
{
    QString tags = tag1;
    int parm1;
    int parm2;
    bool change_tag = false;
    int max = (end_time - start_time) / fx_time;
    int* array = range(start_time, end_time, fx_time);
    for(int i=0; i < max; i++)
    {
        parm1 = array[i];
        parm2 = parm1 + fx_time;
        if(change_tag)
        {
            QString tagAt = tag1;
            tag1 = tag2;
            tag2 = tagAt;
            change_tag = false;
        }
        tags += QString(ASS_TAG_STR_TT).arg(ASS_TAG_TT).arg(QString::number(parm1)).arg(QString::number(parm2)).arg(tag2);
        change_tag = true;
    }
    delete array;
    return tags;
}

int* AssTt::range(int start, int end, int step)
{
    int max = (end - start) / step;
    int* array = new int[max];
    int count = 0;
    for(int i = start; i <= end; i += step)
    {
        array[count] = i;
        count++;
    }
    return array;
}

void AssTt::on_makeBt_clicked()
{
    int startTime = ui->startTimeSpinBox->value();
    int endTime = ui->endTimeSpinBox->value();
    int effectTime = ui->effectTimeSpinBox->value();

    QString tag1 = ui->initTagLineEdit->text();
    QString tag2 = ui->effectTagTineEdit->text();

    QString tag0 = ui->constTagLineEdit->text();
    QString text = ui->constTextLineEdit->text();

    QString tags = tt(startTime, endTime, effectTime, tag1, tag2);

    ui->textEdit->setText(QString(QT_STR_TAG_TEXT).arg(tag0).arg(tags).arg(text));
}
