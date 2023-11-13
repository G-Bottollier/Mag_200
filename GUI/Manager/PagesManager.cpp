#include "stdafx.h"
#include "PagesManager.h"


// CASTED POINTER
#define pSETTINGS       static_cast< SettingsView        *>(Views[PAGE_SETTINGS])
#define pMENU           static_cast< MenuView            *>(Views[PAGE_MENU])
#define pCHANNELS       static_cast< ChannelsView        *>(Views[PAGE_CHANNELS])
#define pIMPORT         static_cast< ImportView          *>(Views[PAGE_IMPORT])
#define pEXPORT         static_cast< PageView            *>(Views[PAGE_EXPORT])
#define pRECORD         static_cast< PageView            *>(Views[PAGE_RECORD])
#define pREPLAY         static_cast< ReplayView          *>(Views[PAGE_REPLAY])
#define pSTK            static_cast< StoragesView        *>(Views[PAGE_STORAGES])
#define pCH10           static_cast< Chapter10View       *>(Views[PAGE_CH10])
#define pCHANNEL        static_cast< ChannelItemManager  *>(Views[PAGE_CHANNEL])
#define pLOADING        static_cast< LoadingView         *>(Views[PAGE_LOADING])
#define pDATE_PICKER    static_cast< DatePickerView      *>(Views[PAGE_DATE_PICKER])
#define pRANGE_PICKER   static_cast< DatePickerView      *>(Views[PAGE_RANGE_PICKER])

/*
 * @brief Constructs a PagesManager object with the specified parent.
 *
 * @param parent The parent widget.
 *
 * @date 31/05/2023
 */
PagesManager::PagesManager(QWidget *parent) : QStackedWidget(parent)
{
  // Setup UI 
  SetupUI();

  // Setup event handlers
  SetupEvent();
}

/*
 * @brief Destroys the PagesManager object.
 *
 * @date 31/05/2023
 */
PagesManager::~PagesManager()
{

}

/*
 * @brief Sets up the UI by creating and initializing the various views and item views.
 *
 * @date 31/05/2023
 */
void PagesManager::SetupUI()
{
  // Create and initialize the various views and item views

  // PAGE 0, 1: Settings, Menu
  Views[PAGE_SETTINGS] = new SettingsView();
  Views[PAGE_MENU] = new MenuView();

  // PAGE 2, 3, 4, 5, 6: DPS, TGS, RnR MainView
  Views[PAGE_CHANNELS] = new ChannelsView();
  Views[PAGE_IMPORT] = new ImportView();
  Views[PAGE_EXPORT] = new PageView();
  Views[PAGE_RECORD] = new PageView();
  Views[PAGE_REPLAY] = new ReplayView();

  // PAGE 7, 8, 9, 10, 11: SubView
  Views[PAGE_STORAGES] = new StoragesView();
  Views[PAGE_CH10] = new Chapter10View();
  Views[PAGE_CHANNEL] = new ChannelItemManager();
  Views[PAGE_LOADING] = new LoadingView();
  Views[PAGE_DATE_PICKER] = new DatePickerView(DATE_PICKER);
  Views[PAGE_RANGE_PICKER] = new DatePickerView(RANGE_PICKER);

  // Add the views and item views to the PagesManager
  for(PageView* View : Views)
    this->addWidget(View);

  this->UpdateMainPage();

  this->setCurrentIndex(this->m_MainPage);
  this->CurrentIndexChanged(this->m_MainPage);
}

/*
 * @brief Sets up the event handlers for signal-slot connections.
 *
 * @date 31/05/2023
 */
void PagesManager::SetupEvent()
{
  QObject::connect(this, &QStackedWidget::currentChanged, this, &PagesManager::CurrentIndexChanged);

  for (PageView* View : Views)
  {
    QObject::connect(View, &PageView::SendLedClicked, this, &PagesManager::LedClicked);
    QObject::connect(View, &PageView::SendCloseClicked, this, &PagesManager::ClosePage);
  }

  QObject::connect(pCHANNELS, &ChannelsView::SendChannelLedClicked, this, &PagesManager::ChannelLedClicked);
  QObject::connect(pCHANNELS, &ChannelsView::SendChannelLedClicked, pCHANNEL, &ChannelItemManager::ChannelLedClicked);

  QObject::connect(pCHANNEL, &ChannelItemManager::SendStopReadChannelData, this, &PagesManager::StopReadChannelData);
  QObject::connect(pCHANNEL, &ChannelItemManager::SendStartReadChannelData, this, &PagesManager::StartReadChannelData);

  QObject::connect(pIMPORT, &ImportView::SendDrivesOpen, this, &PagesManager::NewDrivesOpen);
  QObject::connect(pIMPORT, &ImportView::SendFolderOpen, this, &PagesManager::NewFolderOpen);

  QObject::connect(pLOADING, &LoadingView::SendCancelCopy, this, &PagesManager::NewCancelCopy);
  QObject::connect(pLOADING, &LoadingView::SendCopyFile, this, &PagesManager::NewCopyFile);
}

