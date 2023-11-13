#include "stdafx.h"
#include "ReplayBar.h"

ReplayBar::ReplayBar(QWidget *parent) : QWidget(parent)
{
  m_AnimatedTimer = new QTimer(this);
  m_AnimatedTimer->setInterval(50);
  connect(m_AnimatedTimer, &QTimer::timeout, this, &ReplayBar::Animate);

  qRegisterMetaType<qint64>();
}

void ReplayBar::Animate()
{
  if (m_AnimatedFactor > 1.0)
  {
    m_AnimatedFactor -= 0.1;
    update();
    return;
  }

  this->m_IsTimelinePressed = false;
  m_AnimatedTimer->stop();
  update();
}

ReplayBar::~ReplayBar()
{

}

void ReplayBar::SetDateRange(QDateTime startTime, QDateTime endTime)
{
  this->m_StartTime = startTime.toMSecsSinceEpoch();
  this->m_EndTime = endTime.toMSecsSinceEpoch();

  this->m_LowerBound = m_StartTime;
  this->m_UpperBound = m_EndTime;

  qint64 minSec = this->StartTime();
  qint64 maxSec = this->EndTime();

  qint64 range = maxSec - minSec;

  this->m_Value = qMax(this->m_LowerBound, this->m_Value);
  this->m_Value = qMin(this->m_UpperBound, this->m_Value);

  if (range > INT_MAX)
  {
    // range = 100;
    // usePercentage = true;
    // NOT IMPLEMENTED !
  }
}

void ReplayBar::SetDate(QDateTime date)
{
  this->m_Value = date.toMSecsSinceEpoch();

  this->m_Value = qMax(this->m_LowerBound, this->m_Value);
  this->m_Value = qMin(this->m_UpperBound, this->m_Value);

  if (this->m_OldValue != this->m_Value)
  {
    this->m_OldValue = this->m_Value;
    emit SendNewValue(this->m_Value);
  }

  this->update();
}

void ReplayBar::SetBoundRange(QDateTime lowerBound, QDateTime upperBound)
{
  this->m_LowerBound = lowerBound.toMSecsSinceEpoch();
  this->m_UpperBound = upperBound.toMSecsSinceEpoch();

  this->m_Value = qMax(this->m_LowerBound, this->m_Value);
  this->m_Value = qMin(this->m_UpperBound, this->m_Value);

  if (this->m_OldValue != this->m_Value)
  {
    this->m_OldValue = this->m_Value;
    emit SendNewValue(this->m_Value);
  }
}

QDateTime ReplayBar::Date()
{
  return QDateTime::fromMSecsSinceEpoch(this->m_Value);
}

QDateTime ReplayBar::LowerBoundDate()
{
  return QDateTime::fromMSecsSinceEpoch(this->m_LowerBound);
}

QDateTime ReplayBar::UpperBoundDate()
{
  return QDateTime::fromMSecsSinceEpoch(this->m_UpperBound);
}

double ReplayBar::StartTime(int ref)
{
  return this->m_StartTime;
}

double ReplayBar::EndTime(int ref)
{
  return this->m_EndTime - ref * this->m_StartTime;
}

double ReplayBar::LowerBound(int ref)
{
  return this->m_LowerBound - ref * this->m_StartTime;
}

double ReplayBar::UpperBound(int ref)
{
  return this->m_UpperBound - ref * this->m_StartTime;
}

double ReplayBar::TimeRange()
{
  return (this->m_EndTime - this->m_StartTime);
}

double ReplayBar::BoundRange()
{
  return (this->m_UpperBound - this->m_LowerBound);
}

void ReplayBar::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  QColor baseColor = qThemes->Color(QPalette::Text);
  QColor highlightedColor = qThemes->Color(QPalette::Highlight);

  // Calculate Widget width and margin
  this->m_MarginH = (this->width() * 0.1) / 2;

  // Index of the value
  double index = ValueToIndex(this->m_Value);

  // Separate integer part and decimal part
  int integerValue = index;
  double decimalValue = index - integerValue;

  // Draw highlighted rect from 0 to integerValue
  drawRects(painter, 0, integerValue, highlightedColor);

  // Draw background rect from integerValue to numRects
  drawRects(painter, integerValue, NB_VALUE, baseColor);

  if (decimalValue == 0)
    return;

  // Draw last highlighted rect depending remainingValue
  drawRects(painter, integerValue, integerValue + 1, highlightedColor, decimalValue);
}

