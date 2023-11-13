#include "stdafx.h"
#include "ImportView.h"

#define THIS_PC   0
#define HOME_APP  1

// =======================================================
// Constructor and Destructor Methods
// =======================================================

ImportView::ImportView(QWidget *parent) : PageView(parent)
{
  qRegisterMetaType<QStorageInfo>();

  SetupUI();
  SetupEvent();
}

ImportView::~ImportView()
{

}


// =======================================================
// Setup Methods
// =======================================================

void ImportView::SetupUI()
{
  QWidget *widget = new QWidget();

  QHBoxLayout *MainLayout = new QHBoxLayout();
  MainLayout->setMargin(0);
  MainLayout->setSpacing(0);

  QColor color = qThemes->Color(QPalette::Text);
  QString style = "QFrame { border-top: 5px solid " + color.name() + "; }";

  // Menu
  menuList = new QListWidget();
  menuList->setStyleSheet(style);
  menuList->setMouseTracking(true);
  menuList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  menuList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  menuList->setSelectionMode(QAbstractItemView::SingleSelection);
  menuList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

  QPalette palette = menuList->palette();
  palette.setColor(QPalette::Base, qThemes->Color(QPalette::Window));
  menuList->setPalette(palette);

  // File explorer
  fileList = new QListWidget();
  fileList->setStyleSheet(style); 
  fileList->setMouseTracking(true);
  fileList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  fileList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  fileList->setSelectionMode(QAbstractItemView::SingleSelection);
  fileList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
  
  menuList->viewport()->installEventFilter(this);
  fileList->viewport()->installEventFilter(this);

  // Delegate
  menuButton = new MenuButtonDelegate();
  menuList->setItemDelegate(menuButton);
  
  entryInfo = new EntryInfoDelegate();
  fileList->setItemDelegate(entryInfo);

  // Add menu items
  QStringList list;
  list.append("PC_0.png");
  list.append("HOME_0.png");
  
  // Add new items if needed
  for (int i = 0; i < list.size(); i++)
  {
    QListWidgetItem* item = new QListWidgetItem();
    item->setData(Qt::UserRole, QVariant::fromValue(list[i]));
    menuList->addItem(item);
  }

  // Add Watcher
  folderWatcher = new QFileSystemWatcher(this);

  this->AddGrabGesture(fileList);

  MainLayout->addWidget(menuList);
  MainLayout->addWidget(fileList);

  widget->setLayout(MainLayout);

  this->AddMainWidget(widget);
}

void ImportView::SetupEvent()
{
  connect(menuButton, &MenuButtonDelegate::SendMenuClicked, this, &ImportView::NewMenuClicked);
  connect(entryInfo, &EntryInfoDelegate::SendOpenFolder, this, &ImportView::OpenFolder);
  connect(entryInfo, &EntryInfoDelegate::SendImportFile, this, &ImportView::NewImportFile);

  connect(folderWatcher, &QFileSystemWatcher::directoryChanged, this, &ImportView::DirectoryChanged);
  connect(qStorages, &StoragesManager::SendMountedVolumesUpdate, this, &ImportView::NewMountedVolumesUpdate);
}


// =======================================================
// Event Handling Methods
// =======================================================

void ImportView::resizeEvent(QResizeEvent * event)
{
  QSize size = event->size();

  int headerSize = this->HeaderSize().height();

  int width = Tools::GetWidgetWidth(this, size);
  int height = Tools::GetWidgetHeight(this, size);

  if (height == headerSize)
    return;

  height -= headerSize;

  qreal ratioW = 1.0;

  int availableWidth = width;

  int deviceWidth = Tools::GetWidgetWidth(0.1, width, availableWidth, ratioW);

  menuList->setFixedSize(deviceWidth, height);
  fileList->setFixedSize(availableWidth, height);

  QFont font = fileList->font();
  font.setPixelSize(height * 0.07);
  font.setBold(true);
  fileList->setFont(font);
  menuList->setFont(font);
}

bool ImportView::eventFilter(QObject *obj, QEvent *event)
{
  switch (event->type())
  {
    default:
      return QObject::eventFilter(obj, event);

    case QEvent::MouseButtonRelease:
      break;
  }

  entryInfo->MouseReleased(fileList, event);
  menuButton->MouseReleased(menuList, event);

  return QObject::eventFilter(obj, event);
}


