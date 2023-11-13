#ifndef STORAGEVIEW_H
#define STORAGEVIEW_H

#include "PageView.h"
#include "StoragesManager.h"

/*
 * @brief The StoragesView class represents the view for displaying storage information.
 *
 * This class inherits from the PageView class.
 * The StoragesView class is used to display information about the available storage volumes,
 * including their names, disk space usage, and other relevant details.
 *
 * @date 31/05/2023
 */
class StoragesView : public PageView 
{
	Q_OBJECT

  private:
    QGridLayout *GridLayout = nullptr;
    QSpacerItem *SpacerItem = nullptr;

    const int itemMargin = 20;
    const int itemSpacing = 5;

    void SetupUI();
    void SetupEvent();

    QWidget* GetStorageWidget(QStorageInfo storageInfo, QColor background = Qt::transparent);

  protected:
    void resizeEvent(QResizeEvent *event);

    void resizeStorages(QSize size);

  public:
	  StoragesView(QWidget * parent = Q_NULLPTR);
	  ~StoragesView();

  public slots:
    void MountedVolumesUpdate();
};

#endif // STORAGEVIEW_H