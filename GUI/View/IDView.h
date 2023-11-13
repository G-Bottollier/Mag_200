#ifndef IDVIEW_H
#define IDVIEW_H

#include "PageView.h"
#include "Mag200Data.h"

#include "kchartwidget.h"
#include "kchartaxisitem.h"
#include "kchartcurveitem.h"
#include "kchartlabelitem.h"

#define INPUTS_ID_NB_PERIOD 50

class IDView : public PageView
{
  Q_OBJECT

  private:
    QGridLayout *MainLayout = nullptr;

    CKChartWidget* Chart = nullptr;
    CKChartAxisTItem *AxisT = nullptr;
    CKChartAxisYItem *AxisY = nullptr;
    CKChartCurveItem *Curve = nullptr;
    CKChartLabelItem *Error = nullptr;
    CKChartLabelItem *Channel = nullptr;

    void SetupUI();
    void SetupEvent();

    void resizeChannel();

  protected:
    void resizeEvent(QResizeEvent *event);

  public:
    IDView(QWidget * parent = Q_NULLPTR);
    ~IDView();

    void Update(Mag200Data *data);

    void NewChannelData(QVector<double> ptsX, QVector<double> ptsY);
};

#endif // IDVIEW_H
