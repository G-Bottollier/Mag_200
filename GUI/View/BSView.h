#ifndef BSVIEW_H
#define BSVIEW_H

#include "PageView.h"
#include "Mag200Data.h"

class BSView : public PageView
{
  Q_OBJECT

  private:
    QStringList Names;
    QStringList Methods;
    QList<QLabel*> Fields;

    QGridLayout *MainLayout = nullptr;

    void SetupUI();
    void SetupEvent();

  public:
    BSView(QWidget *parent = Q_NULLPTR);
    ~BSView();

    void Update(Mag200Data *data);
};

#endif // BSVIEW_H