void PagesManager::resizeEvent(QResizeEvent * event)
{
  for (int i = 0; i < this->count(); i++)
  {
    QWidget *widget = this->widget(i);

    if (widget == nullptr)
      continue;

    widget->setFixedSize(event->size());
  }
}

void PagesManager::NewStatus(int ChannelID, int Type, QVariant Value)
 {
   pCHANNELS->NewStatus(ChannelID, Type, Value);
 }

/*
 * @brief Adds channel data to the Identification page view.
 *
 * @param ptsX The x-coordinates of the data points.
 * @param ptsY The y-coordinates of the data points.
 *
 * @date 31/05/2023
 */
void PagesManager::NewChannelData(QVector<double> ptsX, QVector<double> ptsY)
{
  // Call the corresponding function in the Identification page view to add channel data
  pCHANNEL->NewChannelData(ptsX, ptsY);
}

/*
 * @brief Handles the change in current index.
 *
 * Closes the old index and opens the new index, performing necessary actions for each page.
 *
 * @param index The new current index.
 *
 * @date 31/05/2023
 */
void PagesManager::CurrentIndexChanged(int newPage)
{
  // CLOSE OLD INDEX
  switch (m_OldPage) 
  {
    default: break;
    case PAGE_SETTINGS:       SettingsViewClose         (newPage);  break;
    case PAGE_MENU:           MenuViewClose             (newPage);  break;
    case PAGE_CHANNELS:       ChannelsViewClose         (newPage);  break;
    case PAGE_IMPORT:         ImportViewClose           (newPage);  break;
    case PAGE_EXPORT:         ExportViewClose           (newPage);  break;
    case PAGE_RECORD:         RecordViewClose           (newPage);  break;
    case PAGE_REPLAY:         ReplayViewClose           (newPage);  break;
    case PAGE_STORAGES:       StoragesViewClose         (newPage);  break;
    case PAGE_CH10:           Chapter10ViewClose        (newPage);  break;
    case PAGE_CHANNEL:        ChannelItemManagerClose   (newPage);  break;
    case PAGE_LOADING:        LoadingViewClose          (newPage);  break;
    case PAGE_DATE_PICKER:    DatePickerViewClose       (newPage);  break;
    case PAGE_RANGE_PICKER:   DateRangeViewClose        (newPage);  break;
  }

  // OPEN NEW INDEX
  switch (newPage)
  {
    default: break;
    case PAGE_SETTINGS:       SettingsViewOpen          (pSETTINGS);      break;
    case PAGE_MENU:           MenuViewOpen              (pMENU);          break;
    case PAGE_CHANNELS:       ChannelsViewOpen          (pCHANNELS);      break;
    case PAGE_IMPORT:         ImportViewOpen            (pIMPORT);        break;
    case PAGE_EXPORT:         ExportViewOpen            (pEXPORT);        break;
    case PAGE_RECORD:         RecordViewOpen            (pRECORD);        break;
    case PAGE_REPLAY:         ReplayViewOpen            (pREPLAY);        break;
    case PAGE_STORAGES:       StoragesViewOpen          (pSTK);           break;
    case PAGE_CH10:           Chapter10ViewOpen         (pCH10);          break;
    case PAGE_CHANNEL:        ChannelItemManagerOpen    (pCHANNEL);       break;
    case PAGE_LOADING:        LoadingViewOpen           (pLOADING);       break;
    case PAGE_DATE_PICKER:    DatePickerViewOpen        (pDATE_PICKER);   break;
    case PAGE_RANGE_PICKER:   DateRangeViewOpen         (pRANGE_PICKER);  break;
  }

  if (this->PageModeChanged(newPage) == true)
    emit SendUseFullPage(this->m_UseFullPage);

  // Store the current index as the old index for the next iteration
  m_OldPage = newPage;
}

