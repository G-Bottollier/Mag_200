#include "stdafx.h"
#include "MessageManager.h"

MessageManager::MessageManager(QWidget *parent) : QDialog(parent)
{
  SetupUI();
  SetupEvent();
}

MessageManager::~MessageManager()
{

}

MessageManager * MessageManager::Instance()
{
  static MessageManager *Instance = new MessageManager();
  return Instance;
}

// Draw Rounded Dialog
void MessageManager::paintEvent(QPaintEvent*)
{
  if (!(windowFlags() & Qt::FramelessWindowHint) && !testAttribute(Qt::WA_TranslucentBackground))
    return;  // nothing to do

  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing);

  QRectF rect(QPointF(0, 0), size());

  ThemeManager *theme = ThemeManager::Instance();
  QColor borderColor = theme->Color(QPalette::Text);

  p.setPen(QPen(borderColor, CINFO_BORDER_W));
  p.setBrush(palette().brush(backgroundRole()));

  const qreal dlta = CINFO_BORDER_W * 0.5;
  rect.adjust(dlta, dlta, -dlta, -dlta);

  p.drawRoundedRect(rect, CINFO_BORDER_R, CINFO_BORDER_R, Qt::AbsoluteSize);
  p.end();
}

void MessageManager::SetupUI()
{
  this->hide();
  this->setModal(true);
  this->setWindowIcon(qApp->windowIcon());
  this->setAttribute(Qt::WA_TranslucentBackground);
  this->setFixedSize(QSize(CINFO_WIDTH, CINFO_HEIGHT));
  this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog | Qt::FramelessWindowHint);

  QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  QVBoxLayout *MainLayout = new QVBoxLayout();
  MainLayout->setMargin(CINFO_BORDER_W + 15);

  QFont font = this->font();
  font.setBold(true);
  font.setPixelSize(CINFO_TITLE_H);

  QString time = GetTimerStringFromSecond(0);

  Title = new QLabel(tr("MAG_200"));
  Title->setFont(font);
  Title->setSizePolicy(policy);
  Title->setAlignment(Qt::AlignCenter);

  font.setBold(false);
  font.setPixelSize(CINFO_DESCRIPTION_H);

  Description = new QLabel("");
  Description->setFont(font);
  Description->setSizePolicy(policy);
  Description->setAlignment(Qt::AlignCenter);

  font.setBold(false);
  font.setPixelSize(CINFO_PROGRESS_TEXT_H);

  Progress = new QProgressBar();
  Progress->setFont(font);
  Progress->setTextVisible(true);
  Progress->setSizePolicy(policy);
  Progress->setAlignment(Qt::AlignCenter);
  Progress->setFixedHeight(CINFO_PROGRESS_H);

  font.setBold(false);
  font.setPixelSize(CINFO_TIME_H);

  StrTime = new QLabel(time);
  StrTime->setFont(font);
  StrTime->setSizePolicy(policy);
  StrTime->setAlignment(Qt::AlignCenter);

  font.setBold(false);
  font.setPixelSize(CINFO_DSK_H);

  MainLayout->addWidget(Title);
  MainLayout->addWidget(Description);
  MainLayout->addWidget(Progress);
  MainLayout->addWidget(StrTime);

  this->setLayout(MainLayout);
}

void MessageManager::SetupEvent()
{
  Timer = new QTimer();
  Timer->setInterval(1000);
  QObject::connect(Timer, &QTimer::timeout, this, &MessageManager::TimerTimeout, Qt::DirectConnection);
}

void MessageManager::TimerTimeout()
{
  if (isWaiting == true)
  {
    int val = Progress->value() == Progress->maximum() ? 0 : MessageManager::Auto;
    SetScrollPosition(val, isWaiting);
  }

  UpdateElapsedTime(isWaiting);
}

void MessageManager::UpdateElapsedTime(bool force)
{
  if(ElapsedSecond != nullptr)
    StrTime->setText(GetTimerStringFromSecond(ElapsedSecond->elapsed() / 1000));
  else
    StrTime->setText(GetTimerStringFromSecond(0));

  if (force == true)
    QCoreApplication::processEvents();
}

QString MessageManager::GetTimerStringFromSecond(int second)
{
  return tr("ELAPSED_TIME") + QDateTime::fromTime_t(second).toUTC().toString("hh:mm:ss");
}

/********************/
/* STATIC FUNCTIONS */
/********************/

void MessageManager::InitParent(QWidget *parent)
{
  this->setParent(parent, this->windowFlags());
  this->isVisible() ? this->show() : this->hide();
}

void MessageManager::Open()
{
  if (this->isVisible() == false)
  {
    this->UpdateElapsedTime();
    this->ElapsedSecond = new QElapsedTimer();
    this->ElapsedSecond->start();
    this->Timer->start();
  }

  this->show();
  QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
}

void MessageManager::Close()
{
  this->Timer->stop();
  delete this->ElapsedSecond;
  this->ElapsedSecond = nullptr;

  if (this->isWaiting == true)
  {
    this->isWaiting = false;
    this->Progress->setTextVisible(true);
  }

  this->hide();
  this->UpdateElapsedTime();
  QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
}

void MessageManager::EnableWaitingMode()
{
  this->isWaiting = true;
  this->Progress->setTextVisible(false);
}

void MessageManager::SetScrollRange(int min, int max, bool force)
{
  this->Progress->setRange(min, max);
  this->UpdateElapsedTime(force);
}

void MessageManager::SetScrollPosition(int currentPosition, bool force)
{
  int newValue = currentPosition;
  switch (currentPosition)
  {
    case Auto:
      newValue = this->Progress->value() + 1;
      break;
    case Max: // Max
      newValue = this->Progress->maximum();
      break;
    default:
      break;
  }

  this->Progress->setValue(newValue);
  this->UpdateElapsedTime(force);
}

void MessageManager::SetTitle(QString title, bool force)
{
  this->Title->setText(title);
  this->UpdateElapsedTime(force);
}

void MessageManager::SetDescription(QString description, bool force)
{
  this->Description->setText(description);
  this->UpdateElapsedTime(force);
}

void MessageManager::NewLog(int index, QStringList list)
{
  if (index >= MAX_LOG_INDEX)
    return;

  emit this->SendNewLog(index, list);
}

void MessageManager::NewInfo(QString info)
{
  emit this->SendNewInfo(info);
}

void MessageManager::NewError(QString error)
{
  emit this->SendNewError(error);
}

void MessageManager::NewWarning(QString Warning)
{
  emit this->SendNewWarning(Warning);
}
