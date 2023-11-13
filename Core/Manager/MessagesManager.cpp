#include "stdafx.h"
#include "MessagesManager.h"

/*
 * @brief Constructor for MessagesManager.
 *
 * @param parent The parent QWidget of the MessagesManager dialog.
 *
 * @date 31/05/2023
 */
MessagesManager::MessagesManager(QWidget *parent) : QDialog(parent)
{
  this->setAttribute(Qt::WA_TranslucentBackground);
  this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog | Qt::FramelessWindowHint);

  // Create the window handle
  this->createWinId();
}

/*
 * @brief Destructor for MessagesManager.
 *
 * @date 31/05/2023
 */
MessagesManager::~MessagesManager()
{

}

/*
* @brief Returns the instance of MessagesManager.
*
* This function returns a pointer to the instance of MessagesManager.
* If the instance does not exist yet, it creates a static instance of MessagesManager.
* Subsequent calls to this function will always return the same instance.
*
* @return A pointer to the instance of MessagesManager.
*
* @date 31/05/2023
*/
MessagesManager* MessagesManager::Instance()
{
  // Create a static instance of MessagesManager if it doesn't exist yet
  static MessagesManager* Instance = new MessagesManager();

  // Return the instance
  return Instance;
}

/*
* @brief Override the Key press event
*
* @date 31/05/2023
*/
void MessagesManager::keyReleaseEvent(QKeyEvent *event)
{
  QWidget::keyReleaseEvent(event);

  // If Ctrl key is not pressed then return
  if ((event->modifiers() & Qt::ControlModifier) == false)
    return;

  // Check pressed key
  switch (event->key())
  {
  default:
    return;

  // If Ctrl + (top left key) Close MessageManager (Shortcut to bypass the modal window)
  case Qt::Key_twosuperior: // AZERTY
  case Qt::Key_QuoteLeft:   // QWERTY
    // Set Flags to true
    bypass = true;

    // Give 1min before revert the flags to false
    QTimer::singleShot(60000, [=]{ this->bypass = false; });

    // Close MessageManager
    this->Close();
    return;
  }
}

void MessagesManager::resizeEvent(QResizeEvent * event)
{
  QWidget::resizeEvent(event);

  QSize size = event->size();
  this->setFixedSize(size);

  int height = size.height() - borderSize * 2;

  if (this->layout() == nullptr)
    return;

  int count = this->layout()->count();
  double maxCount = qMax(1, count);

  qreal fontsize = height  / maxCount;
  QFont font = this->font();

  font.setPixelSize(fontsize * 0.55);
  font.setBold(true);
  Title->setFont(font);

  font.setPixelSize(fontsize * 0.45);
  font.setBold(false);
  Description->setFont(font);

  font.setPixelSize(fontsize * 0.6);
  font.setBold(true);
  Progress->setFont(font);

  font.setPixelSize(fontsize * 0.5);
  font.setBold(false);
  StrTime->setFont(font);
}

/*
 * @brief Handles the paint event for the MessagesManager dialog.
 *
 * It adjusts the dialog rectangle based on the border size, and draws a rounded
 * rectangle with the adjusted coordinates and border radius.
 *
 * @param event The paint event object.
 *
 * @date 31/05/2023
 */
void MessagesManager::paintEvent(QPaintEvent*)
{
  // Get the painter
  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing);

  // Get the Rect of the dialog
  QRectF rect(QPointF(0, 0), size());

  // Get the border color from the Theme of the app
  QColor borderColor = qThemes->Color(QPalette::Text);

  // Set the pen for drawing the border with the specified border color and width
  p.setPen(QPen(borderColor, borderSize));

  // Set the brush with the widget's background color
  p.setBrush(palette().brush(backgroundRole()));

  // Resize the dialog depending the size of the border
  const qreal dlta = borderSize * 0.5;
  rect.adjust(dlta, dlta, -dlta, -dlta);

  // Draw the rounded rectangle with the adjusted rectangle coordinates and border radius
  p.drawRoundedRect(rect, borderRadius, borderRadius, Qt::AbsoluteSize);
  p.end();
}

/*
* @brief Initialize function
*
* This function initializes the MessagesManager dialog by setting up the user interface (UI) and event handling.
* It is called before the dialog is shown for the first time.
*
* @date 14/06/2023
*/
void MessagesManager::Initialize()
{
  // Setup UI 
  SetupUI();

  // Setup event handlers
  SetupEvent();

  // Set the initialization flag to true
  this->isInitialize = true;
}

