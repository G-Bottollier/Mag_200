#include "stdafx.h"
#include "PageView.h"

/*
 * @brief Constructor of the PageView
 *
 * @param parent The parent widget (if any).
 * @param IsMainView A flag indicating whether the PageView is the main view.
 *
 * @return void
 *
 * @date 31/05/2023
 */
PageView::PageView(QWidget * parent) : QWidget(parent)
{
  // Setup UI
  SetupUI();

  // Setup event handlers
  SetupEvent();
}

/*
 * @brief Destructor of the PageView
 *
 * @return void
 *
 * @date 31/05/2023
 */
PageView::~PageView()
{
}

/*
 * @brief Handles the paint event of the widget.
 *
 * It draws a line from the top-left corner of the widget to a calculated destination point.
 * Using the specified color and line width obtained from the theme.
 *
 * @param e The paint event.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void PageView::paintEvent(QPaintEvent *e)
{
  QWidget::paintEvent(e);

  if (this->m_UseLine == false)
    return;

  // Get the top left point
  QRect rect = this->rect();
  QPoint pos = rect.topLeft();

  // Get the QPainter
  QPainter p(this);

  // Get the pen width and color from the theme
  QColor color = qThemes->Color(QPalette::Text);
  QPen pen(QBrush(color), 10);
  p.setPen(pen);

  // Calculates the destination point for drawing the line
  QPoint dst = QPoint(rect.width(), pos.y());

  // Draws a line from the top-left position to the destination point
  p.drawLine(QLine(pos, dst));
}

void PageView::resizeEvent(QResizeEvent * event)
{
  int width = Tools::GetWidgetWidth(this, event->size());
  int height = Tools::GetWidgetHeight(this, event->size());

  QSize size = event->size();
  this->setFixedSize(QSize(width, height));
  
  if (HeaderWidget != nullptr)
  {
    qreal ratioW = 1.0;
    qreal ratioH = 1.0;

    int pageWidth = Tools::GetWidgetWidth(this, size, HeaderLayout);
    int pageHeight = Tools::GetWidgetHeight(this, size, HeaderLayout);

    int availableWidth = pageWidth;
    int availableHeight = pageHeight;

    int headerHeight = Tools::GetWidgetHeight(0.25, pageHeight, availableHeight, ratioH);
    HeaderWidget->setFixedWidth(pageWidth);
    HeaderWidget->setFixedHeight(headerHeight);

    qreal fontsize = headerHeight * 0.45;

    QFont font = this->font();
    font.setPixelSize(fontsize);
    font.setBold(true);
    this->setFont(font);

    if (LabelTitle != nullptr)
    {
      //int labelWidth = Tools::GetWidgetWidth(0.2, pageWidth, availableWidth, ratioW);
      LabelTitle->setFont(font);
    }

    if (CloseButton != nullptr)
    {
      int closeButttonHeight = 0.80 * headerHeight;

      qreal ratioWidth = closeButttonHeight / (double)pageWidth;
      int buttonWidth = Tools::GetWidgetWidth(ratioWidth, pageWidth, availableWidth, ratioW);

      CloseButton->UseAutoSizeFont(true);
      CloseButton->SetSize(QSize(buttonWidth, buttonWidth));
    }
  }

  // Resize custom child widget using field & title labels only
  // Subclass and use custom resize event for a different behavior
  if(ScrollArea != nullptr)
  {
    int hasHeader = HeaderWidget != nullptr && IsHeaderVisible();
    int areaHeight = height - (hasHeader * this->HeaderSize().height());

    ScrollArea->setFixedSize(width, areaHeight);
    
    if (ScrollWidget == nullptr)
      return;

    int scrollWidth = Tools::GetWidgetWidth(ScrollWidget, size);
    int scrollHeight = Tools::GetWidgetHeight(ScrollWidget, size);
    
    if (FieldLayout == nullptr)
      return;

    int labelWidth = scrollWidth * 0.5;
    int labelHeight = scrollHeight * 0.2;

    QFont font = this->font();
    font.setPixelSize(labelHeight * 0.45);
    font.setBold(true);

    ScrollWidget->setFont(font);
    ScrollWidget->setFixedHeight(labelHeight * Fields.size());

    for (int i = 0; i < FieldLayout->count(); i++)
    {
      QLayoutItem *item = FieldLayout->itemAt(i);
      if (item == nullptr)
        continue;

      QLabel *label = dynamic_cast<QLabel*>(item->widget());
      if (label == nullptr)
        continue;

      int realWidth = labelWidth;
      if (i % 2 == 0)
        realWidth -= 2;

      label->setFixedSize(QSize(realWidth, labelHeight));
      label->setFont(font);
    }
  }
}

/*
 * @brief The SetupUI function sets up the user interface of the PageView.
 *
 * It creates layouts for the main content and the header section.
 * If the PageView is not the main view, it sets margins, creates a QLabel for displaying the ID/name,
 * creates a closeButton as a Led widget, and adds these widgets to the header layout.
 * Finally, it sets the main layout for the current widget.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void PageView::SetupUI()
{
  // Create a vertical layout for the main content
  MainLayout = new QVBoxLayout();
  MainLayout->setMargin(0);
  MainLayout->setSpacing(0);

  // AddMainWidget !!!

  // Set the main layout for the current widget
  this->setLayout(MainLayout);
}

/*
 * @brief The SetupEvent function sets up event connections for the PageView.
 *
 * It checks if the closeButton exists and connects its clicked signal to the CloseClicked slot.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void PageView::SetupEvent()
{
}

int PageView::LedIndex()
{
  Led* led = static_cast<Led*>(QObject::sender());
  if (led == nullptr)
    return -1;

  QList<Led*> leds = Leds.values();
  if (leds.contains(led) == false)
    return -1;

  return Leds.key(led, -1);
}

void PageView::AddFieldView(QStringList Names)
{
  // Create a grid layout for the form
  FieldLayout = new QGridLayout();
  FieldLayout->setMargin(0);
  FieldLayout->setSpacing(0);

  // Define the default and alternate background colors based on the theme
  QString defaultColor = "background:" + qThemes->Color(QPalette::Base).name();
  QString alternateColor = "background:" + qThemes->Color(QPalette::AlternateBase).name();

  // Iterate over the data items and create labels and fields for each item
  for (int i = 0; i < Names.size(); i++)
  {
    // Set the margins and background color based on the item's index
    QMargins margin(20, 10, 20, 10);
    QString strColor = i % 2 == 0 ? defaultColor : alternateColor;

    // Create a label to display the data item name
    QLabel* name = new QLabel(Names[i]);
    name->setContentsMargins(margin);
    name->setStyleSheet(strColor);

    // Create a field to display the data item value
    QLabel* field = new QLabel(tr("NO_DATA"));
    //field->setFixedWidth(SCREEN_WIDTH * (2 / 5.));
    field->setContentsMargins(margin);
    field->setStyleSheet(strColor);

    Fields.append(field);

    // Add the label and field to the form layout
    FieldLayout->addWidget(name, i, 0);
    FieldLayout->addWidget(field, i, 1);
  }

  QSpacerItem *item = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
  FieldLayout->addItem(item, FieldLayout->count(), 0);

  // Add the form layout to the scroll view
  this->AddScrollView(FieldLayout);
}

void PageView::UpdateField(int index, QString value)
{
  Fields[index]->setText(value);
}

void PageView::UpdateFieldView(Mag200Data * data, QStringList Methods)
{
  const QMetaObject* obj = data->metaObject();

  for (int i = 0; i < Methods.size(); i++)
  {
    QString method = Methods[i];
    QString methodSignature = method + "()";

    // Get the Mag200Method
    int index = obj->indexOfMethod(methodSignature.toLatin1().constData());
    QMetaMethod metaMethod = obj->method(index);

    // Get the return value of the method
    int returnType = metaMethod.returnType();

    // Invoke the method and get the return value
    QVariant returnValue = InvokeMethod(returnType, data, method.toLatin1().constData());
    if (returnValue.isValid() == false)
      continue;

    Fields[i]->setText(returnValue.toString());
  }
}

void PageView::UseLine(bool UseLine)
{
  this->m_UseLine = UseLine;
}

/*
 * @brief Deletes the main widgets from the MainLayout.
 *
 * Iterates over the items in the MainLayout in reverse order,
 * For each item delete the associated widget.
 * Excluding the HeaderLayout.
 * Then updates the MainLayout to reflect the changes.
 *
 * @return void
 *
 * @date 31/05/2023
 */
   void PageView::UseFullPage(bool UseFullPage)
 {
   this->m_UseFullPage = UseFullPage;
 }
 void PageView::DeleteMainWidget()
{
  // Get the number of items in the MainLayout
  int nb = MainLayout->count();

  // Iterate over the items in reverse order
  for (int i = nb; i >= 0; i--)
  {
    // Skip the HeaderLayout item
    if (MainLayout->itemAt(i) == HeaderLayout)
      continue;

    // Get the layout item at index i
    QLayoutItem* item = MainLayout->itemAt(i);

    // Check if the item and its associated widget exist
    if (item && item->widget())
    {
      // Remove the item from the MainLayout
      MainLayout->removeItem(item);

      // Delete the widget associated with the item later (to ensure it's safe to delete)
      item->widget()->deleteLater();

      // Set the item pointer to nullptr for safety
      item = nullptr;
    }
  }

  // Update the MainLayout
  MainLayout->update();
}

