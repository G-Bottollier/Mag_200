#ifndef LED_H
#define LED_H

#include <QObject>
#include <QtWidgets>

#include <QPainter>
#include <QBrush>
#include <QRadialGradient>
#include <QTimer>
#include <QtMath>

#include "Tools.h"
#include "Status.h"

#define NO_ALIGNEMENT (Qt::AlignmentFlag)NULL

/*
 * @brief The Led class represents a custom LED button in the GUI.
 *
 * It inherits from QPushButton and provides additional functionality for managing the LED appearance and behavior.
 *
 * @date 31/05/2023
 */
class Led : public QPushButton
{
  Q_OBJECT

  private:
    QRect LedSize;
    Color LedColor;
    Mag200Status Status;
    
    bool useColor = true;
    bool useBorder = true;
    bool useFixedSize = true;
    bool useBackground = true;
    bool useHasButton = false;
    bool useFixedStyle = false;
    bool useAutoSizeFont = true;
    QFlags<Qt::AlignmentFlag> textAlignment = NO_ALIGNEMENT;

    QTimer *frame = nullptr;
    void SetupAnimation(QString path, bool isAnimated);

  protected:
    void changeEvent(QEvent *event);
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

  public:
    Led(const QString& text, int size = 32, QWidget *parent = nullptr);
    ~Led();
    
    void UseColor(bool useColor);
    void UseBorder(bool useBorder);
    void UseBackground(bool useBackground);
    void UseFixedStyle(bool useFixedStyle);
    void UseAutoSizeFont(bool useAutoSizeFont);

    void UseHasButton(bool useHasButton);

    void SetSize(QSize size);
    void SetInfos(QStringList list);
    void SetAlignement(QFlags<Qt::AlignmentFlag> flags);
    void SetState(Mag200Status status, Color color = NO_COLOR);
      
    void SetIcon(QString path, bool isAnimated);
    void SetIcon(QIcon icon);

    void ResetAnimation();
    void StartAnimation();
    void StopAnimation();
      
    Mag200Status GetState();

    bool IsChecked();
    bool IsClickedConnected();

  public slots:
    void NewFrame();
    void ResetFrame();

    void Toggled(bool checked);
};

#endif // LEDSTATUS_H
