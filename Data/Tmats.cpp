#include "stdafx.h"
#include "Tmats.h"

QHash<QString, int> Tmats::WhiteListCodes;

Tmats::Tmats(QObject *parent) : QObject(parent)
{
  // Initialize the static hash only once
  this->Initialize();
}

Tmats::~Tmats()
{
  this->ClearHash();
}

void Tmats::Initialize()
{
  if (this->WhiteListCodes.size() != 0)
    return;

  this->WhiteListCodes["G\\PN"]                 = G_PN;           // G\PN
  this->WhiteListCodes["G\\TA"]                 = G_TA;           // G\TA
  this->WhiteListCodes["G\\FN"]                 = G_FN;           // G\FN
  this->WhiteListCodes["G\\106"]                = G_106;          // G\106
  this->WhiteListCodes["G\\OD"]                 = G_OD;           // G\OD
  this->WhiteListCodes["G\\RN"]                 = G_RN;           // G\RN
  this->WhiteListCodes["G\\RD"]                 = G_RD;           // G\RD
  this->WhiteListCodes["G\\UN"]                 = G_UN;           // G\UN
  this->WhiteListCodes["G\\UD"]                 = G_UD;           // G\UD
  this->WhiteListCodes["G\\TN"]                 = G_TN;           // G\TN
  this->WhiteListCodes["G\\POC\\N"]             = G_POC_N;        // G\POC\N
  this->WhiteListCodes["G\\POC1-"]              = G_POC1;         // G\POC1-n
  this->WhiteListCodes["G\\POC2-"]              = G_POC2;         // G\POC2-n
  this->WhiteListCodes["G\\POC3-"]              = G_POC3;         // G\POC3-n
  this->WhiteListCodes["G\\POC4-"]              = G_POC4;         // G\POC4-n
  this->WhiteListCodes["G\\DSI\\N"]             = G_DSI_N;        // G\DSI\N
  this->WhiteListCodes["G\\DSI-"]               = G_DSI;          // G\DSI-n
  this->WhiteListCodes["G\\DST-"]               = G_DST;          // G\DST-n
  this->WhiteListCodes["G\\DSC-"]               = G_DSC;          // G\DSC-n
  this->WhiteListCodes["G\\TI1"]                = G_TI1;          // G\TI1
  this->WhiteListCodes["G\\TI2"]                = G_TI2;          // G\TI2
  this->WhiteListCodes["G\\TI3"]                = G_TI3;          // G\TI3
  this->WhiteListCodes["G\\SC"]                 = G_SC;           // G\SC
  this->WhiteListCodes["G\\SHA"]                = G_SHA;          // G\SHA
  this->WhiteListCodes["G\\COM"]                = G_COM;          // G\COM
  this->WhiteListCodes["T-\\ID"]                = T_ID;           // T-x\ID
  this->WhiteListCodes["T-\\TID"]               = T_TID;          // T-x\TID
  this->WhiteListCodes["T-\\RF1"]               = T_RF1;          // T-x\RF1
  this->WhiteListCodes["T-\\RF2"]               = T_RF2;          // T-x\RF2
  this->WhiteListCodes["T-\\RF3"]               = T_RF3;          // T-x\RF3
  this->WhiteListCodes["T-\\RF4"]               = T_RF4;          // T-x\RF4
  this->WhiteListCodes["T-\\RF5"]               = T_RF5;          // T-x\RF5
  this->WhiteListCodes["T-\\RF6"]               = T_RF6;          // T-x\RF6
  this->WhiteListCodes["T-\\SCO\\N"]            = T_SCO_N;        // T-x\SCO\N
  this->WhiteListCodes["T-\\SCO1-"]             = T_SCO1;         // T-x\SCO1-n
  this->WhiteListCodes["T-\\SCO2-"]             = T_SCO2;         // T-x\SCO2-n
  this->WhiteListCodes["T-\\RF7"]               = T_RF7;          // T-x\RF7
  this->WhiteListCodes["T-\\PMF1"]              = T_PMF1;         // T-x\PMF1
  this->WhiteListCodes["T-\\PMF2"]              = T_PMF2;         // T-x\PMF2
  this->WhiteListCodes["T-\\PMF3"]              = T_PMF3;         // T-x\PMF3
  this->WhiteListCodes["T-\\AN1"]               = T_AN1;          // T-x\AN1
  this->WhiteListCodes["T-\\AN2"]               = T_AN2;          // T-x\AN2
  this->WhiteListCodes["T-\\AN3"]               = T_AN3;          // T-x\AN3
  this->WhiteListCodes["T-\\AP"]                = T_AP;           // T-x\AP
  this->WhiteListCodes["T-\\AP\\POC1"]          = T_AP_POC1;      // T-x\AP\POC1
  this->WhiteListCodes["T-\\AP\\POC2"]          = T_AP_POC2;      // T-x\AP\POC2
  this->WhiteListCodes["T-\\AP\\POC3"]          = T_AP_POC3;      // T-x\AP\POC3
  this->WhiteListCodes["T-\\AP\\POC4"]          = T_AP_POC4;      // T-x\AP\POC4
  this->WhiteListCodes["T-\\GST1"]              = T_GST1;         // T-x\GST1
  this->WhiteListCodes["T-\\GST2"]              = T_GST2;         // T-x\GST2
  this->WhiteListCodes["T-\\GST3"]              = T_GST3;         // T-x\GST3
  this->WhiteListCodes["T-\\GST4"]              = T_GST4;         // T-x\GST4
  this->WhiteListCodes["T-\\GST5"]              = T_GST5;         // T-x\GST5
  this->WhiteListCodes["T-\\GST6"]              = T_GST6;         // T-x\GST6
  this->WhiteListCodes["T-\\GST7"]              = T_GST7;         // T-x\GST7
  this->WhiteListCodes["T-\\FM1"]               = T_FM1;          // T-x\FM1
  this->WhiteListCodes["T-\\FM2"]               = T_FM2;          // T-x\FM2
  this->WhiteListCodes["T-\\PLL"]               = T_PLL;          // T-x\PLL
  this->WhiteListCodes["T-\\COM"]               = T_COM;          // T-x\COM
  this->WhiteListCodes["R-\\ID"]                = R_ID;           // R-x\ID
  this->WhiteListCodes["R-\\RID"]               = R_RID;          // R-x\RID
  this->WhiteListCodes["R-\\R1"]                = R_R1;           // R-x\R1
  this->WhiteListCodes["R-\\TC1"]               = R_TC1;          // R-x\TC1
  this->WhiteListCodes["R-\\TC2"]               = R_TC2;          // R-x\TC2
  this->WhiteListCodes["R-\\TC3"]               = R_TC3;          // R-x\TC3
  this->WhiteListCodes["R-\\RML"]               = R_RML;          // R-x\RML
  this->WhiteListCodes["R-\\ERBS"]              = R_ERBS;         // R-x\ERBS
  this->WhiteListCodes["R-\\TC4"]               = R_TC4;          // R-x\TC4
  this->WhiteListCodes["R-\\TC5"]               = R_TC5;          // R-x\TC5
  this->WhiteListCodes["R-\\TT"]                = R_TT;           // R-x\TT
  this->WhiteListCodes["R-\\N"]                 = R_N;            // R-x\N
  this->WhiteListCodes["R-\\TC6"]               = R_TC6;          // R-x\TC6
  this->WhiteListCodes["R-\\TC7"]               = R_TC7;          // R-x\TC7
  this->WhiteListCodes["R-\\TC8"]               = R_TC8;          // R-x\TC8
  this->WhiteListCodes["R-\\NSB"]               = R_NSB;          // R-x\NSB
  this->WhiteListCodes["R-\\RI1"]               = R_RI1;          // R-x\RI1
  this->WhiteListCodes["R-\\RI2"]               = R_RI2;          // R-x\RI2
  this->WhiteListCodes["R-\\RI3"]               = R_RI3;          // R-x\RI3
  this->WhiteListCodes["R-\\RI4"]               = R_RI4;          // R-x\RI4
  this->WhiteListCodes["R-\\POC1"]              = R_POC1;         // R-x\POC1
  this->WhiteListCodes["R-\\POC2"]              = R_POC2;         // R-x\POC2
  this->WhiteListCodes["R-\\POC3"]              = R_POC3;         // R-x\POC3
  this->WhiteListCodes["R-\\POC4"]              = R_POC4;         // R-x\POC4
  this->WhiteListCodes["R-\\RI5"]               = R_RI5;          // R-x\RI5
  this->WhiteListCodes["R-\\DPOC1"]             = R_DPOC1;        // R-x\DPOC1
  this->WhiteListCodes["R-\\DPOC2"]             = R_DPOC2;        // R-x\DPOC2
  this->WhiteListCodes["R-\\DPOC3"]             = R_DPOC3;        // R-x\DPOC3
  this->WhiteListCodes["R-\\DPOC4"]             = R_DPOC4;        // R-x\DPOC4
  this->WhiteListCodes["R-\\RI6"]               = R_RI6;          // R-x\RI6
  this->WhiteListCodes["R-\\RI7"]               = R_RI7;          // R-x\RI7
  this->WhiteListCodes["R-\\RI8"]               = R_RI8;          // R-x\RI8
  this->WhiteListCodes["R-\\MPOC1"]             = R_MPOC1;        // R-x\MPOC1
  this->WhiteListCodes["R-\\MPOC2"]             = R_MPOC2;        // R-x\MPOC2
  this->WhiteListCodes["R-\\MPOC3"]             = R_MPOC3;        // R-x\MPOC3
  this->WhiteListCodes["R-\\MPOC4"]             = R_MPOC4;        // R-x\MPOC4
  this->WhiteListCodes["R-\\CRE"]               = R_CRE;          // R-x\CRE
  this->WhiteListCodes["R-\\RSS"]               = R_RSS;          // R-x\RSS
  this->WhiteListCodes["R-\\RI9"]               = R_RI9;          // R-x\RI9
  this->WhiteListCodes["R-\\RI10"]              = R_RI10;         // R-x\RI10
  this->WhiteListCodes["R-\\RIM\\N"]            = R_RIM_N;        // R-x\RIM\N
  this->WhiteListCodes["R-\\RIMI-"]             = R_RIMI;         // R-x\RIMI-n
  this->WhiteListCodes["R-\\RIMS-"]             = R_RIMS;         // R-x\RIMS-n
  this->WhiteListCodes["R-\\RIMF-"]             = R_RIMF;         // R-x\RIMF-n
  this->WhiteListCodes["R-\\RMM\\N"]            = R_RMM_N;        // R-x\RMM\N
  this->WhiteListCodes["R-\\RMMID-"]            = R_RMMID;        // R-x\RMMID-n
  this->WhiteListCodes["R-\\RMMS-"]             = R_RMMS;         // R-x\RMMS-n
  this->WhiteListCodes["R-\\RMMF-"]             = R_RMMF;         // R-x\RMMF-n
  this->WhiteListCodes["R-\\EI\\N"]             = R_EI_N;         // R-x\EI\N
  this->WhiteListCodes["R-\\EINM-"]             = R_EINM;         // R-x\EINM-n
  this->WhiteListCodes["R-\\PEIN-"]             = R_PEIN;         // R-x\PEIN-n
  this->WhiteListCodes["R-\\EILS-"]             = R_EILS;         // R-x\EILS-n
  this->WhiteListCodes["R-\\EIT-"]              = R_EIT;          // R-x\EIT-n
  this->WhiteListCodes["R-\\EIIP-"]             = R_EIIP;         // R-x\EIIP-n
  this->WhiteListCodes["R-\\EIIP\\N-"]          = R_EIIP_N;       // R-x\EIIP\N-n
  this->WhiteListCodes["R-\\EI\\PA--"]          = R_EI_PA;        // R-x\EI\PA-n-m
  this->WhiteListCodes["R-\\EI\\PT--"]          = R_EI_PT;        // R-x\EI\PT-n-m
  this->WhiteListCodes["R-\\CG\\N"]             = R_CG_N;         // R-x\CG\N
  this->WhiteListCodes["R-\\CGNM-"]             = R_CGNM;         // R-x\CGNM-n
  this->WhiteListCodes["R-\\CGSN-"]             = R_CGSN;         // R-x\CGSN-n
  this->WhiteListCodes["R-\\CGCH\\N-"]          = R_CGCH_N;       // R-x\CGCH\N-n
  this->WhiteListCodes["R-\\CGCN--"]            = R_CGCN;         // R-x\CGCN-n-m
  this->WhiteListCodes["R-\\DR\\N"]             = R_DR_N;         // R-x\DR\N
  this->WhiteListCodes["R-\\DRNM-"]             = R_DRNM;         // R-x\DRNM-n
  this->WhiteListCodes["R-\\DRN-"]              = R_DRN;          // R-x\DRN-n
  this->WhiteListCodes["R-\\DRBS-"]             = R_DRBS;         // R-x\DRBS-n
  this->WhiteListCodes["R-\\DRVL\\N-"]          = R_DRVL_N;       // R-x\DRVL\N-n
  this->WhiteListCodes["R-\\VLNM--"]            = R_VLNM;         // R-x\VLNM-n-m
  this->WhiteListCodes["R-\\VLN--"]             = R_VLN;          // R-x\VLN-n-m
  this->WhiteListCodes["R-\\VLBA--"]            = R_VLBA;         // R-x\VLBA-n-m
  this->WhiteListCodes["R-\\VLNB--"]            = R_VLNB;         // R-x\VLNB-n-m
  this->WhiteListCodes["R-\\L\\N"]              = R_L_N;          // R-x\L\N
  this->WhiteListCodes["R-\\LNM-"]              = R_LNM;          // R-x\LNM-n
  this->WhiteListCodes["R-\\LSNM-"]             = R_LSNM;         // R-x\LSNM-n
  this->WhiteListCodes["R-\\LSSN-"]             = R_LSSN;         // R-x\LSSN-n
  this->WhiteListCodes["R-\\LDDN-"]             = R_LDDN;         // R-x\LDDN-n
  this->WhiteListCodes["R-\\LDVN-"]             = R_LDVN;         // R-x\LDVN-n
  this->WhiteListCodes["R-\\EPL\\N"]            = R_EPL_N;        // R-x\EPL\N
  this->WhiteListCodes["R-\\EPL\\LNM-"]         = R_EPL_LNM;      // R-x\EPL\LNM-n
  this->WhiteListCodes["R-\\EPL\\LSNM-"]        = R_EPL_LSNM;     // R-x\EPL\LSNM-n
  this->WhiteListCodes["R-\\EPL\\LSSN-"]        = R_EPL_LSSN;     // R-x\EPL\LSSN-n
  this->WhiteListCodes["R-\\EPL\\LDEIP-"]       = R_EPL_LDEIP;    // R-x\EPL\LDEIP-n
  this->WhiteListCodes["R-\\EPL\\LDEPA-"]       = R_EPL_LDEPA;    // R-x\EPL\LDEPA-n
  this->WhiteListCodes["R-\\UD\\TK1"]           = R_UD_TK1;       // R-x\UD\TK1
  this->WhiteListCodes["R-\\EV\\E"]             = R_EV_E;         // R-x\EV\E
  this->WhiteListCodes["R-\\EV\\TK1"]           = R_EV_TK1;       // R-x\EV\TK1
  this->WhiteListCodes["R-\\EV\\N"]             = R_EV_N;         // R-x\EV\N
  this->WhiteListCodes["R-\\EV\\IEE"]           = R_EV_IEE;       // R-x\EV\IEE
  this->WhiteListCodes["R-\\EV\\ID-"]           = R_EV_ID;        // R-x\EV\ID-n
  this->WhiteListCodes["R-\\EV\\D-"]            = R_EV_D;         // R-x\EV\D-n
  this->WhiteListCodes["R-\\EV\\EDP-"]          = R_EV_EDP;       // R-x\EV\EDP-n
  this->WhiteListCodes["R-\\EV\\T-"]            = R_EV_T;         // R-x\EV\T-n
  this->WhiteListCodes["R-\\EV\\P-"]            = R_EV_P;         // R-x\EV\P-n
  this->WhiteListCodes["R-\\EV\\CM-"]           = R_EV_CM;        // R-x\EV\CM-n
  this->WhiteListCodes["R-\\EV\\IC-"]           = R_EV_IC;        // R-x\EV\IC-n
  this->WhiteListCodes["R-\\EV\\LC-"]           = R_EV_LC;        // R-x\EV\LC-n
  this->WhiteListCodes["R-\\EV\\MS-"]           = R_EV_MS;        // R-x\EV\MS-n
  this->WhiteListCodes["R-\\EV\\MN-"]           = R_EV_MN;        // R-x\EV\MN-n
  this->WhiteListCodes["R-\\EV\\DLN-"]          = R_EV_DLN;       // R-x\EV\DLN-n
  this->WhiteListCodes["R-\\EV\\PM\\N-"]        = R_EV_PM_N;      // R-x\EV\PM\N-n
  this->WhiteListCodes["R-\\EV\\PM\\MN--"]      = R_EV_PM_MN;     // R-x\EV\PM\MN-n-m
  this->WhiteListCodes["R-\\EV\\PM\\PRE--"]     = R_EV_PM_PRE;    // R-x\EV\PM\PRE-n-m
  this->WhiteListCodes["R-\\EV\\PM\\PST--"]     = R_EV_PM_PST;    // R-x\EV\PM\PST-n-m
  this->WhiteListCodes["R-\\IDX\\E"]            = R_IDX_E;        // R-x\IDX\E
  this->WhiteListCodes["R-\\IDX\\TK1"]          = R_IDX_TK1;      // R-x\IDX\TK1
  this->WhiteListCodes["R-\\IDX\\IT"]           = R_IDX_IT;       // R-x\IDX\IT
  this->WhiteListCodes["R-\\IDX\\ITV"]          = R_IDX_ITV;      // R-x\IDX\ITV
  this->WhiteListCodes["R-\\IDX\\ICV"]          = R_IDX_ICV;      // R-x\IDX\ICV
  this->WhiteListCodes["R-\\MRC\\E"]            = R_MRC_E;        // R-x\MRC\E
  this->WhiteListCodes["R-\\MRC\\ID"]           = R_MRC_ID;       // R-x\MRC\ID
  this->WhiteListCodes["R-\\MRC\\RCT"]          = R_MRC_RCT;      // R-x\MRC\RCT
  this->WhiteListCodes["R-\\MRC\\SPM"]          = R_MRC_SPM;      // R-x\MRC\SPM
  this->WhiteListCodes["R-\\MRC\\SRM"]          = R_MRC_SRM;      // R-x\MRC\SRM
  this->WhiteListCodes["R-\\TK1-"]              = R_TK1;          // R-x\TK1-n
  this->WhiteListCodes["R-\\TK2-"]              = R_TK2;          // R-x\TK2-n
  this->WhiteListCodes["R-\\IDDR-"]             = R_IDDR;         // R-x\IDDR-n
  this->WhiteListCodes["R-\\DSI-"]              = R_DSI;          // R-x\DSI-n
  this->WhiteListCodes["R-\\TK3-"]              = R_TK3;          // R-x\TK3-n
  this->WhiteListCodes["R-\\TK4-"]              = R_TK4;          // R-x\TK4-n
  this->WhiteListCodes["R-\\CHE-"]              = R_CHE;          // R-x\CHE-n
  this->WhiteListCodes["R-\\DST-"]              = R_DST;          // R-x\DST-n
  this->WhiteListCodes["R-\\CDT-"]              = R_CDT;          // R-x\CDT-n
  this->WhiteListCodes["R-\\BDLN-"]             = R_BDLN;         // R-x\BDLN-n
  this->WhiteListCodes["R-\\PDLN-"]             = R_PDLN;         // R-x\PDLN-n
  this->WhiteListCodes["R-\\CDLN-"]             = R_CDLN;         // R-x\CDLN-n
  this->WhiteListCodes["R-\\SHTF-"]             = R_SHTF;         // R-x\SHTF-n
  this->WhiteListCodes["R-\\PDTF-"]             = R_PDTF;         // R-x\PDTF-n
  this->WhiteListCodes["R-\\PDP-"]              = R_PDP;          // R-x\PDP-n
  this->WhiteListCodes["R-\\RPS-"]              = R_RPS;          // R-x\RPS-n
  this->WhiteListCodes["R-\\ICE-"]              = R_ICE;          // R-x\ICE-n
  this->WhiteListCodes["R-\\IST-"]              = R_IST;          // R-x\IST-n
  this->WhiteListCodes["R-\\ITH-"]              = R_ITH;          // R-x\ITH-n
  this->WhiteListCodes["R-\\ITM-"]              = R_ITM;          // R-x\ITM-n
  this->WhiteListCodes["R-\\PTF-"]              = R_PTF;          // R-x\PTF-n
  this->WhiteListCodes["R-\\MFF\\E-"]           = R_MFF_E;        // R-x\MFF\E-n
  this->WhiteListCodes["R-\\POF\\E-"]           = R_POF_E;        // R-x\POF\E-n
  this->WhiteListCodes["R-\\POF\\T-"]           = R_POF_T;        // R-x\POF\T-n
  this->WhiteListCodes["R-\\MFF\\FDT-"]         = R_MFF_FDT;      // R-x\MFF\FDT-n
  this->WhiteListCodes["R-\\MFF\\N-"]           = R_MFF_N;        // R-x\MFF\N-n
  this->WhiteListCodes["R-\\MFF\\MFN--"]        = R_MFF_MFN;      // R-x\MFF\MFN-n-m
  this->WhiteListCodes["R-\\MFF\\RPS--"]        = R_MFF_RPS;      // R-x\MFF\RPS-n-m
  this->WhiteListCodes["R-\\SMF\\N-"]           = R_SMF_N;        // R-x\SMF\N-n
  this->WhiteListCodes["R-\\SMF\\SMN--"]        = R_SMF_SMN;      // R-x\SMF\SMN-n-m
  this->WhiteListCodes["R-\\SMF\\MFOT--"]       = R_SMF_MFOT;     // R-x\SMF\MFOT-n-m
  this->WhiteListCodes["R-\\BTF-"]              = R_BTF;          // R-x\BTF-n
  this->WhiteListCodes["R-\\MRF\\E-"]           = R_MRF_E;        // R-x\MRF\E-n
  this->WhiteListCodes["R-\\MOF\\T-"]           = R_MOF_T;        // R-x\MOF\T-n
  this->WhiteListCodes["R-\\MFD\\FDT-"]         = R_MFD_FDT;      // R-x\MFD\FDT-n
  this->WhiteListCodes["R-\\MFD\\N-"]           = R_MFD_N;        // R-x\MFD\N-n
  this->WhiteListCodes["R-\\MFD\\MID--"]        = R_MFD_MID;      // R-x\MFD\MID-n-m
  this->WhiteListCodes["R-\\MFD\\MT--"]         = R_MFD_MT;       // R-x\MFD\MT-n-m
  this->WhiteListCodes["R-\\CWE--"]             = R_CWE;          // R-x\CWE-n-m
  this->WhiteListCodes["R-\\CMD--"]             = R_CMD;          // R-x\CMD-n-m
  this->WhiteListCodes["R-\\MFD\\TRA--"]        = R_MFD_TRA;      // R-x\MFD\TRA-n-m
  this->WhiteListCodes["R-\\MFD\\TRM--"]        = R_MFD_TRM;      // R-x\MFD\TRM-n-m
  this->WhiteListCodes["R-\\MFD\\STA--"]        = R_MFD_STA;      // R-x\MFD\STA-n-m
  this->WhiteListCodes["R-\\MFD\\DWC--"]        = R_MFD_DWC;      // R-x\MFD\DWC-n-m
  this->WhiteListCodes["R-\\RCWE--"]            = R_RCWE;         // R-x\RCWE-n-m
  this->WhiteListCodes["R-\\RCMD--"]            = R_RCMD;         // R-x\RCMD-n-m
  this->WhiteListCodes["R-\\MFD\\RTRA--"]       = R_MFD_RTRA;     // R-x\MFD\RTRA-n-m
  this->WhiteListCodes["R-\\MFD\\RSTA--"]       = R_MFD_RSTA;     // R-x\MFD\RSTA-n-m
  this->WhiteListCodes["R-\\MFD\\RDWC--"]       = R_MFD_RDWC;     // R-x\MFD\RDWC-n-m
  this->WhiteListCodes["R-\\BME\\N-"]           = R_BME_N;        // R-x\BME\N-n
  this->WhiteListCodes["R-\\BME\\SMN--"]        = R_BME_SMN;      // R-x\BME\SMN-n-m
  this->WhiteListCodes["R-\\BME\\MFOT--"]       = R_BME_MFOT;     // R-x\BME\MFOT-n-m
  this->WhiteListCodes["R-\\ATF-"]              = R_ATF;          // R-x\ATF-n
  this->WhiteListCodes["R-\\ACH\\N-"]           = R_ACH_N;        // R-x\ACH\N-n
  this->WhiteListCodes["R-\\ADP-"]              = R_ADP;          // R-x\ADP-n
  this->WhiteListCodes["R-\\ASR-"]              = R_ASR;          // R-x\ASR-n
  this->WhiteListCodes["R-\\AMCE--"]            = R_AMCE;         // R-x\AMCE-n-m
  this->WhiteListCodes["R-\\AMCN-"]             = R_AMCN;         // R-x\AMCN-n
  this->WhiteListCodes["R-\\AMN--"]             = R_AMN;          // R-x\AMN-n-m
  this->WhiteListCodes["R-\\ADL--"]             = R_ADL;          // R-x\ADL-n-m
  this->WhiteListCodes["R-\\AMSK--"]            = R_AMSK;         // R-x\AMSK-n-m
  this->WhiteListCodes["R-\\AMTO--"]            = R_AMTO;         // R-x\AMTO-n-m
  this->WhiteListCodes["R-\\ASF--"]             = R_ASF;          // R-x\ASF-n-m
  this->WhiteListCodes["R-\\ASBW--"]            = R_ASBW;         // R-x\ASBW-n-m
  this->WhiteListCodes["R-\\ACP--"]             = R_ACP;          // R-x\ACP-n-m
  this->WhiteListCodes["R-\\AII--"]             = R_AII;          // R-x\AII-n-m
  this->WhiteListCodes["R-\\AGI--"]             = R_AGI;          // R-x\AGI-n-m
  this->WhiteListCodes["R-\\AFSI--"]            = R_AFSI;         // R-x\AFSI-n-m
  this->WhiteListCodes["R-\\AOVI--"]            = R_AOVI;         // R-x\AOVI-n-m
  this->WhiteListCodes["R-\\ALSV--"]            = R_ALSV;         // R-x\ALSV-n-m
  this->WhiteListCodes["R-\\AECS--"]            = R_AECS;         // R-x\AECS-n-m
  this->WhiteListCodes["R-\\AECO--"]            = R_AECO;         // R-x\AECO-n-m
  this->WhiteListCodes["R-\\AECU--"]            = R_AECU;         // R-x\AECU-n-m
  this->WhiteListCodes["R-\\AF--"]              = R_AF;           // R-x\AF-n-m
  this->WhiteListCodes["R-\\AIT--"]             = R_AIT;          // R-x\AIT-n-m
  this->WhiteListCodes["R-\\AV--"]              = R_AV;           // R-x\AV-n-m
  this->WhiteListCodes["R-\\AVF--"]             = R_AVF;          // R-x\AVF-n-m
  this->WhiteListCodes["R-\\ACF-"]              = R_ACF;          // R-x\ACF-n
  this->WhiteListCodes["R-\\AIF-"]              = R_AIF;          // R-x\AIF-n
  this->WhiteListCodes["R-\\DTF-"]              = R_DTF;          // R-x\DTF-n
  this->WhiteListCodes["R-\\DMOD-"]             = R_DMOD;         // R-x\DMOD-n
  this->WhiteListCodes["R-\\DSR-"]              = R_DSR;          // R-x\DSR-n
  this->WhiteListCodes["R-\\NDM\\N-"]           = R_NDM_N;        // R-x\NDM\N-n
  this->WhiteListCodes["R-\\DMN--"]             = R_DMN;          // R-x\DMN-n-m
  this->WhiteListCodes["R-\\DMSK--"]            = R_DMSK;         // R-x\DMSK-n-m
  this->WhiteListCodes["R-\\DMTO--"]            = R_DMTO;         // R-x\DMTO-n-m
  this->WhiteListCodes["R-\\ABTF-"]             = R_ABTF;         // R-x\ABTF-n
  this->WhiteListCodes["R-\\NAS\\N-"]           = R_NAS_N;        // R-x\NAS\N-n
  this->WhiteListCodes["R-\\ASN--"]             = R_ASN;          // R-x\ASN-n-m
  this->WhiteListCodes["R-\\ANM--"]             = R_ANM;          // R-x\ANM-n-m
  this->WhiteListCodes["R-\\VTF-"]              = R_VTF;          // R-x\VTF-n
  this->WhiteListCodes["R-\\VXF-"]              = R_VXF;          // R-x\VXF-n
  this->WhiteListCodes["R-\\VST-"]              = R_VST;          // R-x\VST-n
  this->WhiteListCodes["R-\\VSF-"]              = R_VSF;          // R-x\VSF-n
  this->WhiteListCodes["R-\\CBR-"]              = R_CBR;          // R-x\CBR-n
  this->WhiteListCodes["R-\\VBR-"]              = R_VBR;          // R-x\VBR-n
  this->WhiteListCodes["R-\\VED-"]              = R_VED;          // R-x\VED-n
  this->WhiteListCodes["R-\\VCO\\OE-"]          = R_VCO_OE;       // R-x\VCO\OE-n
  this->WhiteListCodes["R-\\VCO\\X-"]           = R_VCO_X;        // R-x\VCO\X-n
  this->WhiteListCodes["R-\\VCO\\Y-"]           = R_VCO_Y;        // R-x\VCO\Y-n
  this->WhiteListCodes["R-\\VCO\\OET-"]         = R_VCO_OET;      // R-x\VCO\OET-n
  this->WhiteListCodes["R-\\VCO\\OLF-"]         = R_VCO_OLF;      // R-x\VCO\OLF-n
  this->WhiteListCodes["R-\\VCO\\OBG-"]         = R_VCO_OBG;      // R-x\VCO\OBG-n
  this->WhiteListCodes["R-\\ASI\\ASL-"]         = R_ASI_ASL;      // R-x\ASI\ASL-n
  this->WhiteListCodes["R-\\ASI\\ASR-"]         = R_ASI_ASR;      // R-x\ASI\ASR-n
  this->WhiteListCodes["R-\\VDA-"]             = R_VDA;           // R-x\VDA-n
  this->WhiteListCodes["R-\\VFD-"]             = R_VFD;           // R-x\VFD-n
  this->WhiteListCodes["R-\\VCI-"]             = R_VCI;           // R-x\VCI-n
  this->WhiteListCodes["R-\\VAC-"]             = R_VAC;           // R-x\VAC-n
  this->WhiteListCodes["R-\\VAA-"]             = R_VAA;           // R-x\VAA-n
  this->WhiteListCodes["R-\\VMD-"]             = R_VMD;           // R-x\VMD-n
  this->WhiteListCodes["R-\\VMT-"]             = R_VMT;           // R-x\VMT-n
  this->WhiteListCodes["R-\\TTF-"]             = R_TTF;           // R-x\TTF-n
  this->WhiteListCodes["R-\\TFMT-"]            = R_TFMT;          // R-x\TFMT-n
  this->WhiteListCodes["R-\\TSRC-"]            = R_TSRC;          // R-x\TSRC-n
  this->WhiteListCodes["R-\\ITF-"]             = R_ITF;           // R-x\ITF-n
  this->WhiteListCodes["R-\\SIT-"]             = R_SIT;           // R-x\SIT-n
  this->WhiteListCodes["R-\\DIF-"]             = R_DIF;           // R-x\DIF-n
  this->WhiteListCodes["R-\\ITSM-"]            = R_ITSM;          // R-x\ITSM-n
  this->WhiteListCodes["R-\\DIAM-"]            = R_DIAM;          // R-x\DIAM-n
  this->WhiteListCodes["R-\\IFR-"]             = R_IFR;           // R-x\IFR-n
  this->WhiteListCodes["R-\\PTG-"]             = R_PTG;           // R-x\PTG-n
  this->WhiteListCodes["R-\\TOTF-"]            = R_TOTF;          // R-x\TOTF-n
  this->WhiteListCodes["R-\\EXP-"]             = R_EXP;           // R-x\EXP-n
  this->WhiteListCodes["R-\\ROT-"]             = R_ROT;           // R-x\ROT-n
  this->WhiteListCodes["R-\\SGV-"]             = R_SGV;           // R-x\SGV-n
  this->WhiteListCodes["R-\\SAG-"]             = R_SAG;           // R-x\SAG-n
  this->WhiteListCodes["R-\\ISW-"]             = R_ISW;           // R-x\ISW-n
  this->WhiteListCodes["R-\\ISH-"]             = R_ISH;           // R-x\ISH-n
  this->WhiteListCodes["R-\\MIW-"]             = R_MIW;           // R-x\MIW-n
  this->WhiteListCodes["R-\\MIH-"]             = R_MIH;           // R-x\MIH-n
  this->WhiteListCodes["R-\\IW-"]              = R_IW;            // R-x\IW-n
  this->WhiteListCodes["R-\\IH-"]              = R_IH;            // R-x\IH-n
  this->WhiteListCodes["R-\\IOX-"]             = R_IOX;           // R-x\IOX-n
  this->WhiteListCodes["R-\\IOY-"]             = R_IOY;           // R-x\IOY-n
  this->WhiteListCodes["R-\\ILP-"]             = R_ILP;           // R-x\ILP-n
  this->WhiteListCodes["R-\\IBH-"]             = R_IBH;           // R-x\IBH-n
  this->WhiteListCodes["R-\\IBV-"]             = R_IBV;           // R-x\IBV-n
  this->WhiteListCodes["R-\\IDH-"]             = R_IDH;           // R-x\IDH-n
  this->WhiteListCodes["R-\\IDV-"]             = R_IDV;           // R-x\IDV-n
  this->WhiteListCodes["R-\\IRX-"]             = R_IRX;           // R-x\IRX-n
  this->WhiteListCodes["R-\\IRY-"]             = R_IRY;           // R-x\IRY-n
  this->WhiteListCodes["R-\\IPMN-"]            = R_IPMN;          // R-x\IPMN-n
  this->WhiteListCodes["R-\\IPMX-"]            = R_IPMX;          // R-x\IPMX-n
  this->WhiteListCodes["R-\\TIT-"]             = R_TIT;           // R-x\TIT-n
  this->WhiteListCodes["R-\\UTF-"]             = R_UTF;           // R-x\UTF-n
  this->WhiteListCodes["R-\\NUS\\N-"]          = R_NUS_N;         // R-x\NUS\N-n
  this->WhiteListCodes["R-\\USCN--"]           = R_USCN;          // R-x\USCN-n-m
  this->WhiteListCodes["R-\\UCNM--"]           = R_UCNM;          // R-x\UCNM-n-m
  this->WhiteListCodes["R-\\UCR--"]            = R_UCR;           // R-x\UCR-n-m
  this->WhiteListCodes["R-\\UCB--"]            = R_UCB;           // R-x\UCB-n-m
  this->WhiteListCodes["R-\\UCP--"]            = R_UCP;           // R-x\UCP-n-m
  this->WhiteListCodes["R-\\UCS--"]            = R_UCS;           // R-x\UCS-n-m
  this->WhiteListCodes["R-\\UCIN--"]           = R_UCIN;          // R-x\UCIN-n-m
  this->WhiteListCodes["R-\\UCBS--"]           = R_UCBS;          // R-x\UCBS-n-m
  this->WhiteListCodes["R-\\UCSL--"]           = R_UCSL;          // R-x\UCSL-n-m
  this->WhiteListCodes["R-\\UCSV--"]           = R_UCSV;          // R-x\UCSV-n-m
  this->WhiteListCodes["R-\\UCBR--"]           = R_UCBR;          // R-x\UCBR-n-m
  this->WhiteListCodes["R-\\MTF-"]             = R_MTF;           // R-x\MTF-n
  this->WhiteListCodes["R-\\NMS\\N-"]          = R_NMS_N;         // R-x\NMS\N-n
  this->WhiteListCodes["R-\\MSCN--"]           = R_MSCN;          // R-x\MSCN-n-m
  this->WhiteListCodes["R-\\MCNM--"]           = R_MCNM;          // R-x\MCNM-n-m
  this->WhiteListCodes["R-\\IETF-"]            = R_IETF;          // R-x\IETF-n
  this->WhiteListCodes["R-\\PLTF-"]            = R_PLTF;          // R-x\PLTF-n
  this->WhiteListCodes["R-\\ENTF-"]            = R_ENTF;          // R-x\ENTF-n
  this->WhiteListCodes["R-\\NNET\\N-"]         = R_NNET_N;        // R-x\NNET\N-n
  this->WhiteListCodes["R-\\ENBR--"]           = R_ENBR;          // R-x\ENBR-n-m
  this->WhiteListCodes["R-\\ENAM--"]           = R_ENAM;          // R-x\ENAM-n-m
  this->WhiteListCodes["R-\\TDTF-"]            = R_TDTF;          // R-x\TDTF-n
  this->WhiteListCodes["R-\\CBTF-"]            = R_CBTF;          // R-x\CBTF-n
  this->WhiteListCodes["R-\\NCB\\N-"]          = R_NCB_N;         // R-x\NCB\N-n
  this->WhiteListCodes["R-\\CBN--"]            = R_CBN;           // R-x\CBN-n-m
  this->WhiteListCodes["R-\\CBM--"]            = R_CBM;           // R-x\CBM-n-m
  this->WhiteListCodes["R-\\CBBS--"]           = R_CBBS;          // R-x\CBBS-n-m
  this->WhiteListCodes["R-\\FCTF-"]            = R_FCTF;          // R-x\FCTF-n
  this->WhiteListCodes["R-\\FCSP-"]            = R_FCSP;          // R-x\FCSP-n
  this->WhiteListCodes["R-\\OSNM-"]            = R_OSNM;          // R-x\OSNM-n
  this->WhiteListCodes["R-\\SID-"]             = R_SID;           // R-x\SID-n
  this->WhiteListCodes["R-\\HRATE-"]           = R_HRATE;         // R-x\HRATE-n
  this->WhiteListCodes["R-\\CRATE-"]           = R_CRATE;         // R-x\CRATE-n
  this->WhiteListCodes["R-\\RT\\N"]            = R_RT_N;          // R-x\RT\N
  this->WhiteListCodes["R-\\RT1-"]             = R_RT1;           // R-x\RT1-n
  this->WhiteListCodes["R-\\RT2-"]             = R_RT2;           // R-x\RT2-n
  this->WhiteListCodes["R-\\COM"]              = R_COM;           // R-x\COM
  this->WhiteListCodes["M-\\ID"]               = M_ID;            // M-x\ID
  this->WhiteListCodes["M-\\BB1"]              = M_BB1;           // M-x\BB1
  this->WhiteListCodes["M-\\BB2"]              = M_BB2;           // M-x\BB2
  this->WhiteListCodes["M-\\BB3"]              = M_BB3;           // M-x\BB3
  this->WhiteListCodes["M-\\BSG1"]             = M_BSG1;          // M-x\BSG1
  this->WhiteListCodes["M-\\BSF1"]             = M_BSF1;          // M-x\BSF1
  this->WhiteListCodes["M-\\BSF2"]             = M_BSF2;          // M-x\BSF2
  this->WhiteListCodes["M-\\BB\\DLN"]          = M_BB_DLN;        // M-x\BB\DLN
  this->WhiteListCodes["M-\\BB\\MN"]           = M_BB_MN;         // M-x\BB\MN
  this->WhiteListCodes["M-\\SCO\\N"]           = M_SCO_N;         // M-x\SCO\N
  this->WhiteListCodes["M-\\SI\\N"]            = M_SI_N;          // M-x\SI\N
  this->WhiteListCodes["M-\\SI1-"]             = M_SI1;           // M-x\SI1-n
  this->WhiteListCodes["M-\\SI2-"]             = M_SI2;           // M-x\SI2-n
  this->WhiteListCodes["M-\\SI3-"]             = M_SI3;           // M-x\SI3-n
  this->WhiteListCodes["M-\\SIF1-"]            = M_SIF1;          // M-x\SIF1-n
  this->WhiteListCodes["M-\\SIF2-"]            = M_SIF2;          // M-x\SIF2-n
  this->WhiteListCodes["M-\\SI\\DLN-"]         = M_SI_DLN;        // M-x\SI\DLN-n
  this->WhiteListCodes["M-\\SI\\MN-"]          = M_SI_MN;         // M-x\SI\MN-n
  this->WhiteListCodes["M-\\SO"]               = M_SO;            // M-x\SO
  this->WhiteListCodes["M-\\RC"]               = M_RC;            // M-x\RC
  this->WhiteListCodes["M-\\COM"]              = M_COM;           // M-x\COM
  this->WhiteListCodes["P-\\DLN"]              = P_DLN;           // P-d\DLN
  this->WhiteListCodes["P-\\D1"]               = P_D1;            // P-d\D1
  this->WhiteListCodes["P-\\D2"]               = P_D2;            // P-d\D2
  this->WhiteListCodes["P-\\D3"]               = P_D3;            // P-d\D3
  this->WhiteListCodes["P-\\D4"]               = P_D4;            // P-d\D4
  this->WhiteListCodes["P-\\D5"]               = P_D5;            // P-d\D5
  this->WhiteListCodes["P-\\D6"]               = P_D6;            // P-d\D6
  this->WhiteListCodes["P-\\D7"]               = P_D7;            // P-d\D7
  this->WhiteListCodes["P-\\D8"]               = P_D8;            // P-d\D8
  this->WhiteListCodes["P-\\TF"]               = P_TF;            // P-d\TF
  this->WhiteListCodes["P-\\F1"]               = P_F1;            // P-d\F1
  this->WhiteListCodes["P-\\F2"]               = P_F2;            // P-d\F2
  this->WhiteListCodes["P-\\F3"]               = P_F3;            // P-d\F3
  this->WhiteListCodes["P-\\F4"]               = P_F4;            // P-d\F4
  this->WhiteListCodes["P-\\CRC"]              = P_CRC;           // P-d\CRC
  this->WhiteListCodes["P-\\CRCCB"]            = P_CRCCB;         // P-d\CRCCB
  this->WhiteListCodes["P-\\CRCDB"]            = P_CRCDB;         // P-d\CRCDB
  this->WhiteListCodes["P-\\CRCDN"]            = P_CRCDN;         // P-d\CRCDN
  this->WhiteListCodes["P-\\MF\\N"]            = P_MF_N;          // P-d\MF\N
  this->WhiteListCodes["P-\\MF1"]              = P_MF1;           // P-d\MF1
  this->WhiteListCodes["P-\\MF2"]              = P_MF2;           // P-d\MF2
  this->WhiteListCodes["P-\\MF3"]              = P_MF3;           // P-d\MF3
  this->WhiteListCodes["P-\\MF4"]              = P_MF4;           // P-d\MF4
  this->WhiteListCodes["P-\\MF5"]              = P_MF5;           // P-d\MF5
  this->WhiteListCodes["P-\\SYNC1"]            = P_SYNC1;         // P-d\SYNC1
  this->WhiteListCodes["P-\\SYNC2"]            = P_SYNC2;         // P-d\SYNC2
  this->WhiteListCodes["P-\\SYNC3"]            = P_SYNC3;         // P-d\SYNC3
  this->WhiteListCodes["P-\\SYNC4"]            = P_SYNC4;         // P-d\SYNC4
  this->WhiteListCodes["P-\\SYNC5"]            = P_SYNC5;         // P-d\SYNC5
  this->WhiteListCodes["P-\\MFW\\N"]           = P_MFW_N;         // P-d\MFW\N
  this->WhiteListCodes["P-\\MFW1-"]            = P_MFW1;          // P-d\MFW1-n
  this->WhiteListCodes["P-\\MFW2-"]            = P_MFW2;          // P-d\MFW2-n
  this->WhiteListCodes["P-\\ISF\\N"]           = P_ISF_N;         // P-d\ISF\N
  this->WhiteListCodes["P-\\ISF1-"]            = P_ISF1;          // P-d\ISF1-n
  this->WhiteListCodes["P-\\ISF2-"]            = P_ISF2;          // P-d\ISF2-n
  this->WhiteListCodes["P-\\IDC1-"]            = P_IDC1;          // P-d\IDC1-n
  this->WhiteListCodes["P-\\IDC2-"]            = P_IDC2;          // P-d\IDC2-n
  this->WhiteListCodes["P-\\IDC3-"]            = P_IDC3;          // P-d\IDC3-n
  this->WhiteListCodes["P-\\IDC4-"]            = P_IDC4;          // P-d\IDC4-n
  this->WhiteListCodes["P-\\IDC5-"]            = P_IDC5;          // P-d\IDC5-n
  this->WhiteListCodes["P-\\IDC6-"]            = P_IDC6;          // P-d\IDC6-n
  this->WhiteListCodes["P-\\IDC7-"]            = P_IDC7;          // P-d\IDC7-n
  this->WhiteListCodes["P-\\IDC8-"]            = P_IDC8;          // P-d\IDC8-n
  this->WhiteListCodes["P-\\IDC9-"]            = P_IDC9;          // P-d\IDC9-n
  this->WhiteListCodes["P-\\IDC10-"]           = P_IDC10;         // P-d\IDC10-n
  this->WhiteListCodes["P-\\SF\\N-"]           = P_SF_N;          // P-d\SF\N-n
  this->WhiteListCodes["P-\\SF1--"]            = P_SF1;           // P-d\SF1-n-m
  this->WhiteListCodes["P-\\SF2--"]            = P_SF2;           // P-d\SF2-n-m
  this->WhiteListCodes["P-\\SF3--"]            = P_SF3;           // P-d\SF3-n-m
  this->WhiteListCodes["P-\\SF4---"]           = P_SF4;           // P-d\SF4-n-m-w
  this->WhiteListCodes["P-\\SF5--"]            = P_SF5;           // P-d\SF5-n-m
  this->WhiteListCodes["P-\\SF6--"]            = P_SF6;           // P-d\SF6-n-m
  this->WhiteListCodes["P-\\MFF\\FDT"]         = P_MFF_FDT;       // P-d\MFF\FDT
  this->WhiteListCodes["P-\\MFF\\N"]           = P_MFF_N;         // P-d\MFF\N
  this->WhiteListCodes["P-\\MFF\\MFN-"]        = P_MFF_MFN;       // P-d\MFF\MFN-n
  this->WhiteListCodes["P-\\AEF\\N"]           = P_AEF_N;         // P-d\AEF\N
  this->WhiteListCodes["P-\\AEF\\DLN-"]        = P_AEF_DLN;       // P-d\AEF\DLN-n
  this->WhiteListCodes["P-\\AEF1-"]            = P_AEF1;          // P-d\AEF1-n
  this->WhiteListCodes["P-\\AEF2-"]            = P_AEF2;          // P-d\AEF2-n
  this->WhiteListCodes["P-\\AEF3--"]           = P_AEF3;          // P-d\AEF3-n-w
  this->WhiteListCodes["P-\\AEF4-"]            = P_AEF4;          // P-d\AEF4-n
  this->WhiteListCodes["P-\\AEF5--"]           = P_AEF5;          // P-d\AEF5-n-w
  this->WhiteListCodes["P-\\AEF6--"]           = P_AEF6;          // P-d\AEF6-n-w
  this->WhiteListCodes["P-\\AEF7--"]           = P_AEF7;          // P-d\AEF7-n-w
  this->WhiteListCodes["P-\\AEF8---"]          = P_AEF8;          // P-d\AEF8-n-w-m
  this->WhiteListCodes["P-\\AEF9---"]          = P_AEF9;          // P-d\AEF9-n-w-m
  this->WhiteListCodes["P-\\AEF10---"]         = P_AEF10;         // P-d\AEF10-n-w-m
  this->WhiteListCodes["P-\\FFI1"]             = P_FFI1;          // P-d\FFI1
  this->WhiteListCodes["P-\\FFI2"]             = P_FFI2;          // P-d\FFI2
  this->WhiteListCodes["P-\\MLC\\N"]           = P_MLC_N;         // P-d\MLC\N
  this->WhiteListCodes["P-\\MLC1-"]            = P_MLC1;          // P-d\MLC1-n
  this->WhiteListCodes["P-\\MLC2-"]            = P_MLC2;          // P-d\MLC2-n
  this->WhiteListCodes["P-\\FSC\\N"]           = P_FSC_N;         // P-d\FSC\N
  this->WhiteListCodes["P-\\FSC1-"]            = P_FSC1;          // P-d\FSC1-n
  this->WhiteListCodes["P-\\FSC2-"]            = P_FSC2;          // P-d\FSC2-n
  this->WhiteListCodes["P-\\ALT\\N"]           = P_ALT_N;         // P-d\ALT\N
  this->WhiteListCodes["P-\\ALT1"]             = P_ALT1;          // P-d\ALT1
  this->WhiteListCodes["P-\\ALT2"]             = P_ALT2;          // P-d\ALT2
  this->WhiteListCodes["P-\\ALT3"]             = P_ALT3;          // P-d\ALT3
  this->WhiteListCodes["P-\\ALT4"]             = P_ALT4;          // P-d\ALT4
  this->WhiteListCodes["P-\\ADM\\N"]           = P_ADM_N;         // P-d\ADM\N
  this->WhiteListCodes["P-\\ADM\\DMN-"]        = P_ADM_DMN;       // P-d\ADM\DMN-n
  this->WhiteListCodes["P-\\ADM\\MP-"]         = P_ADM_MP;        // P-d\ADM\MP-n
  this->WhiteListCodes["P-\\ADM\\OHM-"]        = P_ADM_OHM;       // P-d\ADM\OHM-n
  this->WhiteListCodes["P-\\ADM\\FDP-"]        = P_ADM_FDP;       // P-d\ADM\FDP-n
  this->WhiteListCodes["P-\\ADM\\DOP-"]        = P_ADM_DOP;       // P-d\ADM\DOP-n
  this->WhiteListCodes["P-\\ADM\\SDP-"]        = P_ADM_SDP;       // P-d\ADM\SDP-n
  this->WhiteListCodes["P-\\ADM\\UDP-"]        = P_ADM_UDP;       // P-d\ADM\UDP-n
  this->WhiteListCodes["P-\\ADM1-"]            = P_ADM1;          // P-d\ADM1-n
  this->WhiteListCodes["P-\\ADM2-"]            = P_ADM2;          // P-d\ADM2-n
  this->WhiteListCodes["P-\\ADM3--"]           = P_ADM3;          // P-d\ADM3-n-w
  this->WhiteListCodes["P-\\ADM4-"]            = P_ADM4;          // P-d\ADM4-n
  this->WhiteListCodes["P-\\ADM5-"]            = P_ADM5;          // P-d\ADM5-n
  this->WhiteListCodes["P-\\ADM6-"]            = P_ADM6;          // P-d\ADM6-n
  this->WhiteListCodes["P-\\ADM7-"]            = P_ADM7;          // P-d\ADM7-n
  this->WhiteListCodes["P-\\ADM8--"]           = P_ADM8;          // P-d\ADM8-n-w
  this->WhiteListCodes["P-\\ADM9---"]          = P_ADM9;          // P-d\ADM9-n-w-m
  this->WhiteListCodes["P-\\ADM10---"]         = P_ADM10;         // P-d\ADM10-n-w-m
  this->WhiteListCodes["P-\\ADM11---"]         = P_ADM11;         // P-d\ADM11-n-w-m
  this->WhiteListCodes["P-\\C7\\N"]            = P_C7_N;          // P-d\C7\N
  this->WhiteListCodes["P-\\C7FW-"]            = P_C7FW;          // P-d\C7FW-n
  this->WhiteListCodes["P-\\C7NW-"]            = P_C7NW;          // P-d\C7NW-n
  this->WhiteListCodes["P-\\COM"]              = P_COM;           // P-d\COM
  this->WhiteListCodes["D-\\DLN"]              = D_DLN;           // D-x\DLN
  this->WhiteListCodes["D-\\ML\\N"]            = D_ML_N;          // D-x\ML\N
  this->WhiteListCodes["D-\\MLN-"]             = D_MLN;           // D-x\MLN-y
  this->WhiteListCodes["D-\\SMF\\N-"]          = D_SMF_NX;        // D-x\SMF\N-y
  this->WhiteListCodes["D-\\SMF\\N"]           = D_SMF_N;         // D-x\SMF\N
  this->WhiteListCodes["D-\\SMF\\SMN--"]       = D_SMF_SMNX;      // D-x\SMF\SMN-y-n
  this->WhiteListCodes["D-\\SMF\\SMN-"]        = D_SMF_SMN;       // D-x\SMF\SMN-y
  this->WhiteListCodes["D-\\SMF\\MFOT-"]       = D_SMF_MFOT;      // D-x\SMF\MFOT-y
  this->WhiteListCodes["D-\\MN\\N-"]           = D_MN_N;          // D-x\MN\N-y
  this->WhiteListCodes["D-\\MN--"]             = D_MN;            // D-x\MN-y-n
  this->WhiteListCodes["D-\\MN1--"]            = D_MN1;           // D-x\MN1-y-n
  this->WhiteListCodes["D-\\MN2--"]            = D_MN2;           // D-x\MN2-y-n
  this->WhiteListCodes["D-\\MN3--"]            = D_MN3;           // D-x\MN3-y-n
  this->WhiteListCodes["D-\\MFOT--"]           = D_MFOT;          // D-x\MFOT-y-n
  this->WhiteListCodes["D-\\LT--"]             = D_LT;            // D-x\LT-y-n
  this->WhiteListCodes["D-\\MF--"]             = D_MF;            // D-x\MF-y-n
  this->WhiteListCodes["D-\\MFM--"]            = D_MFM;           // D-x\MFM-y-n
  this->WhiteListCodes["D-\\MFS\\N--"]         = D_MFS_N;         // D-x\MFS\N-y-n
  this->WhiteListCodes["D-\\MFS1--"]           = D_MFS1;          // D-x\MFS1-y-n
  this->WhiteListCodes["D-\\MFS2--"]           = D_MFS2;          // D-x\MFS2-y-n
  this->WhiteListCodes["D-\\MFS3--"]           = D_MFS3;          // D-x\MFS3-y-n
  this->WhiteListCodes["D-\\MFS4--"]           = D_MFS4;          // D-x\MFS4-y-n
  this->WhiteListCodes["D-\\MFSW---"]          = D_MFSW;          // D-x\MFSW-y-n-e
  this->WhiteListCodes["D-\\MFSM---"]          = D_MFSM;          // D-x\MFSM-y-n-e
  this->WhiteListCodes["D-\\FMF\\N--"]         = D_FMF_N;         // D-x\FMF\N-y-n
  this->WhiteListCodes["D-\\FMF1--"]           = D_FMF1;          // D-x\FMF1-y-n
  this->WhiteListCodes["D-\\FMF2--"]           = D_FMF2;          // D-x\FMF2-y-n
  this->WhiteListCodes["D-\\FMF3--"]           = D_FMF3;          // D-x\FMF3-y-n
  this->WhiteListCodes["D-\\FMF4--"]           = D_FMF4;          // D-x\FMF4-y-n
  this->WhiteListCodes["D-\\FMF5--"]           = D_FMF5;          // D-x\FMF5-y-n
  this->WhiteListCodes["D-\\FMF6---"]          = D_FMF6;          // D-x\FMF6-y-n-e
  this->WhiteListCodes["D-\\FMF7---"]          = D_FMF7;          // D-x\FMF7-y-n-e
  this->WhiteListCodes["D-\\FMF8---"]          = D_FMF8;          // D-x\FMF8-y-n-e
  this->WhiteListCodes["D-\\FMF9---"]          = D_FMF9;          // D-x\FMF9-y-n-e
  this->WhiteListCodes["D-\\SF1--"]            = D_SF1;           // D-x\SF1-y-n
  this->WhiteListCodes["D-\\SF2--"]            = D_SF2;           // D-x\SF2-y-n
  this->WhiteListCodes["D-\\SFM--"]            = D_SFM;           // D-x\SFM-y-n
  this->WhiteListCodes["D-\\SFS\\N--"]         = D_SFS_N;         // D-x\SFS\N-y-n
  this->WhiteListCodes["D-\\SFS1--"]           = D_SFS1;          // D-x\SFS1-y-n
  this->WhiteListCodes["D-\\SFS2--"]           = D_SFS2;          // D-x\SFS2-y-n
  this->WhiteListCodes["D-\\SFS3--"]           = D_SFS3;          // D-x\SFS3-y-n
  this->WhiteListCodes["D-\\SFS4--"]           = D_SFS4;          // D-x\SFS4-y-n
  this->WhiteListCodes["D-\\SFS5--"]           = D_SFS5;          // D-x\SFS5-y-n
  this->WhiteListCodes["D-\\SFS6---"]          = D_SFS6;          // D-x\SFS6-y-n-e
  this->WhiteListCodes["D-\\SFS7---"]          = D_SFS7;          // D-x\SFS7-y-n-e
  this->WhiteListCodes["D-\\FSF\\N--"]         = D_FSF_N;         // D-x\FSF\N-y-n
  this->WhiteListCodes["D-\\FSF1--"]           = D_FSF1;          // D-x\FSF1-y-n
  this->WhiteListCodes["D-\\FSF2\\N--"]        = D_FSF2_N;        // D-x\FSF2\N-y-n
  this->WhiteListCodes["D-\\FSF3---"]          = D_FSF3;          // D-x\FSF3-y-n-m
  this->WhiteListCodes["D-\\FSF4---"]          = D_FSF4;          // D-x\FSF4-y-n-m
  this->WhiteListCodes["D-\\FSF5---"]          = D_FSF5;          // D-x\FSF5-y-n-m
  this->WhiteListCodes["D-\\FSF6---"]          = D_FSF6;          // D-x\FSF6-y-n-m
  this->WhiteListCodes["D-\\FSF7---"]          = D_FSF7;          // D-x\FSF7-y-n-m
  this->WhiteListCodes["D-\\FSF8----"]         = D_FSF8;          // D-x\FSF8-y-n-m-e
  this->WhiteListCodes["D-\\FSF9----"]         = D_FSF9;          // D-x\FSF9-y-n-m-e
  this->WhiteListCodes["D-\\FSF10----"]        = D_FSF10;         // D-x\FSF10-y-n-m-e
  this->WhiteListCodes["D-\\FSF11----"]        = D_FSF11;         // D-x\FSF11-y-n-m-e
  this->WhiteListCodes["D-\\IDCN--"]           = D_IDCN;          // D-x\IDCN-y-n
  this->WhiteListCodes["D-\\MML\\N--"]         = D_MML_N;         // D-x\MML\N-y-n
  this->WhiteListCodes["D-\\MNF\\N---"]        = D_MNF_N;         // D-x\MNF\N-y-n-m
  this->WhiteListCodes["D-\\MWL---"]           = D_MWL;           // D-x\MWL-y-n-m
  this->WhiteListCodes["D-\\WP----"]           = D_WP;            // D-x\WP-y-n-m-e
  this->WhiteListCodes["D-\\WI----"]           = D_WI;            // D-x\WI-y-n-m-e
  this->WhiteListCodes["D-\\EWP----"]          = D_EWP;           // D-x\EWP-y-n-m-e
  this->WhiteListCodes["D-\\FP----"]           = D_FP;            // D-x\FP-y-n-m-e
  this->WhiteListCodes["D-\\FI----"]           = D_FI;            // D-x\FI-y-n-m-e
  this->WhiteListCodes["D-\\EFP----"]          = D_EFP;           // D-x\EFP-y-n-m-e
  this->WhiteListCodes["D-\\WFM----"]          = D_WFM;           // D-x\WFM-y-n-m-e
  this->WhiteListCodes["D-\\WFT----"]          = D_WFT;           // D-x\WFT-y-n-m-e
  this->WhiteListCodes["D-\\WFP----"]          = D_WFP;           // D-x\WFP-y-n-m-e
  this->WhiteListCodes["D-\\SS--"]             = D_SS;            // D-x\SS-y-n
  this->WhiteListCodes["D-\\SON--"]            = D_SON;           // D-x\SON-y-n
  this->WhiteListCodes["D-\\SMN--"]            = D_SMN;           // D-x\SMN-y-n
  this->WhiteListCodes["D-\\SS\\N--"]          = D_SS_N;          // D-x\SS\N-y-n
  this->WhiteListCodes["D-\\SS1---"]           = D_SS1;           // D-x\SS1-y-n-s
  this->WhiteListCodes["D-\\SS2---"]           = D_SS2;           // D-x\SS2-y-n-s
  this->WhiteListCodes["D-\\TD\\N--"]          = D_TD_N;          // D-x\TD\N-y-n
  this->WhiteListCodes["D-\\TD1--"]            = D_TD1;           // D-x\TD1-y-n
  this->WhiteListCodes["D-\\TD2---"]           = D_TD2;           // D-x\TD2-y-n-m
  this->WhiteListCodes["D-\\TD3---"]           = D_TD3;           // D-x\TD3-y-n-m
  this->WhiteListCodes["D-\\TD4---"]           = D_TD4;           // D-x\TD4-y-n-m
  this->WhiteListCodes["D-\\TD5---"]           = D_TD5;           // D-x\TD5-y-n-m
  this->WhiteListCodes["D-\\REL\\N--"]         = D_REL_N;         // D-x\REL\N-y-n
  this->WhiteListCodes["D-\\REL1---"]          = D_REL1;          // D-x\REL1-y-n-m
  this->WhiteListCodes["D-\\REL2---"]          = D_REL2;          // D-x\REL2-y-n-m
  this->WhiteListCodes["D-\\REL3---"]          = D_REL3;          // D-x\REL3-y-n-m
  this->WhiteListCodes["D-\\REL4---"]          = D_REL4;          // D-x\REL4-y-n-m
  this->WhiteListCodes["D-\\COM"]              = D_COM;           // D-x\COM
  this->WhiteListCodes["B-\\DLN"]              = B_DLN;           // B-x\DLN
  this->WhiteListCodes["B-\\TA"]               = B_TA;            // B-x\TA
  this->WhiteListCodes["B-\\BP"]               = B_BP;            // B-x\BP
  this->WhiteListCodes["B-\\NBS\\N"]           = B_NBS_N;         // B-x\NBS\N
  this->WhiteListCodes["B-\\BID-"]             = B_BID;           // B-x\BID-i
  this->WhiteListCodes["B-\\BNA-"]             = B_BNA;           // B-x\BNA-i
  this->WhiteListCodes["B-\\BT-"]              = B_BT;            // B-x\BT-i
  this->WhiteListCodes["B-\\UMN1-"]            = B_UMN1;          // B-x\UMN1-i
  this->WhiteListCodes["B-\\U1P-"]             = B_U1P;           // B-x\U1P-i
  this->WhiteListCodes["B-\\U1PT-"]            = B_U1PT;          // B-x\U1PT-i
  this->WhiteListCodes["B-\\U1M-"]             = B_U1M;           // B-x\U1M-i
  this->WhiteListCodes["B-\\U1T-"]             = B_U1T;           // B-x\U1T-i
  this->WhiteListCodes["B-\\UMN2-"]            = B_UMN2;          // B-x\UMN2-i
  this->WhiteListCodes["B-\\U2P-"]             = B_U2P;           // B-x\U2P-i
  this->WhiteListCodes["B-\\U2PT-"]            = B_U2PT;          // B-x\U2PT-i
  this->WhiteListCodes["B-\\U2M-"]             = B_U2M;           // B-x\U2M-i
  this->WhiteListCodes["B-\\U2T-"]             = B_U2T;           // B-x\U2T-i
  this->WhiteListCodes["B-\\UMN3-"]            = B_UMN3;          // B-x\UMN3-i
  this->WhiteListCodes["B-\\U3P-"]             = B_U3P;           // B-x\U3P-i
  this->WhiteListCodes["B-\\U3PT-"]            = B_U3PT;          // B-x\U3PT-i
  this->WhiteListCodes["B-\\U3M-"]             = B_U3M;           // B-x\U3M-i
  this->WhiteListCodes["B-\\U3T-"]             = B_U3T;           // B-x\U3T-i
  this->WhiteListCodes["B-\\TK\\N-"]           = B_TK_N;          // B-x\TK\N-i
  this->WhiteListCodes["B-\\TS--"]             = B_TS;            // B-x\TS-i-k
  this->WhiteListCodes["B-\\NMS\\N-"]          = B_NMS_N;         // B-x\NMS\N-i
  this->WhiteListCodes["B-\\MID--"]            = B_MID;           // B-x\MID-i-n
  this->WhiteListCodes["B-\\MNA--"]            = B_MNA;           // B-x\MNA-i-n
  this->WhiteListCodes["B-\\CWE--"]            = B_CWE;           // B-x\CWE-i-n
  this->WhiteListCodes["B-\\CMD--"]            = B_CMD;           // B-x\CMD-i-n
  this->WhiteListCodes["B-\\TRN--"]            = B_TRN;           // B-x\TRN-i-n
  this->WhiteListCodes["B-\\TRA--"]            = B_TRA;           // B-x\TRA-i-n
  this->WhiteListCodes["B-\\STN--"]            = B_STN;           // B-x\STN-i-n
  this->WhiteListCodes["B-\\STA--"]            = B_STA;           // B-x\STA-i-n
  this->WhiteListCodes["B-\\TRM--"]            = B_TRM;           // B-x\TRM-i-n
  this->WhiteListCodes["B-\\DWC--"]            = B_DWC;           // B-x\DWC-i-n
  this->WhiteListCodes["B-\\SPR--"]            = B_SPR;           // B-x\SPR-i-n
  this->WhiteListCodes["B-\\BMG\\FOT--"]       = B_BMG_FOT;       // B-x\BMG\FOT-i-n
  this->WhiteListCodes["B-\\LBL--"]            = B_LBL;           // B-x\LBL-i-n
  this->WhiteListCodes["B-\\SDI--"]            = B_SDI;           // B-x\SDI-i-n
  this->WhiteListCodes["B-\\RCWE--"]           = B_RCWE;          // B-x\RCWE-i-n
  this->WhiteListCodes["B-\\RCMD--"]           = B_RCMD;          // B-x\RCMD-i-n
  this->WhiteListCodes["B-\\RTRN---"]          = B_RTRN;          // B-x\RTRN-i-n-m
  this->WhiteListCodes["B-\\RTRA---"]          = B_RTRA;          // B-x\RTRA-i-n-m
  this->WhiteListCodes["B-\\RSTN---"]          = B_RSTN;          // B-x\RSTN-i-n-m
  this->WhiteListCodes["B-\\RSTA---"]          = B_RSTA;          // B-x\RSTA-i-n-m
  this->WhiteListCodes["B-\\RDWC---"]          = B_RDWC;          // B-x\RDWC-i-n-m
  this->WhiteListCodes["B-\\MCD--"]            = B_MCD;           // B-x\MCD-i-n
  this->WhiteListCodes["B-\\MCW--"]            = B_MCW;           // B-x\MCW-i-n
  this->WhiteListCodes["B-\\MN\\N--"]          = B_MN_N;          // B-x\MN\N-i-n
  this->WhiteListCodes["B-\\MN---"]            = B_MN;            // B-x\MN-i-n-p
  this->WhiteListCodes["B-\\MT---"]            = B_MT;            // B-x\MT-i-n-p
  this->WhiteListCodes["B-\\MN1---"]           = B_MN1;           // B-x\MN1-i-n-p
  this->WhiteListCodes["B-\\MN2---"]           = B_MN2;           // B-x\MN2-i-n-p
  this->WhiteListCodes["B-\\BME\\FOT---"]      = B_BME_FOT;       // B-x\BME\FOT-i-n-p
  this->WhiteListCodes["B-\\NML\\N---"]        = B_NML_N;         // B-x\NML\N-i-n-p
  this->WhiteListCodes["B-\\MWN----"]          = B_MWN;           // B-x\MWN-i-n-p-e
  this->WhiteListCodes["B-\\MBM----"]          = B_MBM;           // B-x\MBM-i-n-p-e
  this->WhiteListCodes["B-\\MTO----"]          = B_MTO;           // B-x\MTO-i-n-p-e
  this->WhiteListCodes["B-\\MFP----"]          = B_MFP;           // B-x\MFP-i-n-p-e
  this->WhiteListCodes["B-\\COM"]              = B_COM;           // B-x\COM
  this->WhiteListCodes["S-\\DLN"]              = S_DLN;           // S-d\DLN
  this->WhiteListCodes["S-\\ASM"]              = S_ASM;           // S-x\ASM
  this->WhiteListCodes["S-\\FEF"]              = S_FEF;           // S-x\FEF
  this->WhiteListCodes["S-\\CEF"]              = S_CEF;           // S-x\CEF
  this->WhiteListCodes["S-\\TFL"]              = S_TFL;           // S-x\TFL
  this->WhiteListCodes["S-\\TA"]               = S_TA;            // S-d\TA
  this->WhiteListCodes["S-\\VID"]              = S_VID;           // S-x\VID
  this->WhiteListCodes["S-\\OCF"]              = S_OCF;           // S-x\OCF
  this->WhiteListCodes["S-\\SHF"]              = S_SHF;           // S-x\SHF
  this->WhiteListCodes["S-\\OSF"]              = S_OSF;           // S-x\OSF
  this->WhiteListCodes["S-\\SHL"]              = S_SHL;           // S-x\SHL
  this->WhiteListCodes["S-\\TNMS\\N"]          = S_TNMS_N;        // S-x\TNMS\N
  this->WhiteListCodes["S-\\TMN-"]             = S_TMN;           // S-x\TMN-n
  this->WhiteListCodes["S-\\TPAR-"]            = S_TPAR;          // S-x\TPAR-n
  this->WhiteListCodes["S-\\TPTO-"]            = S_TPTO;          // S-x\TPTO-n
  this->WhiteListCodes["S-\\TNML\\N-"]         = S_TNML_N;        // S-x\TNML\N-n
  this->WhiteListCodes["S-\\TWP--"]            = S_TWP;           // S-x\TWP-n-m
  this->WhiteListCodes["S-\\TWL--"]            = S_TWL;           // S-x\TWL-n-m
  this->WhiteListCodes["S-\\TBM--"]            = S_TBM;           // S-x\TBM-n-m
  this->WhiteListCodes["S-\\TTO--"]            = S_TTO;           // S-x\TTO-n-m
  this->WhiteListCodes["S-\\TFP--"]            = S_TFP;           // S-x\TFP-n-m
  this->WhiteListCodes["S-\\DFL"]              = S_DFL;           // S-x\DFL
  this->WhiteListCodes["S-\\SP\\N"]            = S_SP_N;          // S-x\SP\N
  this->WhiteListCodes["S-\\PSHF-"]            = S_PSHF;          // S-x\PSHF-n
  this->WhiteListCodes["S-\\APID-"]            = S_APID;          // S-x\APID-n
  this->WhiteListCodes["S-\\PDL-"]             = S_PDL;           // S-x\PDL-n
  this->WhiteListCodes["S-\\PSHL-"]            = S_PSHL;          // S-x\PSHL-n
  this->WhiteListCodes["S-\\HNMS\\N-"]         = S_HNMS_N;        // S-x\HNMS\N-n
  this->WhiteListCodes["S-\\HMN--"]            = S_HMN;           // S-x\HMN-n-m
  this->WhiteListCodes["S-\\HPAR--"]           = S_HPAR;          // S-x\HPAR-n-m
  this->WhiteListCodes["S-\\HPTO--"]           = S_HPTO;          // S-x\HPTO-n-m
  this->WhiteListCodes["S-\\HNML\\N--"]        = S_HNML_N;        // S-x\HNML\N-n-m
  this->WhiteListCodes["S-\\HWP---"]           = S_HWP;           // S-x\HWP-n-m-e
  this->WhiteListCodes["S-\\HWL---"]           = S_HWL;           // S-x\HWL-n-m-e
  this->WhiteListCodes["S-\\HBM---"]           = S_HBM;           // S-x\HBM-n-m-e
  this->WhiteListCodes["S-\\HTO---"]           = S_HTO;           // S-x\HTO-n-m-e
  this->WhiteListCodes["S-\\HFP---"]           = S_HFP;           // S-x\HFP-n-m-e
  this->WhiteListCodes["S-\\SNMS\\N-"]         = S_SNMS_N;        // S-x\SNMS\N-n
  this->WhiteListCodes["S-\\SMN--"]            = S_SMN;           // S-x\SMN-n-m
  this->WhiteListCodes["S-\\SPAR--"]           = S_SPAR;          // S-x\SPAR-n-m
  this->WhiteListCodes["S-\\SPTO--"]           = S_SPTO;          // S-x\SPTO-n-m
  this->WhiteListCodes["S-\\SNML\\N--"]        = S_SNML_N;        // S-x\SNML\N-n-m
  this->WhiteListCodes["S-\\SWP---"]           = S_SWP;           // S-x\SWP-n-m-e
  this->WhiteListCodes["S-\\SWL---"]           = S_SWL;           // S-x\SWL-n-m-e
  this->WhiteListCodes["S-\\SBM---"]           = S_SBM;           // S-x\SBM-n-m-e
  this->WhiteListCodes["S-\\STO---"]           = S_STO;           // S-x\STO-n-m-e
  this->WhiteListCodes["S-\\SFP---"]           = S_SFP;           // S-x\SFP-n-m-e
  this->WhiteListCodes["S-\\NS\\N"]            = S_NS_N;          // S-d\NS\N
  this->WhiteListCodes["S-\\SNA-"]             = S_SNA;           // S-d\SNA-i
  this->WhiteListCodes["S-\\MDT-"]             = S_MDT;           // S-d\MDT-i
  this->WhiteListCodes["S-\\MDL-"]             = S_MDL;           // S-d\MDL-i
  this->WhiteListCodes["S-\\MES-"]             = S_MES;           // S-d\MES-i
  this->WhiteListCodes["S-\\MIDL-"]            = S_MIDL;          // S-d\MIDL-i
  this->WhiteListCodes["S-\\MLEN-"]            = S_MLEN;          // S-d\MLEN-i
  this->WhiteListCodes["S-\\MDEL-"]            = S_MDEL;          // S-d\MDEL-i
  this->WhiteListCodes["S-\\MDLEN-"]           = S_MDLEN;         // S-d\MDLEN-i
  this->WhiteListCodes["S-\\FDEL-"]            = S_FDEL;          // S-d\FDEL-i
  this->WhiteListCodes["S-\\DO-"]              = S_DO;            // S-d\DO-i
  this->WhiteListCodes["S-\\NMS\\N-"]          = S_NMS_N;         // S-d\NMS\N-i
  this->WhiteListCodes["S-\\MID--"]            = S_MID;           // S-d\MID-i-n
  this->WhiteListCodes["S-\\MNA--"]            = S_MNA;           // S-d\MNA-i-n
  this->WhiteListCodes["S-\\NFLDS\\N--"]       = S_NFLDS_N;       // S-d\NFLDS\N-i-n
  this->WhiteListCodes["S-\\FNUM---"]          = S_FNUM;          // S-d\FNUM-i-n-m
  this->WhiteListCodes["S-\\FPOS---"]          = S_FPOS;          // S-d\FPOS-i-n-m
  this->WhiteListCodes["S-\\FLEN---"]          = S_FLEN;          // S-d\FLEN-i-n-m
  this->WhiteListCodes["S-\\MN\\N--"]          = S_MN_N;          // S-d\MN\N-i-n
  this->WhiteListCodes["S-\\MN---"]            = S_MN;            // S-d\MN-i-n-p
  this->WhiteListCodes["S-\\MN1---"]           = S_MN1;           // S-d\MN1-i-n-p
  this->WhiteListCodes["S-\\MN2---"]           = S_MN2;           // S-d\MN2-i-n-p
  this->WhiteListCodes["S-\\MBFM---"]          = S_MBFM;          // S-d\MBFM-i-n-p
  this->WhiteListCodes["S-\\MFPF---"]          = S_MFPF;          // S-d\MFPF-i-n-p
  this->WhiteListCodes["S-\\MDO---"]           = S_MDO;           // S-d\MDO-i-n-p
  this->WhiteListCodes["S-\\NML\\N---"]        = S_NML_N;         // S-d\NML\N-i-n-p
  this->WhiteListCodes["S-\\MFN----"]          = S_MFN;           // S-d\MFN-i-n-p-e
  this->WhiteListCodes["S-\\MBM----"]          = S_MBM;           // S-d\MBM-i-n-p-e
  this->WhiteListCodes["S-\\MTO----"]          = S_MTO;           // S-d\MTO-i-n-p-e
  this->WhiteListCodes["S-\\MFP----"]          = S_MFP;           // S-d\MFP-i-n-p-e
  this->WhiteListCodes["S-\\COM-"]             = S_COMX;          // S-x\COM-n
  this->WhiteListCodes["S-\\COM"]              = S_COM;           // S-d\COM
  this->WhiteListCodes["A-\\DLN"]              = A_DLN;           // A-x\DLN
  this->WhiteListCodes["A-\\A1"]               = A_A1;            // A-x\A1
  this->WhiteListCodes["A-\\A2"]               = A_A2;            // A-x\A2
  this->WhiteListCodes["A-\\A3"]               = A_A3;            // A-x\A3
  this->WhiteListCodes["A-\\A4"]               = A_A4;            // A-x\A4
  this->WhiteListCodes["A-\\A5"]               = A_A5;            // A-x\A5
  this->WhiteListCodes["A-\\A\\N"]             = A_A_N;           // A-x\A\N
  this->WhiteListCodes["A-\\A\\MN\\N"]         = A_A_MN_N;        // A-x\A\MN\N
  this->WhiteListCodes["A-\\RC1"]              = A_RC1;           // A-x\RC1
  this->WhiteListCodes["A-\\RC2"]              = A_RC2;           // A-x\RC2
  this->WhiteListCodes["A-\\RC3"]              = A_RC3;           // A-x\RC3
  this->WhiteListCodes["A-\\SF\\N"]            = A_SF_N;          // A-x\SF\N
  this->WhiteListCodes["A-\\SF1-"]             = A_SF1;           // A-x\SF1-n
  this->WhiteListCodes["A-\\SF2-"]             = A_SF2;           // A-x\SF2-n
  this->WhiteListCodes["A-\\SF3-"]             = A_SF3;           // A-x\SF3-n
  this->WhiteListCodes["A-\\MN1-"]             = A_MN1;           // A-x\MN1-n
  this->WhiteListCodes["A-\\MN2-"]             = A_MN2;           // A-x\MN2-n
  this->WhiteListCodes["A-\\MN3-"]             = A_MN3;           // A-x\MN3-n
  this->WhiteListCodes["A-\\LCW--"]            = A_LCW;           // A-x\LCW-n-s
  this->WhiteListCodes["A-\\LCN---"]           = A_LCN;           // A-x\LCN-n-s-r
  this->WhiteListCodes["A-\\COM"]              = A_COM;           // A-x\COM
  this->WhiteListCodes["C-\\DCN"]              = C_DCN;           // C-d\DCN
  this->WhiteListCodes["C-\\TRD1"]             = C_TRD1;          // C-d\TRD1
  this->WhiteListCodes["C-\\TRD2"]             = C_TRD2;          // C-d\TRD2
  this->WhiteListCodes["C-\\TRD3"]             = C_TRD3;          // C-d\TRD3
  this->WhiteListCodes["C-\\TRD4"]             = C_TRD4;          // C-d\TRD4
  this->WhiteListCodes["C-\\TRD5"]             = C_TRD5;          // C-d\TRD5
  this->WhiteListCodes["C-\\TRD6"]             = C_TRD6;          // C-d\TRD6
  this->WhiteListCodes["C-\\TRD7"]             = C_TRD7;          // C-d\TRD7
  this->WhiteListCodes["C-\\POC1"]             = C_POC1;          // C-d\POC1
  this->WhiteListCodes["C-\\POC2"]             = C_POC2;          // C-d\POC2
  this->WhiteListCodes["C-\\POC3"]             = C_POC3;          // C-d\POC3
  this->WhiteListCodes["C-\\POC4"]             = C_POC4;          // C-d\POC4
  this->WhiteListCodes["C-\\MN1"]              = C_MN1;           // C-d\MN1
  this->WhiteListCodes["C-\\MNA"]              = C_MNA;           // C-d\MNA
  this->WhiteListCodes["C-\\MN2"]              = C_MN2;           // C-d\MN2
  this->WhiteListCodes["C-\\MN3"]              = C_MN3;           // C-d\MN3
  this->WhiteListCodes["C-\\MN4"]              = C_MN4;           // C-d\MN4
  this->WhiteListCodes["C-\\BFM"]              = C_BFM;           // C-d\BFM
  this->WhiteListCodes["C-\\FPF"]              = C_FPF;           // C-d\FPF
  this->WhiteListCodes["C-\\BWT\\N"]           = C_BWT_N;         // C-d\BWT\N
  this->WhiteListCodes["C-\\BWTB-"]            = C_BWTB;          // C-d\BWTB-n
  this->WhiteListCodes["C-\\BWTV-"]            = C_BWTV;          // C-d\BWTV-n
  this->WhiteListCodes["C-\\MC\\N"]            = C_MC_N;          // C-d\MC\N
  this->WhiteListCodes["C-\\MC1-"]             = C_MC1;           // C-d\MC1-n
  this->WhiteListCodes["C-\\MC2-"]             = C_MC2;           // C-d\MC2-n
  this->WhiteListCodes["C-\\MC3-"]             = C_MC3;           // C-d\MC3-n
  this->WhiteListCodes["C-\\MA\\N"]            = C_MA_N;          // C-d\MA\N
  this->WhiteListCodes["C-\\MA1-"]             = C_MA1;           // C-d\MA1-n
  this->WhiteListCodes["C-\\MA2-"]             = C_MA2;           // C-d\MA2-n
  this->WhiteListCodes["C-\\MA3-"]             = C_MA3;           // C-d\MA3-n
  this->WhiteListCodes["C-\\FEN"]              = C_FEN;           // C-d\FEN
  this->WhiteListCodes["C-\\FDL"]              = C_FDL;           // C-d\FDL
  this->WhiteListCodes["C-\\F\\N"]             = C_F_N;           // C-d\F\N
  this->WhiteListCodes["C-\\FTY-"]             = C_FTY;           // C-d\FTY-n
  this->WhiteListCodes["C-\\FNPS-"]            = C_FNPS;          // C-d\FNPS-n
  this->WhiteListCodes["C-\\MOT1"]             = C_MOT1;          // C-d\MOT1
  this->WhiteListCodes["C-\\MOT2"]             = C_MOT2;          // C-d\MOT2
  this->WhiteListCodes["C-\\MOT3"]             = C_MOT3;          // C-d\MOT3
  this->WhiteListCodes["C-\\MOT4"]             = C_MOT4;          // C-d\MOT4
  this->WhiteListCodes["C-\\MOT5"]             = C_MOT5;          // C-d\MOT5
  this->WhiteListCodes["C-\\MOT6"]             = C_MOT6;          // C-d\MOT6
  this->WhiteListCodes["C-\\MOT7"]             = C_MOT7;          // C-d\MOT7
  this->WhiteListCodes["C-\\SR"]               = C_SR;            // C-d\SR
  this->WhiteListCodes["C-\\CRT"]              = C_CRT;           // C-d\CRT
  this->WhiteListCodes["C-\\DCT"]              = C_DCT;           // C-d\DCT
  this->WhiteListCodes["C-\\PS\\N"]            = C_PS_N;          // C-d\PS\N
  this->WhiteListCodes["C-\\PS1"]              = C_PS1;           // C-d\PS1
  this->WhiteListCodes["C-\\PS2"]              = C_PS2;           // C-d\PS2
  this->WhiteListCodes["C-\\PS3-"]             = C_PS3;           // C-d\PS3-n
  this->WhiteListCodes["C-\\PS4-"]             = C_PS4;           // C-d\PS4-n
  this->WhiteListCodes["C-\\CO\\N"]            = C_CO_N;          // C-d\CO\N
  this->WhiteListCodes["C-\\CO1"]              = C_CO1;           // C-d\CO1
  this->WhiteListCodes["C-\\CO"]               = C_CO;            // C-d\CO
  this->WhiteListCodes["C-\\CO-"]              = C_COX;           // C-d\CO-n
  this->WhiteListCodes["C-\\NPC\\N"]           = C_NPC_N;         // C-d\NPC\N
  this->WhiteListCodes["C-\\NPC1"]             = C_NPC1;          // C-d\NPC1
  this->WhiteListCodes["C-\\NPC"]              = C_NPC;           // C-d\NPC
  this->WhiteListCodes["C-\\NPC-"]             = C_NPCX;          // C-d\NPC-n
  this->WhiteListCodes["C-\\OTH"]              = C_OTH;           // C-d\OTH
  this->WhiteListCodes["C-\\DPAT"]             = C_DPAT;          // C-d\DPAT
  this->WhiteListCodes["C-\\DPA"]              = C_DPA;           // C-d\DPA
  this->WhiteListCodes["C-\\DPTM"]             = C_DPTM;          // C-d\DPTM
  this->WhiteListCodes["C-\\DPNO"]             = C_DPNO;          // C-d\DPNO
  this->WhiteListCodes["C-\\DP\\N"]            = C_DP_N;          // C-d\DP\N
  this->WhiteListCodes["C-\\DP-"]              = C_DP;            // C-d\DP-n
  this->WhiteListCodes["C-\\DPC\\N"]           = C_DPC_N;         // C-d\DPC\N
  this->WhiteListCodes["C-\\DPC-"]             = C_DPC;           // C-d\DPC-n
  this->WhiteListCodes["C-\\DIC\\N"]           = C_DIC_N;         // C-d\DIC\N
  this->WhiteListCodes["C-\\DICI\\N"]          = C_DICI_N;        // C-d\DICI\N
  this->WhiteListCodes["C-\\DICC-"]            = C_DICC;          // C-d\DICC-n
  this->WhiteListCodes["C-\\DICP-"]            = C_DICP;          // C-d\DICP-n
  this->WhiteListCodes["C-\\PTM"]              = C_PTM;           // C-d\PTM
  this->WhiteListCodes["C-\\BTM"]              = C_BTM;           // C-d\BTM
  this->WhiteListCodes["C-\\VOI\\E"]           = C_VOI_E;         // C-d\VOI\E
  this->WhiteListCodes["C-\\VOI\\D"]           = C_VOI_D;         // C-d\VOI\D
  this->WhiteListCodes["C-\\VID\\E"]           = C_VID_E;         // C-d\VID\E
  this->WhiteListCodes["C-\\VID\\D"]           = C_VID_D;         // C-d\VID\D
  this->WhiteListCodes["C-\\COM"]              = C_COM;           // C-d\COM
}

