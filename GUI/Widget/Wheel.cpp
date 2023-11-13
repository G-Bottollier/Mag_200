#include "stdafx.h"
#include "Wheel.h"

Wheel::Wheel(int m_MinValue, int m_MaxValue, QWidget *parent) : QListWidget(parent)
{
  SetupUI();

  SetupEvent();

  this->m_MinValue = m_MinValue;
  this->m_MinValue = m_MaxValue;

  if(m_MinValue != -1 && m_MaxValue != -1)
    this->SetRange(m_MinValue, m_MaxValue);
}

Wheel::~Wheel()
{

}

void Wheel::SetupUI()
{
  this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);       // Allow to scroll smoothly
  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);           // Hide horizontal scrollbar
  this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);             // Hide vertical scrollbar
  this->setEditTriggers(QAbstractItemView::NoEditTriggers);             // Disable item editor
  this->setContentsMargins(QMargins(0, 0, 0, 0));                       // Remove margins
  this->setFrameShape(QFrame::NoFrame);                                 // No border frame
  this->setUniformItemSizes(true);
  this->setMouseTracking(true);                                         // Allow to redraw
  this->setAutoScroll(false);
  
  // Remove background
  QWidget *view = this->viewport();
  if (view != nullptr)
    view->setAutoFillBackground(false);

  // Add empty item to center real item
  for (int i = 0; i < EMPTY_ITEMS; i++)
    this->addItem("");

  QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);

  QScroller *scroller = QScroller::scroller(this);
  if (scroller == nullptr)
    return;

  QScrollerProperties properties = scroller->scrollerProperties();
  properties.setScrollMetric(QScrollerProperties::ScrollingCurve, QEasingCurve::Linear);
  properties.setScrollMetric(QScrollerProperties::DecelerationFactor, 2);
  properties.setScrollMetric(QScrollerProperties::MaximumVelocity, 1);
  scroller->setScrollerProperties(properties);
}

void Wheel::SetupEvent()
{
  QScroller *scroller = QScroller::scroller(this);
  if (scroller == nullptr)
    return;

  QObject::connect(scroller, &QScroller::stateChanged, this, &Wheel::ScrollerStateChanged);
}

void Wheel::paintEvent(QPaintEvent* event)
{
  if (m_UseItemSelection == false)
  {
    QListWidget::paintEvent(event);         // Draw QListWidget Item
    return;
  }

  QRect HighlightRect(0, 0, this->width(), this->m_ItemHeight);
  QPoint CenterOffset = this->rect().center() - HighlightRect.center();

  QColor Highlight = ItemBackground();
  QRect CenteredRect = HighlightRect.translated(CenterOffset);

  // DRAWING SELECTION
  QPainter painter(this->viewport());     // Create the painter
  painter.setClipRect(CenteredRect);      // Set the region to limit
  painter.setClipping(true);              // Limit the drawing region
  painter.setPen(Qt::NoPen);              // No border
  painter.setBrush(Highlight);            // Set the fill color
  painter.drawRect(CenteredRect);         // Draw the selection rect

  QListWidget::paintEvent(event);         // Draw QListWidget Item
}

void Wheel::showEvent(QShowEvent *)
{
  m_IsVisible = false;

  // Move to first or last item if out of bounds
  ScrollerStateChanged(QScroller::Inactive);

  QWidget *view = this->viewport();
  if (view == nullptr)
    return;

  QPoint position(0, view->rect().center().y());
  QListWidgetItem *item = this->itemAt(position);
  if (item == nullptr)
    return;

  if (this->Item() == nullptr)
    return;

  bool previous = m_UseAnimation;
  m_UseAnimation = false;

  this->ItemScrollAnimation(this->Item());

  m_UseAnimation = previous;
}

void Wheel::wheelEvent(QWheelEvent * event)
{
  QListWidgetItem *item = this->itemAt(this->rect().center());
  if (item == nullptr)
    return;

  bool inverted = event->angleDelta().y() < 0;
  qreal span = inverted ? 1 * this->m_ItemHeight : -1 * this->m_ItemHeight;

  this->ItemScrollAnimation(item, span);
}

