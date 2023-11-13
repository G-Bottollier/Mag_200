#ifndef IMP_TOOLBAR_H
#define IMP_TOOLBAR_H

#include "Toolbar.h"

class IMP_Toolbar : public Toolbar
{
  Q_OBJECT

  private:
    QLabel *labelFolder = nullptr;

    void SetupUI();
    void SetupEvent();

  public:
    IMP_Toolbar(QWidget *parent = nullptr);
    ~IMP_Toolbar();

    void setFolderName(QString name);

  public slots:
    void LedClicked();

  signals:
    void SendOpenParentFolder();
};

#endif // IMP_TOOLBAR_H
