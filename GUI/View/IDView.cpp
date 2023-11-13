#include "stdafx.h"
#include "IDView.h"

IDView::IDView(QWidget *parent) : PageView(parent)
{
  // Setup UI 
  SetupUI();

  // Setup event handlers
  SetupEvent();
}

IDView::~IDView()
{

}

void IDView::SetupUI()
{
  // Create a CKChartWidget for ID visualization
  Chart = new CKChartWidget();
  Chart->setMinimumHeight(100);
  Chart->SetAutomaticSize(true);
  Chart->SetBackgroundBrush(QColor(Color::BLACK_03).darker());
  Chart->SetAxesPositioning(CKChartWidget::AutomaticPosition);

  // Create the T-axis for time display
  AxisT = Chart->CreateAxisT();
  AxisT->SetTimeFormat(CKChartWidget::LabelTimeFormatHHMMSS);
  AxisT->SetMode(CKChartWidget::AxisTModePlot);
  AxisT->SetAutomaticPosition(Qt::AlignBottom);
  AxisT->SetMarkerColor(Qt::red);
  AxisT->SetMajorTicksCount(2);
  AxisT->SetColor(Qt::white);

  // Create the Y-axis for ID values
  AxisY = Chart->CreateAxisY();
  AxisY->SetAutomaticPosition(Qt::AlignLeft);
  AxisY->SetReferenceAxis(AxisT);
  AxisY->SetMajorTicksCount(4);
  AxisY->SetColor(Qt::white);

  // Create a curve for ID data
  Curve = Chart->CreateCurve();
  Curve->SetLineWidth(4);
  Curve->SetVisible(true);
  Curve->SetReferenceAxis(AxisY);
  Curve->SetColor(QColor(Color::GREEN_02));

  // Create a label for displaying the channel name
  Channel = Chart->CreateLabel();
  Channel->SetVisible(true);
  Channel->SetColor(Qt::white);
  Channel->SetAutomaticSize(true);
  Channel->SetBackgroundBrush(QColor(Color::BLACK_03).darker());

  // Add the ID widget to the main widget
  this->AddMainWidget(Chart);
}

void IDView::SetupEvent()
{
}

void IDView::resizeChannel()
{
  QSize size = this->size();

  int margin = 20;
  QString text = Channel->GetText();
  QPoint pos = Channel->GetPosition();
  QFontMetrics metrics(Channel->GetFont());
  pos.setX(size.width() - metrics.width(text) - margin);

  Channel->SetPosition(pos);
}

void IDView::resizeEvent(QResizeEvent * event)
{
  PageView::resizeEvent(event);

  QSize size = event->size();
  Chart->setFixedSize(size);

  // Set the font for the ID widget
  QFont font = Chart->font();
  font.setBold(true);
  Chart->setFont(font);

  font.setPixelSize(size.height() * 0.07);

  AxisY->SetFont(font);
  AxisT->SetFont(font);

  font.setPixelSize(size.height() * 0.13);

  Channel->SetFont(font);
  resizeChannel();
}

void IDView::Update(Mag200Data * data)
{
  if (data == nullptr)
    return;

  // Clear the data of the Curve
  Curve->Clear();

  // Get the channel name and bit rate
  QString name = data->SFID_Name();
  double bitRate = data->Bitrate();

  // Set the new position and text of the Channel widget
  Channel->SetText(name);
  resizeChannel();

  // Get the first and last ID values from the frame sync object
  int firstIdValue = data->MajorFrameFirstValue_ID();
  int lastIdValue = data->MajorFrameLastValue_ID();

  // Check if data are valid
  if (firstIdValue == 0 && lastIdValue == 0)
    return;

  // Set the Y-axis range of the AxisY
  AxisY->SetRange(firstIdValue, lastIdValue);

  // Get the minor frame, bits per word, and minor frame words from the frame sync object
  int minorFrame = data->MajorFrameLength();
  if (minorFrame <= 0)
    minorFrame = 1;

  int minorFrameLength = data->MinorFrameLength();

  // Calculate the number of displayed frames and the corresponding time value
  int nbDisplayedFrame = INPUTS_ID_NB_PERIOD;

  double frame = minorFrame * minorFrameLength * nbDisplayedFrame;
  double value = bitRate == 0 ? 60 : (frame / bitRate);

  // Set the time range of the AxisT
  AxisT->SetRange(value);
}

/*
* @brief Adds channel data to the ID curve.
*
* This function adds the specified data points to the ID curve in the ID_Widget.
*
* @param ptsX The X-coordinates of the data points.
* @param ptsY The Y-coordinates of the data points.
* @return void
*
* @date 31/05/2023
*/
void IDView::NewChannelData(QVector<double> ptsX, QVector<double> ptsY)
{
  // Check if the ID_Widget exists
  if (Chart == nullptr)
    return;

  // Check if the ID_Widget is visible
  if (Chart->isVisible() == false)
    return;

  // Determine the number of data points to be added
  int nb = qMin(ptsX.size(), ptsY.size());

  // Emit a signal to add the data points to the Curve
  emit Curve->sgAddSamples(ptsX.data(), ptsY.data(), nb);

  // Emit a signal to set the current time in the ID_Widget
  emit Chart->sgSetCurrentTime(ptsX.last());
}