int Tmats::GetCode(QByteArray bytesCode, QList<int> &indexes)
{
  QString strCode = bytesCode;
  int indexCount = bytesCode.count(TMATS_INDEX_SEPARATOR);

  for (int sep1 = strCode.size(); sep1 > 0; sep1--)
  {
    if (strCode[sep1] != TMATS_INDEX_SEPARATOR)
      continue;

    int sep2;
    for (sep2 = sep1 + 1; sep2 < strCode.size(); sep2++)
    {
      QChar ch = strCode[sep2];

      if (ch == TMATS_INDEX_SEPARATOR)
        break;

      if (ch == TMATS_FIELD_SEPARATOR)
        break;
    }

    int start = sep1 + 1;
    int count = sep2 - start;

    if (count <= 0)
      continue;

    bool ok;
    QString integer = strCode.mid(start, count);

    int index = integer.toInt(&ok, 10);
    
    if (ok == false)
      continue;

    indexes.append(index);
    strCode.remove(start, count);
  }
  
  if (indexCount != indexes.size())
    return -1;

  int code = this->WhiteListCodes.value(strCode, -1);

  return code;
}

void Tmats::DecodeTmats(QString Tmats)
{
  this->RawData = Tmats.toLocal8Bit();

  G_Record *g_record = new G_Record(this);
  this->G_Records[0] = g_record;

  QByteArrayList lines = this->RawData.split(TMATS_LINE_SEPARATOR);

  for (QByteArray line : lines)
  {
    // Get the line
    line = line.trimmed();

    // Decode the TMATS line
    int index = line.indexOf(TMATS_VALUE_SEPARATOR);
    if (index == -1)
      continue;

    QByteArray bytesCode = line.left(index);
    QByteArray bytesData = line.mid(index + 1);

    // Determine and decode different TMATS types
    switch (bytesCode[0])
    {
      case TMATS_GROUP_G: this->Decode_G(bytesCode, bytesData); break;  // General Information
      case TMATS_GROUP_B: this->Decode_B(bytesCode, bytesData); break;  // Bus Data Attributes
      case TMATS_GROUP_R: this->Decode_R(bytesCode, bytesData); break;  // Tape/Storage Source Attributes
      case TMATS_GROUP_M: this->Decode_M(bytesCode, bytesData); break;  // Multiplexing/Modulation Attributes
      case TMATS_GROUP_P: this->Decode_P(bytesCode, bytesData); break;  // PCM Format Attributes

      /* Unsupported */
      default:
      case TMATS_GROUP_T: // Transmission Attributes
      case TMATS_GROUP_D: // PCM Measurement Description
      case TMATS_GROUP_S: // Packet Format Attributes
      case TMATS_GROUP_A: // PAM Attributes
      case TMATS_GROUP_C: // Data Conversion Attributes
      case TMATS_GROUP_H: // Airborne Hardware Attributes
      case TMATS_GROUP_V: // Vendor Specific Attributes
        continue;
    }
  }

  /* Now link the various records together into a tree.  This is a bit involved. */
  G_Record::Connect_G(this->G_Records, this->R_Records);
  R_Record::Connect_R(this->R_Records, this->M_Records, this->P_Records);
  M_Record::Connect_M(this->M_Records, this->B_Records, this->P_Records);
  P_Record::Connect_P(this->P_Records);
}

