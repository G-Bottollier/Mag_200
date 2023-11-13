#include "stdafx.h"
#include "SettingsView.h"

/*
* @brief Constructor of the Chapter10View
*
* @param parent The parent widget (if any).
*
* @return void
*
* @date 31/05/2023
*/
SettingsView::SettingsView(QWidget * parent) : PageView(parent)
{
  // Setup UI 
  SetupUI();

  // Setup event handlers
  SetupEvent();
}

/*
* @brief Destructor of the Chapter10View
*
* @return void
*
* @date 31/05/2023
*/
SettingsView::~SettingsView()
{
}

/*
* @brief Sets up the user interface for Chapter10View.
*
* Sets the header name to "PAGE_CH10".
*
* @return void
*
* @date 31/05/2023
*/
void SettingsView::SetupUI()
{
  this->UseLine(true);
  this->AddHeaderView();
  this->AddHeaderTitle();
  this->SetHeaderName(tr("PAGE_SETTINGS"));

  ViewLayout = new QFormLayout();
  
  // Create THEME button
  m_themes = new QPushButton();
  m_themes->setText(qSettings->Settings_Theme());

  // Create and fill SCREENS Data
  QStringList screens = qSettings->Settings_Screens();
  for (int i = 0; i < screens.size(); i++)
  {
    QLineEdit *screen = new QLineEdit();
    screen->setText(screens[i]);
    screen->setReadOnly(true);

    m_screens.append(screen);
  }

  // Create LANGUAGE button
  m_languages = new QPushButton();
  m_languages->setText(qSettings->Settings_Language());

  QString mag200Type = qSettings->Mag200_Type();
  if (mag200Type.contains("&") == true)
    mag200Type.replace("&", "&&");

  m_Mag200Type = new QPushButton();
  m_Mag200Type->setText(mag200Type);

  m_KillButton = new QPushButton();
  m_KillButton->setText("Kill Application");

  // Add widget to the main layout
  ViewLayout->addRow(SETTINGS_THEME, m_themes);
  for (int i = 0; i < m_screens.size(); i++)
    ViewLayout->addRow("SCREEN_" + QString::number(i), m_screens[i]);
  ViewLayout->addRow(SETTINGS_LANGUAGE, m_languages);
  ViewLayout->addRow(SETTINGS_TYPE, m_Mag200Type);
  ViewLayout->addRow("KILL", m_KillButton);

  // Add the form layout to the scroll view
  this->AddScrollView(ViewLayout);
}

/*
* @brief Sets up event connections for Chapter10View.
*
* @return void
*
* @date 31/05/2023
*/
void SettingsView::SetupEvent()
{
  QObject::connect(m_KillButton, &QPushButton::clicked, qApp, &QApplication::quit);
  QObject::connect(m_themes, &QPushButton::clicked, this, &SettingsView::ThemesButtonClicked);
  QObject::connect(m_languages, &QPushButton::clicked, this, &SettingsView::LanguagesButtonClicked);
  QObject::connect(m_Mag200Type, &QPushButton::clicked, this, &SettingsView::Mag200TypeButtonClicked);
}

void SettingsView::resizeEvent(QResizeEvent * event)
{
  PageView::resizeEvent(event);

  QSize size = event->size();

  QSize HeaderSize = this->HeaderSize();

  int margin = ViewLayout->margin() * 2;
  int spacing = ViewLayout->spacing();

  int width = qMax(0, Tools::GetWidgetWidth(this, size) - margin - spacing - 5);
  int height = qMax(0, size.height() - HeaderSize.height());

  int itemPerView = 5;
  int itemHeight = height / itemPerView;

  int fontsize = itemHeight;

  QFont font = this->font();
  font.setPixelSize(itemHeight);
  font.setFamily("dubai");
  font.setBold(true);

  for (;;)
  {
    QFontMetrics metrics(font);
    if (metrics.height() < itemHeight)
      break;

    fontsize *= 0.9;
    font.setPixelSize(fontsize);
  }

  for (int i = 0; i < ViewLayout->count(); i++)
  {
    QLayoutItem *item = ViewLayout->itemAt(i);
    if (item == nullptr)
      continue;

    QWidget *widget = item->widget();
    if (widget == nullptr)
      continue;

    widget->setFont(font);
    widget->setFixedWidth(width / 2);
  }
  
}

void SettingsView::CloseClicked()
{
  bool changed = false;

  QString theme = m_themes->text();
  if (theme != qSettings->Settings_Theme())
    changed = true;

  QString language = m_languages->text();
  if (language != qSettings->Settings_Language())
    changed = true;

  QString mag200Type = m_Mag200Type->text();
  if (mag200Type.contains("&&") == true)
    mag200Type.replace("&&", "&");

  if (mag200Type != qSettings->Mag200_Type())
    changed = true;

  if (changed == false)
  {
    PageView::CloseClicked();
    return;
  }

  qSettings->SetSettings(theme, language, mag200Type);

  qSettings->ReloadSettings();

  PageView::CloseClicked();
}

void SettingsView::ThemesButtonClicked()
{
  QStringList themes = qThemes->Themes();
  int index = themes.indexOf(m_themes->text());

  OptionDialog dialog(this);
  dialog.SetItems(qThemes->Themes());
  dialog.SetSelectedItem(index);

  if (dialog.exec() != QDialog::Accepted)
    return;

  QString option = dialog.SelectedItem();
  if (option.isEmpty() == true)
    return;

  if (option == m_themes->text())
    return;

  m_themes->setText(option);
}