// =======================================================
// Menu Management
// =======================================================

void ImportView::NewMenuClicked(int index)
{
  switch (index)
  {
  default:
    return;

  case THIS_PC:     this->OpenDrives();                       return;
  case HOME_APP:    this->OpenFolder(qSettings->Root_Path());  return;
  }
}


// =======================================================
// Entry Management
// =======================================================

void ImportView::UpdateWatcher(QString oldPath, QString newPath)
{
  QStringList watching = folderWatcher->directories();

  if (oldPath == newPath)
    if (watching.contains(oldPath))
      return;

  if (watching.contains(oldPath) == true)
  {
    bool isRemoved = folderWatcher->removePath(oldPath);
    if (isRemoved == false)
    {
      // Disconnect and delete old watcher
      folderWatcher->disconnect();
      folderWatcher->deleteLater();

      // Create new watcher
      folderWatcher = new QFileSystemWatcher();
      QObject::connect(folderWatcher, &QFileSystemWatcher::directoryChanged, this, &ImportView::DirectoryChanged);

      // Update new watcher path
      folderWatcher->addPath(newPath);
      watching = folderWatcher->directories();
    }
  }

  if (watching.contains(newPath) == true)
    return;

  bool isAdded = folderWatcher->addPath(newPath);
  if (isAdded == true)
    return;

  qDebug() << "failed to add directory !!!";
}

void ImportView::AddDotDot(QFileInfoList &list, QFileInfo info)
{
  QString path = info.absoluteFilePath();

  // If already exist
  if (list.size() != 0 && list.first().fileName() == "..")
    return;

  // Add DotDot
  list.append(QFileInfo(path + "/.."));
  return;
}

void ImportView::DirectoryChanged(QString oldPath)
{
  QString newPath = Tools::NearestParentPath(oldPath);

  UpdateWatcher(oldPath, newPath);

  this->OpenFolder(QFileInfo(newPath));
}

void ImportView::OpenFolder(QFileInfo info)
{
  if (info.isDir() == false)
    return;

  fileList->verticalScrollBar()->setValue(0);

  // Open drives when info equal C:/..
  if (HasToOpenDrives(info) == true)
    return;

  UpdateWatcher(this->current.absoluteFilePath(), info.absoluteFilePath());

  this->current = info;
  QFileInfoList list = QDir(info.absoluteFilePath()).entryInfoList(QDir::AllEntries | QDir::NoDot | QDir::Readable);

  // Add ".." as drives when info equal C:/
  AddDotDot(list, info);

  qSort(list.begin(), list.end(), Tools::CompareFileInfo);

  int currentItemCount = fileList->count();
  int newEntryCount = list.size();

  // Update existing items
  for (int i = 0; i < qMin(currentItemCount, newEntryCount); i++)
  {
    QFileInfo itemInfo = list[i];
    QListWidgetItem* item = fileList->item(i);
    item->setData(Qt::UserRole, QVariant::fromValue(QFileInfo()));
    item->setData(Qt::UserRole, QVariant::fromValue(itemInfo));
  }

  // Add new items if needed
  for (int i = currentItemCount; i < newEntryCount; i++)
  {
    QFileInfo itemInfo = list[i];
    QListWidgetItem* item = new QListWidgetItem();
    item->setData(Qt::UserRole, QVariant::fromValue(itemInfo));
    fileList->addItem(item);
  }

  // Delete unnecessary items
  for (int i = currentItemCount - 1; i >= newEntryCount; i--)
    delete fileList->takeItem(i);

  emit SendFolderOpen(this->current);
}

void ImportView::OpenParentFolder()
{
  QString name = this->current.fileName();
  QString path = this->current.absolutePath();

  // If drive letter add DotDot
  // Drive letter have an empty fileName
  if (name.isEmpty())
    path += "..";

  this->OpenFolder(QFileInfo(path));
}

// =======================================================
// Volume Management
// =======================================================

bool ImportView::HasToOpenDrives(QFileInfo info)
{
  QString path = this->current.absoluteFilePath();
  QStorageInfo root(path);

  if (root.rootPath() != path)
    return false;

  if (info.fileName() != "..")
    return false;

  OpenDrives();
  return true;
}

