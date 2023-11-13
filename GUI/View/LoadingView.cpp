#include "stdafx.h"
#include "LoadingView.h"

LoadingView::LoadingView(QWidget *parent) : PageView(parent)
{
  SetupUI();
  SetupEvent();
}

LoadingView::~LoadingView()
{

}

void LoadingView::UpdateCopy(qint64 progress)
{
  this->UpdateUI(progress);
}

void LoadingView::SetupUI()
{
  this->UseFullPage(true);

  MainLayout = new QVBoxLayout();
  MainLayout->setMargin(25);
  MainLayout->setSpacing(25);

  Labels[DOWNLOAD_FILE] = new QLabel();
  Labels[DOWNLOAD_FILE]->setAlignment(Qt::AlignCenter);

  QHBoxLayout *progressLayout = new QHBoxLayout();

  progressBar = new QProgressBar();
  progressBar->setTextVisible(true);
  progressBar->setValue(0);
  progressBar->setFormat("%p %");

  progressLayout->addStretch();
  progressLayout->addWidget(progressBar);
  progressLayout->addStretch();

  QHBoxLayout *timeLayout = new QHBoxLayout();

  Labels[DOWNLOAD_TIME_FIELD] = new QLabel(tr("DOWNLOAD_TIME"));
  Labels[DOWNLOAD_TIME_VALUE] = new QLabel(Tools::ChronoToString(0, false));

  Labels[DOWNLOAD_TIME_FIELD]->setAlignment(Qt::AlignLeft);
  Labels[DOWNLOAD_TIME_VALUE]->setAlignment(Qt::AlignLeft);

  timeLayout->addStretch();
  timeLayout->addWidget(Labels[DOWNLOAD_TIME_FIELD]);
  timeLayout->addWidget(Labels[DOWNLOAD_TIME_VALUE]);
  timeLayout->addStretch();

  QHBoxLayout *speedLayout = new QHBoxLayout();

  Labels[DOWNLOAD_SPEED_FIELD]  = new QLabel(tr("DOWNLOAD_SPEED"));
  Labels[DOWNLOAD_SPEED_VALUE]  = new QLabel("0 KB/s");


  Labels[DOWNLOAD_SPEED_FIELD]->setAlignment(Qt::AlignLeft);
  Labels[DOWNLOAD_SPEED_VALUE]->setAlignment(Qt::AlignLeft);

  speedLayout->addStretch();
  speedLayout->addWidget(Labels[DOWNLOAD_SPEED_FIELD]);
  speedLayout->addWidget(Labels[DOWNLOAD_SPEED_VALUE]);
  speedLayout->addStretch();

  QHBoxLayout *buttonsLayout = new QHBoxLayout();

  Leds[BUTTON_CANCEL]        = new Led("CANCEL");
  Leds[BUTTON_OK]            = new Led("REPLAY");
  
  buttonsLayout->addWidget(Leds[BUTTON_OK]);
  buttonsLayout->addWidget(Leds[BUTTON_CANCEL]);

  for (Led* led : Leds)
  {
    led->UseHasButton(true);
    led->SetState(Mag200Status::UNLOCK, Color::GRAY_03);
  }

  timer = new QElapsedTimer();

  MainLayout->addWidget(Labels[DOWNLOAD_FILE]);
  MainLayout->addLayout(progressLayout);
  MainLayout->addLayout(timeLayout);
  MainLayout->addLayout(speedLayout);
  MainLayout->addLayout(buttonsLayout);

  QWidget *widget = new QWidget();
  widget->setLayout(MainLayout);
  this->AddMainWidget(widget);
}

void LoadingView::SetupEvent()
{
  for (Led* led : Leds)
    QObject::connect(led, &Led::clicked, this, &LoadingView::LedClicked);
}

