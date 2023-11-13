#include "stdafx.h"
#include "SimIRIG.h"

/*
 * @brief Constructor for the SimIRIG class.
 *
 * Initializes an SimIRIG with the given PCM_Input and parent widget.
 *
 * @param input The PCM_Input object associated with the SimIRIG.
 * @param parent The parent widget of the SimIRIG.
 * @return void
 *
 * @date 31/05/2023
 */
SimIRIG::SimIRIG(Mag200Data *board, QWidget *parent) : ChannelItem(board, parent)
{
  // Setup UI 
  SetupUI();

  // Setup event handlers
  SetupEvent();
}

/*
 * @brief Destructor for the SimIRIG class.
 *
 * @return void
 *
 * @date 31/05/2023
 */
SimIRIG::~SimIRIG()
{
}

/*
 * @brief Sets up the user interface for the SimIRIG.
 *
 * Creates and configures the necessary widgets for displaying the input information.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void SimIRIG::SetupUI()
{
  QHBoxLayout *MainLayout = static_cast<QHBoxLayout*>(this->layout());

  // Create the LEDs for displaying the input state
  this->Leds[BS] = new Led(tr("BS"));
  this->Leds[FS] = new Led(tr("FS"));
  this->Leds[ID] = new Led(tr("ID"));

  for(Led *led : Leds)
  {
    led->UseBorder(false);
    led->UseBackground(true);
    MainLayout->addWidget(led);
  }

  // Create and configure the label for displaying the input bitrate
  this->Texts[BITRATE] = new QLabel(tr("UNKNOW_BPS"));
  this->Texts[BITRATE]->setAlignment(Qt::AlignCenter);
  //this->Texts[BITRATE]->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  MainLayout->addWidget(this->Texts[BITRATE]);

  MainLayout->addStretch(0);
}

/*
 * @brief Sets up the event connections for the SimIRIG.
 *
 * @return void
 *
 * @date 31/05/2023
 */
void SimIRIG::SetupEvent()
{
  for (Led *led : Leds)
    QObject::connect(led, &Led::clicked, this, &SimIRIG::LedClicked);
}
