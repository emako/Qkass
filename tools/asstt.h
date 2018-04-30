#ifndef ASSTT_H
#define ASSTT_H

#include "com/tag.h"
#include "com/common.h"

#include <QWidget>
#include <QTime>
#include <QDebug>

namespace Ui {
    class AssTt;
}

class AssTt : public QWidget
{
    Q_OBJECT

public:
    explicit AssTt(QWidget *parent = 0);
    ~AssTt();
    void setupUi(void);
    void setEndTime(QString text);
    QString getPrevEffectText(void);
    QString tt(int start_time, int end_time, int fx_time, QString tag1, QString tag2);
    int* range(int start, int end, int step);

private slots:
    void on_hideBt_clicked();
    void on_makeBt_clicked();

private:
    Ui::AssTt *ui;
    QString prev_calc_text;
};
#endif // ASSTT_H
