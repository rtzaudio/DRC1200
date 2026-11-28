#include "PropPages.h"
#include "DRC1200.h"          // control ID constants (ID_EDIT_*, ID_CHECK_*, ...)
#include <wx/valnum.h>
#include <wx/valgen.h>

///////////////////////////////////////////////////////////////////////////////
// CPropPageBase

CPropPageBase::CPropPageBase(wxWindow* parent)
    : wxPanel(parent, wxID_ANY)
{
    // Lets this page's own TransferDataToWindow/FromWindow/Validate reach the
    // validators on its controls (DDX/DDV equivalent).
    SetExtraStyle(GetExtraStyle() | wxWS_EX_VALIDATE_RECURSIVELY);
}

wxStaticBoxSizer* CPropPageBase::AddGroup(const wxString& label)
{
    wxStaticBoxSizer* sizer = new wxStaticBoxSizer(wxVERTICAL, this, label);
    sizer->GetStaticBox()->SetExtraStyle(
        sizer->GetStaticBox()->GetExtraStyle() | wxWS_EX_VALIDATE_RECURSIVELY);
    return sizer;
}

wxFlexGridSizer* CPropPageBase::NewRowGrid()
{
    return new wxFlexGridSizer(2, FromDIP(6), FromDIP(10));
}

void CPropPageBase::AddIntRow(wxFlexGridSizer* grid, wxWindow* parent,
                              const wxString& label, int id, int* value,
                              int minVal, int maxVal)
{
    wxTextCtrl* edit = new wxTextCtrl(parent, id, wxEmptyString,
                            wxDefaultPosition, FromDIP(wxSize(60, -1)),
                            wxTE_RIGHT);
    wxIntegerValidator<int> val(value);
    val.SetRange(minVal, maxVal);
    edit->SetValidator(val);

    grid->Add(new wxStaticText(parent, wxID_ANY, label), 0, wxALIGN_CENTER_VERTICAL);
    grid->Add(edit, 0, wxALIGN_CENTER_VERTICAL);
}

void CPropPageBase::AddFloatRow(wxFlexGridSizer* grid, wxWindow* parent,
                                const wxString& label, int id, float* value,
                                float minVal, float maxVal)
{
    wxTextCtrl* edit = new wxTextCtrl(parent, id, wxEmptyString,
                            wxDefaultPosition, FromDIP(wxSize(60, -1)),
                            wxTE_RIGHT);
    wxFloatingPointValidator<float> val(4, value, wxNUM_VAL_NO_TRAILING_ZEROES);
    val.SetRange(minVal, maxVal);
    edit->SetValidator(val);

    grid->Add(new wxStaticText(parent, wxID_ANY, label), 0, wxALIGN_CENTER_VERTICAL);
    grid->Add(edit, 0, wxALIGN_CENTER_VERTICAL);
}

wxCheckBox* CPropPageBase::AddCheck(wxWindow* parent, const wxString& label,
                                    int id, bool* value)
{
    wxCheckBox* chk = new wxCheckBox(parent, id, label);
    chk->SetValidator(wxGenericValidator(value));
    return chk;
}

///////////////////////////////////////////////////////////////////////////////
// CPropSheetDlg

CPropSheetDlg::CPropSheetDlg(wxWindow* parent, const wxString& title)
{
    Create(parent, wxID_ANY, title);
    CreateButtons(wxOK | wxCANCEL);
}

void CPropSheetDlg::AddPage(CPropPageBase* page, const wxString& title)
{
    GetBookCtrl()->AddPage(page, title);
    m_pages.push_back(page);
}

bool CPropSheetDlg::Validate()
{
    for (size_t i = 0; i < m_pages.size(); i++)
    {
        if (!m_pages[i]->Validate())
        {
            GetBookCtrl()->SetSelection(i);   // show the page with the error
            return false;
        }
    }
    return true;
}

bool CPropSheetDlg::TransferDataToWindow()
{
    for (size_t i = 0; i < m_pages.size(); i++)
        if (!m_pages[i]->TransferDataToWindow())
            return false;
    return true;
}

