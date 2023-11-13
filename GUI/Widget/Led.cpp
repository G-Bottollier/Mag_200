#include "stdafx.h"
#include "Led.h"

/*
 * @brief Constructor for the Led class.
 *
 * @param text The text displayed on the LED button.
 * @param size The size of the LED button.
 * @param parent The parent widget.
 *
 * @return void
 *
 * @date 31/05/2023
 */
Led::Led(const QString& text, int size, QWidget *parent) : QPushButton(text, parent)
{
  // Initialize the frame timer
  frame = new QTimer(this);

  // Set the custom property "name" to the given text
  this->setProperty("name", text);

  // Set the button text and size policies
  setText(text);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  // Set the initial state and size of the LED
  SetState(Mag200Status::UNLOCK);

  this->useFixedSize = size != QWIDGETSIZE_MAX;
  if (this->useFixedSize == true)
    SetSize(QSize(size, size));

  QObject::connect(this, &QPushButton::toggled, this, &Led::Toggled);
}

/*
 * @brief Destructor for the Led class.
 *
 * @return void
 *
 * @date 31/05/2023
 */
Led::~Led()
{
}

/*
 * @brief Sets whether to use color for the LED.
 *
 * @param useColor Whether to use color for the LED.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void Led::UseColor(bool useColor)
{
  if (this->useFixedStyle == true)
    return;

  this->useColor = useColor;
  update();
}

/*
 * @brief Sets whether to use border for the LED.
 *
 * @param useBorder Whether to use border for the LED.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void Led::UseBorder(bool useBorder)
{
  if (this->useFixedStyle == true)
    return;

  this->useBorder = useBorder;
  update();
}

/*
 * @brief Sets whether to use background for the LED.
 *
 * @param useBackground Whether to use background for the LED.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void Led::UseBackground(bool useBackground)
{
  if (this->useFixedStyle == true)
    return;

  this->useBackground = useBackground;
  update();
}

/*
 * @brief Sets whether to auto-adjust the font size based on available space.
 *
 * @param useAutoSizeFont Whether to auto-adjust the font size.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void Led::UseAutoSizeFont(bool useAutoSizeFont)
{
  if (this->useFixedStyle == true)
    return;

  this->useAutoSizeFont = useAutoSizeFont;
}

void Led::UseHasButton(bool useHasButton)
{
  this->useHasButton = useHasButton;
}

void Led::UseFixedStyle(bool useFixedStyle)
{
  this->useFixedStyle = useFixedStyle;
}

/*
 * @brief Paints the LED button.
 *
 * It sets the style of the button based on the LED properties such as :
 *  - color
 *  - border
 *  - background
 *  - text alignment
 *
 * It also adjusts the font size if useAutoSizeFont is true.
 *
 * @param e The paint event.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void Led::paintEvent(QPaintEvent*)
{
  QPainter painter(this);
  painter.setPen(Qt::NoPen);
  painter.setClipRect(rect());
  painter.setClipping(true);
  painter.setRenderHint(QPainter::Antialiasing);
  
  QColor ledColor = LedColor;
  QRect backgroundRect = rect();

  if (this->useBorder == true)
  {
    int borderSize = qMin(LedSize.width(), LedSize.height()) * 0.07;
    backgroundRect = QRect(rect().adjusted(borderSize, borderSize, -borderSize, -borderSize));

    QPainterPath borderPath;
    borderPath.addRoundedRect(rect(), 12, 12);

    QPainterPath backgroundPath;
    backgroundPath.addRoundedRect(backgroundRect, 8, 8);
    borderPath = borderPath.subtracted(backgroundPath);

    painter.setBrush(QBrush(ledColor));
    painter.drawPath(borderPath);
  }

  if (this->useBackground == true)
  {
    QPainterPath backgroundPath;
    backgroundPath.addRoundedRect(backgroundRect, 8, 8);

    painter.setBrush(QBrush(ledColor));
    painter.drawPath(backgroundPath);
  }

  // Adjust the font size
  if (this->useAutoSizeFont == true)
  {
    QString text = this->text();
    QFont font = this->font();
    font.setBold(true);
    font.setPixelSize(LedSize.height());

    QFontMetrics fontMetrics(font);
    int fontsize = font.pixelSize();
    int MaxValueW = LedSize.width() * 0.80;
    int MaxValueH = LedSize.height() * 0.80;

    QRect maxRect(0, 0, MaxValueW, MaxValueH);
    QRect rect = fontMetrics.boundingRect(LedSize, Qt::AlignCenter, text);
    bool isBigger = qMax(backgroundRect.width(), backgroundRect.height());

    double step = isBigger ? 0.9 : 1.1;
    for (;;)
    {
      font.setPixelSize(fontsize);
      QFontMetrics fontMetrics(font);
      rect = fontMetrics.boundingRect(backgroundRect, Qt::AlignCenter, text);

      if (fontsize <= 1)
        break;

      if (step > 1 && rect.contains(maxRect))
        break;
      if (step < 1 && maxRect.contains(rect))
        break;

      fontsize *= step;
    }

    font.setPixelSize(fontsize);
    setFont(font);

    this->useAutoSizeFont = false;
  }

  // Set the font color
  if (this->useColor == true)
  {
    QColor fontColor = qThemes->Color(QPalette::BrightText);
    if (this->useBackground == false)
      if (qThemes->IsDark() == false)
        fontColor = qThemes->Color(QPalette::Text);

    painter.setPen(fontColor);
  }
  else
    painter.setPen(QColor(Qt::transparent));

  // Draw text
  if (textAlignment != NO_ALIGNEMENT)
  {
    painter.drawText(backgroundRect, textAlignment, this->text());
  }
  else
    painter.drawText(backgroundRect, Qt::AlignCenter, this->text());

  QIcon icon = this->icon();
  if (icon.isNull())
    return;

  // Convert QIcon to QPixmap
  QPixmap pixmap = icon.pixmap(backgroundRect.size()).scaled(backgroundRect.size());

  // Calculate the position where the icon should be drawn.
  int x = backgroundRect.x() + (backgroundRect.width() - pixmap.width()) / 2;
  int y = backgroundRect.y() + (backgroundRect.height() - pixmap.height()) / 2;

  // Draw the icon
  painter.drawPixmap(x, y, pixmap);
}

void Led::changeEvent(QEvent * event)
{
  if (event->type() != QEvent::FontChange)
  {
    QPushButton::changeEvent(event);
    return;
  }

  if (this->layout() == nullptr)
  {
    QPushButton::changeEvent(event);
    return;
  }

  QFont font = this->font();
  int count = this->layout()->count();

  font.setFamily("FORCED_SQUARE_MONO");
  font.setPixelSize(LedSize.height() * (1. / count));

  for (int i = 0; i < count; i++)
  {
    QLayoutItem *item = this->layout()->itemAt(i);
    if (item == nullptr)
      continue;

    QLabel* label = static_cast<QLabel*>(item->widget());
    if (label == nullptr)
      continue;

    label->setFont(font);
  }

  QPushButton::changeEvent(event);
}

void Led::resizeEvent(QResizeEvent* event)
{
  QPushButton::resizeEvent(event);

  if (this->useFixedSize == true)
    return;

  LedSize = QRect(QPoint(0, 0), QSize(this->width(), this->height()));
}

void Led::mousePressEvent(QMouseEvent* event)
{
  QPushButton::mousePressEvent(event);

  if (useHasButton == false)
    return;

  useBackground = false;
  move(this->pos() + QPoint(5, 5));

  update();

  event->accept();
}

void Led::mouseReleaseEvent(QMouseEvent* event)
{
  QPushButton::mouseReleaseEvent(event);

  if (useHasButton == false)
    return;

  if(this->isChecked() == false)
    useBackground = true;

  move(this->pos() + QPoint(-5, -5));

  update();

  event->accept();
}

/*
 * @brief Sets the size of the LED button.
 *
 * @param size The size of the LED button.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void Led::SetSize(QSize size)
{
  LedSize = QRect(QPoint(0, 0), size);

  this->setFixedSize(size);
  this->update();
}

/*
 * @brief Sets the state and color of the LED button.
 *
 * It sets the appropriate color and frame behavior based on the status.
 *
 * @param status The new state of the LED.
 * @param color The color to set for the LED (optional).
 *
 * @return void
 *
 * @date 31/05/2023
 */