void PageView::SetScrollAreaSize(QSize size)
{
  if (ScrollArea == nullptr)
    return;
  ScrollArea->setFixedSize(size);
}

void PageView::SetScrollWidgetSize(QSize size)
{
  if (ScrollWidget == nullptr)
    return;
  ScrollWidget->setFixedSize(size);
}

void  PageView::AddHeaderView()
{
  if (HeaderWidget != nullptr)
    return;

  HeaderWidget = new QWidget();

  // Set margins for the main layout
  QMargins margin(0, 0, 0, 0);
  MainLayout->setContentsMargins(margin);

  // Create a horizontal layout for the header section
  HeaderLayout = new QHBoxLayout();
  HeaderLayout->setMargin(0);
  HeaderLayout->setSpacing(5);

  // Create a closeButton as a Led widget
  CloseButton = new Led(tr("X"));
  CloseButton->UseHasButton(true);
  CloseButton->SetState(Mag200Status::BLUE);

  HeaderLayout->addWidget(CloseButton);

  // Add the header layout to the main layout
  HeaderWidget->setLayout(HeaderLayout);
  MainLayout->insertWidget(0, HeaderWidget);

  QObject::connect(CloseButton, &QPushButton::clicked, this, &PageView::CloseClicked);
}

void  PageView::AddHeaderTitle()
{
  if (HeaderLayout == nullptr)
    return;

  if (LabelTitle != nullptr)
    return;

  // Create a QLabel for displaying the ID/name
  LabelTitle = new QLabel();
  LabelTitle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

  this->AddHeaderWidget(LabelTitle);
}

