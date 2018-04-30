#include "qkass.h"
#include "ui_qkass.h"

Qkass::Qkass(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Qkass)
{
    ui->setupUi(this);
    this->setupUi();
}

Qkass::~Qkass()
{
    delete ui;
}

void Qkass::setupUi(void)
{
    /*メモリー獲得、パラメータ初期化*/
    this->initParm();

    /*翻訳ファイルを導入する*/
    //this->setTrans();/*コメント：クラスがクリエイトされる前にする*/

    /*ハイライトを設定する*/
    this->setHighlight();

    /*フォント設定*/
    //this->setupUiFont();

    /*ツールバー設定*/
    this->setupToolBar();

    /*ステータスバー有効にする*/
    this->statusBar();
}

void Qkass::restart(void)
{
    qApp->exit(QT_RETCODE_RESTART);
}

void Qkass::initParm(void)
{
    m_asstt = new AssTt();
    m_ass2text = new Ass2text();
    m_ass2lrc = new Ass2lrc();
    m_input_highlighter = new Highlighter(ui->inputEdit->document());
    m_output_highlighter = new Highlighter(ui->outputEdit->document());
    m_config = new QSettings(CONFIG_FILE, QSettings::IniFormat);
    m_clipboard = QApplication::clipboard();

    zoom_in_count = QT_INDEX_0;
    zoom_out_count = QT_INDEX_0;

    isTimeLimted = false;
    kCount = QT_INDEX_0;

    ui->clearBt->hide();

    ui->inputEdit->setWordWrapMode(QTextOption::NoWrap);
    ui->outputEdit->setWordWrapMode(QTextOption::NoWrap);

    this->setWindowTitle(QString(QT_STR_ARG_3).arg(APP_NAME).arg(QT_BLANK).arg(APP_VERSION));
    this->setWindowOpacity(QKASS_OPACITY);
    this->setAcceptDrops(true);

    ui->inputEdit->setAcceptDrops(false);
    ui->outputEdit->setAcceptDrops(false);

    connect(ui->inputEdit->verticalScrollBar(), &QAbstractSlider::valueChanged, this, Qkass::slotTextEditSliderVertical);
    connect(ui->outputEdit->verticalScrollBar(), &QAbstractSlider::valueChanged, this, Qkass::slotTextEditSliderVertical);
    connect(ui->inputEdit->horizontalScrollBar(), &QAbstractSlider::valueChanged, this, Qkass::slotTextEditSliderHorizontal);
    connect(ui->outputEdit->horizontalScrollBar(), &QAbstractSlider::valueChanged, this, Qkass::slotTextEditSliderHorizontal);
}

void Qkass::setHighlight(void)
{
    this->setEditorHighlightSynth(m_input_highlighter);
    this->setEditorHighlightSynth(m_output_highlighter);
}

void Qkass::setEditorHighlightSynth(Highlighter *highlighter)
{
    QStringList keywords;
    keywords << QString(ASS_TAG_SOFT_EOL).left(QT_INDEX_1) + QString(ASS_TAG_SOFT_EOL)
             << QString(ASS_TAG_HARD_EOL).left(QT_INDEX_1) + QString(ASS_TAG_HARD_EOL)
             << QString(ASS_TAG_BLANK).left(QT_INDEX_1) + QString(ASS_TAG_BLANK);
    highlighter->addSyntax(QString(ASS_TAG_BRACE_REGEXP), highlighter->keyword_format);
    highlighter->addSyntax(QString(ASS_TAG_COMMENT_SCRIPT_REGEXP), highlighter->comment_line_format);
    highlighter->addSyntax(QString(ASS_TAG_COMMENT_TEXT_REGEXP), highlighter->comment_line_format);
    highlighter->addSyntax(QString(ASS_TAG_SECTION_REGEXP), highlighter->section_line_format);
    highlighter->addSyntax(QString(ASS_TAG_FORMAT_REGEXP), highlighter->format_line_format);
    highlighter->addSyntax(QString(ASS_TAG_STYLE_REGEXP), highlighter->style_line_format);
    highlighter->addSyntaxKeywords(keywords, highlighter->keyword_format);
}