void Tmats::Clear()
{
  this->RawData.clear();

  this->C10Version = -1;
  this->ConfigChange = -1;

  this->ClearHash();
}

void Tmats::ClearHash()
{
  qDeleteAll(this->G_Records);
  qDeleteAll(this->R_Records);
  qDeleteAll(this->M_Records);
  qDeleteAll(this->B_Records);
  qDeleteAll(this->P_Records);
  qDeleteAll(this->T_Records);
  qDeleteAll(this->D_Records);
  qDeleteAll(this->S_Records);
  qDeleteAll(this->A_Records);
  qDeleteAll(this->C_Records);
  qDeleteAll(this->H_Records);
  qDeleteAll(this->V_Records);

  this->G_Records.clear();
  this->R_Records.clear();
  this->M_Records.clear();
  this->B_Records.clear();
  this->P_Records.clear();
  this->T_Records.clear();
  this->D_Records.clear();
  this->S_Records.clear();
  this->A_Records.clear();
  this->C_Records.clear();
  this->H_Records.clear();
  this->V_Records.clear();
}

void Tmats::Decode_G(QByteArray bytesCode, QByteArray bytesData)
{
  QList<int> indexes;
  int code = GetCode(bytesCode, indexes);

  if (code == -1)
    return;

  G_Record *g_record = G_Record::Get_G(0, this, this->G_Records);

  if (g_record == nullptr)
    return;

  g_record->Decode(code, bytesData, indexes);
}