void ImportView::OpenDrives()
{
  fileList->verticalScrollBar()->setValue(0);

  this->current = QFileInfo();
  QFileInfoList list = QDir::drives();

  fileList->clear();
  for (int i = 0; i < list.size(); i++)
  {
    QFileInfo info = list[i];
    QListWidgetItem* item = new QListWidgetItem();
    item->setData(Qt::UserRole, QVariant::fromValue(info));

    fileList->addItem(item);
  }

  emit SendDrivesOpen();
}

void ImportView::NewMountedVolumesUpdate()
{
  if (this->current.exists() == true)
    return;

  this->OpenDrives();
}


// =======================================================
// Import / Export Management
// =======================================================

void ImportView::NewImportFile(QFileInfo info)
{
  this->import = info;
  this->SendLedClicked(PAGE_LOADING);
}


// =======================================================
// Getter
// =======================================================

QString ImportView::ImportPath()
{
  return this->import.absoluteFilePath();
}


/******************************/
/**   CUSTOM ITEM DELEGATE   **/
/******************************/

// =======================================================
// EntryInfoDelegate
// =======================================================
EntryInfoDelegate::EntryInfoDelegate(QObject *parent) : QItemDelegate(parent) 
{

}

void EntryInfoDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  if (!index.isValid())
    return;

  // Extract the QFileInfo from the model's user role
  QFileInfo Info = index.data(Qt::UserRole).value<QFileInfo>();

  if (!Info.exists())
    return;

  // Render icon
  QRectF Item = getItem(option.rect);

  Item = drawIcon(painter, Info, Item);
  Item = drawButton(painter, Info, Item, index);
  Item = drawFileSize(painter, Info, Item);
  Item = drawFileDate(painter, Info, Item);
  Item = drawFileName(painter, Info, Item);
}

bool EntryInfoDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
  QMouseEvent *mouseEvent = nullptr;
  QFileInfo Info;
  int y;

  switch (event->type())
  {
    default:
      return QItemDelegate::editorEvent(event, model, option, index);

    case QEvent::MouseButtonPress:
      mouseEvent = static_cast<QMouseEvent*>(event);
      mousePos = mouseEvent->pos();

      row = index.row();
      isPressed = true;
      isReleased = false;

      if(option.widget)
        const_cast<QWidget *>(option.widget)->repaint();

      return true;

    case QEvent::MouseMove:
      event->ignore();
      return false;

    case QEvent::MouseButtonRelease:
      mouseEvent = static_cast<QMouseEvent*>(event);
      mousePos = mouseEvent->pos();

      isPressed = false;
      isReleased = true;

      if (option.widget)
        const_cast<QWidget *>(option.widget)->repaint();

      return true;
  }

  return QItemDelegate::editorEvent(event, model, option, index);
}

void EntryInfoDelegate::MouseReleased(QObject * obj, QEvent *event)
{
  if (isPressed == false)
    return;

  QListWidget *widget = static_cast<QListWidget*>(obj);
  if (widget == nullptr)
    return;

  QStyleOptionViewItem option;
  option.widget = widget;

  QModelIndex index = widget->indexAt(mousePos);

  this->editorEvent(event, widget->model(), option, index);
}

QSize EntryInfoDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QSize size = QItemDelegate::sizeHint(option, index);

  const QWidget* parentWidget = option.widget;
  if (parentWidget == nullptr)
    return size;

  return QSize(size.width(), parentWidget->height() * 0.25);
}

QRectF EntryInfoDelegate::drawIcon(QPainter *Paint, QFileInfo Info, QRectF Item) const
{
  double width = Item.height();
  double  height = Item.height();
  
  double  x = Item.x();
  double  y = Item.y() + (Item.height() - height) / 2;

  QSize iconSize(width, height);

  QString filePath = Info.absoluteFilePath();
  QPixmap pixmap = qStorages->PixmapFromPath(filePath, iconSize);

  QSize pix = pixmap.size();
  if(pixmap.size().width() < width && pixmap.size().height() < height)
    iconSize = pixmap.size();

  QPoint center = QRect(x, y, width, height).center() - pixmap.rect().center();

  // Draw the pixmap
  Paint->drawPixmap(QRect(center, iconSize), pixmap);

  return Item.adjusted(width + spacing, 0, 0, 0);
}