/*
 * @brief The AddScrollView function adds a scrollable area to the PageView.
 *
 * It creates a QScrollArea widget, customizes its scrolling behavior, creates a container widget
 * to hold the provided layout, sets the provided layout as the layout for the container widget,
 * and adds the container widget to the scroll area. Finally, it adds the scroll area to the main widget.
 *
 * @param layout The layout to be added to the scrollable area.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void PageView::AddScrollView(QLayout* layout)
{
  // Check if the layout is null
  if (layout == nullptr)
    return;

  // Create a QScrollArea widget
  ScrollArea = new QScrollArea();
  ScrollArea->setObjectName("Scroll");
  ScrollArea->setWidgetResizable(true);
  ScrollArea->horizontalScrollBar()->setEnabled(false);
  ScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ScrollArea->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

  QColor color = qThemes->Color(QPalette::Text);

  QPalette palette = ScrollArea->palette();
  palette.setColor(QPalette::AlternateBase, color);
  ScrollArea->setPalette(palette);

  if(HeaderLayout == nullptr)
    ScrollArea->setStyleSheet("QScrollArea#Scroll{border-top:4px solid " + color.name() + "}");

  this->AddGrabGesture(ScrollArea);

  // Create a QWidget to hold the layout
  ScrollWidget = new QWidget();
  ScrollWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  // Set the provided layout as the layout for the ScrollWidget
  ScrollWidget->setLayout(layout);

  // Set the ScrollWidget as the widget inside the ScrollArea
  ScrollArea->setWidget(ScrollWidget);

  // Add the ScrollArea to the main widget
  this->AddMainWidget(ScrollArea);
}

QSize PageView::HeaderSize()
{
  if (HeaderWidget == nullptr)
    return QSize(0, 0);

  int pageWidth = Tools::GetWidgetWidth(HeaderWidget, HeaderWidget->size(), HeaderLayout);
  int pageHeight = Tools::GetWidgetHeight(HeaderWidget, HeaderWidget->size(), HeaderLayout);
  return QSize(pageWidth, pageHeight);
}

QSize PageView::CloseButtonSize()
{
  if (CloseButton == nullptr)
    return QSize(0, 0);
  return CloseButton->size();
}

/*
 * @brief Sets the name of the header in the PageView.
 *
 * Updates the text of the label with the provided name.
 *
 * @param name The name to be displayed in the header.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void PageView::SetHeaderName(QString name)
{
  if (LabelTitle == nullptr)
    return;

  LabelTitle->setText(name);
}

bool PageView::IsHeaderVisible()
{
  if (this->HeaderWidget == nullptr)
    return false;

  return HeaderWidget->isVisibleTo(HeaderWidget->parentWidget());
}

void PageView::SetHeaderVisible(bool visible)
{
  if (this->HeaderWidget == nullptr)
    return;

  HeaderWidget->setVisible(visible);
  
  QResizeEvent *event = new QResizeEvent(this->size(), this->size());
  this->resizeEvent(event);
  delete event;
}

/*
 * @brief Adds a stretch to the header layout in the PageView.
 *
 * Inserts a stretch item into the HeaderLayout before the last item in the layout
 * (assumed to be the exit button). The stretch item adjusts the layout's spacing
 * and helps in distributing the widgets within the header.
 *
 * @param stretch The stretch factor to be applied.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void PageView::AddHeaderStretch(int stretch)
{
  if (HeaderLayout == nullptr)
    return;

  HeaderLayout->insertStretch(HeaderLayout->count() - 1, stretch);
}

void PageView::AddMainWidget(QWidget* widget)
{
  // Add the provided widget to the MainLayout
  MainLayout->addWidget(widget);
}

void PageView::AddGrabGesture(QObject * object)
{
  // Enable scrolling with the left mouse button gesture
  QScroller* scroller = QScroller::scroller(object);
  scroller->grabGesture(object, QScroller::LeftMouseButtonGesture);

  // Customize the scrolling behavior
  QScrollerProperties scrollerProperties = scroller->scrollerProperties();
  scrollerProperties.setScrollMetric(QScrollerProperties::OvershootDragDistanceFactor, 0);
  scrollerProperties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0);
  scrollerProperties.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0.1);
  scroller->setScrollerProperties(scrollerProperties);
}

bool PageView::UseFullPage()
{
  return this->m_UseFullPage;
}

/*
 * @brief The AddHeaderWidget function adds a widget to the header layout of the PageView.
 *
 * It inserts the provided widget into the HeaderLayout, before the last item in the layout
 * (assumed to be the exit button). This allows adding custom widgets to the header section of the PageView.
 *
 * @param widget The widget to be added to the header layout.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void PageView::AddHeaderWidget(QWidget* widget)
{
  if (HeaderLayout == nullptr)
    return;

  HeaderLayout->insertWidget(HeaderLayout->count() - 1, widget, 1);
}

void PageView::RemoveHeaderWidget(QWidget* widget)
{
  if (HeaderLayout == nullptr)
    return;

  HeaderLayout->removeWidget(widget);
}

/*
 * @brief Handles the close button clicked event.
 *
 * Emits the SendCloseClicked() signal to notify
 * that the close button has been clicked.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void PageView::CloseClicked()
{
  emit SendCloseClicked();
}

QVariant PageView::InvokeMethod(int returnType, QObject *obj, const char *method)
{
  int _int_;
  bool _bool_;
  double _double_;
  QString _string_;

  QVariant _variant_;
  bool invoked = false;

  switch (returnType)
  {
  default:
    return QVariant();

  case QMetaType::Int:
    invoked = QMetaObject::invokeMethod(obj, method, Qt::DirectConnection, Q_RETURN_ARG(int, _int_));
    _variant_ = QVariant(_int_);
    break;

  case QMetaType::Bool:
    invoked = QMetaObject::invokeMethod(obj, method, Qt::DirectConnection, Q_RETURN_ARG(bool, _bool_));
    _variant_ = QVariant(_bool_);
    break;

  case QMetaType::Double:
    invoked = QMetaObject::invokeMethod(obj, method, Qt::DirectConnection, Q_RETURN_ARG(double, _double_));
    _variant_ = QVariant(_double_);
    break;

  case QMetaType::QString:
    invoked = QMetaObject::invokeMethod(obj, method, Qt::DirectConnection, Q_RETURN_ARG(QString, _string_));
    _variant_ = QVariant(_string_);
    break;
  }

  if (invoked == false)
    return QVariant();
  return _variant_;
}



