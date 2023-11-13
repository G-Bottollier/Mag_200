#ifndef PAGEVIEW_H
#define PAGEVIEW_H

#include <QtWidgets>

#include "Led.h"
#include "Tools.h"
#include "Mag200Data.h"
#include "MessagesManager.h"
#include "CommandsManager.h"
#include "PageDefinition.h"

/*
 * @brief The PageView class represents a widget for displaying a page view.
 *
 * This class is responsible for initializing the GUI, setting up event connections,
 * managing the interaction between various components of the application, and providing
 * functionality for adding, deleting, and customizing widgets within the page view.
 *
 * @date 31/05/2023
 */
class PageView : public QWidget 
{
	Q_OBJECT

  private:
    // Generic
    bool m_UseLine = false;
    bool m_UseFullPage = false;
    
    // Main View
    QVBoxLayout *MainLayout = nullptr;

    // Header View
    QWidget *HeaderWidget = nullptr;
    QHBoxLayout *HeaderLayout = nullptr;

    QLabel *LabelTitle = nullptr;
    Led *CloseButton = nullptr;

    // Scroll View
    QScrollArea *ScrollArea = nullptr;
    QWidget *ScrollWidget = nullptr;

    // Field View 
    QGridLayout *FieldLayout = nullptr;
    QList<QLabel *> Fields;

    void SetupUI();
    void SetupEvent();

  protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

    // LEDS
    int LedIndex();
    QMap<int, Led*> Leds;

    // HEADER
    QSize HeaderSize();
    QSize CloseButtonSize();
    void SetHeaderName(QString name);
    void AddHeaderStretch(int stretch = 0);
    void AddHeaderWidget(QWidget * widget);
    void RemoveHeaderWidget(QWidget* widget);

    // MAIN WIDGET
    void UseLine(bool UseLine);
    void UseFullPage(bool UseFullPage);

    void DeleteMainWidget();

    // SCROLL VIEW
    void SetScrollAreaSize(QSize size);
    void SetScrollWidgetSize(QSize size);

    // FIELD VIEW
    void AddFieldView(QStringList Names);
    void UpdateField(int index, QString value);
    void UpdateFieldView(Mag200Data * data, QStringList Methods);

    // INVOKE
    QVariant InvokeMethod(int returnType, QObject *obj, const char *method);

  public:
	  PageView(QWidget * parent = Q_NULLPTR);
	  ~PageView();

    void AddHeaderView();
    void AddHeaderTitle();
    void AddScrollView(QLayout *layout);
    void AddMainWidget(QWidget *widget);
    void AddGrabGesture(QObject *object);

    bool IsHeaderVisible();
    void SetHeaderVisible(bool visible);

    bool UseFullPage();

  public slots:
    virtual void CloseClicked();

  signals:
    void SendCloseClicked();
    void SendLedClicked(int Page);
};

#endif // PAGEVIEW_H