#ifndef SETTINGSVIEW_H
#define SETTINGSVIEW_H

#include "PageView.h"
#include "SettingsManager.h"

class SettingsView : public PageView
{
  Q_OBJECT

  private:
    QFormLayout *ViewLayout = nullptr;

    QPushButton *m_themes = nullptr;
    QPushButton *m_languages = nullptr;
    QPushButton *m_Mag200Type = nullptr;
    QPushButton *m_KillButton = nullptr;

    QList<QLineEdit*> m_screens;

    void SetupUI();
    void SetupEvent();

  protected:
    void resizeEvent(QResizeEvent *event);

  public:
    SettingsView(QWidget *parent = nullptr);
    ~SettingsView();

  public slots:
    void CloseClicked();
    void ThemesButtonClicked();
    void LanguagesButtonClicked();
    void Mag200TypeButtonClicked();
};

/*******************/
/** CUSTOM DIALOG **/
/*******************/
class OptionDialog : public QDialog
{
  Q_OBJECT

  private:
    QListWidget *listWidget = nullptr;

    QPushButton *okButton = nullptr;
    QPushButton *cancelButton = nullptr;

    // Draw
    const int borderSize = 20;
    const int borderRadius = 30;

    void SetupUI();
    void SetupEvent();
    void UpdateSize();

  protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *event);

  public:
    OptionDialog(QWidget *parent = nullptr);

    void SetItems(QStringList items);

    void SetSelectedItem(int index);
    QString SelectedItem();
};

#endif // SETTINGSVIEW_H