void Qkass::slotHighlight(void)
{
    if(ui->inputEdit->toPlainText().isEmpty() && ui->inputEdit->toPlainText().isEmpty())
    {
        return;
    }
    if(!m_input_highlighter->isHasRules() || !m_output_highlighter->isHasRules())
    {
        m_input_highlighter->clearSyntax();
        m_output_highlighter->clearSyntax();
    }
    else
    {
        this->setHighlight();
    }

    /*Save scroll bar value*/
    int input_ver_value = ui->inputEdit->verticalScrollBar()->value();
    int output_ver_value = ui->outputEdit->verticalScrollBar()->value();
    int input_hor_value = ui->inputEdit->horizontalScrollBar()->value();
    int output_hor_value = ui->outputEdit->horizontalScrollBar()->value();

    /*Set text*/
    ui->inputEdit->document()->setPlainText(ui->inputEdit->toPlainText());
    ui->outputEdit->document()->setPlainText(ui->outputEdit->toPlainText());

    /*Reload scroll bar value*/
    ui->inputEdit->verticalScrollBar()->setValue(input_ver_value);
    ui->outputEdit->verticalScrollBar()->setValue(output_ver_value);
    ui->inputEdit->horizontalScrollBar()->setValue(input_hor_value);
    ui->outputEdit->horizontalScrollBar()->setValue(output_hor_value);
}

void Qkass::setupUiFont(void)
{
    int font_id = QFontDatabase::addApplicationFont(":/font/res/fonts/DigitalMini.ttf");
    QString font_family = QFontDatabase::applicationFontFamilies(font_id).at(QT_INDEX_0);
    QFont font(font_family, QT_FONT_SIZE);
    QApplication::setFont(font);
}

