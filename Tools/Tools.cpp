#include "stdafx.h"
#include "Tools.h"

/*
 * @brief Constructor for the Tools class.
 *
 * @param parent The parent object.
 *
 * @date 31/05/2023
 */
Tools::Tools(QObject *parent) : QObject(parent)
{

}

/*
 * @brief Destructor for the Tools class.
 *
 * @date 31/05/2023
 */
Tools::~Tools()
{

}

/*
 * @brief Convert a double value representing a date to a QString in the format "YYYY-DDD-hh:mm:ss.mmm".
 *
 * @param date The double value representing the date.
 * @return QString The formatted date string.
 *
 * @date 31/05/2023
 */
QString Tools::DateToString(double date)
{
  QString strDate;
  QDateTime dateTime = QDateTime::fromTime_t(date, Qt::UTC);

  // Extract the fractional part of the date to get milliseconds
  double delta = date - (int)date;
  int ms = qRound(delta * 1e3);

  // Format the date components into the desired string format
  strDate += QStringLiteral("%1-").arg(dateTime.date().dayOfYear(), 3, 10, QLatin1Char('0'));   // Add Year of day
  strDate += dateTime.toString("hh:mm:ss.");                                                    // Add Time
  strDate += QStringLiteral("%1").arg(ms, 3, 10, QLatin1Char('0'));                             // Add MSec

  return strDate;
}

/*
 * @brief Convert a duration in milliseconds to a QString in the format "DDDD-hh:mm:ss.mmm".
 *
 * @param milliseconds The duration in milliseconds.
 * @return QString The formatted duration string.
 *
 * @date 31/05/2023
 */
QString Tools::ChronoToString(qint64 milliseconds, bool useDays)
{
  const int SEC_TO_MS   = 1000;
  const int MIN_TO_MS   = 60;
  const int HOUR_TO_MS  = 60;
  const int DAY_TO_MS   = 24;

  // Calculate the individual components of the duration
  qint64 seconds = milliseconds / SEC_TO_MS;
  milliseconds %= SEC_TO_MS;

  qint64 minutes = seconds / MIN_TO_MS;
  seconds %= MIN_TO_MS;

  qint64 hours = minutes / HOUR_TO_MS;
  minutes %= HOUR_TO_MS;

  qint64 days = hours / DAY_TO_MS;

  if(useDays)
    hours %= DAY_TO_MS;

  // Format the duration components into the desired string format
  QString date;
  if (useDays)
    date += QStringLiteral("%1").arg(days, 4, 10, QLatin1Char('0')) + "-";        // Add days
  date += QStringLiteral("%1").arg(hours, 2, 10, QLatin1Char('0')) + ":";       // Add hours
  date += QStringLiteral("%1").arg(minutes, 2, 10, QLatin1Char('0')) + ":";     // Add minutes
  date += QStringLiteral("%1").arg(seconds, 2, 10, QLatin1Char('0')) + ".";     // Add seconds
  date += QStringLiteral("%1").arg(milliseconds, 3, 10, QLatin1Char('0'));      // Add milliseconds

  return date;
}

/*
 * @brief Convert a bitrate value to a human-readable string with appropriate units.
 *
 * @param bitrate The bitrate value.
 * @param isKbps Flag indicating whether the bitrate should be displayed in Kbps or not.
 * @param nbDecimal Define the double precision
 * @return QString The formatted bitrate string.
 *
 * @date 31/05/2023
 */