void Led::SetState(Mag200Status status, Color color)
{
  this->Status = status;

  // Set the appropriate color and frame behavior based on the status
  switch (status)
  {
    case UNLOCK:
      frame->stop();
      ResetFrame();
      LedColor = (color == Color::NO_COLOR) ? RED_01 : color;
      break;

    case VERIFY:
      frame->stop();
      LedColor = (color == Color::NO_COLOR) ? YELLOW_01 : color;
      break;

    case LOCK:
      frame->start();
      LedColor = (color == Color::NO_COLOR) ? GREEN_02 : color;
      break;

    default:
    case UNDEFINED:
      frame->stop();
      ResetFrame();
      LedColor = (color == Color::NO_COLOR) ? BLACK_01 : color;
      break;

    case WHITE:
      frame->stop();
      ResetFrame();
      LedColor = (color == Color::NO_COLOR) ? WHITE_01 : color;
      break;

    case GRAY:
      frame->stop();
      ResetFrame();
      LedColor = (color == Color::NO_COLOR) ? GRAY_02 : color;
      break;

    case BLUE:
      frame->stop();
      ResetFrame();
      LedColor = (color == Color::NO_COLOR) ? BLUE_06 : color;
      break;
  }
  QString s = this->text();

  if (parentWidget() == nullptr)
    return;

  if (this->parentWidget()->layout() == nullptr)
    return;

  if (status == UNDEFINED)
    this->setVisible(false);
  else
    this->setVisible(true);
}