/*
 * @brief Set up the user interface for the MessagesManager dialog.
 *
 * @date 31/05/2023
 */
void MessagesManager::SetupUI()
{
  // Set properties of the dialog itseld
  this->hide();
  this->setModal(true);
  this->setWindowIcon(qApp->windowIcon());

  // Create a size policy for expanding horizontally and fixed vertically
  QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  // Create the main vertical layout for the widget
  QVBoxLayout* MainLayout = new QVBoxLayout();
  MainLayout->setMargin(35);

  // Create the title label and set its properties
  Title = new QLabel(tr("MAG_200"));
  Title->setSizePolicy(policy);
  Title->setAlignment(Qt::AlignCenter);

  // Create the description label and set its properties
  Description = new QLabel("");
  Description->setSizePolicy(policy);
  Description->setAlignment(Qt::AlignCenter);

  // Create the progress bar and set its properties
  Progress = new QProgressBar();
  Progress->setTextVisible(true);
  Progress->setSizePolicy(policy);
  Progress->setAlignment(Qt::AlignCenter);

  // Create the time label and set its properties
  StrTime = new QLabel(GetTimerStringFromSecond(0));
  StrTime->setSizePolicy(policy);
  StrTime->setAlignment(Qt::AlignCenter);

  // Add the title, description, progress bar, and time label to the main layout
  MainLayout->addWidget(Title);
  MainLayout->addWidget(Description);
  MainLayout->addWidget(Progress);
  MainLayout->addWidget(StrTime);

  // Set the main layout for the widget
  this->setLayout(MainLayout);
}

/*
 * @brief Set up event handling for the MessagesManager dialog.
 *
 * @date 31/05/2023
 */
void MessagesManager::SetupEvent()
{
  // Create a QTimer object for handling the timer events
  Timer = new QTimer();

  // Set the interval of the timer to 1000 milliseconds (1 second)
  Timer->setInterval(1000);

  // Use the Qt::DirectConnection connection type to ensure the slot is executed immediately
  QObject::connect(Timer, &QTimer::timeout, this, &MessagesManager::TimerTimeout, Qt::DirectConnection);
}

/*
 * @brief Slot function called when the timer times out.
 *
 * It checks if the MessagesManager is in waiting mode and
 * updates the scroll position and elapsed time display accordingly.
 *
 * @date 31/05/2023
 */
void MessagesManager::TimerTimeout()
{
  // Check if the MessagesManager is in waiting mode
  if (isWaiting == true)
  {
    // Determine the new scroll position based on the current value of the progress bar
    // If the current value is at the maximum, set the new value to 0; otherwise, set it to MessagesManager::Auto
    int val = Progress->value() == Progress->maximum() ? 0 : MessagesManager::Auto;

    // Set the new scroll position (if isWaiting is true, force the refresh)
    SetScrollPosition(val, isWaiting);
  }

  // Update the elapsed time display (if isWaiting is true, force the refresh)
  UpdateElapsedTime(isWaiting);
}

/*
 * @brief Update the elapsed time on the MessagesManager dialog.
 *
 * it calculates the current elapsed time in seconds
 * and sets the text of the StrTime label to the timer.
 * If the ElapsedSecond object is null, it sets the text to 0 seconds.
 * If the 'force' parameter is true, it processes any pending events in the event loop.
 *
 * @param force A boolean indicating whether to force processing pending events in the event loop.
 *
 * @date 31/05/2023
 */
void MessagesManager::UpdateElapsedTime(bool force)
{
  // Check if the ElapsedSecond object is not null
  if (ElapsedSecond != nullptr)
  {
    // Set the text of the StrTime label to the timer string calculated from the elapsed time in seconds
    StrTime->setText(GetTimerStringFromSecond(ElapsedSecond->elapsed() / 1000));
  }
  else
  {
    // Set the text of the StrTime label to the timer string calculated from 0 seconds
    StrTime->setText(GetTimerStringFromSecond(0));
  }

  // Check if force is true
  if (force == true)
  {
    // Process any pending events in the event loop
    QCoreApplication::processEvents();
  }
}

/*
 * @brief Get the timer string from the provided number of seconds.
 *
 * The time (second) is converted to a QDateTime object and formatted as "hh:mm:ss".
 * Then The string is returned.
 *
 * @param second The number of seconds for the elapsed time.
 * @return The timer string in the format "ELAPSED_TIME hh:mm:ss".
 *
 * @date 31/05/2023
 */
