#include "stdafx.h"
#include "Chapter10View.h"

/*
* @brief Constructor of the Chapter10View
*
* @param parent The parent widget (if any).
*
* @return void
*
* @date 31/05/2023
*/
Chapter10View::Chapter10View(QWidget * parent) : PageView(parent) 
{
  // Setup UI 
  SetupUI();

  // Setup event handlers
  SetupEvent();
}

/*
* @brief Destructor of the Chapter10View
*
* @return void
*
* @date 31/05/2023
*/
Chapter10View::~Chapter10View() 
{
}

/*
 * @brief Sets up the user interface for Chapter10View.
 *
 * Sets the header name to "PAGE_CH10".
 *
 * @return void
 *
 * @date 31/05/2023
 */
void Chapter10View::SetupUI()
{
  this->UseLine(true);
  this->AddHeaderView();
  this->AddHeaderTitle();
  this->SetHeaderName(tr("PAGE_CH10"));

  QWidget *w = new QWidget();
  QVBoxLayout *layout = new QVBoxLayout();
  QSpacerItem *SpacerItem = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
  layout->addSpacerItem(SpacerItem);
  w->setLayout(layout);
  this->AddMainWidget(w);
}

/*
 * @brief Sets up event connections for Chapter10View.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void Chapter10View::SetupEvent()
{

}