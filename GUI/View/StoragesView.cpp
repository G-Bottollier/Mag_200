#include "stdafx.h"
#include "StoragesView.h"

/*
 * @brief Constructor of the StoragesView
 *
 * @return void
 *
 * @date 31/05/2023
 */
StoragesView::StoragesView(QWidget * parent) : PageView(parent)
{
  // Setup UI 
  SetupUI();

  // Setup event handlers
  SetupEvent();
}

/*
 * @brief Destructor of the StoragesView
 *
 * @return void
 *
 * @date 31/05/2023
 */
StoragesView::~StoragesView()
{
}

/*
 * @brief Set up the user interface for the storages view.
 *
 * This function sets the header name for the storages view,
 * creates a grid layout for the storages view
 * Calls MountedVolumesUpdate function to update the mounted volumes.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void StoragesView::SetupUI()
{
  // Set the header name for the storages view
  this->UseLine(true);
  this->AddHeaderView();
  this->AddHeaderTitle();
  this->SetHeaderName(tr("PAGE_STK"));

  // Create a grid layout for the storages view
  GridLayout = new QGridLayout();
  GridLayout->setMargin(0);
  GridLayout->setSpacing(0);

  // Add the grid layout as the scrollable view for the storages view
  this->AddScrollView(GridLayout);

  // Update mounted volumes
  MountedVolumesUpdate();
}

/*
 * @brief Set up event connections for the storages view.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void StoragesView::SetupEvent()
{
  QObject::connect(qStorages, &StoragesManager::SendMountedVolumesUpdate, this, &StoragesView::MountedVolumesUpdate);
}

/*
 * @brief Update the mounted volumes in the storages view.
 *
 * This function retrieves the list of mounted volumes from qStorages.
 * updates the grid layout with the corresponding storage widgets.
 * and adds or removes items based on the mounted volumes.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void StoragesView::MountedVolumesUpdate()
{
  // Retrieve the list of mounted volumes from qStorages
  QList<QStorageInfo> storages = qStorages->MountedVolumes();

  // Get the default and alternate background colors from qThemes
  QColor defaultColor = qThemes->Color(QPalette::Base);
  QColor alternateColor = qThemes->Color(QPalette::AlternateBase);

  // Remove Spacer Item
  if(SpacerItem == nullptr)
    SpacerItem = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
  GridLayout->removeItem(SpacerItem);

  // Remove excess items from the grid layout
  for (int i = GridLayout->count(); i >= storages.count(); i--)
  {
    QLayoutItem *item = GridLayout->itemAt(i);

    if (item == nullptr)
      continue;
    if (item->widget() == nullptr)
      continue;

    GridLayout->removeItem(item);
    item->widget()->deleteLater();
  }

  // Replace existing items in the grid layout with updated ones
  for (int i = 0; i < GridLayout->count(); i++)
  {
    QLayoutItem *item = GridLayout->itemAt(i);

    if (item == nullptr)
      continue;
    if (item->widget() == nullptr)
      continue;

    // Get the background color based on the item's index
    QColor color = i % 2 == 0 ? defaultColor : alternateColor;

    // Get the storage widget for the corresponding storage info
    QWidget* storageWidget = GetStorageWidget(storages[i], color);

    // Replace the existing widget with the updated storage widget
    GridLayout->replaceWidget(item->widget(), storageWidget);
  }

  // Add new items to the grid layout
  for (int i = GridLayout->count(); i < storages.count(); i++)
  {
    // Get the background color based on the item's index
    QColor color = i % 2 == 0 ? defaultColor : alternateColor;

    // Get the storage widget for the corresponding storage info
    QWidget* storageWidget = GetStorageWidget(storages[i], color);

    // Add the storage widget to the grid layout
    GridLayout->addWidget(storageWidget, i, 0);
  }

  // Add Spacer Item
  GridLayout->addItem(SpacerItem, GridLayout->count(), 0);

  QResizeEvent *event = new QResizeEvent(this->size(), this->size());
  this->resizeEvent(event);
  delete event;
}

/*
 * @brief Create a widget for displaying storage information.
 *
 * This function creates a QWidget object to display the storage information.
 * It sets the appropriate style and layout for the widget.
 * Then returns the created widget.
 *
 * @param storageInfo The QStorageInfo object representing the storage.
 * @param background The background color for the storage widget.
 * @return The created widget for displaying storage information.
 *
 * @date 31/05/2023
 */
