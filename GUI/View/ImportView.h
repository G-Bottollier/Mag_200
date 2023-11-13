#ifndef IMPORTVIEW_H
#define IMPORTVIEW_H

#include "PageView.h"
#include "SettingsManager.h"

class EntryInfoDelegate;
class MenuButtonDelegate;

class ImportView : public PageView
{
  Q_OBJECT

    // Setup
    QListWidget *menuList = nullptr;
    QListWidget *fileList = nullptr;

    MenuButtonDelegate *menuButton = nullptr;
    EntryInfoDelegate *entryInfo = nullptr;

    void SetupUI();
    void SetupEvent();

    // Entry Management
    QFileInfo import;
    QFileInfo current;
    QFileSystemWatcher *folderWatcher;

    void UpdateWatcher(QString oldPath, QString newPath);
    void AddDotDot(QFileInfoList &list, QFileInfo info);

    // Volume Management
    bool HasToOpenDrives(QFileInfo info);
    
  protected:
    // Event Handling Methods
    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

  public:
    // Constructor and Destructor
    ImportView(QWidget *parent = nullptr);
    ~ImportView();

    // Getter
    QString ImportPath();


  public slots:
    // Menu Management
    void NewMenuClicked(int index);

    // Volume Management
    void OpenDrives();
    void NewMountedVolumesUpdate();

    // Entry Management
    void OpenParentFolder();
    void OpenFolder(QFileInfo info);
    void DirectoryChanged(QString path);

    // File Import Handling
    void NewImportFile(QFileInfo info);

  signals:
    // Notify the current directory
    void SendDrivesOpen();
    void SendFolderOpen(QFileInfo info);

    // Notify to import a new file
    void SendImportFile(QString path);
};


/******************************/
/**   CUSTOM ITEM DELEGATE   **/
/******************************/

// =======================================================
// EntryInfoDelegate                                      
// =======================================================
class EntryInfoDelegate : public QItemDelegate 
{
  Q_OBJECT

  private:
    int margin = 10;
    int spacing = 25;
    const QList<QString> filter = { "ch10" };

    mutable int row = -1;
    mutable bool isPressed = false;
    mutable bool isReleased = false;
    QPoint mousePos = QPoint();

    QRectF getItem(QRectF Item) const;
    QRectF drawIcon(QPainter *Paint, QFileInfo Info, QRectF Item) const;
    QRectF drawButton(QPainter *Paint, QFileInfo Info, QRectF Item, const QModelIndex &index) const;
    QRectF drawFileSize(QPainter *Paint, QFileInfo Info, QRectF Item) const;
    QRectF drawFileDate(QPainter *Paint, QFileInfo Info, QRectF Item) const;
    QRectF drawFileName(QPainter *Paint, QFileInfo Info, QRectF Item) const;

  protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

  public:
    EntryInfoDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void MouseReleased(QObject *obj, QEvent *event);

  signals:
    void SendOpenFolder(QFileInfo info);
    void SendImportFile(QFileInfo info);
    void SendExportFile(QFileInfo info);
};

// =======================================================
// MenuButtonDelegate                                      
// =======================================================
class MenuButtonDelegate : public QItemDelegate
{
  Q_OBJECT

  private:
    int margin = 0;
    int spacing = 5;

    mutable int row = -1;
    mutable bool isPressed = false;
    mutable bool isReleased = false;
    QPoint mousePos = QPoint();

    QRectF getItem(QRectF Item) const;
    QRectF drawButton(QPainter *Paint, QRectF Item, const QModelIndex &index) const;

  protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

  public:
    MenuButtonDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void MouseReleased(QObject *obj, QEvent *event);

  signals:
    void SendMenuClicked(int index);
};

#endif // IMPORTVIEW_H