void Wheel::resizeEvent(QResizeEvent *event)
{
  QListWidget::resizeEvent(event);

  QFontMetrics metrics(font());
  int newFontHeight = metrics.height();

  if (m_ItemHeight != newFontHeight)
  {
    m_ItemHeight = newFontHeight;
    emit SendNewItemHeight(m_ItemHeight);
  }

  updateSnapPositions();

  if (this->Item() == nullptr)
    return;

  m_IsVisible = false;
  bool previous = m_UseAnimation;
  m_UseAnimation = false;

  this->ItemScrollAnimation(this->Item());

  m_UseAnimation = previous;
  m_IsVisible = true;
}

void Wheel::mouseMoveEvent(QMouseEvent *)
{
  // Override the QListWidget::mouseMoveEvent() method to do nothing
  // to prevent any further processing from Qt
  return;
}

void  Wheel::mousePressEvent(QMouseEvent *event)
{
  if (event->button() != Qt::LeftButton)
    return;

  event->accept();
  m_PressedPosition = event->pos();
  return;
}

void Wheel::mouseReleaseEvent(QMouseEvent * event)
{
  if (event->button() != Qt::LeftButton)
    return;

  event->accept();
  this->ItemScrollAnimation(this->itemAt(m_PressedPosition));
}

void Wheel::ItemScrollAnimation(QListWidgetItem *item, qreal span)
{
  if (m_IsVisible == false)
    this->scrollToItem(item, ScrollHint::PositionAtCenter);

  if (m_UseAnimation == true)
  {
    qreal center = this->m_ItemHeight * 0.50;
    qreal height = this->row(item) * this->m_ItemHeight;

    int viewportCenter = this->viewport()->height() / 2;
    qreal position = height - (viewportCenter - center) + span;

    qreal currentPosition = this->verticalScrollBar()->value();

    int pixel = qMax(currentPosition, position) - qMin(currentPosition, position);
    qreal MillisecondsPerPixel = (int)m_IsVisible * 1;

    pixel = qMin(pixel, MAX_SCROLL_TIME);

    QScroller::scroller(this)->scrollTo(QPointF(0, position), pixel * MillisecondsPerPixel);
  }
  else
    this->scrollToItem(item, ScrollHint::PositionAtCenter);

  NewValue(item->text());
}

void Wheel::NewValue(QString value)
{
  if (value.isEmpty() == true)
    return;

  if (value == m_OldValue)
    return;

  emit SendNewValue(value);
  m_OldValue = value;
}

QListWidgetItem * Wheel::Item()
{
  int span = EMPTY_ITEMS / 2;
  QListWidgetItem *item = nullptr;

  for (int i = 0; i < this->m_Items.count(); i++)
  {
    if (this->item(span + i)->text() == m_OldValue)
    {
      item = this->item(span + i);
      break;
    }
  }

  return item;
}

void Wheel::updateItemCount()
{
  int span = EMPTY_ITEMS / 2;
  int itemCount = this->m_Items.count();
  int currentCount = this->count() - EMPTY_ITEMS;

  for (int i = currentCount; i < itemCount; i++)
    this->insertItem(span + i, this->m_Items[i]);

  currentCount = this->count() - EMPTY_ITEMS;

  for (int i = currentCount - 1; i >= itemCount; i--)
    delete this->takeItem(span + i);

  QString NewValue = this->Value();
}

void Wheel::updateItemText()
{
  int span = EMPTY_ITEMS / 2;
  for (int i = 0; i < this->m_Items.size(); i++)
  {
    QListWidgetItem *item = this->item(span + i);
    item->setTextAlignment(Qt::AlignCenter);
    item->setFlags(Qt::ItemIsEnabled);
    item->setText(this->m_Items[i]);
  }
}

void Wheel::updateSnapPositions()
{
  QList<qreal> positions;
  int span = EMPTY_ITEMS / 2;

  // Get the viewport height.
  int viewportHeight = this->viewport()->height();

  // Calculate the offset based on viewport height and item height.
  int offset = (viewportHeight - this->m_ItemHeight) / 2;

  for (int i = span; i < span + this->m_Items.size(); i++)
    positions.append((i * this->m_ItemHeight) - offset);

  if (positions.size() == 0)
    return;

  QScroller::scroller(this)->setSnapPositionsY(positions);
}

void Wheel::updateItemList(QStringList items)
{
  QStringList uniqueList;
  QSet<QString> uniqueSet;

  // Iterate over the original list
  for(QString item : items) 
  {
    // Check if the item is already in the uniqueSet
    if (uniqueSet.contains(item) == false) 
    {
      uniqueSet.insert(item);
      uniqueList.append(item);
    }
  }

  this->m_Items = uniqueList;
}