bool CPropSheetDlg::TransferDataFromWindow()
{
    for (size_t i = 0; i < m_pages.size(); i++)
        if (!m_pages[i]->TransferDataFromWindow())
            return false;
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// CPropPageSettings (IDD_PROPPAGE_SETTINGS)

CPropPageSettings::CPropPageSettings(wxWindow* parent)
    : CPropPageBase(parent)
    , m_nTheme(0)
    , m_bShowTracks(false)
    , m_bShowVelocity(false)
    , m_bShowTimecode(false)
    , m_bShowTime(false)
    , m_bConfirmZeroReset(false)
    , m_bUseTapeMachineTime(false)
{
    // Left: Display Options group
    wxStaticBoxSizer* grpDisplay = AddGroup(wxT("Display Options"));
    wxStaticBox* box = grpDisplay->GetStaticBox();
    const int pad = FromDIP(5);

    grpDisplay->Add(AddCheck(box, wxT("Show track assignment pane"),
                    ID_CHECK_SHOW_TRACKS, &m_bShowTracks), 0, wxALL, pad);
    grpDisplay->Add(AddCheck(box, wxT("Show graph of tape velocity"),
                    ID_CHECK_SHOW_VELOCITY, &m_bShowVelocity), 0, wxALL, pad);
    grpDisplay->Add(AddCheck(box, wxT("Show time/date from local computer"),
                    ID_CHECK_SHOW_TIME_DATE, &m_bShowTime), 0, wxALL, pad);
    grpDisplay->Add(AddCheck(box, wxT("Show SMPTE timecode when available"),
                    ID_CHECK_SHOW_TIMECODE, &m_bShowTimecode), 0, wxALL, pad);

    // Right: theme combo + two more options
    wxArrayString themes;
    themes.Add(wxT("Blue"));
    themes.Add(wxT("Black"));
    themes.Add(wxT("Silver"));
    themes.Add(wxT("Aqua"));

    wxChoice* choTheme = new wxChoice(this, ID_COMBO_THEME,
                                wxDefaultPosition, wxDefaultSize, themes);
    choTheme->SetValidator(wxGenericValidator(&m_nTheme));

    wxBoxSizer* themeRow = new wxBoxSizer(wxHORIZONTAL);
    themeRow->Add(new wxStaticText(this, wxID_ANY, wxT("UI Theme Color")),
                  0, wxALIGN_CENTER_VERTICAL | wxRIGHT, FromDIP(8));
    themeRow->Add(choTheme, 0, wxALIGN_CENTER_VERTICAL);

    wxBoxSizer* right = new wxBoxSizer(wxVERTICAL);
    right->Add(themeRow, 0, wxBOTTOM, FromDIP(14));
    right->Add(AddCheck(this, wxT("Confirm zero reset of tape time counter"),
               ID_CHECK_CONFIRM_ZERO_RESET, &m_bConfirmZeroReset), 0, wxALL, pad);
    right->Add(AddCheck(this, wxT("Use time from tape machine for time/date"),
               ID_CHECK_USE_TAPE_MACHINE_TIME, &m_bUseTapeMachineTime), 0, wxALL, pad);

    wxBoxSizer* top = new wxBoxSizer(wxHORIZONTAL);
    top->Add(grpDisplay, 0, wxALL, FromDIP(12));
    top->Add(right, 0, wxALL, FromDIP(12));
    SetSizerAndFit(top);
}

///////////////////////////////////////////////////////////////////////////////
// CPropPageConfigTransport (IDD_PROPPAGE_CONFIG_TRANSPORT)

CPropPageConfigTransport::CPropPageConfigTransport(wxWindow* parent)
    : CPropPageBase(parent)
    , m_nVelocityDetectThreshold(0)
    , m_nRecordStrobePulseTime(0)
    , m_nRecordHoldSettleTime(0)
    , m_nButtonDebounceTime(0)
{
    wxFlexGridSizer* grid = NewRowGrid();
    AddIntRow(grid, this, wxT("Velocity detect threshold"),
              ID_EDIT_VEL_DETECT_THRESHOLD, &m_nVelocityDetectThreshold, 1, 50);
    AddIntRow(grid, this, wxT("Record strobe pulse time"),
              ID_EDIT_REC_PULSE_TIME, &m_nRecordStrobePulseTime, 10, 100);
    AddIntRow(grid, this, wxT("Record hold settle time"),
              ID_EDIT_REC_HOLD_SETTLE_TIME, &m_nRecordHoldSettleTime, 5, 20);
    AddIntRow(grid, this, wxT("Transport button debounce time"),
              ID_EDIT_BUTTON_DEBOUNCE_TIME, &m_nButtonDebounceTime, 5, 50);

    wxBoxSizer* top = new wxBoxSizer(wxVERTICAL);
    top->Add(grid, 0, wxALL, FromDIP(16));
    SetSizerAndFit(top);
}

///////////////////////////////////////////////////////////////////////////////
// CPropPageConfigStop (IDD_PROPPAGE_CONFIG_STOP)

CPropPageConfigStop::CPropPageConfigStop(wxWindow* parent)
    : CPropPageBase(parent)
    , m_nStopBrakeTorque(0)
    , m_bLiftersAtStop(false)
    , m_BrakesAtStop(false)
    , m_bEndOfTapeDetect(false)
{
    wxFlexGridSizer* grid = NewRowGrid();
    AddIntRow(grid, this, wxT("Dynamic stop brake torque"),
              ID_EDIT_STOP_BRAKE_TORQUE, &m_nStopBrakeTorque, 300, 900);

    const int pad = FromDIP(5);
    wxBoxSizer* top = new wxBoxSizer(wxVERTICAL);
    top->Add(grid, 0, wxALL, FromDIP(16));
    top->Add(AddCheck(this, wxT("Leave lifters engaged at stop"),
             ID_CHECK_LIFTERS_AT_STOP, &m_bLiftersAtStop), 0, wxALL, pad);
    top->Add(AddCheck(this, wxT("Leave brakes engaged at stop"),
             ID_CHECK_BRAKES_AT_STOP, &m_BrakesAtStop), 0, wxALL, pad);
    top->Add(AddCheck(this, wxT("Stop at end-of-tape leader detect (opto sensor required)"),
             ID_CHECK_STOP_AT_EOT, &m_bEndOfTapeDetect), 0, wxALL, pad);
    SetSizerAndFit(top);
}

///////////////////////////////////////////////////////////////////////////////
// CPropPageConfigShuttle (IDD_PROPPAGE_CONFIG_SHUTTLE)

CPropPageConfigShuttle::CPropPageConfigShuttle(wxWindow* parent)
    : CPropPageBase(parent)
    , m_fFwdBackTensionGain(0.0f)
    , m_fRewBackTensionGain(0.0f)
    , m_nShuttleVelocity(0)
    , m_nLibWindVelocity(0)
    , m_nAutoSlowVelocity(0)
    , m_nAutoSlowAtOffset(0)
    , m_nAutoSlowAtVelocity(0)
    , m_nLifterSettleTime(0)
    , m_fShuttlePGain(0.0f)
    , m_fShuttleIGain(0.0f)
    , m_fShuttleDGain(0.0f)
{
    wxFlexGridSizer* grid = NewRowGrid();
    AddFloatRow(grid, this, wxT("FWD shuttle back-tension gain"),
                ID_EDIT_HOLDBACK_FWD_GAIN, &m_fFwdBackTensionGain, 0.005f, 0.25f);
    AddFloatRow(grid, this, wxT("REW shuttle back-tension gain"),
                ID_EDIT_HOLDBACK_REW_GAIN, &m_fRewBackTensionGain, 0.005f, 0.25f);
    AddIntRow(grid, this, wxT("Shuttle mode max velocity"),
              ID_EDIT_SHUTTLE_VELOCITY, &m_nShuttleVelocity, 100, 1200);
    AddIntRow(grid, this, wxT("Library wind mode velocity"),
              ID_EDIT_LIBWIND_VELOCITY, &m_nLibWindVelocity, 50, 500);
    AddIntRow(grid, this, wxT("Auto-Slow wind velocity"),
              ID_EDIT_SLOW_VELOCITY, &m_nAutoSlowVelocity, 0, 500);
    AddIntRow(grid, this, wxT("Auto-slow triggers at offset"),
              ID_EDIT_SLOW_AT_OFFSET, &m_nAutoSlowAtOffset, 0, 100);
    AddIntRow(grid, this, wxT("Auto-Slow min velocity to trigger"),
              ID_EDIT_SLOW_AT_VELOCITY, &m_nAutoSlowAtVelocity, 0, 800);
    AddIntRow(grid, this, wxT("Lifter settle time after engage"),
              ID_EDIT_LIFTER_SETTLE_TIME, &m_nLifterSettleTime, 0, 2000);

    wxStaticBoxSizer* grpPID = AddGroup(wxT("Shuttle Servo PID"));
    wxStaticBox* box = grpPID->GetStaticBox();
    wxFlexGridSizer* pid = NewRowGrid();
    AddFloatRow(pid, box, wxT("P-gain"), ID_EDIT_P_GAIN, &m_fShuttlePGain, 0.0f, 10.0f);
    AddFloatRow(pid, box, wxT("I-gain"), ID_EDIT_I_GAIN, &m_fShuttleIGain, 0.0f, 5.0f);
    AddFloatRow(pid, box, wxT("D-gain"), ID_EDIT_D_GAIN, &m_fShuttleDGain, 0.0f, 5.0f);
    grpPID->Add(pid, 0, wxALL, FromDIP(8));

    wxBoxSizer* top = new wxBoxSizer(wxHORIZONTAL);
    top->Add(grid, 0, wxALL, FromDIP(16));
    top->Add(grpPID, 0, wxALL, FromDIP(12));
    SetSizerAndFit(top);
}

///////////////////////////////////////////////////////////////////////////////
// CPropPageConfigLocator (IDD_PROPPAGE_CONFIG_LOCATOR)

CPropPageConfigLocator::CPropPageConfigLocator(wxWindow* parent)
    : CPropPageBase(parent)
    , m_nJogNear(0)
    , m_nJogMid(0)
    , m_nJogFar(0)
{
    wxStaticBoxSizer* grp = AddGroup(wxT("Search"));
    wxStaticBox* box = grp->GetStaticBox();

    wxFlexGridSizer* grid = NewRowGrid();
    AddIntRow(grid, box, wxT("Velocity when near the cue point"),
              ID_EDIT_JOG_NEAR, &m_nJogNear, 0, 400);
    AddIntRow(grid, box, wxT("Velocity when close to the cue point"),
              ID_EDIT_JOG_MID, &m_nJogMid, 0, 800);
    AddIntRow(grid, box, wxT("Velocity when far from cue point (0=max)"),
              ID_EDIT_JOG_FAR, &m_nJogFar, 0, 1000);
    grp->Add(grid, 0, wxALL, FromDIP(8));

    wxBoxSizer* top = new wxBoxSizer(wxVERTICAL);
    top->Add(grp, 0, wxALL, FromDIP(12));
    SetSizerAndFit(top);
}

///////////////////////////////////////////////////////////////////////////////
// CPropPageConfigSMPTE (IDD_PROPPAGE_CONFIG_SMPTE)

CPropPageConfigSMPTE::CPropPageConfigSMPTE(wxWindow* parent)
    : CPropPageBase(parent)
    , m_fRefClock(9600.0f)
    , m_fps(0)
{
    wxStaticBoxSizer* grp = AddGroup(wxT("SMPTE"));
    wxStaticBox* box = grp->GetStaticBox();

    wxFlexGridSizer* grid = NewRowGrid();
    AddFloatRow(grid, box, wxT("Default ref clock frequency"),
                ID_EDIT_REFCLK, &m_fRefClock, 2400.0f, 18000.0f);

    wxArrayString rates;
    rates.Add(wxT("24"));
    rates.Add(wxT("25"));
    rates.Add(wxT("30"));
    rates.Add(wxT("30D"));

    wxChoice* choFps = new wxChoice(box, ID_COMBO_FRAME_RATE,
                            wxDefaultPosition, wxDefaultSize, rates);
    choFps->SetValidator(wxGenericValidator(&m_fps));

    grid->Add(new wxStaticText(box, wxID_ANY, wxT("Default Frame Rate (FPS)")),
              0, wxALIGN_CENTER_VERTICAL);
    grid->Add(choFps, 0, wxALIGN_CENTER_VERTICAL);
    grp->Add(grid, 0, wxALL, FromDIP(8));

    wxBoxSizer* top = new wxBoxSizer(wxVERTICAL);
    top->Add(grp, 0, wxALL, FromDIP(12));
    SetSizerAndFit(top);
}

///////////////////////////////////////////////////////////////////////////////
// CPropPageConfigMIDI (IDD_PROPPAGE_CONFIG_MIDI)

CPropPageConfigMIDI::CPropPageConfigMIDI(wxWindow* parent)
    : CPropPageBase(parent)
    , m_nDeviceID(0)
{
    wxStaticBoxSizer* grp = AddGroup(wxT("MIDI MMC"));
    wxStaticBox* box = grp->GetStaticBox();

    wxFlexGridSizer* grid = NewRowGrid();
    AddIntRow(grid, box, wxT("Device ID"), ID_EDIT_DEVID, &m_nDeviceID, 0, 255);
    grp->Add(grid, 0, wxALL, FromDIP(8));

    wxBoxSizer* top = new wxBoxSizer(wxVERTICAL);
    top->Add(grp, 0, wxALL, FromDIP(12));
    SetSizerAndFit(top);
}

///////////////////////////////////////////////////////////////////////////////
// CPropPageConfigGeneral (IDD_PROPPAGE_CONFIG_GENERAL)

CPropPageConfigGeneral::CPropPageConfigGeneral(wxWindow* parent)
    : CPropPageBase(parent)
    , m_nShowTimecode(false)
    , m_nBlink7Seg(false)
{
    // Helper: right-aligned caption + value text
    struct Row
    {
        static wxStaticText* Add(wxFlexGridSizer* g, wxWindow* p,
                                 const wxString& caption, int id,
                                 const wxString& initial)
        {
            g->Add(new wxStaticText(p, wxID_ANY, caption), 0,
                   wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
            wxStaticText* st = new wxStaticText(p, id, initial);
            g->Add(st, 0, wxALIGN_CENTER_VERTICAL);
            return st;
        }
    };

    // STC-1200 group
    wxStaticBoxSizer* grpSTC = AddGroup(wxT("STC-1200"));
    wxFlexGridSizer* gSTC = new wxFlexGridSizer(2, FromDIP(6), FromDIP(8));
    m_stSTCVersion = Row::Add(gSTC, grpSTC->GetStaticBox(), wxT("Firmware:"),
                              ID_STATIC_STC_VERSION, wxT("(version)"));
    m_stSTCSN      = Row::Add(gSTC, grpSTC->GetStaticBox(), wxT("S/N:"),
                              ID_STATIC_STC_SN, wxT("(sn)"));
    m_stSTCMAC     = Row::Add(gSTC, grpSTC->GetStaticBox(), wxT("MAC:"),
                              ID_STATIC_STC_MAC, wxT("(mac)"));
    grpSTC->Add(gSTC, 0, wxALL, FromDIP(8));

    // DTC-1200 group
    wxStaticBoxSizer* grpDTC = AddGroup(wxT("DTC-1200"));
    wxFlexGridSizer* gDTC = new wxFlexGridSizer(2, FromDIP(6), FromDIP(8));
    m_stDTCVersion = Row::Add(gDTC, grpDTC->GetStaticBox(), wxT("Firmware:"),
                              ID_STATIC_DTC_VERSION, wxT("(version)"));
    m_stDTCSN      = Row::Add(gDTC, grpDTC->GetStaticBox(), wxT("S/N:"),
                              ID_STATIC_DTC_SN, wxT("(sn)"));
    grpDTC->Add(gDTC, 0, wxALL, FromDIP(8));

    const int pad = FromDIP(4);
    wxBoxSizer* top = new wxBoxSizer(wxVERTICAL);
    top->Add(grpSTC, 0, wxEXPAND | wxALL, FromDIP(10));
    top->Add(grpDTC, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(10));
    top->Add(AddCheck(this, wxT("Wired remote displays time in timecode form"),
             ID_CHECK_SHOW_TIMECODE, &m_nShowTimecode), 0, wxALL, pad + FromDIP(8));
    top->Add(AddCheck(this, wxT("Blink 7-seg display on machine during locates"),
             ID_CHECK_BLINK_7SEG, &m_nBlink7Seg), 0, wxLEFT | wxBOTTOM, pad + FromDIP(8));
    SetSizerAndFit(top);
}

bool CPropPageConfigGeneral::TransferDataToWindow()
{
    // The version/serial/MAC values are display-only (DDX_Text into statics)
    m_stSTCVersion->SetLabel(m_strSTCVersion);
    m_stDTCVersion->SetLabel(m_strDTCVersion);
    m_stSTCSN->SetLabel(m_stcSN);
    m_stSTCMAC->SetLabel(m_stcMAC);
    m_stDTCSN->SetLabel(m_dtcSN);

    return CPropPageBase::TransferDataToWindow();
}

///////////////////////////////////////////////////////////////////////////////
// CPropPageConfigTensions (IDD_PROPPAGE_CONFIG_TENSIONS)

CPropPageConfigTensions::CPropPageConfigTensions(wxWindow* parent)
    : CPropPageBase(parent)
    , m_nSupplyStopTension(0)
    , m_nSupplyShuttleTension(0)
    , m_nSupplyPlayTensionHI(0)
    , m_nSupplyPlayTensionLO(0)
    , m_nSupplyThreadTension(0)
    , m_nTakeupStopTension(0)
    , m_nTakeupShuttleTension(0)
    , m_nTakeupPlayTensionHI(0)
    , m_nTakeupPlayTensionLO(0)
    , m_nTakeupThreadTension(0)
    , m_fTensionSensorGain(0.0f)
    , m_fTensionSensorMidScale1(0.0f)
    , m_fTensionSensorMidScale2(0.0f)
    , m_fReelRadiusGain(0.0f)
    , m_fReelOffsetGain(0.0f)
{
    const int gpad = FromDIP(8);

    // SUPPLY tension
    wxStaticBoxSizer* grpSupply = AddGroup(wxT("SUPPLY Tension"));
    wxStaticBox* bs = grpSupply->GetStaticBox();
    wxFlexGridSizer* gs = NewRowGrid();
    AddIntRow(gs, bs, wxT("Stop"),    ID_EDIT_SUPPLY_TENSION_STOP,    &m_nSupplyStopTension,    1, 1023);
    AddIntRow(gs, bs, wxT("Shuttle"), ID_EDIT_SUPPLY_TENSION_SHUTTLE, &m_nSupplyShuttleTension, 1, 1023);
    AddIntRow(gs, bs, wxT("Play LO"), ID_EDIT_SUPPLY_TENSION_PLAY_LO, &m_nSupplyPlayTensionLO,  1, 1023);
    AddIntRow(gs, bs, wxT("Play HI"), ID_EDIT_SUPPLY_TENSION_PLAY_HI, &m_nSupplyPlayTensionHI, 10, 1023);
    AddIntRow(gs, bs, wxT("Thread"),  ID_EDIT_SUPPLY_TENSION_THREAD,  &m_nSupplyThreadTension,  0,  200);
    grpSupply->Add(gs, 0, wxALL, gpad);

    // TAKEUP tension
    wxStaticBoxSizer* grpTakeup = AddGroup(wxT("TAKEUP Tension"));
    wxStaticBox* bt = grpTakeup->GetStaticBox();
    wxFlexGridSizer* gt = NewRowGrid();
    AddIntRow(gt, bt, wxT("Stop"),    ID_EDIT_TAKEUP_TENSION_STOP,    &m_nTakeupStopTension,    1, 1023);
    AddIntRow(gt, bt, wxT("Shuttle"), ID_EDIT_TAKEUP_TENSION_SHUTTLE, &m_nTakeupShuttleTension, 1, 1023);
    AddIntRow(gt, bt, wxT("Play LO"), ID_EDIT_TAKEUP_TENSION_PLAY_LO, &m_nTakeupPlayTensionLO,  1, 1023);
    AddIntRow(gt, bt, wxT("Play HI"), ID_EDIT_TAKEUP_TENSION_PLAY_HI, &m_nTakeupPlayTensionHI, 10, 1023);
    AddIntRow(gt, bt, wxT("Thread"),  ID_EDIT_TAKEUP_TENSION_THREAD,  &m_nTakeupThreadTension,  0,  200);
    grpTakeup->Add(gt, 0, wxALL, gpad);

    // Tension sensor
    wxStaticBoxSizer* grpSensor = AddGroup(wxT("Tension Sensor"));
    wxStaticBox* bx = grpSensor->GetStaticBox();
    wxFlexGridSizer* gx = NewRowGrid();
    AddFloatRow(gx, bx, wxT("Tension sensor gain"),
                ID_EDIT_TENSION_SENSOR_GAIN, &m_fTensionSensorGain, 0.0f, 1.0f);
    AddFloatRow(gx, bx, wxT("ADC mid-scale offset 1\""),
                ID_EDIT_TENSION_SENSOR_OFFSET_1, &m_fTensionSensorMidScale1, 1500.0f, 2500.0f);
    AddFloatRow(gx, bx, wxT("ADC mid-scale offset 2\""),
                ID_EDIT_TENSION_SENSOR_OFFSET_2, &m_fTensionSensorMidScale2, 1500.0f, 2500.0f);
    grpSensor->Add(gx, 0, wxALL, gpad);

    // Reeling radius
    wxStaticBoxSizer* grpReel = AddGroup(wxT("Reeling Radius"));
    wxStaticBox* br = grpReel->GetStaticBox();
    wxFlexGridSizer* gr = NewRowGrid();
    AddFloatRow(gr, br, wxT("Reeling radius gain"),
                ID_EDIT_REEL_RADIUS_GAIN, &m_fReelRadiusGain, 0.01f, 1.0f);
    AddFloatRow(gr, br, wxT("Reel offset gain"),
                ID_EDIT_REEL_OFFSET_GAIN, &m_fReelOffsetGain, 0.05f, 1.0f);
    grpReel->Add(gr, 0, wxALL, gpad);

    wxBoxSizer* right = new wxBoxSizer(wxVERTICAL);
    right->Add(grpSensor, 0, wxEXPAND | wxBOTTOM, FromDIP(8));
    right->Add(grpReel, 0, wxEXPAND);

    wxBoxSizer* top = new wxBoxSizer(wxHORIZONTAL);
    top->Add(grpSupply, 0, wxALL, FromDIP(8));
    top->Add(grpTakeup, 0, wxTOP | wxBOTTOM | wxRIGHT, FromDIP(8));
    top->Add(right, 0, wxTOP | wxBOTTOM | wxRIGHT, FromDIP(8));
    SetSizerAndFit(top);
}

///////////////////////////////////////////////////////////////////////////////
// CPropPageConfigPlay (IDD_PROPPAGE_CONFIG_PLAY)

CPropPageConfigPlay::CPropPageConfigPlay(wxWindow* parent)
    : CPropPageBase(parent)
    , m_fPlayBoostPGainLO(0.0f)
    , m_fPlayBoostIGainLO(0.0f)
    , m_fPlayBoostPGainHI(0.0f)
    , m_fPlayBoostIGainHI(0.0f)
    , m_nPlayBoostEndVelLO(0)
    , m_nPlayBoostEndVelHI(0)
    , m_nPinchRollerSettleTime(0)
    , m_nPlaySettleTime(0)
    , m_nBrakeSettleTime(0)
    , m_bBrakesStopPlay(false)
    , m_bEngagePinchRoller(false)
{
    const int gpad = FromDIP(8);

    // Play Boost LO-Speed
    wxStaticBoxSizer* grpLO = AddGroup(wxT("Play Boost LO-Speed"));
    wxStaticBox* bl = grpLO->GetStaticBox();
    wxFlexGridSizer* gl = NewRowGrid();
    AddFloatRow(gl, bl, wxT("Boost ramp P-Gain"),
                ID_EDIT_PLAY_BOOST_PGAIN_LO, &m_fPlayBoostPGainLO, 0.0f, 5.0f);
    AddFloatRow(gl, bl, wxT("Boost ramp I-Gain"),
                ID_EDIT_PLAY_BOOST_IGAIN_LO, &m_fPlayBoostIGainLO, 0.0f, 2.0f);
    AddIntRow(gl, bl, wxT("Exit boost at velocity"),
              ID_EDIT_EXIT_BOOST_AT_VEL_LO, &m_nPlayBoostEndVelLO, 25, 100);
    grpLO->Add(gl, 0, wxALL, gpad);

    // Play Boost HI-Speed
    wxStaticBoxSizer* grpHI = AddGroup(wxT("Play Boost HI-Speed"));
    wxStaticBox* bh = grpHI->GetStaticBox();
    wxFlexGridSizer* gh = NewRowGrid();
    AddFloatRow(gh, bh, wxT("Boost ramp P-Gain"),
                ID_EDIT_PLAY_BOOST_PGAIN_HI, &m_fPlayBoostPGainHI, 0.0f, 5.0f);
    AddFloatRow(gh, bh, wxT("Boost ramp I-Gain"),
                ID_EDIT_PLAY_BOOST_IGAIN_HI, &m_fPlayBoostIGainHI, 0.0f, 2.0f);
    AddIntRow(gh, bh, wxT("Exit boost at velocity"),
              ID_EDIT_EXIT_BOOST_AT_VEL_HI, &m_nPlayBoostEndVelHI, 100, 150);
    grpHI->Add(gh, 0, wxALL, gpad);

    wxBoxSizer* groups = new wxBoxSizer(wxHORIZONTAL);
    groups->Add(grpLO, 1, wxEXPAND | wxRIGHT, FromDIP(12));
    groups->Add(grpHI, 1, wxEXPAND);

    // Settle times (left) and options (right)
    wxFlexGridSizer* settle = NewRowGrid();
    AddIntRow(settle, this, wxT("Pinch roller settle time after engage"),
              ID_EDIT_PINCH_ROLLER_SETTLE_TIME, &m_nPinchRollerSettleTime, 0, 1000);
    AddIntRow(settle, this, wxT("Delay entering play after shuttle"),
              ID_EDIT_PLAY_SETTLE_TIME, &m_nPlaySettleTime, 0, 1000);
    AddIntRow(settle, this, wxT("Brake settle time after engage"),
              ID_EDIT_BRAKE_SETTLE_TIME, &m_nBrakeSettleTime, 0, 2000);

    wxBoxSizer* opts = new wxBoxSizer(wxVERTICAL);
    opts->Add(AddCheck(this, wxT("Use brakes to stop play mode"),
              ID_CHECK_BRAKES_STOP_PLAY, &m_bBrakesStopPlay), 0, wxALL, FromDIP(5));
    opts->Add(AddCheck(this, wxT("Engage pinch roller at play"),
              ID_CHECK_ENGAGE_PINCH_ROLLER, &m_bEngagePinchRoller), 0, wxALL, FromDIP(5));

    wxBoxSizer* bottom = new wxBoxSizer(wxHORIZONTAL);
    bottom->Add(settle, 0, wxRIGHT, FromDIP(30));
    bottom->Add(opts, 0, wxALIGN_TOP);

    wxBoxSizer* top = new wxBoxSizer(wxVERTICAL);
    top->Add(groups, 0, wxEXPAND | wxALL, FromDIP(12));
    top->Add(bottom, 0, wxLEFT | wxRIGHT | wxBOTTOM, FromDIP(16));
    SetSizerAndFit(top);
}
