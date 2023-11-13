#ifndef WHEEL_H
#define WHEEL_H

#include <QtWidgets>
#include "Tools.h"
#include "ThemesManager.h"

#define MAX_SCROLL_TIME 2000
#define EMPTY_ITEMS 10

class Wheel : public QListWidget
{
  Q_OBJECT

  private:
    int m_ItemHeight = 0;
    QStringList m_Items;
    
    int m_MinValue = -1;
    int m_MaxValue = -1;

    QString m_OldValue;
    QPoint m_PressedPosition;

    bool m_IsVisible = false;

    bool m_UseAnimation = true;
    bool m_UseLeadingZeros = true;
    bool m_UseItemSelection = false;
    
    QScroller::State m_OldScrollState;

    void SetupUI();
    void SetupEvent();

    void NewValue(QString value);
    QListWidgetItem *Item();

    void updateItemCount();
    void updateItemText();
    void updateSnapPositions();
    void updateItemList(QStringList items);
    void ItemScrollAnimation(QListWidgetItem *item, qreal span = 0.0);

  protected:
    void paintEvent(QPaintEvent *event);
    
    void showEvent(QShowEvent *event);
    void wheelEvent(QWheelEvent *event);
    void resizeEvent(QResizeEvent* event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

  public:
    Wheel(int m_MinValue = -1, int m_MaxValue = -1, QWidget *parent = 0);
    ~Wheel();

    void SetRange(int m_MinValue, int m_MaxValue, bool NoSetValue = false);

    QStringList Items();
    void SetItems(QStringList items);

    void SetValue(QString value);
    QString Value();

    void UseAnimation(bool m_UseAnimation);

    int ItemHeight();
    QColor ItemBackground();

  public slots:
    void ScrollerStateChanged(QScroller::State newState);
    
  signals:
    void SendNewValue(QString value);
    void SendNewItemHeight(int height);
};

#endif // WHEEL_H