void Qkass::setupToolBar(void)
{
    /*openAction*/
    QAction *openAction = new QAction(QIcon(":/res/add.png"), tr("&Open"), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing text file."));
    connect(openAction, &QAction::triggered, this, Qkass::openDiskFile);
    /*saveAction*/
    QAction *saveAction = new QAction(QIcon(":/res/disk.png"), tr("&Save"), this);
    saveAction->setShortcuts(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save the text file, named from input file."));
    connect(saveAction, &QAction::triggered, this, Qkass::saveDiskFile);
    /*saveAsAction*/
    QAction *saveAsAction = new QAction(QIcon(":/res/disk_edit.png"), tr("&Save as"), this);
    saveAsAction->setShortcuts(QKeySequence::SaveAs);
    saveAsAction->setStatusTip(tr("Save text as a file."));
    connect(saveAsAction, &QAction::triggered, this, Qkass::saveAsDiskFile);
    /*copyAction*/
    QAction *copyAction = new QAction(QIcon(":/res/page_white_text.png"), tr("&Copy"), this);
    copyAction->setShortcuts(QKeySequence::UnknownKey);
    copyAction->setStatusTip(tr("Copy output text."));
    connect(copyAction, &QAction::triggered, this, Qkass::on_copyBt_clicked);
    /*clearAction*/
    QAction *clearAction = new QAction(QIcon(":/res/delete.png"), tr("&Clear"), this);
    clearAction->setShortcuts(QKeySequence::UnknownKey);
    clearAction->setStatusTip(tr("Clear all text."));
    connect(clearAction, &QAction::triggered, this, Qkass::on_clearBt_clicked);
    /*exitAction*/
    QAction *exitAction = new QAction(QIcon(":/res/door_in.png"), tr("&Exit"), this);
    exitAction->setShortcuts(QKeySequence::Close);
    exitAction->setStatusTip(tr("Exit this program."));
    connect(exitAction, &QAction::triggered, this, Qkass::on_exitBt_clicked);
    /*pasteAction*/
    QAction *pasteAction = new QAction(QIcon(":/res/paste_plain.png"), tr("&Paste"), this);
    pasteAction->setShortcuts(QKeySequence::Close);
    pasteAction->setStatusTip(tr("Paste text to input editor."));
    connect(pasteAction, &QAction::triggered, this, Qkass::pasteText);
    /*startAction*/
    QAction *startAction = new QAction(QIcon(":/res/play_green.png"), tr("&Start"), this);
    startAction->setShortcuts(QKeySequence::UnknownKey);
    startAction->setStatusTip(tr("Start to convert."));
    connect(startAction, &QAction::triggered, this, Qkass::on_startBt_clicked);
    /*lisenceAction*/
    QAction *lisenceAction = new QAction(QIcon(":/res/github.ico"), tr("&Lisence"), this);
    lisenceAction->setShortcuts(QKeySequence::UnknownKey);
    lisenceAction->setStatusTip(tr("Show lisence info."));
    connect(lisenceAction, &QAction::triggered, this, Qkass::showLisenceInfo);
    /*githubAction*/
    QAction *githubAction = new QAction(QIcon(":/res/link.png"), tr("&Github"), this);
    githubAction->setShortcuts(QKeySequence::UnknownKey);
    githubAction->setStatusTip(tr("Open project github link."));
    connect(githubAction, &QAction::triggered, this, Qkass::slotGithub);
    /*weiboAction*/
    QAction *weiboAction = new QAction(QIcon(":/res/link.png"), tr("&Weibo"), this);
    weiboAction->setShortcuts(QKeySequence::UnknownKey);
    weiboAction->setStatusTip(tr("Open weibo link."));
    connect(weiboAction, &QAction::triggered, this, Qkass::slotWeibo);
    /*highlightAction*/
    QAction *highlightAction = new QAction(QIcon(":/res/toggle_tag_hiding.png"), tr("&Highlight"), this);
    highlightAction->setShortcuts(QKeySequence::UnknownKey);
    highlightAction->setStatusTip(tr("Show or hide highlight of ass syntax."));
    connect(highlightAction, &QAction::triggered, this, Qkass::slotHighlight);
    /*zoomInAction*/
    zoomInAction = new QAction(QIcon("://res/zoom_in.png"), tr("&Zoom in"), this);
    zoomInAction->setShortcuts(QKeySequence::UnknownKey);
    zoomInAction->setStatusTip(tr("Zoom in editor."));
    connect(zoomInAction, &QAction::triggered, this, Qkass::slotZoomIn);
    /*zoomOutAction*/
    zoomOutAction = new QAction(QIcon(":/res/zoom_out.png"), tr("&Zoom out"), this);
    zoomOutAction->setShortcuts(QKeySequence::UnknownKey);
    zoomOutAction->setStatusTip(tr("Zoom out editor."));
    connect(zoomOutAction, &QAction::triggered, this, Qkass::slotZoomOut);
    /*zoomFitAction*/
    QAction *zoomFitAction = new QAction(QIcon(":/res/zoom_fit.png"), tr("&Zoom fit"), this);
    zoomFitAction->setShortcuts(QKeySequence::UnknownKey);
    zoomFitAction->setStatusTip(tr("Zoom fit editor."));
    connect(zoomFitAction, &QAction::triggered, this, Qkass::slotZoomFit);
    /*assTtAction*/
    QAction *assTtAction = new QAction(QIcon(":/res/tt_smallest.png"), tr("&AssTt"), this);
    assTtAction->setShortcuts(QKeySequence::UnknownKey);
    assTtAction->setStatusTip(tr("Open AssTt tool."));
    connect(assTtAction, &QAction::triggered, this, Qkass::slotOpenAssTt);
    /*ass2textAction*/
    QAction *ass2textAction = new QAction(QIcon(":/res/properties.png"), tr("&Ass2text"), this);
    ass2textAction->setShortcuts(QKeySequence::UnknownKey);
    ass2textAction->setStatusTip(tr("Open Ass2text tool."));
    connect(ass2textAction, &QAction::triggered, this, Qkass::slotOpenAss2text);
    /*ass2lrcAction*/
    QAction *ass2lrcAction = new QAction(QIcon(":/res/trill_smallest.png"), tr("&Ass2lrc"), this);
    ass2lrcAction->setShortcuts(QKeySequence::UnknownKey);
    ass2lrcAction->setStatusTip(tr("Open Ass2lrc tool."));
    connect(ass2lrcAction, &QAction::triggered, this, Qkass::slotOpenAss2lrc);
    /*languageDefaultAction*/
    QAction *languageDefaultAction = new QAction(QIcon(":/res/lang/def.ico"), tr("&Default"), this);
    languageDefaultAction->setShortcuts(QKeySequence::UnknownKey);
    connect(languageDefaultAction, &QAction::triggered, this, Qkass::slotLanguageDefault);
    /*languageChineseAction*/
    QAction *languageChineseAction = new QAction(QIcon(":/res/lang/chinese.ico"), tr("&Chinese"), this);
    languageChineseAction->setShortcuts(QKeySequence::UnknownKey);
    connect(languageChineseAction, &QAction::triggered, this, Qkass::slotLanguageChinese);
    /*languageJapaneseAction*/
    QAction *languageJapaneseAction = new QAction(QIcon(":/res/lang/japanese.ico"), tr("&Japanese"), this);
    languageJapaneseAction->setShortcuts(QKeySequence::UnknownKey);
    connect(languageJapaneseAction, &QAction::triggered, this, Qkass::slotLanguageJapanese);

    QMenu *menufile = ui->menuBar->addMenu(tr("&File"));
    menufile->addAction(openAction);
    menufile->addSeparator();
    menufile->addAction(startAction);
    menufile->addAction(copyAction);
    menufile->addAction(pasteAction);
    menufile->addAction(clearAction);
    menufile->addSeparator();
    menufile->addAction(saveAction);
    menufile->addAction(saveAsAction);
    menufile->addAction(exitAction);
    QMenu *menutools = ui->menuBar->addMenu(tr("&Tools"));
    menutools->addAction(assTtAction);
    menutools->addAction(ass2lrcAction);
    menutools->addAction(ass2textAction);
    QMenu *menuoptions = ui->menuBar->addMenu(tr("&Options"));
    QMenu *menulanguage = menuoptions->addMenu(QIcon(":/res/lang/lang_menu.png"), tr("&Language"));
    menulanguage->addAction(languageDefaultAction);
    menulanguage->addSeparator();
    menulanguage->addAction(languageChineseAction);
    menulanguage->addAction(languageJapaneseAction);
    menuoptions->addSeparator();
    menuoptions->addAction(zoomInAction);
    menuoptions->addAction(zoomOutAction);
    menuoptions->addAction(zoomFitAction);
    menuoptions->addSeparator();
    menuoptions->addAction(highlightAction);
    QMenu *menuabout = ui->menuBar->addMenu(tr("&About"));
    menuabout->addAction(lisenceAction);
    menuabout->addSeparator();
    menuabout->addAction(githubAction);
    menuabout->addAction(weiboAction);

    ui->toolBar->addAction(openAction);
    ui->toolBar->addAction(startAction);
    ui->toolBar->addAction(copyAction);
    ui->toolBar->addAction(pasteAction);
    ui->toolBar->addAction(clearAction);
    ui->toolBar->addAction(saveAction);
    ui->toolBar->addAction(saveAsAction);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(zoomInAction);
    ui->toolBar->addAction(zoomOutAction);
    ui->toolBar->addAction(zoomFitAction);
    ui->toolBar->addAction(highlightAction);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(lisenceAction);
}

void Qkass::slotZoomIn(bool checked)
{
    if(!checked)
    {
        if(zoom_in_count >= QT_INDEX_7)
        {
            return;
        }
    }
    zoom_in_count++;
    ui->inputEdit->zoomIn(QT_INDEX_2);
    ui->outputEdit->zoomIn(QT_INDEX_2);
}

void Qkass::slotZoomOut(bool checked)
{
    if(!checked)
    {
        if(zoom_out_count >= QT_INDEX_4)
        {
            return;
        }
    }
    zoom_out_count++;
    ui->inputEdit->zoomOut(QT_INDEX_2);
    ui->outputEdit->zoomOut(QT_INDEX_2);
}

void Qkass::slotZoomFit(void)
{
    int count = zoom_in_count - zoom_out_count;
    for(int i=0; i < qAbs(count); i++)
    {
        if(count == qAbs(count))
        {
            /*zoom_in_value >= zoom_out_value*/
            emit zoomOutAction->triggered(true);
        }
        else
        {
            emit zoomInAction->triggered(true);
        }
    }
    zoom_in_count = QT_INDEX_0;
    zoom_out_count = QT_INDEX_0;
}

void Qkass::reloadInput(QString path)
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
    ui->inputEdit->document()->setPlainText(lines.join(QT_NOR_EOL));
    emit ui->startBt->clicked();
}

void Qkass::slotGithub(void)
{
    const QUrl url(QKASS_GITHUB_URL);
    QDesktopServices::openUrl(url);
}

void Qkass::slotWeibo(void)
{
    const QUrl url(QKASS_WEIBO_URL);
    QDesktopServices::openUrl(url);
}

void Qkass::showLisenceInfo(void)
{
    QResource lisence_res(":lisence.txt");
    QByteArray lisence_data((const char *)lisence_res.data(), lisence_res.size());
    QString lisence_info = QString::fromUtf8(lisence_data);
    QMessageBox message(QMessageBox::NoIcon, "Lisence", lisence_info, QMessageBox::Ok, this);
    QPixmap pixmap(":/res/github_large.png");
    //pixmap = pixmap.scaled(QSize(QT_INDEX_2, QT_INDEX_2), Qt::KeepAspectRatio);
    message.setIconPixmap(pixmap);
    message.exec();
}

void Qkass::openDiskFile(void)
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), "/", tr("Ass Files (*.ass *.ssa *.txt)"));
    if(path.isEmpty())
    {
        return;
    }
    input_path = path;
    reloadInput(path);
}