void PagesManager::UpdateMainPage()
{
  QStringList types = qSettings->Mag200_Types();
  QString type = qSettings->Mag200_Type();

  int index = types.indexOf(type);

  switch (index)
  {
    default:            this->m_MainPage = -1;            return;
    case MAG200_DPS:    this->m_MainPage = PAGE_CHANNELS;   return;
    case MAG200_TGS:    this->m_MainPage = PAGE_CHANNELS;   return;
    case MAG200_RnR:    this->m_MainPage = PAGE_MENU;     return;
  }
}

bool PagesManager::PageModeChanged(int index)
{
  PageView *view = static_cast<PageView*>(this->widget(index));
  if (view == nullptr)
    return false;

  bool mode = view->UseFullPage();

  if (mode == this->m_UseFullPage)
    return false;

  this->m_UseFullPage = mode;
  return true;
}

void PagesManager::LedClicked(int Page)
{
  this->setCurrentIndex(Page);
}

/*
* @brief Closes the current page by setting the current index to PAGE_INPUTS.
*
* @date 31/05/2023
*/
void PagesManager::ClosePage()
{
  int page = this->m_SubPage == -1 ? this->m_MainPage : this->m_SubPage;
  this->setCurrentIndex(page);
}

bool PagesManager::UseFullPage()
{
  return this->m_UseFullPage;
}

#pragma region PAGE_SETTINGS

void PagesManager::SettingsViewOpen(SettingsView * Page)
{
  // Called after closing the Old Page; Opens this Page
}

void PagesManager::SettingsViewClose(int newPage)
{
  // Called before opening a New Page; Closes this Page
  switch (newPage)
  {
    default: break;
  }
}

#pragma endregion

#pragma region PAGE_MENU

void PagesManager::MenuViewOpen(MenuView * Page)
{
  // Called after closing the Old Page; Opens this Page

  this->m_MainPage = PAGE_MENU;
}

void PagesManager::MenuViewClose(int newPage)
{
  // Called before opening a New Page; Closes this Page
  switch (newPage)
  {
    default: break;
  }
}

#pragma endregion

#pragma region PAGE_CHANNELS

void PagesManager::ChannelsViewOpen(ChannelsView * Page)
{
  // Called after closing the Old Page; Opens this Page

  if(m_MainPage == PAGE_REPLAY)
    Page->SetHeaderVisible(true);
}

void PagesManager::ChannelsViewClose(int newPage)
{
  // Called before opening a New Page; Closes this Page
  switch (newPage)
  {
    default: pCHANNELS->SetHeaderVisible(false); break;
  }
}

void PagesManager::ChannelLedClicked(ChannelItem *item, QString ledText)
{
  pCHANNEL->SetChannelItem(item, ledText);
  this->setCurrentIndex(PAGE_CHANNEL);
}

#pragma endregion

#pragma region PAGE_IMPORT

void PagesManager::ImportViewOpen(ImportView * Page)
{
  // Called after closing the Old Page; Opens this Page

  this->m_MainPage = PAGE_IMPORT;
  emit UpdateToolbarIndex(TOOLBAR_IMP);
}

void PagesManager::ImportViewClose(int newPage)
{
  // Called before opening a New Page; Closes this Page
  switch (newPage)
  {
    default:              pIMPORT->OpenDrives();                          break;
    case PAGE_LOADING:    pLOADING->StartCopy(pIMPORT->ImportPath());     break;
  }
}

// Slots
void PagesManager::NewOpenParentFolder()                  { pIMPORT->OpenParentFolder();                  }

// Signals
void PagesManager::NewDrivesOpen()                        { emit this->SendDrivesOpen();                  }
void PagesManager::NewFolderOpen(QFileInfo info)          { emit this->SendFolderOpen(info);              }

#pragma endregion

#pragma region PAGE_EXPORT

void PagesManager::ExportViewOpen(PageView * Page)
{
  // Called after closing the Old Page; Opens this Page

  this->m_MainPage = PAGE_EXPORT;
  emit UpdateToolbarIndex(TOOLBAR_EXP);
}

void PagesManager::ExportViewClose(int newPage)
{
  // Called before opening a New Page; Closes this Page
  switch (newPage)
  {
    default: break;
  }
}

#pragma endregion

#pragma region PAGE_RECORD

void PagesManager::RecordViewOpen(PageView * Page)
{
  // Called after closing the Old Page; Opens this Page

  this->m_MainPage = PAGE_CHANNELS;
  this->setCurrentIndex(PAGE_CHANNELS); // TEMP
  emit UpdateToolbarIndex(TOOLBAR_REC);
}