QString MessagesManager::GetTimerStringFromSecond(int second)
{
  // Construct the elapsed time string using the provided seconds
  // The string format is "ELAPSED_TIME hh:mm:ss"
  return tr("ELAPSED_TIME") + QDateTime::fromTime_t(second).toUTC().toString("hh:mm:ss");
}

/*
 * @brief Initialize the parent widget of the MessagesManager dialog.
 *
 * It sets the parent widget and preserves the window flags of the MessagesManager dialog.
 * If the MessagesManager window was previously visible, it reshow the MessagesManager window.
 * Because changing parent automatically hide the window.
 * 
 * @param parent The new parent widget for the MessagesManager dialog.
 *
 * @date 31/05/2023
 */
void MessagesManager::InitParent(QWidget *parent)
{
  // Check if the MessagesManager window is currently visible
  bool reshow = false;
  if (this->isVisible() == true)
    reshow = true;

  // Set the parent widget and preserve the window flags
  this->setParent(parent, this->windowFlags());

  // Reshow the MessagesManager window if it was previously visible
  //if (reshow == true)
  //  this->show();
}

/*
 * @brief Open the MessagesManager dialog.
 *
 * This function opens the MessagesManager dialog if it is not already visible.
 * It also starts the timer that updates the elapsed time.
 * After that, it shows the MessagesManager window.
 *
 * @date 31/05/2023
 */
void MessagesManager::Open()
{
  if (isInitialize == false)
  {
    this->Initialize();
  }

  // If flags is true ignore open to keep the window closed
  if (bypass == true)
    return;

  // Check if the MessagesManager window is not already visible
  if (this->isVisible() == false)
  {
    // Update the elapsed time value
    this->UpdateElapsedTime();

    // Create a new QElapsedTimer object and start it
    this->ElapsedSecond = new QElapsedTimer();
    this->ElapsedSecond->start();

    // Start the timer that updates the elapsed time
    this->Timer->start();
  }

  // Show the MessagesManager window
  //this->show();

  // Process events, excluding user input events
  QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
}

/*
 * @brief Close the MessagesManager window.
 *
 * This function stops the timer
 * If the waiting mode is enabled, it disables the waiting mode
 * It then hides the MessagesManager window and set the elapsed time to 0.
 *
 * @date 31/05/2023
 */
void MessagesManager::Close()
{
  // Stop the timer
  this->Timer->stop();

  // Delete the ElapsedSecond object and set it to nullptr
  delete this->ElapsedSecond;
  this->ElapsedSecond = nullptr;

  // Check if the waiting mode is enabled
  if (this->isWaiting == true)
  {
    // Disable the waiting mode
    this->isWaiting = false;

    // Make the text display of the progress bar visible
    this->Progress->setTextVisible(true);
  }

  // Hide the MessagesManager window
  this->hide();

  // Update the elapsed time value
  this->UpdateElapsedTime();

  // Process events, excluding user input events
  QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
}

/*
 * @brief Enable waiting mode in the MessagesManager dialog.
 *
 * This function sets the waiting mode flag to true and hides the text display of the progress bar.
 *
 * @date 31/05/2023
 */
void MessagesManager::EnableWaitingMode()
{
  // Set the waiting mode flag to true
  this->isWaiting = true;

  // Hide the text display of the progress bar
  this->Progress->setTextVisible(false);
}

/*
 * @brief Set the scroll range of the progress bar in the MessagesManager dialog.
 *
 * This function sets the range of the progress bar to the specified minimum and maximum values.
 * It also updates the elapsed time value
 *
 * @param min The minimum value of the progress bar range.
 * @param max The maximum value of the progress bar range.
 * @param force Flag indicating whether to force the refresh of the elapsed time value.
 *
 * @date 31/05/2023
 */
void MessagesManager::SetScrollRange(int min, int max, bool force)
{
  // Set the range of the progress bar
  this->Progress->setRange(min, max);

  // Update the elapsed time value (if force is true, force the refresh)
  this->UpdateElapsedTime(force);
}

/*
 * @brief Set the scroll position of the progress bar in the MessagesManager dialog.
 *
 * This function sets the value of the progress bar to the specified currentPosition.
 *
 * The currentPosition parameter can be one of the following values:
 * - default : use custom value to set the progress bar at the given value
 * - Auto: Increase the current value of the progress bar by 1.
 * - Max: Set the new value to the maximum value of the progress bar.
 *
 * @param currentPosition The new scroll position to set.
 * @param force Flag indicating whether to force the refresh of the elapsed time value.
 *
 * @date 31/05/2023
 */