void Qkass::pasteText(void)
{
    ui->inputEdit->document()->setPlainText(m_clipboard->text());
}

void Qkass::saveDiskFile(void)
{
    QString text = ui->outputEdit->toPlainText();
    if(text.isEmpty())
    {
        QMessageBox::information(this, "Warning", "Empty output.");
        return;
    }
    if(input_path.isEmpty())
    {
        QMessageBox::information(this, "Warning", "Empty input path.");
        return;
    }
    QString path = Common::getPathOutput(input_path, ASS_TAG_SAVE_NAME);
    if(QFile::exists(path))
    {
        int result = QMessageBox::warning(this, "Warning", "Overwrite existing file?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(result == QMessageBox::No)
        {
            return;
        }
    }
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        QMessageBox::warning(this, "Warning","Can't open output file.", QMessageBox::Yes);
    }
    QTextStream out(&file);
    out << text;
    file.close();
//    qDebug()<<"Saved file in: "+path;
}

void Qkass::saveAsDiskFile(void)
{
    QString text = ui->outputEdit->toPlainText();
    if(text.isEmpty())
    {
        QMessageBox::information(this, "Warning", "Empty output.");
        return;
    }
    QString path = QFileDialog::getSaveFileName(this, tr("Open File"), "/", tr("Ass Files (*.ass *.ssa *.txt)"));
    if(path.isEmpty())
    {
        return;
    }
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        QMessageBox::warning(this, "Warning","Can't open output file.", QMessageBox::Yes);
    }
    QTextStream out(&file);
    out << text;
    file.close();