void ReplayBar::drawRects(QPainter &painter, int start, int end, QColor color, double widthFactor)
{
  start = qMax(start, 0);
  end = qMin(end, 100);

  int minEdgeIndex = LowerBoundIndex();                                             // Index for minimum edge
  int maxEdgeIndex = UpperBoundIndex() - 1;                                         // Index for maximum edge

  double stepRect = this->WidthPerRect();                                           // Rect Step
  int width = qMin(stepRect * 0.5, stepRect * widthFactor);                         // Rect width (width factor used for animation)

  int baseHeight = height() * 0.15;                                                 // Rect Base height
  int edgeHeight = baseHeight * 1.75;                                               // Rect Edge height
  int maxHeight = edgeHeight * MAX_RATIO;                                           // Maximum possible height for rects (include animations)

  int yOffset = (maxHeight - baseHeight) / 2;                                       // Vertical offset for aligning rects

  painter.setPen(Qt::NoPen);                                                        // Disable outline

  QColor outEdgeColor = Qt::gray;                                                   // Set color of rect out of edge bound
  outEdgeColor.setAlpha(128);                                                       // Set alpha of rect out of edge bound

  int centerY = this->rect().center().y();                                          // Get the center y value of the rect

  for (int i = start; i < end; ++i)                                                 // Draw rects from start to end
  {
    int x = m_MarginH + stepRect * i;                                               // X-position for current rect
    int y = centerY - baseHeight / 2;                                               //this->height() - baseHeight - yOffset - m_MarginV;                      // Y-position for current rect
    int height = baseHeight;                                                        // Height for current rect

    QColor currentColor = color;
    if (i < minEdgeIndex || i > maxEdgeIndex)                                       // If out of edge bound, Adjust opacity and color
      currentColor = outEdgeColor;                                                  // Adjust rect color

    if (i == minEdgeIndex || i == maxEdgeIndex)                                     // If Edge Rect, adjust height and vertical position
    {
      y = centerY - edgeHeight / 2;                                                 // this->height() - edgeHeight - (yOffset / 2) - m_MarginV;                  // Adjusted Y-position for edge rects
      height = edgeHeight;                                                          // Adjusted Height for edge rects

      if (widthFactor >= 1.0)                                                       // Prevent Animated rect to draw pixmap 
      {
        QString path = i == minEdgeIndex ? "SDATE_0.png" : "EDATE_0.png";           // Get icon path depending min or max edge
        QIcon icon(qThemes->Icons(path));                                           // Get icon from themes

        int iconOffset = (edgeHeight - baseHeight) / 2;                             // Vertical offset for prevent icon overlap
        
        if (m_IsTimelinePressed && x == m_AnimatedRect.x())                         // If animating and animation are on edge
          iconOffset += edgeHeight * 0.25;                                          // adjust icon offset to prevent overlap

        int iconX = x + (width - height) / 2;                                       // X-position for current icon
        int iconY = y - iconOffset - height;                                        // Y-position for current icon

        if (i == minEdgeIndex)
          this->m_LowerBoundRect = QRectF(iconX, iconY, height, height);            // Cache the Min edge rect of the icon
        else
          this->m_UpperBoundRect = QRectF(iconX, iconY, height, height);            // Cache the Max edge rect of the icon

        icon.paint(&painter, iconX, iconY, height, height, Qt::AlignCenter);        // Draw icon
      }
    }

    if (widthFactor < 1.0)                                                          // If Animated Rect
    {
      currentColor = color;                                                         // Adjust color

      if (start > maxEdgeIndex)                                                     // If index > maxIndex
        x = m_MarginH + stepRect * maxEdgeIndex;                                    // adjust X-position to maxEdgeIndex

      if (m_IsTimelinePressed)                                                      // If Animating Rect
      {

        double additionalHeight = height * (m_AnimatedFactor - 1);                  // Additional height for animated rect
        y -= additionalHeight / 2;                                                  // Adjust Y-position for animated rect
        height *= m_AnimatedFactor;                                                 // Adjusted height for animated rect

        this->m_AnimatedRect = QRectF(x, y, width, height);                         // Cache the AnimatedRect
      }
    }


    painter.setBrush(currentColor);                                                 // Set color and opacity
    QRectF rect(x, y, width, height);                                               // Current rect

    if (widthFactor >= 1.0)                                                         // Ignore Animated Rect
      this->m_TimelineRect = m_TimelineRect.united(rect);                           // Get bounding rect of all rects

    painter.drawRoundedRect(rect, 5, 5);                                            // Draw the rect
  }
}

double ReplayBar::TimelineWidth()
{
  return this->width() - m_MarginH * 2;
}

double ReplayBar::WidthPerRect()
{
  return this->TimelineWidth() / static_cast<double>(NB_VALUE);
}

double ReplayBar::TimePerRect()
{
  return this->TimeRange() / static_cast<double>(NB_VALUE);
}

double ReplayBar::LowerBoundIndex()
{
  return ValueToIndex(this->LowerBound());
}

double ReplayBar::UpperBoundIndex()
{
  return ValueToIndex(this->UpperBound());
}

double ReplayBar::IndexFromPosition(int x)
{
  return (x - m_MarginH) / this->WidthPerRect();
}