void Wheel::SetRange(int m_MinValue, int m_MaxValue, bool NoSetValue)
{
  QString oldValue = this->Value();

  QStringList list;

  // Checking if set range is needed
  if (this->m_Items.size() != 0)
    if (this->m_MinValue == m_MinValue)
      if (this->m_MaxValue == m_MaxValue)
        return;

  if (m_MaxValue < m_MinValue)
    return;

  int numberOfDigits = 0;
  if (m_UseLeadingZeros == true)
    numberOfDigits = 2;

  for (int i = m_MinValue; i <= m_MaxValue; i++)
    list.append(QString("%1").arg(i, numberOfDigits, 10, QChar('0')));

  this->m_MinValue = m_MinValue;
  this->m_MaxValue = m_MaxValue;

  this->SetItems(list);

  if (NoSetValue == true)
    return;

  if (oldValue.isEmpty() == false)
  {
    this->m_UseAnimation = false;

    QString value = oldValue;
    QString newValue = this->Value();
    
    if (oldValue.toInt() < m_MinValue)
      value = QString("%1").arg(m_MinValue, numberOfDigits, 10, QChar('0'));
    if (oldValue.toInt() > m_MaxValue)
      value = QString("%1").arg(m_MaxValue, numberOfDigits, 10, QChar('0'));

    this->SetValue(value);

    this->m_UseAnimation = true;
  }
  else
    ScrollerStateChanged(QScroller::Inactive);
}

QStringList Wheel::Items()
{
  return this->m_Items;
}

void Wheel::SetItems(QStringList items)
{
  // Removes duplicates
  updateItemList(items);

  // Update list items count
  updateItemCount();

  // Update item text
  updateItemText();

  // Update snap to item while scrolling
  updateSnapPositions();
}

void Wheel::ScrollerStateChanged(QScroller::State newState)
{
  if (newState != QScroller::State::Inactive)
  {
    m_OldScrollState = newState;
    return;
  }

  // Get the vertical scroll bar of the QListWidget
  QScrollBar *scrollBar = this->verticalScrollBar();

  // Check if the scroll bar is at the top or bottom
  if (scrollBar == nullptr)
    return;

  QWidget *view = this->viewport();
  if (view == nullptr)
    return;

  QPoint position(0, view->rect().center().y());
  QListWidgetItem *item = this->itemAt(position);

  if (this->Value().isEmpty() == true)
  {
    int indexFirst = EMPTY_ITEMS / 2;
    int indexLast = this->m_Items.size() + indexFirst - 1;

    int minValue = indexFirst;
    int value = this->row(item);
    
    if (value == -1)
    {
      minValue = this->m_MinValue;
      value = m_OldValue.toInt();
    }

    if(value < minValue)
      item = this->item(indexFirst);
    else
      item = this->item(indexLast);

    if (item == nullptr)
      return;

    ItemScrollAnimation(item);
  }

  if (item == nullptr)
    return;

  if (m_OldScrollState == QScroller::State::Scrolling)
    NewValue(item->text());

  m_OldScrollState = newState;
}

/*********************/
/** GETTER / SETTER **/
/*********************/

void Wheel::SetValue(QString value)
{
  int span = EMPTY_ITEMS / 2;
  QListWidgetItem *item = nullptr;

  for (int i = 0; i < this->m_Items.count(); i++)
  {
    if (this->item(span + i)->text() == value)
    {
      item = this->item(span + i);
      break;
    }
  }

  if (item == nullptr)
    return;

  this->ItemScrollAnimation(item);
}

QString Wheel::Value()
{
  QWidget *view = this->viewport();

  QPoint position(0, view->rect().center().y());
  QListWidgetItem *item = this->itemAt(position);
  if (item == nullptr)
    return QString();

  return item->text();
}

void Wheel::UseAnimation(bool m_UseAnimation)
{
  this->m_UseAnimation = m_UseAnimation;
}

int Wheel::ItemHeight()
{
  return this->m_ItemHeight;
}

QColor Wheel::ItemBackground()
{
  QColor color = qThemes->Color(QPalette::Highlight);
  color.setAlpha(150);
  return color;
}