//    qDebug()<<"Saved file in: "+path;
}

void Qkass::on_copyBt_clicked()
{
    QString text = ui->outputEdit->toPlainText();
    if(text.isEmpty())
    {
        return;
    }
    m_clipboard->setText(text);
}

void Qkass::on_clearBt_clicked()
{
    ui->inputEdit->clear();
    ui->outputEdit->clear();
}

void Qkass::on_exitBt_clicked(bool clicked)
{
    m_asstt->close();
    m_ass2text->close();
    m_ass2lrc->close();
    if(clicked)
    {
        this->restart();
    }
    else
    {
        this->close();
    }
}

void Qkass::closeEvent(QCloseEvent *event)
{
    event->accept();/*警告対応*/
    emit ui->exitBt->clicked();
}

void Qkass::on_startBt_clicked()
{
    QString ass_text = ui->inputEdit->toPlainText();
    if(ass_text.isEmpty())
    {
        return;
    }

    QStringList ass_text_k_list;
    QStringList ass_text_list = ass_text.split(QT_NOR_EOL);
    for(QString ass_text : ass_text_list)
    {
        if(ass_text.startsWith(ASS_TAG_EVENT_FORMAT_DIALOGUE) || ass_text.startsWith(ASS_TAG_EVENT_FORMAT_COMMENT))
        {
            if(ui->stripTagCheckBox->isChecked())
            {
                ass_text = stripTag(ass_text);
            }
            QString ass_text_k = callKass(ass_text);
            if(!ass_text_k.isEmpty())
            {
                ass_text_k_list.append(ass_text_k);
//                qDebug()<<ass_text_k;
            }
        }
        else
        {
            ass_text_k_list.append(ass_text);
        }
    }
    /*Save scroll bar value*/
    int input_ver_value = ui->inputEdit->verticalScrollBar()->value();
    int output_ver_value = ui->outputEdit->verticalScrollBar()->value();
    int input_hor_value = ui->inputEdit->horizontalScrollBar()->value();
    int output_hor_value = ui->outputEdit->horizontalScrollBar()->value();

    /*Set text*/
    ui->outputEdit->document()->setPlainText(ass_text_k_list.join(QT_NOR_EOL));

    /*Reload scroll bar value*/
    ui->inputEdit->verticalScrollBar()->setValue(input_ver_value);
    ui->outputEdit->verticalScrollBar()->setValue(output_ver_value);
    ui->inputEdit->horizontalScrollBar()->setValue(input_hor_value);
    ui->outputEdit->horizontalScrollBar()->setValue(output_hor_value);
}