QString Tools::BitrateToString(double bitrate, bool isKbps, int nbDecimal)
{
  const long long Multiply = qPow(10, nbDecimal);                   // Multiply double to int(long) for better precision
  const long long v1 = (1e3 * Multiply) - 1;                        // v1 = Threshold between bps & kbps
  const long long v2 = (1e6 * Multiply) - 1;                        // v2 = Threshold between Kbps & Mbps
  const long long v3 = (1e9 * Multiply) - 1;                        // v3 = Threshold between Mbps & Gbps
  const long long v4 = (1e12 * Multiply) - 1;                       // v4 = Threshold between Gbps & Tbps

  const QVector<QString> unitsBps = { tr("BPS"), tr("KBPS"), tr("MBPS"), tr("GBPS"), tr("TBPS") };  // Units string representation
  const QVector<QString> unitsKbps = { tr("O"), tr("KO"), tr("MO"), tr("GO"), tr("TO") };           // Units string representation

  // Round the bitrate value and multiply by the precision factor
  long long bps = QString::number(bitrate, 'f', nbDecimal).toDouble() * Multiply;

  // Append bps in the array for comparison
  QVector<long long> range = { v1, v2, v3, v4 };
  range.append(bps);

  // Sort the range to determine the appropriate unit
  qSort(range);

  // Get the index of the bitrate in the range
  int idx = range.indexOf(bps);

  // Get the string representation of the unit and value
  QString unit = isKbps == true ? unitsBps.at(idx) : unitsKbps.at(idx);
  QString value = "";

  // Calculate the value by dividing the bitrate with the previous range value
  int prevIdx = idx - 1;
  if (prevIdx >= 0)
    value = QString::number(bps / (double)range.at(prevIdx), 'f', nbDecimal);
  else
    value = QString::number(bps / Multiply);

  // Return the formatted result with the unit
  return value + " " + unit;
}

/*
 * @brief Get a list of screens available in the application.
 *
 * @return QList<QScreen*> The list of screens.
 *
 * @date 31/05/2023
 */
QList<QScreen*> Tools::GetScreens()
{
  return qApp->screens();
}

/*
 * @brief Get the names of screens available in the application.
 *
 * @return QStringList The list of screen names.
 *
 * @date 31/05/2023
 */
QStringList Tools::GetScreensNames()
{
  QStringList names;
  QList<QScreen*> screens = qApp->screens();

  // Iterate over the screens and add their names to the list
  for (int i = 0; i < screens.size(); i++)
  {
    names.append(screens[i]->name());
  }

  return names;
}

int Tools::GetWidgetWidth(QWidget *widget, QSize size, QBoxLayout *mainLayout, bool useSpacing)
{
  if (widget == nullptr)
    return 0;

  // Calculate the width of the widget
  QMargins margins = widget->contentsMargins();
  int width = size.width() - margins.left() - margins.right();

  QLayout *layout = mainLayout == nullptr ? widget->layout() : mainLayout;

  // Calculate the width with widget's layout margins / spacing
  // Calculate the height with widget's layout margins / spacing
  if (layout == nullptr)
    return width;

  margins = layout->contentsMargins();
  width = width - margins.left() - margins.right();

  if (useSpacing == false)
    return width;

  QVBoxLayout *vLayout = dynamic_cast<QVBoxLayout*>(layout);
  if (vLayout != nullptr)
    return width;

  int spacing = layout->spacing();
  int count = layout->count();
  width -= qMax(0, spacing * (count - 1));

  return width;
}

int Tools::GetWidgetHeight(QWidget *widget, QSize size, QBoxLayout *mainLayout, bool useSpacing)
{
  if (widget == nullptr)
    return 0;

  // Calculate the height of the widget
  QMargins margins = widget->contentsMargins();
  int height = size.height() - margins.top() - margins.bottom();

  QLayout *layout = mainLayout == nullptr ? widget->layout() : mainLayout;

  // Calculate the height with widget's layout margins / spacing
  if (layout == nullptr)
    return height;

  margins = layout->contentsMargins();
  height = height - margins.top() - margins.bottom();

  if (useSpacing == false)
    return height;

  QHBoxLayout *hLayout = dynamic_cast<QHBoxLayout*>(layout);
  if (hLayout != nullptr)
    return height;

  int spacing = layout->spacing();
  int count = layout->count();
  height -= qMax(0, spacing * (count - 1));

  return height;
}

int Tools::GetWidgetWidth(qreal ratio, int &width, int &availableWidth, qreal &availableRatio)
{
  // Prevent negative
  availableRatio = qMax(0.0, availableRatio); 

  // Calculate width
  int calculatedWidth = qMin(availableWidth, qRound(width * qMin(availableRatio, ratio)));

  // Update remaining width & ratio
  availableWidth -= calculatedWidth;
  availableRatio -= ratio;

  return calculatedWidth;
}

