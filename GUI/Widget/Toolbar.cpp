#include "stdafx.h"
#include "Toolbar.h"
#include "NdaSdkCore.h"

/*
 * @brief Constructor for the Toolbar class.
 *
 * @param parent The parent QWidget.
 * @return void
 *
 * @date 31/05/2023
 */
Toolbar::Toolbar(QWidget *parent) : QFrame(parent)
{
  SetupUI();
  SetupEvent();
  Initialize();
}

/*
 * @brief Destructor for the Toolbar class.
 *
 * @return void
 *
 * @date 31/05/2023
 */
Toolbar::~Toolbar()
{
  for (Led* led : Leds)
    led->disconnect();
}


/*
 * @brief Sets up the user interface of the toolbar.
 *
 * This function creates and initializes the LED of the toolbar
 *
 * @return void
 *
 * @date 31/05/2023
 */
void Toolbar::SetupUI()
{  
  // Set margins and spacing for the main layout
  QMargins margin(5, 0, 5, 0);
  MainLayout = new QHBoxLayout();
  MainLayout->setContentsMargins(margin);
  MainLayout->setSpacing(2);

  // Set the main layout for the toolbar
  this->setLayout(MainLayout);
}

/*
 * @brief Sets up the event connections.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void Toolbar::SetupEvent()
{
}

void Toolbar::Initialize()
{
  // Reset flags
  this->m_HasRF = false;
  this->m_TimeType = FLYWHEEL;

  int typeCounter = 0;
  QMap<int, Mag200Data*> datas = qSettings->Mag200Datas();

  // Check if there is RF
  for (Mag200Data *data : datas)
    if (data->IsCH10() == true)
      this->m_HasRF = true;

  // Check Intern or Extern Time
  for (Mag200Data *data : datas)
    typeCounter += data->IrigCode();

  // Define Time Type
  if      (typeCounter == 0)             this->m_TimeType = INTERN;
  else if (typeCounter == datas.count()) this->m_TimeType = EXTERN;
  else                                   this->m_TimeType = DO_NOT_EXIST;
}

void Toolbar::NewStatus(int, int, QVariant)
{

}

bool Toolbar::IsValidStatus(int ChannelID, int)
{
  if (ChannelID == NDA_SDK_CHANNEL)
    return true;

  QMap<int, Mag200Data*> data = qSettings->Mag200Datas();
  if (data.contains(ChannelID) == false)
    return false;

  Mag200Data *channel = data[ChannelID];
  if (channel == nullptr)
    return false;

  return true;
}

QString Toolbar::TimeType()
{
  return this->m_TimeType;
}

bool Toolbar::HasRF()
{
  return this->m_HasRF;
}

void Toolbar::resizeEvent(QResizeEvent * event)
{
  QWidget::resizeEvent(event);

  qreal ratioW = 1.0;
  qreal ratioH = 1.0;
  int width = Tools::GetWidgetWidth(this, event->size(), MainLayout);
  int height = Tools::GetWidgetHeight(this, event->size(), MainLayout);

  if (height == 0)
    return;

  // Calculate widgets height
  int availableWidth = width;
  int availableHeight = height;

  QFont font = this->font();
  font.setBold(true);
  int ledSize = 0;

  int ledHeight = Tools::GetWidgetHeight(0.75, height, availableHeight, ratioH);
  qreal ratio = ledHeight / (double)width;

  for (Led* led : Leds)
  {
    int index = Leds.key(led);

    switch (index)
    {
      default:
        ledSize = Tools::GetWidgetWidth(ratio, width, availableWidth, ratioW);
        font.setPixelSize(ledSize * 0.6);

        led->SetSize(QSize(ledSize, ledSize));
        led->setFont(font);
        continue;

      case TIME:
      case REPLAY_TIME:
      case ELAPSED_TIME:
        continue;
    }
  }
  this->setFont(font);
  QStringList letters = this->TimeType().split("", QString::SkipEmptyParts);

  int ledWidth = Tools::GetWidgetWidth(0.5, width, availableWidth, ratioW);
  int fontSize = this->LedFontSize(ledWidth, ledSize, letters);
  font.setPixelSize(fontSize);

  if (Leds.contains(TIME) == true)
  {
    Leds[TIME]->SetSize(QSize(ledWidth, ledSize));
    Leds[TIME]->setFont(font);
  }

  if (Leds.contains(REPLAY_TIME) == true)
  {
    Leds[REPLAY_TIME]->SetSize(QSize(ledWidth, ledSize));
    Leds[REPLAY_TIME]->setFont(font);
  }

  if (Leds.contains(ELAPSED_TIME) == true)
  {
    Leds[ELAPSED_TIME]->SetSize(QSize(ledWidth, ledSize));
    Leds[ELAPSED_TIME]->setFont(font);
  }
}

// We use "4" because this is the widest number with the given font
#define MAX_STR_WIDTH_TIME " 444-44:44:44.444" 

int Toolbar::LedIndex()
{
  Led* led = static_cast<Led*>(QObject::sender());
  if (led == nullptr)
    return -1;

  QList<Led*> leds = Leds.values();
  if (leds.contains(led) == false)
    return -1;

  return Leds.key(led, -1);
}

int Toolbar::LedFontSize(int maxWidth, int maxHeight, QStringList letters)
{
  QString text = MAX_STR_WIDTH_TIME " ";
  QFont font = this->font();
  font.setFamily("FORCED_SQUARE_MONO");
  font.setBold(true);
  font.setPixelSize(maxHeight);

  QFontMetrics fontMetrics(font);
  int fontsize = font.pixelSize();
//  int MaxValue = maxHeight * 0.80;

  QRect rect = fontMetrics.boundingRect(QRect(0, 0, maxWidth, maxHeight), Qt::AlignLeft, text);

  int letterWidth = 0;
  for (const QString& letter : letters)
    letterWidth = qMax(letterWidth, fontMetrics.width(letter + " "));
  rect.adjust(0, 0, 0, letterWidth);

  bool isBigger = qMax(maxWidth, maxHeight);

  double step = isBigger ? 0.9 : 1.1;
  for (;;)
  {
    font.setPixelSize(fontsize);
    QFontMetrics fontMetrics(font);
    rect = fontMetrics.boundingRect(QRect(0, 0, maxWidth, maxHeight), Qt::AlignLeft, text);

    int letterWidth = 0;
    for (const QString& letter : letters)
      letterWidth = qMax(letterWidth, fontMetrics.width(letter + " "));
    rect.adjust(0, 0, 0, letterWidth);

    if (fontsize <= 1)
      break;

    if (step > 1 && rect.width() > maxWidth && rect.height() > maxHeight)
      break;
    if (step < 1 && rect.width() < maxWidth && rect.height() < maxHeight)
      break;

    fontsize *= step;
  }

  return fontsize;
}