#ifndef TGS_TOOLBAR_H
#define TGS_TOOLBAR_H

#include "DPS_Toolbar.h"

class TGS_Toolbar : public DPS_Toolbar
{
  Q_OBJECT

  private:
    void SetupUI();
    void SetupEvent();

  public:
    TGS_Toolbar(QWidget *parent = nullptr);
    ~TGS_Toolbar();

};

#endif // TGSTOOLBAR_H