QRectF EntryInfoDelegate::drawButton(QPainter *Paint, QFileInfo Info, QRectF Item, const QModelIndex &index) const
{
  double width = Item.height() * 0.85;
  double height = Item.height() * 0.85;

  double x = Item.x() + Item.width() - width;
  double y = Item.y() + (Item.height() - height) / 2;

  QString IconName = "NEXT_0.png";
  Color color = Color::GRAY_03;
  QString extension = Info.suffix();

  bool isCh10 = filter.contains(Info.suffix());
  bool isDir = Info.isDir();

  if (isDir == false)
  {
    if (isCh10 == true)
    {
      IconName = "IMPORT_0.png";
      color = (Color)0x2A82DA;//0x366bba;
    }
    else
      return Item.adjusted(0, 0, -width - spacing, 0);
  }

  QStyleOptionButton btnOpt;
  QColor penColor(color);
  QColor brushColor(color);

  QRect btnRect(x, y, height, height);

  if (isPressed && btnRect.contains(mousePos))
  {
    brushColor = Qt::transparent;
    btnRect.moveTopLeft(btnRect.topLeft() + QPoint(2, 2));
  }


  if (isReleased && btnRect.contains(mousePos))
  {
    if (index.row() == row)
    {
      row = -1;
      isReleased = false;

      if (isDir == true)    emit const_cast<EntryInfoDelegate*>(this)->SendOpenFolder(Info);
      if (isCh10 == true)   emit const_cast<EntryInfoDelegate*>(this)->SendImportFile(Info);
    }
  }

  btnOpt.rect = btnRect;
  btnOpt.iconSize = QSize(width, height);
  btnOpt.features |= QStyleOptionButton::Flat;
  btnOpt.icon = QIcon(qThemes->Icons(IconName));

  Paint->setBrush(brushColor);
  Paint->setPen(QPen(penColor, 15));
  Paint->drawRoundRect(btnOpt.rect);

  QApplication::style()->drawControl(QStyle::CE_PushButton, &btnOpt, Paint);

  return Item.adjusted(0, 0, - width - spacing, 0);
}

QRectF EntryInfoDelegate::drawFileSize(QPainter *Paint, QFileInfo Info, QRectF Item) const
{
  QFontMetrics metrics(Paint->font());

  double width = metrics.width("999 MB");
  double height = Item.height();

  double x = Item.x() + Item.width() - width;
  double y = Item.y();

  if (Info.isDir() == true)
    return Item.adjusted(0, 0, - width - spacing, 0);

  qint64 size = Info.size();
  QString text = Tools::BitrateToString(size, false, 0);

  Paint->setPen(qThemes->Color(QPalette::Text));
  Paint->drawText(QRectF(x, y, width, height), Qt::AlignCenter, text);

  return Item.adjusted(0, 0, - width - spacing, 0);
}

QRectF EntryInfoDelegate::drawFileDate(QPainter *Paint, QFileInfo Info, QRectF Item) const
{
  QFontMetrics metrics(Paint->font());

  QLocale local = QLocale::system();
  QString dateFormat = local.dateFormat(QLocale::ShortFormat);

  dateFormat.replace("yyyy", "yy");
  dateFormat.replace("MM", "MM");
  dateFormat.replace("dd", "dd");
  QString dateMetrics = dateFormat;

  for (int i = 0; i < dateMetrics.size(); i++)
  {
    if (dateMetrics[i].isLetter())
      dateMetrics[i] = '9';
  }

  double width = metrics.width(dateMetrics);
  double height = Item.height();

  double x = Item.x() + Item.width() - width;
  double y = Item.y();

  QString text = Info.created().toString(dateFormat);

  Paint->setPen(qThemes->Color(QPalette::Text));
  Paint->drawText(QRectF(x, y, width, height), Qt::AlignCenter, text);

  return Item.adjusted(0, 0, - width - spacing, 0);
}

QRectF EntryInfoDelegate::drawFileName(QPainter *Paint, QFileInfo Info, QRectF Item) const
{
  QString fileName = Info.fileName();
  if(fileName.isEmpty())
    fileName = Info.absolutePath();

  Paint->setPen(qThemes->Color(QPalette::Text));
  Paint->drawText(Item, Qt::AlignLeft | Qt::AlignVCenter, fileName);

  return Item.adjusted(0, 0, - Item.width(), 0);
}