float Qkass::calcTextLength(QString text)
{
    QString text_prev = text;
    float length = (float)text.length();
    if(    text.indexOf(ASS_TAG_BLANK)    == QT_INDEX_NOT_FIND
        && text.indexOf(ASS_TAG_BLANK2)   == QT_INDEX_NOT_FIND
        && text.indexOf(ASS_TAG_HARD_EOL) == QT_INDEX_NOT_FIND
        && text.indexOf(ASS_TAG_SOFT_EOL) == QT_INDEX_NOT_FIND
        && text.indexOf(QT_BLANK)         == QT_INDEX_NOT_FIND )
    {
        /* Calc all. */
        QT_PASS;
    }
    else
    {
        /* Calc whitout blank. */
        QStringList skip_texts;
        skip_texts << ASS_TAG_BLANK << ASS_TAG_BLANK2 << ASS_TAG_HARD_EOL << ASS_TAG_SOFT_EOL;
        for(QString skip_text : skip_texts)
        {
            text = text.remove(skip_text);
        }
//        qDebug()<<"Real length is "+QString::number(length);
        length = (float)text.length();
//        qDebug()<<"Effect length is "+QString::number(length);
    }
    int syl_num = ui->sylComboBox->currentIndex();
    if(syl_num != QT_INDEX_0)
    {
        syl_num++;
        length /= (float)syl_num;
        if(length < (float)QT_INDEX_1)
        {
            length = (float)QT_INDEX_1;
        }
    }
    if(length != (float)text_prev.length())
    {
//        qDebug()<<"Latest length is "+QString::number(length);
    }
    return length;
}

QString Qkass::getAssText(QStringList events)
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

