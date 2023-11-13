#pragma once

/* INCLUDES MAGALI (a mettre apres ceux de Qt pour eviter les conflits) */
// Used to get Mag200Status & STATUS Define
#include <decl10.h>
#include <base.h>
#include <atlbase.h>
#include <shcxt.h>

#include <n_owl.h>
#include <c_atr.h>
#include <c_ftime.h>
#include <commlib.h>
#include <c_sthead.h>
/* FIN INCLUDES MAGALI */

#define NULL_STRUCT { 0 }

enum Mag200Status
{
  UNLOCK       = STATUS_RED,      // RED      0
  VERIFY       = STATUS_YELLOW,   // YELLOW   1
  LOCK         = STATUS_GREEN,    // GREEN    2
  UNDEFINED    = STATUS_BLACK,    // BLACK    3
  WHITE        = STATUS_WHITE,    // WHITE    4
  GRAY         = STATUS_GRAY,     // GRAY     5
  BLUE         = STATUS_BLUE,     // BLUE     6
}; 
Q_DECLARE_METATYPE(Mag200Status);

#define DATE_FORMAT         0 /* 0 : Format     */
#define DATE_YEAR           1 /* 1 : Annee      */
#define DATE_MONTH          2 /* 2 : Mois       */
#define DATE_DAY            3 /* 3 : Jour       */
#define DATE_HOUR           4 /* 4 : Heure      */
#define DATE_MINUTE         5 /* 5 : Minute     */
#define DATE_SECOND         6 /* 6 : Seconde    */
#define DATE_MILLISECOND    7 /* 7 : Ms         */
#define DATE_MICROSECOND    8 /* 8 : µs         */
#define DATE_YEAR_DAY       9 /* 9 : Jour annee */
#define bcd2(a) (((a) >> 4) * 10 + ((a) & 0x0F))

// Following Hendsolt color code
enum Color
{
  NO_COLOR = -1,

  // Primary Color
  WHITE_01 = 0xffffff,  // Pantone : White
  GREEN_01 = 0x008522,  // Pantone : 2258 C
  GREEN_02 = 0x50b264,  // Pantone : 2256 C
  GREEN_03 = 0x95c994,  // Pantone : 2255 C
  GRAY_01 = 0x5e5e5d,   // Pantone : 425 C
  GRAY_02 = 0x8b8b8a,   // Pantone : 424 C
  GRAY_03 = 0x5e5e5d,   // Pantone : Cool Grey 5 C
  WHITE_02 = 0xeaeae9,  // Pantone : 420 C 
  BLACK_01 = 0x181715,  // Pantone : Black C
  BLACK_02 = 0x191919,  // Pantone : Black 3 2X
  BLACK_03 = 0x353535,  // Pantone : 8602 C

                        // Secondary Color
  BLUE_01 = 0x5f85c0,   // Pantone : 660 C
  BLUE_02 = 0xa9ccf0,   // Pantone : 292 C
  BLUE_03 = 0x4c7a91,   // Pantone : 2223 C
  BLUE_04 = 0x1aa1b1,   // Pantone : 7467 C
  BLUE_05 = 0xa0cccd,   // Pantone : 7472 C
  BLUE_06 = 0x2a82da,   // Pantone : 299 2X
  ORANGE_01 = 0xd68a27, // Pantone : 151 C
  YELLOW_01 = 0xf4ef1b, // Pantone : 3955 C
  RED_01 = 0xc4182f,    // Pantone : 186 C 
  GREEN_04 = 0xa4d233,  // Pantone : 2299 C
  GREEN_05 = 0x6f8846,  // Pantone : 364 C
  YELLOW_02 = 0xdfca7d, // Pantone : 2017 C 
  YELLOW_03 = 0xf7ea9a, // Pantone : 2005 C

  IMPORT_01 = 0x91b0df,
  EXPORT_01 = 0xffb074,
  RECORD_01 = 0xff67b6,
  REPLAY_01 = 0x9c51b6
};

#define ICON_CURRENT_FRAME  "frame"
#define ICON_ALL_FRAME      "frameList"