QRectF EntryInfoDelegate::getItem(QRectF Item) const
{
  int x = Item.x() + margin;
  int y = Item.y() + margin;
  int width = Item.width() - (margin * 2);
  int height = Item.height() - margin;

  return QRectF(x, y, width, height);
}


// =======================================================
// MenuButtonDelegate
// =======================================================
MenuButtonDelegate::MenuButtonDelegate(QObject * parent) : QItemDelegate(parent)
{

}

void MenuButtonDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  if (!index.isValid())
    return;

  // Render icon
  QRectF Item = getItem(option.rect);
  
  Item = drawButton(painter, Item, index);
}

bool MenuButtonDelegate::editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index)
{
  QMouseEvent *mouseEvent = nullptr;
  int y;

  switch (event->type())
  {
  default:
    return QItemDelegate::editorEvent(event, model, option, index);

  case QEvent::MouseButtonPress:
    mouseEvent = static_cast<QMouseEvent*>(event);
    mousePos = mouseEvent->pos();

    row = index.row();
    isPressed = true;
    isReleased = false;

    if (option.widget)
      const_cast<QWidget *>(option.widget)->repaint();

    return true;

  case QEvent::MouseButtonRelease:
    mouseEvent = static_cast<QMouseEvent*>(event);
    mousePos = mouseEvent->pos();

    isPressed = false;
    isReleased = true;

    if (option.widget)
      const_cast<QWidget *>(option.widget)->repaint();

    return true;
  }

  return QItemDelegate::editorEvent(event, model, option, index);
}

void MenuButtonDelegate::MouseReleased(QObject *obj, QEvent *event)
{
  if (isPressed == false)
    return;

  QListWidget *widget = static_cast<QListWidget*>(obj);
  if (widget == nullptr)
    return;

  QStyleOptionViewItem option;
  option.widget = widget;

  QModelIndex index = widget->indexAt(mousePos);

  this->editorEvent(event, widget->model(), option, index);
}

QSize MenuButtonDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  QSize size = QItemDelegate::sizeHint(option, index);

  const QWidget* parentWidget = option.widget;
  if (parentWidget == nullptr)
    return size;

  QSize parentSize = parentWidget->size();
  return QSize(size.width(), parentWidget->height() * 0.25);
}

QRectF MenuButtonDelegate::drawButton(QPainter *Paint, QRectF Item, const QModelIndex &index) const
{
  double width = Item.width() * 0.75;
  double height = Item.height() * 0.75;

  double x = Item.x() + (Item.width() - width) / 2;
  double y = Item.y() + (Item.height() - height) / 2;

  QString IconName = index.data(Qt::UserRole).toString();
  Color color = Color::GRAY_02;

  QStyleOptionButton btnOpt;
  QColor penColor(color);
  QColor brushColor(color);

  QRect btnRect(x, y, width, height);

  if (isPressed && btnRect.contains(mousePos))
  {
    brushColor = Qt::transparent;
    btnRect.moveTopLeft(btnRect.topLeft() + QPoint(2, 2));
  }

  if (isReleased && btnRect.contains(mousePos))
  {
    if (index.row() == row)
    {
      row = -1;
      isReleased = false;

      emit const_cast<MenuButtonDelegate*>(this)->SendMenuClicked(index.row());
    }
  }

  btnOpt.rect = btnRect;
  btnOpt.iconSize = QSize(width, height);
  btnOpt.features |= QStyleOptionButton::Flat;
  btnOpt.icon = QIcon(qThemes->Icons(IconName));

  Paint->setBrush(brushColor);
  Paint->setPen(QPen(penColor, 15));
  Paint->drawRoundRect(btnOpt.rect);

  QApplication::style()->drawControl(QStyle::CE_PushButton, &btnOpt, Paint);

  return Item.adjusted(0, 0, 0, - height - spacing);
}

QRectF MenuButtonDelegate::getItem(QRectF Item) const
{
  int x = Item.x() + margin;
  int y = Item.y() + margin;
  int width = Item.width() - (margin * 2);
  int height = Item.height() - margin;

  return QRectF(x, y, width, height);
}