QString Qkass::callKass(QString rv_text)
{
    QString text_k = QString(QT_EMPTY);
    QStringList events = rv_text.split(ASS_TAG_COMMA);
    if(events.length() < (int)AssEvents::MaxEvent)
    {
        /*メモリー溢れ回避*/
        return text_k;
    }

    QString start = events.at((int)AssEvents::Start);
    QString end = events.at((int)AssEvents::End);
    QString texts = getAssText(events);
    float text_length = calcTextLength(texts);
    QString k_tag = this->calcKassValue(start, end, text_length);

    /* Foreach without text. */
    int at_count = QT_INDEX_0;
    for(QString event : events)
    {
        if(at_count >= ((int)AssEvents::MaxEvent - QT_INDEX_1))
        {
            text_k += QString(ASS_TAG_COMMA);
            break;
        }
        if(at_count == QT_INDEX_0)
        {
            text_k = event;
        }
        else
        {
            text_k = QString(QT_STR_ARG_3).arg(text_k).arg(ASS_TAG_COMMA).arg(event);
        }
        at_count++;
    }
    /* Call K Value */
    at_count = QT_INDEX_0;
    int at_skip_count = QT_INDEX_0;
    int at_syl_num = ui->sylComboBox->currentIndex();
    int at_syl_count = QT_INDEX_0;
    bool is_blank_skip = false;
    bool is_syl_skip = false;
    bool is_syl_first_not_skip_ok = false;
    int text_length_count = QT_INDEX_0;
    for(QString text : texts)
    {
        if(ui->skipBlankCheckBox->isChecked())
        {
            if(text == ASS_TAG_SKIP_FLAG)
            {
                if(at_count + QT_INDEX_1 <= texts.length() - QT_INDEX_1 )
                {
                    /* Index not overflow */
                    QString at_check = QString(QT_STR_ARG_2).arg(ASS_TAG_SKIP_FLAG).arg(texts.at(at_count + QT_INDEX_1));
                    QStringList skip_texts;
                    skip_texts << ASS_TAG_BLANK << ASS_TAG_BLANK2 << ASS_TAG_HARD_EOL << ASS_TAG_SOFT_EOL;
                    for(QString skip_text : skip_texts)
                    {
                        if(at_check == skip_text)
                        {
                            is_blank_skip = true;
                            break;
                        }
                    }
                }
            }
        }

        if(at_syl_num != QT_INDEX_0)
        {
            if(is_syl_first_not_skip_ok)
            {
                is_syl_skip = true;
            }
            else if(!is_blank_skip)
            {
                is_syl_first_not_skip_ok = true;
            }
        }

        if(is_syl_skip)
        {
            at_syl_count++;
            if(at_syl_count >= at_syl_num + QT_INDEX_1/*Cause of Not Skip*/)
            {
                is_syl_skip = false;
                at_syl_count = QT_INDEX_0;
            }
        }
        if(is_blank_skip || is_syl_skip)
        {
            /* タグ スキップ */
            text_k = QString(QT_STR_ARG_2).arg(text_k).arg(text);
        }
        else
        {
            /* タグ 追加 */
            if(isTimeLimted)
            {
                /* The k_tag is {\\k1} */
                text_length_count++;
                if(text_length_count <=  kCount)
                {
                    /* Add tag {\\k1} */
                    QT_PASS;
                }
                else
                {
                    /* Add tag {\\k0} */
                    k_tag = QString(QT_STR_TAG).arg(ASS_TAG_KARAOKE_K_LOWER).arg(QString::number(QT_INDEX_0));
                }
            }
            text_k = QString(QT_STR_ARG_3).arg(text_k).arg(k_tag).arg(text);
        }

        if(is_blank_skip)
        {
            at_skip_count++;
            if(at_skip_count >= ASS_TAG_SKIP_NUM)
            {
                is_blank_skip = false;
                at_skip_count = QT_INDEX_0;
            }
        }
        at_count++;
    }
    return text_k;
}


QString Qkass::calcKassValue(QString start, QString end, float length)
{
    isTimeLimted = false;
    kCount = QT_INDEX_0;
    QTime startTime = QTime::fromString(start, ASS_TAG_TIMECODE_FORMAT);
    QTime endTime = QTime::fromString(end, ASS_TAG_TIMECODE_FORMAT);
    QString k_tag = QT_EMPTY;
    int k_value = QT_INDEX_0;

    if(length == QT_INDEX_0)
    {
        return QT_EMPTY;
    }
    if(!ui->otherTagCheckBox->isChecked())
    {
        /* K(lower) tag. */
        switch((CallK)ui->karaokeComboBox->currentIndex())
        {
        case CallK::Average:
            do
            {
                int elapsed_msecs = startTime.msecsTo(endTime);
                k_value = (int)(((float)elapsed_msecs / (float)length) / 10);
                if(k_value == QT_INDEX_0)
                {
                    /*Time is limited*/
                    isTimeLimted = true;
                    kCount = (int)(elapsed_msecs / 10);
                    k_value = QT_INDEX_1;
                }
            }while(false);
            break;
        case CallK::Fixed:
            k_value = ui->fixedKaraokeSpinBox->value();
            break;
        default:
            break;
        }
        k_tag = QString(QT_STR_TAG).arg(ASS_TAG_KARAOKE_K_LOWER).arg(QString::number(k_value));
    }
    else
    {
        /* Other tag. */
        k_tag = ui->otherTagLineEdit->text();
        k_tag = QString(QT_STR_TAG_OTR).arg(k_tag);
    }
    return k_tag;
}

QString Qkass::stripTag(QString text)
{
    return text.replace(QRegExp(ASS_TAG_BRACE_REGEXP), QT_EMPTY);
}

void Qkass::on_karaokeComboBox_activated(int index)
{
    switch((CallK)index)
    {
    case CallK::Average:
        ui->fixedKaraokeSpinBox->setEnabled(false);
        break;
    case CallK::Fixed:
        ui->fixedKaraokeSpinBox->setEnabled(true);
        break;
    case CallK::Other:
    default:
        break;
    }
}