void PagesManager::RecordViewClose(int newPage)
{
  // Called before opening a New Page; Closes this Page
  switch (newPage)
  {
  default: break;
  }
}

#pragma endregion

#pragma region PAGE_REPLAY

void PagesManager::ReplayViewOpen(ReplayView * Page)
{
  // Called after closing the Old Page; Opens this Page

  // (TEMP_DEMO) To get file start date & end date
  qCommands->NewCommand(CMD_FILES);

  this->m_MainPage = PAGE_REPLAY;
  emit UpdateToolbarIndex(TOOLBAR_REP);
}

void PagesManager::ReplayViewClose(int newPage)
{
  // Called before opening a New Page; Closes this Page
  switch (newPage)
  {
    default: break;
  }
}

#pragma endregion

#pragma region PAGE_STORAGES

void PagesManager::StoragesViewOpen(StoragesView * Page)
{
  // Called after closing the Old Page; Opens this Page

  this->m_SubPage = PAGE_NONE;
}

void PagesManager::StoragesViewClose(int newPage)
{
  // Called before opening a New Page; Closes this Page
  switch (newPage)
  {
    default: break;
  }
}

#pragma endregion

#pragma region PAGE_CH10

void PagesManager::Chapter10ViewOpen(Chapter10View * Page)
{
  // Called after closing the Old Page; Opens this Page

  this->m_SubPage = PAGE_NONE;
}

void PagesManager::Chapter10ViewClose(int newPage)
{
  // Called before opening a New Page; Closes this Page
  switch (newPage)
  {
    default: break;
  }
}

#pragma endregion

#pragma region PAGE_CHANNEL

void PagesManager::ChannelItemManagerOpen(ChannelItemManager * Page)
{
  // Called after closing the Old Page; Opens this Page

  this->m_SubPage = PAGE_CHANNELS;
}

void PagesManager::ChannelItemManagerClose(int newPage)
{
  // Called before opening a New Page; Closes this Page
  switch (newPage)
  {
    default:
      pCHANNELS->AddChannelItem(pCHANNEL->RemoveChannelItem());
      this->m_SubPage = PAGE_NONE;

      break;
  }
}

void PagesManager::StartReadChannelData(QString channel)  { emit this->SendStartReadChannelData(channel); }
void PagesManager::StopReadChannelData()                  { emit this->SendStopReadChannelData();         }

#pragma endregion

#pragma region PAGE_LOADING

void PagesManager::LoadingViewOpen(LoadingView * Page)
{
  // Called after closing the Old Page; Opens this Page
}

void PagesManager::LoadingViewClose(int newPage)
{
  // Called before opening a New Page; Closes this Page
  switch (newPage)
  {
  default: break;
  }
}

// Slots
void PagesManager::NewCopyProgress(qint64 progress)       { pLOADING->UpdateCopy(progress); }

// Signals
void PagesManager::NewCancelCopy()                        { emit SendCancelCopy();          }
void PagesManager::NewCopyFile(QString src, QString dst)  { emit SendCopyFile(src, dst);    }

#pragma endregion

#pragma region PAGE_DATE_PICKER

void PagesManager::DatePickerViewOpen(DatePickerView * Page)
{
  // Called after closing the Old Page; Opens this Page
  qDebug() << pREPLAY->Date();
  Page->SetDateRange(pREPLAY->LowerBoundDate(), pREPLAY->UpperBoundDate());
  Page->SetDate(pREPLAY->Date());
}

void PagesManager::DatePickerViewClose(int newPage)
{
  // Called before opening a New Page; Closes this Page
  switch (newPage)
  {
    default:                                                    break;
    case PAGE_REPLAY: pREPLAY->SetDate(pDATE_PICKER->Date());   break;
  }
}

#pragma endregion

#pragma region PAGE_RANGE_PICKER

void PagesManager::DateRangeViewOpen(DatePickerView * Page)
{
  // Called after closing the Old Page; Opens this Page

  Page->SetDateRange(pREPLAY->StartDate(), pREPLAY->EndDate());
  Page->SetBoundRange(pREPLAY->LowerBoundDate(), pREPLAY->UpperBoundDate());
}

void PagesManager::DateRangeViewClose(int newPage)
{
  // Called before opening a New Page; Closes this Page
  switch (newPage)
  {
    default:                                                                                                      break;
    case PAGE_REPLAY: pREPLAY->SetBoundRange(pRANGE_PICKER->LowerBoundDate(), pRANGE_PICKER->UpperBoundDate());   break;
  }
}

#pragma endregion
