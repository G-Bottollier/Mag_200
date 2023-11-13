#ifndef PAGESMANAGER_H
#define PAGESMANAGER_H

#include <QWidget>

#include "MenuView.h"
#include "ImportView.h"
#include "ReplayView.h"
#include "LoadingView.h"
#include "StoragesView.h"
#include "SettingsView.h"
#include "ChannelsView.h"
#include "Chapter10View.h"
#include "DatePickerView.h"

#include "ToolbarManager.h"
#include "ChannelItemManager.h"

/*
 * @brief The PagesManager class represents a manager for the various pages in the application.
 *
 * This class is responsible for managing the interaction between different pages in the application,
 * initializing the GUI, setting up event connections, and handling input and output data.
 *
 * The PagesManager class inherits from QStackedWidget, a widget that provides a stack of widgets
 * where only one widget is visible at a time. This allows for easy switching between different pages.
 *
 * @date 31/05/2023
 */
class PagesManager : public QStackedWidget
{
  Q_OBJECT

  private:
    int m_OldPage = -1;

    int m_SubPage = -1;
    int m_MainPage = -1;
    bool m_UseFullPage = false;

    QMap<int, PageView*> Views;

    void SetupUI();
    void SetupEvent();

    void UpdateMainPage();
    bool PageModeChanged(int index);

    #pragma region OPEN_PAGE
    void SettingsViewOpen           (SettingsView         *Page);
    void MenuViewOpen               (MenuView             *Page);
    void ChannelsViewOpen           (ChannelsView         *Page);
    void ImportViewOpen             (ImportView           *Page);
    void ExportViewOpen             (PageView             *Page);
    void RecordViewOpen             (PageView             *Page);
    void ReplayViewOpen             (ReplayView           *Page);
    void StoragesViewOpen           (StoragesView         *Page);
    void Chapter10ViewOpen          (Chapter10View        *Page);
    void ChannelItemManagerOpen     (ChannelItemManager   *Page);
    void LoadingViewOpen            (LoadingView          *Page);
    void DatePickerViewOpen         (DatePickerView       *Page);
    void DateRangeViewOpen          (DatePickerView       *Page);
    #pragma endregion

    #pragma region CLOSE_PAGE
    void SettingsViewClose          (int               newPage);
    void MenuViewClose              (int               newPage);
    void ChannelsViewClose          (int               newPage);
    void ImportViewClose            (int               newPage);
    void ExportViewClose            (int               newPage);
    void RecordViewClose            (int               newPage);
    void ReplayViewClose            (int               newPage);
    void StoragesViewClose          (int               newPage);
    void Chapter10ViewClose         (int               newPage);
    void ChannelItemManagerClose    (int               newPage);
    void LoadingViewClose           (int               newPage);
    void DatePickerViewClose        (int               newPage);
    void DateRangeViewClose         (int               newPage);
    #pragma endregion

  protected:
    void resizeEvent(QResizeEvent *event);

  public:
    PagesManager(QWidget *parent = nullptr);
    ~PagesManager();

    bool UseFullPage();

  public slots:
    void NewChannelData(QVector<double> ptsX, QVector<double> ptsY);

    // Input status
    void NewStatus(int ChannelID, int Type, QVariant Value);

    void ChannelLedClicked(ChannelItem *item, QString ledText);
    void CurrentIndexChanged(int index);
    
    void LedClicked(int Page);

    void ClosePage();

    void StartReadChannelData(QString channel);
    void StopReadChannelData();

    void NewDrivesOpen();
    void NewOpenParentFolder();
    void NewFolderOpen(QFileInfo info);

    void NewCancelCopy();
    void NewCopyProgress(qint64 progress);

    void NewCopyFile(QString src, QString dst);

  signals:
    void UpdateChannelStatus(int inputIndex);
    void UpdateToolbarIndex(int index);

    void StartReadLog();
    void StopReadLog();

    void SendStartReadChannelData(QString channel);
    void SendStopReadChannelData();

    void SendUseFullPage(bool UseFullPage);

    void SendDrivesOpen();
    void SendFolderOpen(QFileInfo info);

    void SendCancelCopy();
    void SendCopyFile(QString src, QString dst);
};

#endif // PAGESMANAGER_H