int Tools::GetWidgetHeight(qreal ratio, int &height, int &availableHeight, qreal &availableRatio)
{
  // Prevent negative
  availableRatio = qMax(0.0, availableRatio);

  // Calculate height
  int calculatedHeight = qMin(availableHeight, qRound(height * qMin(availableRatio, ratio)));

  // Update remaining width & ratio
  availableHeight -= calculatedHeight;
  availableRatio -= ratio;

  return calculatedHeight;
}

int Tools::AutoFontSize(int maxWidth, int maxHeight, QString text, QFont defaultFont)
{
  QFont font = defaultFont;
  font.setPixelSize(maxHeight);

  QFontMetrics fontMetrics(font);
  int fontsize = font.pixelSize();

  QRect rect = fontMetrics.boundingRect(QRect(0, 0, maxWidth, maxHeight), Qt::AlignLeft, text);

  bool isBigger = qMax(maxWidth, maxHeight);

  double step = isBigger ? 0.9 : 1.1;
  for (;;)
  {
    font.setPixelSize(fontsize);
    QFontMetrics fontMetrics(font);
    rect = fontMetrics.boundingRect(QRect(0, 0, maxWidth, maxHeight), Qt::AlignLeft, text);

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

bool Tools::CompareFileInfo(const QFileInfo &a, const QFileInfo &b)
{
  // If 'a' is a directory and 'b' is not, return true
  if (a.isDir() && !b.isDir())
    return true;

  // If 'b' is a directory and 'a' is not, return false
  else if (!a.isDir() && b.isDir())
    return false;

  // Otherwise, compare the file names in alphabetical order
  return a.fileName().compare(b.fileName(), Qt::CaseInsensitive) < 0;
}

QString Tools::NearestParentPath(QString path)
{
  if (QFileInfo::exists(path)) return path;

  QDir dir(path);
  if (!dir.makeAbsolute()) 
    return QString();

  do
  {
    dir.setPath(QDir::cleanPath(dir.filePath(QStringLiteral(".."))));
  } while (!dir.exists() && !dir.isRoot());

  return dir.exists() ? dir.path() : QString();
}

/**
* @brief Checks if a given QPixmap is considered 'jumbo'.
*
* This function is specifically designed to detect the behavior of `SHIL_JUMBO` from the Windows API.
* The Windows API sometimes returns a 'fake jumbo' image where, to preserve the quality of a scaled image,
* the icon is not actually scaled up. Instead, it retains its default size (e.g., 32x32) but is
* embedded within a larger transparent background (e.g., 256x256). 
*
* This function checks for this behavior by determining if a QPixmap is 'jumbo' based on the proportion 
* of pixels sampled from the pixmap that are completely transparent (i.e., RGB value of 0).
*
* It samples the pixmap in a grid fashion, where the number of samples is determined by the 'sample' variable.
*
* The function return true if 90% or less of the sampled pixels are completely transparent.
* The function return false if more of 90% sampled pixels are completely transparent.
*
* @param pixmap The QPixmap to be checked.
* @return Returns true if the pixmap is 'jumbo', false otherwise.
*
* @date 31/05/2023
*/
bool Tools::IsJumboPixmap(QPixmap &pixmap)
{
  QSize size = pixmap.size();

  int count = 0;
  int total = 0;
  double sample = 16;
  double stepW = size.width() / qSqrt(sample);
  double stepH = size.height() / qSqrt(sample);

  QImage image = pixmap.toImage();

  for (double x = 0; x < size.width(); x += stepW)
  {
    for (double y = 0; y < size.height(); y += stepH)
    {
      QRgb rgb = image.pixel(x, y);
      if (rgb == 0)
        count++;
      total++;
    }
  }
  
  return (count / (double)total) <= 0.9;
}



/*********************************************************/
/** ALLOW TO CHECK PROPERLY CLEAN ALL CONSTRUCTED CLASS **/
/*********************************************************/
//static QMap<QString, QStringList> Objects;
//void Tools::Constructed(QString name)
//{
//  // Tools::Constructed(QString(typeid(this).name()) + "(0x" + QString::number((int)this, 16) + ")");
//  Objects[name] = QStringList();
//  Objects[name] << QString::number(NB) << "Construct !";
//}
//
//void Tools::Destroyed(QString name)
//{
//  // Tools::Destroyed(QString(typeid(this).name()) + "(0x" + QString::number((int)this, 16) + ")");
//  Objects[name] << "Destruct !" ;
//}