void Led::ResetAnimation()
{
  ResetFrame();
}

void Led::StartAnimation()
{
  frame->start();
}

void Led::StopAnimation()
{
  frame->stop();
}

/*
 * @brief Returns the current state of the LED button.
 *
 * @return The current state of the LED button.
 *
 * @date 31/05/2023
 */
Mag200Status Led::GetState()
{
  return this->Status;
}

bool Led::IsChecked()
{
  return this->useBorder == true && this->useBackground == false;
}

bool Led::IsClickedConnected()
{
  return isSignalConnected(QMetaMethod::fromSignal(&QPushButton::clicked));
}

/*
 * @brief Sets additional information for the LED button.
 *
 * It sets the "infos" custom property to the given list.
 * Creates a grid layout for displaying the information labels.
 * And adds QLabel objects for each information in the list.
 * Widget are displayd vertically.
 *
 * @param list The list of additional information.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void Led::SetInfos(QStringList list)
{
  // Set the "infos" custom property to the given list
  this->setProperty("infos", list);

  // Create a grid layout for displaying the information labels
  this->setLayout(new QGridLayout);
  this->layout()->setSpacing(0);
  this->layout()->setMargin(0);

  QFont font = this->font();
  font.setBold(true);
  font.setFamily("FORCED_SQUARE_MONO");
  font.setPixelSize(LedSize.height() * (1. / list.size()));

  // Create and add QLabel objects for each information in the list
  for (int i = 0; i < list.size(); i++)
  {
    QLabel* label = new QLabel(list[i]);
    label->setFont(font);
    label->setStyleSheet("border:0px;padding:1px;");
    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    this->layout()->addWidget(label);
  }
}

/*
 * @brief Sets the text alignment for the LED button.
 *
 * @param flag The text alignment flag.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void Led::SetAlignement(QFlags<Qt::AlignmentFlag> flags)
{
  this->textAlignment = flags;
  update();
}

/*
 * @brief Sets the icon for the LED button.
 *
 * It clears the button text and sets the icon size to match the LED size,
 * and calls the SetupAnimation() function to set up the animation frames if required.
 *
 * @param path The path to the icon image.
 * @param isAnimated Whether the icon is animated.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void Led::SetIcon(QString path, bool isAnimated)
{
  // If the path is empty, return
  if (path.isEmpty() == true)
    return;

  // Clear the button text and set the icon size to match the LED size
  this->setText(QString());
  SetupAnimation(path, isAnimated);

  update();
}

void Led::SetIcon(QIcon icon)
{
  this->setIcon(icon);

  // Clear the button text and set the icon size to match the LED size
  this->setText(QString());

  update();
}

/*
 * @brief Sets up the animation frames for the LED icon.
 *
 * It adds the file paths of the animation frames to the "ICON_ALL_FRAME" custom property.
 * 
 * if isAnimated is true :
 *  - connects the frame timer to the NewFrame() slot
 *  - Otherwise, display only the initial frame
 *
 * @param path The path to the first frame of the animation.
 * @param isAnimated Whether the icon is animated.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void Led::SetupAnimation(QString path, bool isAnimated)
{
  QString filepath = path;
  QFileInfo info(path);
  QStringList list;
  int index;

  while (info.exists() == true)
  {
    list.append(filepath);

    QStringList filename = info.baseName().split("_");
    index = filename.last().toInt() + 1;

    QString file = filename.first() + "_" + QString::number(index);
    filepath = info.absolutePath() + "/" + file + "." + info.completeSuffix();
    info.setFile(filepath);

    // If there is only one image in the list, stop adding frames if isAnimated is false
    if (isAnimated == false)
      break;
  }

  // If there are no frames in the list, return
  if (list.size() == 0)
    return;

  // Set the "ICON_ALL_FRAME" custom property to the frame list
  this->setProperty(ICON_ALL_FRAME, list);

  // Set the initial frame
  NewFrame();

  // If the animation is enabled, connect the frame timer to the NewFrame() slot
  if (isAnimated == true)
  {
    frame->disconnect();
    frame->setInterval(1600 / list.size());
    QObject::connect(frame, &QTimer::timeout, this, &Led::NewFrame);
  }
}

/*
 * @brief Displays the next frame in the animation.
 *
 * It gets the current frame and frame list from the custom properties.
 * Selects the next frame and sets the LED icon.
 * Update the property "ICON_CURRENT_FRAME".
 *
 * @return void
 *
 * @date 31/05/2023
 */
