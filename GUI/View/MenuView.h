#ifndef MENUVIEW_H
#define MENUVIEW_H

#include "PageView.h"

#define MENU_IMPORT 0
#define MENU_EXPORT 1
#define MENU_RECORD 2
#define MENU_REPLAY 3

class MenuView : public PageView
{
  Q_OBJECT

  private:
    void SetupUI();
    void SetupEvent();

    int spacing = 50;
    QVBoxLayout *MainLayout = nullptr;

    void LedClicked();

  protected:
    void resizeEvent(QResizeEvent *event);

  public:
    MenuView(QWidget *parent = nullptr);
    ~MenuView();
};

#endif // MENUVIEW_H
