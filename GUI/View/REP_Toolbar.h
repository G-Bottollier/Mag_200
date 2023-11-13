#ifndef REP_TOOLBAR_H
#define REP_TOOLBAR_H

#include "Toolbar.h"

class REP_Toolbar : public Toolbar
{
  Q_OBJECT

  private:
    void SetupUI();
    void SetupEvent();

    void LedClicked();

  public:
    REP_Toolbar(QWidget *parent = nullptr);
    ~REP_Toolbar();
};

#endif // REP_TOOLBAR_H
