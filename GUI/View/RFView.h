#ifndef RFVIEW_H
#define RFVIEW_H

#include "PageView.h"
#include "LS_X8.h"

class RFView : public PageView
{
  Q_OBJECT

  private:
    double m_Strength = 0;
    double m_Deviation = 0;

    QStringList Names;
    QList<QLabel*> Fields;

    QGridLayout *MainLayout = nullptr;

    QString StrengthToString(double Strength);
    QString DeviationToString(double Deviation);

    void SetupUI();
    void SetupEvent();

  public:
    RFView(QWidget * parent = Q_NULLPTR);
    ~RFView();

    void Update(ChannelItem *item);

  public slots:
    void NewSignalStrength(double Strength);
    void NewSignalDeviation(double Deviation);
};

#endif // RFVIEW_H
