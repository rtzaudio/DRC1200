#ifndef PROPPAGES_H
#define PROPPAGES_H

#include <wx/wx.h>
#include <wx/propdlg.h>
#include <wx/bookctrl.h>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
// Base class for all property pages. Builds on wxPanel and turns on recursive
// validation so the DDX/DDV-style validators on each page's controls work.

class CPropPageBase : public wxPanel
{
public:
    CPropPageBase(wxWindow* parent);

protected:
    wxStaticBoxSizer* AddGroup(const wxString& label);
    wxFlexGridSizer*  NewRowGrid();

    void AddIntRow(wxFlexGridSizer* grid, wxWindow* parent, const wxString& label,
                   int id, int* value, int minVal, int maxVal);
    void AddFloatRow(wxFlexGridSizer* grid, wxWindow* parent, const wxString& label,
                     int id, float* value, float minVal, float maxVal);
    wxCheckBox* AddCheck(wxWindow* parent, const wxString& label,
                         int id, bool* value);
};

///////////////////////////////////////////////////////////////////////////////
// Property sheet host (replaces CPropertySheet). Create the pages with
// sheet.GetBookCtrl() as the parent, add them with AddPage(), then Finish().

class CPropSheetDlg : public wxPropertySheetDialog
{
public:
    CPropSheetDlg(wxWindow* parent, const wxString& title);

    void AddPage(CPropPageBase* page, const wxString& title);
    void Finish() { LayoutDialog(); }

    virtual bool Validate() wxOVERRIDE;
    virtual bool TransferDataToWindow() wxOVERRIDE;
    virtual bool TransferDataFromWindow() wxOVERRIDE;

private:
    std::vector<CPropPageBase*> m_pages;
};

///////////////////////////////////////////////////////////////////////////////
// Settings page (IDD_PROPPAGE_SETTINGS)

class CPropPageSettings : public CPropPageBase
{
public:
    CPropPageSettings(wxWindow* parent);

    int  m_nTheme;               // 0=Blue 1=Black 2=Silver 3=Aqua
    bool m_bShowTracks;
    bool m_bShowVelocity;
    bool m_bShowTimecode;
    bool m_bShowTime;
    bool m_bConfirmZeroReset;
    bool m_bUseTapeMachineTime;
};

///////////////////////////////////////////////////////////////////////////////
// Config pages

class CPropPageConfigTransport : public CPropPageBase
{
public:
    CPropPageConfigTransport(wxWindow* parent);

    int m_nVelocityDetectThreshold;
    int m_nRecordStrobePulseTime;
    int m_nRecordHoldSettleTime;
    int m_nButtonDebounceTime;
};

class CPropPageConfigStop : public CPropPageBase
{
public:
    CPropPageConfigStop(wxWindow* parent);

    int  m_nStopBrakeTorque;
    bool m_bLiftersAtStop;
    bool m_BrakesAtStop;
    bool m_bEndOfTapeDetect;
};

class CPropPageConfigShuttle : public CPropPageBase
{
public:
    CPropPageConfigShuttle(wxWindow* parent);

    float m_fFwdBackTensionGain;
    float m_fRewBackTensionGain;
    int   m_nShuttleVelocity;
    int   m_nLibWindVelocity;
    int   m_nAutoSlowVelocity;
    int   m_nAutoSlowAtOffset;
    int   m_nAutoSlowAtVelocity;
    int   m_nLifterSettleTime;
    float m_fShuttlePGain;
    float m_fShuttleIGain;
    float m_fShuttleDGain;
};

class CPropPageConfigLocator : public CPropPageBase
{
public:
    CPropPageConfigLocator(wxWindow* parent);

    int m_nJogNear;
    int m_nJogMid;
    int m_nJogFar;
};

class CPropPageConfigSMPTE : public CPropPageBase
{
public:
    CPropPageConfigSMPTE(wxWindow* parent);

    float m_fRefClock;
    int   m_fps;                 // 0=24 1=25 2=30 3=30D
};

class CPropPageConfigMIDI : public CPropPageBase
{
public:
    CPropPageConfigMIDI(wxWindow* parent);

    int m_nDeviceID;
};

class CPropPageConfigGeneral : public CPropPageBase
{
public:
    CPropPageConfigGeneral(wxWindow* parent);

    virtual bool TransferDataToWindow() wxOVERRIDE;

    bool     m_nShowTimecode;
    bool     m_nBlink7Seg;
    wxString m_strSTCVersion;
    wxString m_strDTCVersion;
    wxString m_stcSN;
    wxString m_stcMAC;
    wxString m_dtcSN;

private:
    wxStaticText* m_stSTCVersion;
    wxStaticText* m_stDTCVersion;
    wxStaticText* m_stSTCSN;
    wxStaticText* m_stSTCMAC;
    wxStaticText* m_stDTCSN;
};

class CPropPageConfigTensions : public CPropPageBase
{
public:
    CPropPageConfigTensions(wxWindow* parent);

    int   m_nSupplyStopTension;
    int   m_nSupplyShuttleTension;
    int   m_nSupplyPlayTensionHI;
    int   m_nSupplyPlayTensionLO;
    int   m_nSupplyThreadTension;
    int   m_nTakeupStopTension;
    int   m_nTakeupShuttleTension;
    int   m_nTakeupPlayTensionHI;
    int   m_nTakeupPlayTensionLO;
    int   m_nTakeupThreadTension;
    float m_fTensionSensorGain;
    float m_fTensionSensorMidScale1;
    float m_fTensionSensorMidScale2;
    float m_fReelRadiusGain;
    float m_fReelOffsetGain;
};

class CPropPageConfigPlay : public CPropPageBase
{
public:
    CPropPageConfigPlay(wxWindow* parent);

    float m_fPlayBoostPGainLO;
    float m_fPlayBoostIGainLO;
    float m_fPlayBoostPGainHI;
    float m_fPlayBoostIGainHI;
    int   m_nPlayBoostEndVelLO;
    int   m_nPlayBoostEndVelHI;
    int   m_nPinchRollerSettleTime;
    int   m_nPlaySettleTime;
    int   m_nBrakeSettleTime;
    bool  m_bBrakesStopPlay;
    bool  m_bEngagePinchRoller;
};

#endif // PROPPAGES_H
