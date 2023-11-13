#ifndef CHAPTER10VIEW_H
#define CHAPTER10VIEW_H

#include "PageView.h"
#include <QNetworkInterface>
#include <QNetworkConfigurationManager>

/*
 * @brief The Chapter10View class represents a specific view for Chapter 10 in the application.
 *
 * This class is derived from the PageView class and provides specialized UI setup and event handling
 * for Chapter 10. It sets the header name to "PAGE_CH10" and can be customized further as needed.
 */
class Chapter10View : public PageView 
{
  Q_OBJECT

  private:
    void SetupUI();
    void SetupEvent();

  public:
    Chapter10View(QWidget * parent = Q_NULLPTR);
    ~Chapter10View();
};

#endif // CHAPTER10VIEW_H