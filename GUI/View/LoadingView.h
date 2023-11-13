#ifndef LOADINGVIEW_H
#define LOADINGVIEW_H

#include "Led.h"
#include "PageView.h"
#include "SettingsManager.h"

#define BUTTON_OK             0
#define BUTTON_CANCEL         1

#define DOWNLOAD_FILE         0
#define DOWNLOAD_TIME_FIELD   1
#define DOWNLOAD_TIME_VALUE   2
#define DOWNLOAD_SPEED_FIELD  3
#define DOWNLOAD_SPEED_VALUE  4

class LoadingView : public PageView
{
  Q_OBJECT

  private:
    QMap<int, QLabel*> Labels;
    QProgressBar *progressBar;
    QVBoxLayout *MainLayout;

    QElapsedTimer *timer;
    QFileInfo file;

    qint64 lastTime = 0;
    qint64 totalTime = 0;

    void SetupUI();
    void SetupEvent();

    void UpdateUI(qint64 totalBytesCopied);

  protected:
    void resizeEvent(QResizeEvent *event);

  public:
    LoadingView(QWidget *parent = nullptr);
    ~LoadingView();

    void UpdateCopy(qint64 progress);

  public slots:
    void LedClicked();

    void StartCopy(QString path);
    void CancelCopy();

  signals:
    void SendCancelCopy();
    void SendCopyFile(QString src, QString dst);
};

#endif // LOADINGVIEW_H