void LoadingView::UpdateUI(qint64 totalBytesCopied)
{
  qint64 elapsed = timer->nsecsElapsed();
  qint64 time = elapsed - lastTime;
  this->totalTime += time;

  qint64 fileSize = file.size();
  double progress = totalBytesCopied / (double)fileSize;
  
  QString timeStr = Tools::ChronoToString(totalTime * 1e-6, false);
  QString speedStr = Tools::BitrateToString(totalBytesCopied / (totalTime * 1e-9), false) + "/s";

  double size = totalBytesCopied;
  while (size >= 1000.0)
    size /= 1000.0;

  QString sizeStr = Tools::BitrateToString(fileSize, false);
  QString format = QString::number(size, 'f', 2) + " / " + sizeStr + " (%p %)";

  this->progressBar->setFormat(format);
  this->progressBar->setValue(progress * 100);
  Labels[DOWNLOAD_TIME_VALUE]->setText(timeStr);
  Labels[DOWNLOAD_SPEED_VALUE]->setText(speedStr);

  this->lastTime = elapsed;

  if (fileSize == totalBytesCopied)
  {
    Leds[BUTTON_OK]->setVisible(true);
  }
}

void LoadingView::resizeEvent(QResizeEvent * event)
{
  QSize size = event->size();

  int width = Tools::GetWidgetWidth(this, size, MainLayout);
  int height = Tools::GetWidgetHeight(this, size, MainLayout);

  qreal ratioH = 1.0;
  int availableHeight = height;

  int buttonWidth = width * 0.4;
  int buttonHeight = Tools::GetWidgetHeight(0.2, height, availableHeight, ratioH);

  for (Led* led : Leds)
  {
    led->SetSize(QSize(buttonWidth, buttonHeight));
    led->UseAutoSizeFont(true);
  }

  QFont font = this->font();
  font.setBold(true);

  int progressWidth = width * 0.8;
  int progressHeight = Tools::GetWidgetHeight(0.2, height, availableHeight, ratioH);
  int progressFontSize = Tools::AutoFontSize(progressWidth, progressHeight, "100 %");
  progressFontSize *= 0.45;

  font.setPixelSize(progressFontSize);

  progressBar->setFixedSize(progressWidth, progressHeight);
  progressBar->setFont(font);

  int labelHeight = availableHeight / qMax(1, Labels.size());
  int labelFontSize = labelHeight * 0.45;

  font.setPixelSize(labelFontSize);

  for (QLabel* label : Labels)
  {
    label->setFont(font);
    label->setFixedHeight(labelHeight);
  }
  Labels[DOWNLOAD_SPEED_FIELD]->setFixedWidth(progressWidth / 2);
  Labels[DOWNLOAD_TIME_FIELD]->setFixedWidth(progressWidth / 2);

  font.setPixelSize(labelHeight * 0.8);
  Labels[DOWNLOAD_FILE]->setFont(font);

  QFontMetrics metrics(font);
  QString text = Labels[DOWNLOAD_FILE]->text();
  
  if (metrics.width(text) > width)
  {
    QString filename = this->file.fileName();
    QString baseName = this->file.baseName();
    QString extension = this->file.suffix();
    QString fileSize = Tools::BitrateToString(file.size(), false);

    int ellipsisWidth = metrics.width("...");
    int middle = baseName.length() / 2;
    while (metrics.width(text) + ellipsisWidth > this->width() && text.length() > 0)
    {
      baseName = baseName.left(middle - 1) + "..." + baseName.right(middle - 1);
      text = baseName + "." + extension;
      middle--;
    }

    Labels[DOWNLOAD_FILE]->setText(text);
  }
}

void LoadingView::LedClicked()
{
  int index = LedIndex();

  switch (index)
  {
    default:                                                  return;
    case BUTTON_OK:      emit /* SendLedClicked(PAGE_); */    return;
    case BUTTON_CANCEL:  CancelCopy();                        return;
  }
}

void LoadingView::StartCopy(QString path)
{
  QFileInfo tmp(path);
  if (tmp.exists() == false)
    return;

  this->file = tmp;
  QString fileSize = Tools::BitrateToString(file.size(), false);
  QString text = file.fileName(); 

  Leds[BUTTON_OK]->setVisible(false);
  Labels[DOWNLOAD_FILE]->setText(text);
  
  timer->restart();
  this->lastTime = 0;
  this->totalTime = 0;

  QString dst = qSettings->Root_Path() + "\\TMP\\" + this->file.fileName();
  emit SendCopyFile(path, dst);
}

void LoadingView::CancelCopy()
{
  emit this->SendCancelCopy();
  emit this->SendLedClicked(PAGE_IMPORT);
}