void SettingsView::LanguagesButtonClicked()
{
  QStringList languages = qSettings->Languages();
  int index = languages.indexOf(m_languages->text());

  OptionDialog dialog(this);
  dialog.SetItems(languages);
  dialog.SetSelectedItem(index);

  if (dialog.exec() != QDialog::Accepted)
    return;

  QString option = dialog.SelectedItem();
  if (option.isEmpty() == true)
    return;

  if (option == m_languages->text())
    return;

  m_languages->setText(option);
  return;
}

void SettingsView::Mag200TypeButtonClicked()
{
  QStringList Mag200_Types = qSettings->Mag200_Types();
  QString Mag200_Type = m_Mag200Type->text();
  if (Mag200_Type.contains("&&") == true)
    Mag200_Type.replace("&&", "&");

  int index = Mag200_Types.indexOf(Mag200_Type);

  OptionDialog dialog(this);
  dialog.SetItems(Mag200_Types);
  dialog.SetSelectedItem(index);

  if (dialog.exec() != QDialog::Accepted)
    return;

  QString option = dialog.SelectedItem();
  if (option.isEmpty() == true)
    return;

  if (option == m_Mag200Type->text())
    return;

  if (option.contains("&") == true)
    option.replace("&", "&&");

  m_Mag200Type->setText(option);


  return;
}

/****************************/
/**      CUSTOM DIALOG     **/
/****************************/
/*
* @brief Constructor for MessagesManager.
*
* @param parent The parent QWidget of the MessagesManager dialog.
*
* @date 31/05/2023
*/
OptionDialog::OptionDialog(QWidget *parent) : QDialog(parent)
{
  SetupUI();
  SetupEvent();
}

/*
* @brief Handles the paint event for the OptionDialog dialog.
*
* It adjusts the dialog rectangle based on the border size, and draws a rounded
* rectangle with the adjusted coordinates and border radius.
*
* @param event The paint event object.
*
* @date 31/05/2023
*/
void OptionDialog::paintEvent(QPaintEvent*)
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

void OptionDialog::resizeEvent(QResizeEvent * event)
{
  QWidget::resizeEvent(event);

  UpdateSize();

  QSize size = this->size();

  int height = size.height() - borderSize * 2;

  if (this->layout() == nullptr)
    return;

  int count = this->layout()->count();
  double maxCount = qMax(1, count);

  qreal fontsize = height / maxCount;
  QFont font = this->font();
  font.setBold(true);
  font.setFamily("Dubai");
  font.setPixelSize(fontsize * 0.30);
  this->setFont(font);

  font.setPixelSize(fontsize * 0.20);
  listWidget->setFont(font);
  okButton->setFont(font);
  cancelButton->setFont(font);
}

/*
* @brief Set up the user interface for the OptionDialog dialog.
*
* @date 31/05/2023
*/
void OptionDialog::SetupUI()
{
  this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Dialog | Qt::FramelessWindowHint);

  QVBoxLayout *MainLayout = new QVBoxLayout();
  MainLayout->setMargin(35);

  // Create the instruction label
  QLabel *description = new QLabel(tr("DIALOG_OPTION_INSTRUCTION"));

  // Create the list widget and add options
  listWidget = new QListWidget();

  // Build buttons layout
  QHBoxLayout *buttonLayout = new QHBoxLayout;

  // Create buttons
  okButton = new QPushButton(tr("DIALOG_OPTION_OK"));
  cancelButton = new QPushButton(tr("DIALOG_OPTION_CANCEL"));

  // Set up the buttons layout
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);

  // Set up the MainLayout
  MainLayout->addWidget(description);
  MainLayout->addWidget(listWidget);
  MainLayout->addLayout(buttonLayout);
  setLayout(MainLayout);
}

/*
* @brief Set up event handling for the OptionDialog dialog.
*
* @date 31/05/2023
*/
void OptionDialog::SetupEvent()
{
  QObject::connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
  QObject::connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void OptionDialog::UpdateSize()
{
  QSize size = qScreens->CurrentScreen()->size();

  double min = qMin(size.width(), size.height());
  double max = qMax(size.width(), size.height());

  // ensure 0
  max = qMax(1.0, max);
  qreal minRatio = 0.5;
  qreal ratio = min / max;

  ratio = ratio < minRatio ? minRatio : ratio;

  int width = size.width() * ratio;
  int height = size.height() * 0.8;

  this->setFixedSize(QSize(width, height));
}

/*
* @brief Set items to the list
*
* @date 31/05/2023
*/
void OptionDialog::SetItems(QStringList items)
{
  listWidget->clear();
  listWidget->addItems(items);
}

/*
* @brief Set the current selected item
*
* @date 31/05/2023
*/
void OptionDialog::SetSelectedItem(int index)
{
  listWidget->setCurrentRow(index);
}

/*
* @brief Return the current selected item
*
* @date 31/05/2023
*/
QString OptionDialog::SelectedItem()
{
  if (listWidget->currentItem() == false)
    return QString();
  return listWidget->currentItem()->text();
}