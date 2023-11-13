#include "stdafx.h"
#include "StatusBar.h"

/*
 * @brief Constructor for the StatusBar class.
 *
 * The constructor sets up the user interface of the status bar.
 *
 * @param parent The parent widget.
 *
 * @return void
 *
 * @date 31/05/2023
 */
StatusBar::StatusBar(QWidget *parent) : QFrame(parent)
{
  // Setup UI 
  SetupUI();

  // Setup event handlers
  SetupEvent();
}

/*
 * @brief Destructor for the StatusBar class.
 *
 * @return void
 *
 * @date 31/05/2023
 */
StatusBar::~StatusBar()
{
}

/*
 * @brief The function sets up the user interface of the status bar.
 *
 * The function configures the appearance and layout of the status bar by creating and positioning the necessary widgets.
 * It also initializes the default text and theme for the status label.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void StatusBar::SetupUI()
{
  // Set background and height
  this->setAutoFillBackground(true);

  // Set margins and spacing for the main layout
  QMargins margin(5, 0, 5, 0);
  MainLayout = new QHBoxLayout();
  MainLayout->setContentsMargins(margin);
  MainLayout->setSpacing(2);

  // Create the status label
  Status = new QLabel;
  Status->setAlignment(Qt::AlignVCenter);
  Status->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);

  // Create the test area label
  QString workArea = qSettings->WorkArea();
  if (workArea.isEmpty())
    workArea = "NONE";

  TestArea = new QLabel(workArea);
  TestArea->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

  // Add widgets to the main layout
  MainLayout->addWidget(Status);
  MainLayout->addStretch(1);
  MainLayout->addWidget(TestArea);
  MainLayout->addSpacing(25);

  // Set the main layout for the status bar
  this->setLayout(MainLayout);

  // (TEMP_DEMO) Prevent message
  // AddInfo(tr("STATUS_START"));
}

/*
 * @brief The function sets up event connections.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void StatusBar::SetupEvent()
{
  // Disconnect previous connections and connect new signal-slot pairs
  qMessages->disconnect(this);
  QObject::connect(qMessages, &MessagesManager::SendNewInfo, this, &StatusBar::AddInfo);
  QObject::connect(qMessages, &MessagesManager::SendNewError, this, &StatusBar::AddError);
  QObject::connect(qMessages, &MessagesManager::SendNewWarning, this, &StatusBar::AddWarning);

  // Chapter 6
  QObject::connect(qCommands, &CommandsManager::SendNewStatus, this, &StatusBar::NewStatusAcknowledge);
}

void StatusBar::resizeEvent(QResizeEvent * event)
{
  QSize size = event->size();

  qreal ratio = 1.0;
  int width = Tools::GetWidgetWidth(this, size, MainLayout);
  int height = Tools::GetWidgetHeight(this, size, MainLayout);

  if (height == 0)
    return;

  // Calculate widgets height
  int availableWidth = width;
  int statusWidth = Tools::GetWidgetHeight(0.3, width, availableWidth, ratio);
  int testAreaWidth = Tools::GetWidgetHeight(1.0, width, availableWidth, ratio);

  Status->setFixedWidth(statusWidth);
  TestArea->setFixedWidth(testAreaWidth);

  QFont font = this->font();
  font.setPixelSize(height * 0.6);
  font.setBold(true);
  this->setFont(font);

  Status->setFont(font);
  TestArea->setFont(font);

  QWidget::resizeEvent(event);
}

/*
 * @brief The function adds information to the status bar.
 *
 * The function sets the text of the info status label.
 * Change the color of the bar according the info theme.
 *
 * @param info The information to be displayed.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void StatusBar::AddInfo(QString info)
{
  // (TEMP_DEMO) Prevent message
  return;

  // Set the text
  Status->setText(info);

  // apply the info theme
  QPalette palette = qThemes->InfoTheme(this->palette());
  this->setPalette(palette);
  this->update();

  // Adjust the size of the status label
  Status->adjustSize();
}

/*
 * @brief The function adds an m_State message to the status bar.
 *
 * The function sets the text of the m_State status label.
 * Change the color of the bar according the m_State theme.
 *
 * @param m_State The m_State message to be displayed.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void StatusBar::AddError(QString m_State)
{
  // (TEMP_DEMO) Prevent message
  return;

  // Set the text
  Status->setText(m_State);

  // Get the theme manager and apply the m_State theme
  QPalette palette = qThemes->ErrorTheme(this->palette());
  this->setPalette(palette);
  this->update();

  // Adjust the size of the status label
  Status->adjustSize();
}

/*
 * @brief The function adds a warning message to the status bar.
 *
 * The function sets the text of the warning status label.
 * Change the color of the bar according the warning theme.
 *
 * @param warning The warning message to be displayed.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void StatusBar::AddWarning(QString warning)
{
  // (TEMP_DEMO) Prevent message
  return;

  // Set the text
  Status->setText(warning);

  // Get the theme manager and apply the warning theme
  QPalette palette = qThemes->WarningTheme(this->palette());
  this->setPalette(palette);
  this->update();

  // Adjust the size of the status label
  Status->adjustSize();
}

/*
 * @brief The function sets the text of the test area label.
 *
 * The function updates the text of the test area label to the provided test area value.
 *
 * @param testArea The new value for the test area.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void StatusBar::NewTestArea(QString testArea)
{
  TestArea->setText(testArea);
}

void StatusBar::NewStatusAcknowledge(int status)
{
  this->current = status;

  switch (status)
  {
    default:                      Status->setText(tr("STATE_NO_MAG_COM"));        break;
    case STATE_FAIL:              Status->setText(tr("STATE_FAIL"));              break;
    case STATE_IDLE:              Status->setText(tr("STATE_IDLE"));              break;
    case STATE_BIT:               Status->setText(tr("STATE_BIT"));               break;
    case STATE_ERASE:             Status->setText(tr("STATE_ERASE"));             break;
    case STATE_DECLASSIFY:        Status->setText(tr("STATE_DECLASSIFY"));        break;
    case STATE_RECORD:            Status->setText(tr("STATE_RECORD"));            break;
    case STATE_PLAY:              Status->setText(tr("STATE_PLAY"));              break;
    case STATE_RECORD_N_PLAY:     Status->setText(tr("STATE_RECORD_N_PLAY"));     break;
    case STATE_FIND:              Status->setText(tr("STATE_FIND"));              break;
    case STATE_BUSY:              Status->setText(tr("STATE_BUSY"));              break;
    case STATE_ERROR:             Status->setText(tr("STATE_ERROR"));             break;
  }

  // Adjust the size of the status label
  Status->adjustSize();
}