void MessagesManager::SetScrollPosition(int currentPosition, bool force)
{
  int newValue = currentPosition;

  // Update the new value based on the current position
  switch (currentPosition)
  {
  case Auto:
    // Increase the current value of the progress bar by 1
    newValue = this->Progress->value() + 1;
    break;
  case Max:
    // Set the new value to the maximum value of the progress bar
    newValue = this->Progress->maximum();
    break;
  default:
    break;
  }

  // Set the value of the progress bar to the new value
  this->Progress->setValue(newValue);

  // Update the elapsed time value (if force is true, force the refresh)
  this->UpdateElapsedTime(force);
}

/*
 * @brief Set the title of the MessagesManager dialog.
 *
 * @param title The new title to set.
 * @param force Flag indicating whether to force the refresh of the elapsed time value.
 *
 * @date 31/05/2023
 */
void MessagesManager::SetTitle(QString title, bool force)
{
  // Set the text of the Title label to the specified title
  this->Title->setText(title);

  // Update the elapsed time value (if force is true force the refresh)
  this->UpdateElapsedTime(force);
}

/*
 * @brief Set the description of the MessagesManager dialog.
 *
 * @param description The new description to set.
 * @param force Flag indicating whether to force the refresh of the elapsed time value.
 *
 * @date 31/05/2023
 */
void MessagesManager::SetDescription(QString description, bool force)
{
  // Set the text of the Description label to the specified description
  this->Description->setText(description);

  // Update the elapsed time value (if force is true force the refresh)
  this->UpdateElapsedTime(force);
}

/*
 * @brief Send New Log signal
 *
 * @param index The index of the log.
 * @param list The list of log messages.
 *
 * @date 31/05/2023
 */
void MessagesManager::NewLog(int index, QStringList list)
{
  // Check if the index is within the valid range
  if (index >= MAX_LOG_INDEX)
    return;

  // Emit a signal to send the new log information with the given index and list
  emit this->SendNewLog(index, list);
}

/*
 * @brief Send New Info signal
 *
 * @param info The new information to add.
 *
 * @date 31/05/2023
 */
void MessagesManager::NewInfo(QString info)
{
  // Emit a signal to send the new information
  emit this->SendNewInfo(info);
}

/*
 * @brief Send New Error signal
 *
 * @param m_State The new m_State to add.
 *
 * @date 31/05/2023
 */
void MessagesManager::NewError(QString m_State)
{
  // Emit a signal to send the new m_State
  emit this->SendNewError(m_State);
}

/*
 * @brief Send New Warning signal

 *
 * @param warning The new warning to add.
 *
 * @date 31/05/2023
 */
void MessagesManager::NewWarning(QString Warning)
{
  // Emit a signal to send the new warning
  emit this->SendNewWarning(Warning);
}

/*
* @brief OpenLog function
*
* This function opens the log file using the default text editor (Notepad).
* It creates the necessary arguments and starts the Notepad process.
*
* @date 14/06/2023
*/
void MessagesManager::OpenLog()
{
  // Relative path from the executable (TMP folder must exist)
  const QString logPath = LOG_FILENAME;
  const QString notepadPath = "notepad.exe";

  // Add paths to the arguments list
  QStringList arguments;
  arguments << logPath;

  // Start Notepad with the log file as an argument
  QProcess::startDetached(notepadPath, arguments);
  QThread::sleep(1);
}

/*
* @brief Log function
*
* This function logs the provided message to a log file.
* It opens or creates the log file, writes the log message with the current date and time,
* and closes the file.
*
* @param function The name of the function or module where the log message is generated.
* @param log The log message to be written to the log file.
*
* @date 14/06/2023
*/
void MessagesManager::Log(QString function, QString log)
{
  QString date = "[" + QDateTime::currentDateTime().toString(LOG_DATE_FORMAT) + "]";
  QString functionSpaced = function.leftJustified(LOG_FUNCTION_SIZE, '.');

  // Create or open the log file
  QFile file(LOG_FILENAME);
  if (file.open(QIODevice::Append | QIODevice::Text) == false)
    return;

  // Create a text stream for writing to the file
  QTextStream stream(&file);

  // Write the log message with the current date and time to the log file
  stream << date << " " << functionSpaced << log << endl;

  // Close the file
  file.close();
}