void Led::NewFrame()
{
  // Get the current frame and frame list from the custom properties
  QString currentFrame = this->property(ICON_CURRENT_FRAME).toString();
  QStringList list = this->property(ICON_ALL_FRAME).toStringList();

  // If there are no frames in the list, return
  if (list.size() < 1)
    return;

  // Get the index of the current frame and select the next frame
  int index = list.indexOf(currentFrame) + 1;
  index = index >= list.size() ? 0 : index;

  QString newFrame = list.at(index);

  // Set the LED icon to the new frame and update the "ICON_CURRENT_FRAME" property
  this->setIcon(QIcon(newFrame));
  this->setProperty(ICON_CURRENT_FRAME, newFrame);

  update();
}

/*
 * @brief Resets the animation frame to the first frame.
 *
 * It sets the LED icon to the first frame in the list.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void Led::ResetFrame()
{
  QStringList list = this->property(ICON_ALL_FRAME).toStringList();
  if (list.size() <= 1)
    return;

  // Set the LED icon to the first frame in the list
  this->setIcon(QIcon(list.first()));

  update();
}

void Led::Toggled(bool checked)
{
  switch (checked)
  {
    case true:
      useBackground = false;
      useBorder = true;
      frame->start();
      break;

    case false:
      useBackground = true;
      frame->stop();
      break;
  }

  update();
}