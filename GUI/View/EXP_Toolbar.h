#ifndef EXP_TOOLBAR_H
#define EXP_TOOLBAR_H

#include "Toolbar.h"

class EXP_Toolbar : public Toolbar
{
  Q_OBJECT

  private:
    void SetupUI();
    void SetupEvent();

  public:
    EXP_Toolbar(QWidget *parent = nullptr);
    ~EXP_Toolbar();

  public slots:
    void LedClicked();
};

#endif // EXP_TOOLBAR_H
