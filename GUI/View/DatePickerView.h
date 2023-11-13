#ifndef DATEPICKERVIEW_H
#define DATEPICKERVIEW_H

#include "PageView.h"

#include "DatePicker.h"

#define DATE_PICKER   0
#define RANGE_PICKER  1

class DatePickerView : public PageView
{
  Q_OBJECT

  private:
    int m_Type;
    QMap<int, DatePicker*> m_Pickers;

    void SetupUI();
    void SetupEvent();

  protected:
    void resizeEvent(QResizeEvent *event);

  public:
    DatePickerView(int type, QWidget *parent = nullptr);
    ~DatePickerView();

    void SetDate(QDateTime date);
    void SetDateRange(QDateTime startTime, QDateTime endTime);
    void SetBoundRange(QDateTime lowerBound, QDateTime upperBound);
    
    QString StartDate();
    QString EndDate();

    QDateTime LowerBoundDate();
    QDateTime UpperBoundDate();

    QDateTime Date();

  public slots:
    void CloseClicked() override;
};

#endif // DATEPICKERVIEW_H