double ReplayBar::IndexToValue(double index)
{
  return this->StartTime() + (index * this->TimePerRect());
}

double ReplayBar::ValueToIndex(double value)
{
  return (value - this->StartTime()) / this->TimePerRect();
}

void ReplayBar::mousePressEvent(QMouseEvent *event)
{
  if (event->button() != Qt::LeftButton)
    return;

  if (m_TimelineRect.contains(event->pos()) == true)     this->m_IsTimelinePressed = true;
  else if (m_LowerBoundRect.contains(event->pos()) == true)    this->m_IsLowerBoundPressed = true;
  else if (m_UpperBoundRect.contains(event->pos()) == true)    this->m_IsUpperBoundPressed = true;
  else                                                    return;

  if (m_IsTimelinePressed)
  {
    m_AnimatedFactor = MAX_RATIO;

    double indexPosX = this->IndexFromPosition(event->pos().x());
    double indexMinEdge = this->LowerBoundIndex();
    double indexMaxEdge = this->UpperBoundIndex();

    indexPosX = qMax(indexPosX, indexMinEdge);
    indexPosX = qMin(indexPosX, indexMaxEdge);

    double newDate = this->IndexToValue(indexPosX);
    double roundedEdge = qRound64(newDate);

    double index = indexPosX;
    this->m_Value = qRound64(this->IndexToValue(index));
    this->m_Value = qMax(this->m_LowerBound, this->m_Value);
    this->m_Value = qMin(this->m_UpperBound, this->m_Value);
  }

  if (this->m_OldValue != this->m_Value)
  {
    this->m_OldValue = this->m_Value;
    emit SendNewValue(this->m_Value);
  }

  update();
  event->accept();
}

void ReplayBar::mouseMoveEvent(QMouseEvent * event)
{
  if ((event->buttons() & Qt::LeftButton) == false)
    return;

  if (this->m_IsTimelinePressed == false && this->m_IsLowerBoundPressed == false && this->m_IsUpperBoundPressed == false)
    return;

  double indexPosX = this->IndexFromPosition(event->pos().x());
  double indexMinEdge = this->LowerBoundIndex();
  double indexMaxEdge = this->UpperBoundIndex();

  if (this->m_IsLowerBoundPressed)
  {
    indexPosX = qMax(indexPosX, 0.0);
    indexPosX = qMin(indexPosX, indexMaxEdge - 2);

    int index = (int)indexPosX;
    this->m_LowerBound = qRound64(this->IndexToValue(index));

    this->m_Value = qMax(this->m_Value, this->m_LowerBound);
  }

  if (this->m_IsUpperBoundPressed)
  {
    indexPosX = qMax(indexPosX, indexMinEdge + 2);
    indexPosX = qMin(indexPosX, (double)NB_VALUE);

    int index = (int)indexPosX;
    this->m_UpperBound = qRound64(this->IndexToValue(index));

    this->m_Value = qMin(this->m_Value, this->m_UpperBound);
  }

  if (this->m_IsTimelinePressed)
  {
    indexPosX = qMax(indexPosX, indexMinEdge);
    indexPosX = qMin(indexPosX, indexMaxEdge);

    double index = indexPosX;
    this->m_Value = qRound64(this->IndexToValue(index));
    this->m_Value = qMax(this->m_LowerBound, this->m_Value);
    this->m_Value = qMin(this->m_UpperBound, this->m_Value);
  }

  if (this->m_OldValue != this->m_Value)
  {
    this->m_OldValue = this->m_Value;
    emit SendNewValue(this->m_Value);
  }

  update();
  event->accept();
}

void ReplayBar::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() != Qt::LeftButton)
    return;

  if (this->m_IsTimelinePressed == false && this->m_IsLowerBoundPressed == false && this->m_IsUpperBoundPressed == false)
    return;

  m_AnimatedTimer->start();

  if (this->m_IsLowerBoundPressed || this->m_IsUpperBoundPressed)
  {

  }

  if (this->m_IsTimelinePressed)
  {
    m_AnimatedTimer->start();                                     // Start the release animation

    double indexPosX = this->IndexFromPosition(event->pos().x());
    double indexMinEdge = this->LowerBoundIndex();
    double indexMaxEdge = this->UpperBoundIndex();

    indexPosX = qMax(indexPosX, indexMinEdge);
    indexPosX = qMin(indexPosX, indexMaxEdge);

    double index = indexPosX;
    this->m_Value = qRound64(this->IndexToValue(index));
    this->m_Value = qMax(this->m_LowerBound, this->m_Value);
    this->m_Value = qMin(this->m_UpperBound, this->m_Value);

    if (this->m_OldValue != this->m_Value)
    {
      this->m_OldValue = this->m_Value;
      emit SendNewValue(this->m_Value);
    }
  }

  this->m_IsLowerBoundPressed = false;
  this->m_IsUpperBoundPressed = false;
}