QWidget* StoragesView::GetStorageWidget(QStorageInfo storageInfo, QColor background)
{
  // Create a widget for displaying the storage info
  QWidget *storageWidget = new QWidget();

  // Set the background color for the storage widget
  QString strColor = "background:" + background.name();
  storageWidget->setStyleSheet(strColor);

  // Create the main layout for the storage widget
  QHBoxLayout *MainLayout = new QHBoxLayout();
  MainLayout->setSpacing(itemSpacing);
  MainLayout->setMargin(itemMargin);

  // Create the icon label for the storage widget
  Led *deviceIcon = new Led("Device");
  deviceIcon->UseBorder(false);
  deviceIcon->UseBackground(false);
  deviceIcon->SetIcon(qStorages->IconFromPath(storageInfo.rootPath()));

  // Create the layout for displaying storage information
  QVBoxLayout *InfoLayout = new QVBoxLayout();
  InfoLayout->setSpacing(itemSpacing);
  InfoLayout->setMargin(0);

  // Create the label for displaying the storage name
  QLabel *labelName = new QLabel();
  labelName->setText(StoragesManager::Name(storageInfo));

  // Calculate the available and total space in human-readable format
  int percentage = StoragesManager::AvailableSpace(storageInfo);
  QString StrMax = Tools::BitrateToString(storageInfo.bytesTotal(), false);
  QString StrFree = Tools::BitrateToString(storageInfo.bytesFree(), false);

  // Create the label for displaying the storage description
  QLabel *labelDesc = new QLabel();
  labelDesc->setText(" " + StrFree + tr("FREE_DISK_SPACE_ON") + StrMax);

  // Create the progress bar for displaying the disk space usage
  QProgressBar *diskSpace = new QProgressBar();
  diskSpace->setRange(0, 100);
  diskSpace->setValue(percentage);
  diskSpace->setTextVisible(false);

  // Set the highlight color for the progress bar
  QPalette p = diskSpace->palette();
  p.setColor(QPalette::Highlight, StoragesManager::AvailableSpaceColor(storageInfo));
  diskSpace->setPalette(p);
  diskSpace->setStyleSheet("background:white;");

  // Add the labels and progress bar to the info layout
  InfoLayout->addWidget(labelName);
  InfoLayout->addWidget(labelDesc);
  InfoLayout->addWidget(diskSpace);

  // Add the icon, info layout, and stretch to the main layout
  MainLayout->addWidget(deviceIcon);
  MainLayout->addLayout(InfoLayout);
  MainLayout->addStretch();

  // Set the main layout for the storage widget
  storageWidget->setLayout(MainLayout);

  return storageWidget;
}

void StoragesView::resizeEvent(QResizeEvent * event)
{
  PageView::resizeEvent(event);

  resizeStorages(event->size());
}

void StoragesView::resizeStorages(QSize size)
{
  int headerSize = this->HeaderSize().height();

  int width = Tools::GetWidgetWidth(this, size);
  int height = Tools::GetWidgetHeight(this, size);

  if (height == headerSize)
    return;

  height -= headerSize;

  int itemInView = 2;
  int itemWidth = qMax(0, width - 2);
  int itemHeight = height / itemInView;
  int itemHeightMargin = qMax(0, (itemHeight - (itemMargin * 2)));

  int count = GridLayout->count() - 1;
  this->SetScrollWidgetSize(QSize(itemWidth, itemHeight  * count));

  for (int i = 0; i < GridLayout->count(); i++)
  {
    QLayoutItem *item = GridLayout->itemAt(i);
    if (item == nullptr)
      continue;

    QWidget *widget = item->widget();
    if (widget == nullptr)
      continue;

    widget->setFixedSize(QSize(width, itemHeight));
    QHBoxLayout *mainLayout = dynamic_cast<QHBoxLayout*>(widget->layout());

    if (mainLayout == nullptr)
      continue;

    if (mainLayout->count() < 2)
      continue;

    int idx = 0;
    Led *icon = dynamic_cast     <Led*>(mainLayout->itemAt(idx++)->widget());
    QVBoxLayout *layoutInfo = dynamic_cast<QVBoxLayout*>(mainLayout->itemAt(idx++)->layout());

    QSize iconSize = QSize(itemHeightMargin, itemHeightMargin);
    icon->SetSize(iconSize);

    if (layoutInfo->count() < 3)
      continue;

    idx = 0;
    int infoHeight = itemHeightMargin - (itemSpacing * (layoutInfo->count() - 1));

    int lineHeight = infoHeight / layoutInfo->count();

    
    QLabel *labelName = dynamic_cast<QLabel*>(layoutInfo->itemAt(idx++)->widget());
    QLabel *labelDesc = dynamic_cast<QLabel*>(layoutInfo->itemAt(idx++)->widget());
    QProgressBar *diskSpace = dynamic_cast<QProgressBar*>(layoutInfo->itemAt(idx++)->widget());

    QFont font = this->font();
    font.setBold(true);
    font.setPixelSize(lineHeight * 0.8);

    labelName->setFont(font);
    labelDesc->setFont(font);
    diskSpace->setFixedHeight(lineHeight);
  }
}