void Tmats::Decode_R(QByteArray bytesCode, QByteArray bytesData)
{
  QList<int> indexes;
  int code = GetCode(bytesCode, indexes);

  if (code == -1)
    return;

  int index = indexes.takeFirst();
  R_Record *r_record = R_Record::Get_R(index, this, this->R_Records);

  if (r_record == nullptr)
    return;

  r_record->Decode(code, bytesData, indexes);
}

void Tmats::Decode_M(QByteArray bytesCode, QByteArray bytesData)
{
  QList<int> indexes;
  int code = GetCode(bytesCode, indexes);

  if (code == -1)
    return;

  int index = indexes.takeFirst();
  M_Record *m_record = M_Record::Get_M(index, this, this->M_Records);

  if (m_record == nullptr)
    return;

  m_record->Decode(code, bytesData, indexes);
}

void Tmats::Decode_B(QByteArray bytesCode, QByteArray bytesData)
{
  QList<int> indexes;
  int code = GetCode(bytesCode, indexes);

  if (code == -1)
    return;

  int index = indexes.takeFirst();
  B_Record *b_record = B_Record::Get_B(index, this, this->B_Records);

  if (b_record == nullptr)
    return;

  b_record->Decode(code, bytesData, indexes);
}

void Tmats::Decode_P(QByteArray bytesCode, QByteArray bytesData)
{
  QList<int> indexes;
  int code = GetCode(bytesCode, indexes);

  if (code == -1)
    return;

  int index = indexes.takeFirst();
  P_Record *p_record = P_Record::Get_P(index, this, this->P_Records);

  if (p_record == nullptr)
    return;

  p_record->Decode(code, bytesData, indexes);
}