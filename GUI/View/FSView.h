#ifndef FSVIEW_H
#define FSVIEW_H

#include "PageView.h"
#include "Mag200Data.h"

class FSView : public PageView
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
    FSView(QWidget * parent = Q_NULLPTR);
    ~FSView();

    void Update(Mag200Data *data);
};

#endif // FSVIEW_H