void Qkass::on_otherTagCheckBox_stateChanged(int arg1)
{
    if(arg1)
    {
        ui->typeLabel->setEnabled(false);
        ui->karaokeComboBox->setEnabled(false);
        ui->fixedKaraokeSpinBox->setEnabled(false);
        ui->otherTagLineEdit->setEnabled(true);
    }
    else
    {
        ui->typeLabel->setEnabled(true);
        ui->karaokeComboBox->setEnabled(true);
        emit ui->karaokeComboBox->activated(ui->karaokeComboBox->currentIndex());
        ui->otherTagLineEdit->setEnabled(false);
    }
}

void Qkass::dragEnterEvent(QDragEnterEvent* event)
{
    if(event->mimeData()->hasText())
    {
        event->acceptProposedAction();
    }
}

void Qkass::dropEvent(QDropEvent* event)
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
            QString ext = Common::getPathExt(droppath).toLower();
            if(ext == ASS_EXT_TYPE_A2 || ext == ASS_EXT_TYPE_B2 || ext == ASS_EXT_TYPE_C2)
            {
                input_path = droppath;
                emit ui->clearBt->clicked();
                reloadInput(droppath);
            }
            break;
        }
    }
    event->acceptProposedAction();
}

void Qkass::dragMoveEvent(QDragMoveEvent *event)
{
    event->pos();/*警告対応*/
    QT_PASS;
}

void Qkass::slotTextEditSliderVertical(int value)
{
    ui->inputEdit->verticalScrollBar()->setValue(value);
    ui->outputEdit->verticalScrollBar()->setValue(value);
}

void Qkass::slotTextEditSliderHorizontal(int value)
{
    ui->inputEdit->horizontalScrollBar()->setValue(value);
    ui->outputEdit->horizontalScrollBar()->setValue(value);
}

void Qkass::slotOpenAssTt(void)
{
    if(m_asstt->isHidden())
    {
        m_asstt->show();
        QString text;
        if(ui->inputEdit->hasFocus())
        {
            text = ui->inputEdit->getCurrentCursorText();
        }
        else
        {
            text = ui->outputEdit->getCurrentCursorText();
        }
        if(text != m_asstt->getPrevEffectText())
        {
            m_asstt->setEndTime(text);
        }
    }
    else
    {
        m_asstt->hide();
    }
}

void Qkass::slotOpenAss2text(void)
{
    if(m_ass2text->isHidden())
    {
        m_ass2text->show();
        QString text;
        if(ui->inputEdit->hasFocus())
        {
            text = ui->inputEdit->toPlainText();
        }
        else
        {
            text = ui->outputEdit->toPlainText();
        }
        m_ass2text->ass2Text(text);
    }
    else
    {
        m_ass2text->hide();
    }
}

void Qkass::slotOpenAss2lrc(void)
{
    if(m_ass2lrc->isHidden())
    {
        m_ass2lrc->show();
    }
    else
    {
        m_ass2lrc->hide();
    }
}

void Qkass::slotLanguageDefault(void)
{
    setConfig(CONFIG_PREFIX_COMMON, CONFIG_KEY_LANGUAGE, QLocale::English);
    int result = QMessageBox::information(this, "Message", "Restart program?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(result == QMessageBox::Yes)
    {
        emit ui->exitBt->clicked(true);
    }
}

void Qkass::slotLanguageChinese(void)
{
    setConfig(CONFIG_PREFIX_COMMON, CONFIG_KEY_LANGUAGE, QLocale::Chinese);
    int result = QMessageBox::information(this, "Message", "Restart program?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(result == QMessageBox::Yes)
    {
        emit ui->exitBt->clicked(true);
    }
}

void Qkass::slotLanguageJapanese(void)
{
    setConfig(CONFIG_PREFIX_COMMON, CONFIG_KEY_LANGUAGE, QLocale::Japanese);
    int result = QMessageBox::information(this, "Message", "Restart program?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(result == QMessageBox::Yes)
    {
        emit ui->exitBt->clicked(true);
    }
}

void Qkass::setConfig(const QString &prefix, const QString &key, const QVariant &value)
{
    m_config->beginGroup(prefix);
    m_config->setValue(key, value);
    m_config->endGroup();
}

QVariant Qkass::getConfig(const QString &prefix, const QString &key)
{
    m_config->beginGroup(prefix);
    QVariant value = m_config->value(key);
    m_config->endGroup();
    return value;
}
