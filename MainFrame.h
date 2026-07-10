#pragma once
#include "STC1200TCP.h"
#include "TimePanel.h"
#include "VelocityPanel.h"
#include "STCCommand.h"

#define MAX_VEL_SAMPLES	500

/* The Ampex MM1200 tape roller quadrature encoder wheel has 40 ppr. This gives
 * either 80 or 160 edges per revolution depending on the quadrature encoder
 * configuration set by QEIConfig(). Currently we use Cha-A mode which
 * gives 80 edges per revolution. If Cha-A/B mode is used this must be
 * set to 160.
 */

#define ROLLER_TICKS_PER_REV        80
#define ROLLER_TICKS_PER_REV_F      80.0f

/* This is the diameter of the tape timer roller */
#define ROLLER_CIRCUMFERENCE_F      5.0014f

/* This is the maximum signed position value we can have. Anything past
 * this is treated as a negative position value.
 */
#define MAX_ROLLER_POSITION			(0x7FFFFFFF - 1UL)
#define MIN_ROLLER_POSITION			(-MAX_ROLLER_POSITION - 1)

///////////////////////////////////////////////////////////////////////////////
// Forward Declarations

class TrackFrame;

///////////////////////////////////////////////////////////////////////////////
// Sub panels of tape transport panel

class OptionPanel : public wxPanel
{
public:
	OptionPanel(wxPanel* parent);

   	wxCheckBox* m_checkAutoPlay;
	wxCheckBox* m_checkLibWind;
    wxCheckBox* m_checkStandbyMon;
	wxStaticText* m_txtStatus;
};

class CommandButton : public wxButton
{
public:
	CommandButton(wxPanel* panel, int id, const wxString& label);

	void OnCommandButtonClicked(wxCommandEvent& event);
};

class CommandPanel : public wxPanel
{
public:
	CommandPanel(wxPanel* parent);

	CommandButton* m_cmdButton[9];
};

class TopContainer : public wxPanel
{
public:
	TopContainer(wxPanel* parent);
};

class MiddleContainer : public wxPanel
{
public:
	MiddleContainer(wxPanel* parent);

	wxStaticText* m_txtStatus;
};

///////////////////////////////////////////////////////////////////////////////
// Tape Transport Panel, left hand side view

class TransportButton : public wxButton
{
public:
	TransportButton(wxPanel* panel, int id, const wxString& label);

	void OnTransportButtonClicked(wxCommandEvent& event);
};

class BottomContainer : public wxPanel
{
public:
	BottomContainer(wxPanel* parent);

public:
	TransportButton* m_btnRec;
	TransportButton* m_btnFwd;
	TransportButton* m_btnRew;
	TransportButton* m_btnPlay;
	TransportButton* m_btnStop;
	TransportButton* m_btnLift;
};

///////////////////////////////////////////////////////////////////////////////
// Left hand view container panel, left hand side view

class LeftContainer : public wxPanel
{
public:
	LeftContainer(wxWindow* parent);

	TopContainer* m_panelTop;
	MiddleContainer* m_panelMiddle;
	BottomContainer* m_panelBottom;
};

///////////////////////////////////////////////////////////////////////////////
// Right hand view container panel, locator buttons

class LocatorButton : public wxButton
{
public:
	LocatorButton(wxPanel* panel, int id, const wxString& label);

	void OnLocatorButtonClicked(wxCommandEvent& event);
};

class RightContainer : public wxPanel
{
public:
	RightContainer(wxWindow* parent);

	LocatorButton* m_btnDigit0;
	LocatorButton* m_btnDigit1;
	LocatorButton* m_btnDigit2;
	LocatorButton* m_btnDigit3;
	LocatorButton* m_btnDigit4;
	LocatorButton* m_btnDigit5;
	LocatorButton* m_btnDigit6;
	LocatorButton* m_btnDigit7;
	LocatorButton* m_btnDigit8;
	LocatorButton* m_btnDigit9;
	LocatorButton* m_btnCueMode;
	LocatorButton* m_btnStoreMode;
	LocatorButton* m_btnAutoPunch;
	LocatorButton* m_btnPunchIn;
	LocatorButton* m_btnPunchOut;
	LocatorButton* m_btnAutoLoop;
	LocatorButton* m_btnMarkIn;
	LocatorButton* m_btnMarkOut;
    LocatorButton* m_btnZeroReturn;
	LocatorButton* m_btnCancel;
};

///////////////////////////////////////////////////////////////////////////////
// Main Application Window Frame

class MainFrame : public wxFrame
{
public:
	MainFrame();
	~MainFrame();

public:
	STC_STATE_MSG& GetTransportState()
	{ return m_state; }
	wxSocketClient* GetSocketState()
	{ return m_sockState; }
	STCCommand& GetSocketCommand()
	{ return m_sockCommand; }

	bool IsConnected(void)
	{ return m_sockCommand.IsConnected() ? true : false; }
	bool IsTransportMode(uint32_t mode)
	{ return ((m_state.transportMode & STC_MODE_MASK) == mode) ? true : false; }
	bool IsTransportModeFlags(uint32_t flags)
	{ return ((m_state.transportMode & flags) == flags) ? true : false; }
	bool IsTransportReady(void)
	{ return (IsTransportMode(STC_MODE_HALT) || IsTransportMode(STC_MODE_THREAD)) ? false : true; }
	bool IsTransportSearching(void)
	{ return IsTransportModeFlags(STC_M_SEARCH) ? true : false; }
	bool IsTransportLooping(void)
	{ return IsTransportModeFlags(STC_M_LOOP) ? true : false; }
	bool IsTransportHaltMode(void)
	{ return IsTransportMode(STC_MODE_HALT); }
	bool IsTransportThreadMode(void)
	{ return IsTransportMode(STC_MODE_THREAD); }

	uint32_t GetLocatorButtonMask(void)
	{ return m_state.ledMaskButton; }
	bool IsLocatorButtonMask(uint32_t mask)
	{ return (GetLocatorButtonMask() & mask) ? true : false; }

	bool IsHardwareRTC()
	{ return (m_state.hardwareFlags & STC_HF_RTC) ? true : false; }
	bool IsHardwareDCS()
	{ return (m_state.hardwareFlags & STC_HF_DCS) ? true : false; }
	bool IsHardwareSMPTE()
	{ return (m_state.hardwareFlags & STC_HF_SMPTE) ? true : false; }
	bool IsHardwareNCO()
	{ return (m_state.hardwareFlags & STC_HF_NCO) ? true : false; }

	bool IsTapeSpeed(int speed)
	{ return (m_state.tapeSpeed == (uint8_t)speed) ? true : false; }
	float GetTapeSpeed(void)
	{ return (float)m_state.tapeSpeed; }

	bool IsRecord(void)
	{ return m_bRecord; }
	bool IsAutoPlay(void)
	{ return m_bAutoPlay; }
	bool IsLibWind(void)
	{ return m_bLibWind; }
	bool IsStandbyMonitor(void)
	{ return (m_state.monitorFlags) ? true : false; }

	OptionPanel* GetOptionPanel(void)
	{ return m_panelOptions; }

    //BottomContainer* GetBottomContainer()
    //{ return m_panelBottom; }

	void UpdateTimePanel(void);
	void UpdateVelocityPanel(void);
    void UpdateTransportButtonStates(bool repain=false);
    void UpdateLocateButtonStates(bool repaint=false);
    void UpdateCommandButtonStates(void);

    bool IsTrackControllerReady(void);
    bool IsAnyTracksArmed(void);

    float GetVelocitySample(size_t i)
    { return m_velSample[i]; }

    void GetModeText(STC_STATE_MSG& msg, wxString& str);

private:
	void CreateMenuBar(void);
	bool ConnectionOpen(wxSockAddress::Family family, wxString hostname);
	void ConnectionClose(void);
	void UpdateStatusBar(void);
	void HandleReceiveData(void);
	void HandleConnect(void);
	void HandleDisconnect(void);
    void SearchOrStore(uint32_t cueIndex);
    void UpdateAllControls();
    void UpdateLocatorButtonUI(wxUpdateUIEvent& event, uint32_t ledmask);
    void UpdateLocateButtonDelta(LocatorButton* button, uint32_t mask, int cueIndex=-1);
	void UpdateLocatorButton(LocatorButton* button, uint32_t mask, int cueIndex = -1);
	void UpdateTransportButtonDelta(TransportButton* button, uint32_t mask);
    void UpdateTransportButton(TransportButton* button, uint32_t mask);
    void UpdateCommandButton(CommandButton* button, uint32_t mask);

	wxMenu*				m_menuConnect;
	wxPanel*			m_panel;
	TimePanel*			m_panelTime;;
	OptionPanel*		m_panelOptions;
	VelocityPanel*		m_panelVelocity;
	LeftContainer*		m_panelLeft;
	RightContainer*		m_panelRight;
	TrackFrame*			m_trackFrame;
	STCCommand			m_sockCommand;
	wxSocketClient*		m_sockState;

	bool				m_bConnecting;
	bool				m_bConnected;

public:
	wxProgressDialog*   m_dlgProgress;

private:
	bool				m_busy;
	bool				m_bRecord;
	bool	            m_bLibWind;
	bool	            m_bAutoPlay;
    bool	            m_bAutoConnect;

	bool	            m_bShowTracks;
	bool                m_bConfirmZeroReset;

	int		            m_nShowVelocity;
	bool	            m_bShowTimecode;
	bool	            m_bShowTimeDate;
	bool	            m_bUseTapeMachineTime;
	wxString			m_strHostname;
	int					m_nPortNumber;

	STC_STATE_MSG		m_state;
	STC_STATE_MSG		m_state_prev;

	unsigned int		m_nRxPacketCount;
	int					m_velSampleIndex;
	float				m_velSample[MAX_VEL_SAMPLES];

	uint8_t				m_mac[6];
	uint8_t				m_stcSN[16];
	uint8_t				m_dtcSN[16];
	uint32_t			m_stcVersion;
	uint32_t			m_dtcVersion;
	int					m_smpteHour;
	int					m_smpteMins;
	int					m_smpteSecs;
	int					m_smpteFrame;

	// Event Handlers
	wxDECLARE_EVENT_TABLE();

    bool Destroy();

	void OnIdle(wxIdleEvent& event);
	void OnConnectEvent(wxCommandEvent& WXUNUSED(event));
	void OnSocketEvent(wxSocketEvent& event);
	void OnConnectTcp(wxCommandEvent& WXUNUSED(event));
    void OnUpdateUIConnectTcp(wxUpdateUIEvent& event);
	void OnConnectClose(wxCommandEvent& WXUNUSED(event));
    void OnUpdateUIConnectClose(wxUpdateUIEvent& event);
	void OnExit(wxCommandEvent& WXUNUSED(event));

    void OnRangeRecordTrack(wxCommandEvent& event);
    void OnUpdateUIRangeRecordTrack(wxUpdateUIEvent& event);
    void OnRangeInputTrack(wxCommandEvent& event);
    void OnUpdateRangeInputTrack(wxUpdateUIEvent& event);
    void OnRangeReproTrack(wxCommandEvent& event);
    void OnUpdateRangeReproTrack(wxUpdateUIEvent& event);
    void OnRangeSyncTrack(wxCommandEvent& event);
    void OnUpdateRangeSyncTrack(wxUpdateUIEvent& event);
    void OnRangeTrackToggleMode(wxCommandEvent& event);
    void OnUpdateRangeTrackToggleMode(wxUpdateUIEvent& event);
    void OnRangeMonitorTrack(wxCommandEvent& event);
    void OnUpdateUIRangeMonitorTrack(wxUpdateUIEvent& event);
    void OnTrackAllSafe(wxCommandEvent& event);
    void OnUpdateTrackAllSafe(wxUpdateUIEvent& event);
    void OnTrackAllReady(wxCommandEvent& event);
    void OnUpdateTrackAllReady(wxUpdateUIEvent& event);
    void OnTrackAllInput(wxCommandEvent& event);
    void OnUpdateTrackAllInput(wxUpdateUIEvent& event);
    void OnTrackAllRepro(wxCommandEvent& event);
    void OnUpdateTrackAllRepro(wxUpdateUIEvent& event);
    void OnTrackAllSync(wxCommandEvent& event);
    void OnUpdateTrackAllSync(wxUpdateUIEvent& event);
    void OnTrackAllMonitorOff(wxCommandEvent& event);
    void OnUpdateTrackAllMonitorOn(wxUpdateUIEvent& event);
    void OnTrackAllMonitorOn(wxCommandEvent& event);
    void OnUpdateTrackAllMonitorOff(wxUpdateUIEvent& event);
    void OnTrackStandbymonitor(wxCommandEvent& event);
    void OnUpdateTrackStandbymonitor(wxUpdateUIEvent& event);
	void OnLocatorAutoPlay(wxCommandEvent& event);
	void OnUpdateUILocatorAutoPlay(wxUpdateUIEvent& event);
	void OnViewShowTracks(wxCommandEvent& WXUNUSED(event));
	void OnUpdateUIViewShowTracks(wxUpdateUIEvent& event);
    void OnCheckLibraryWind(wxCommandEvent& WXUNUSED(event));
	void OnUpdateUICheckLibraryWind(wxUpdateUIEvent& event);
	void OnCheckAutoPlay(wxCommandEvent& event);
	void OnUpdateUICheckAutoPlay(wxUpdateUIEvent& event);
    void OnCheckStandbyMon(wxCommandEvent& WXUNUSED(event));
    void OnUpdateUICheckStandbyMon(wxUpdateUIEvent& event);
    void OnConfigurationRecall(wxCommandEvent& event);
    void OnUpdateConfigurationRecall(wxUpdateUIEvent& event);
    void OnConfigurationSave(wxCommandEvent& event);
    void OnUpdateConfigurationSave(wxUpdateUIEvent& event);
    void OnConfigurationReset(wxCommandEvent& event);
    void OnUpdateConfigurationReset(wxUpdateUIEvent& event);

    void OnTransportRecord(wxCommandEvent& WXUNUSED(event));
	void OnUpdateUITransportRecord(wxUpdateUIEvent& event);
	void OnTransportPlay(wxCommandEvent& WXUNUSED(event));
	void OnUpdateUITransportPlay(wxUpdateUIEvent& event);
	void OnTransportRewind(wxCommandEvent& WXUNUSED(event));
	void OnUpdateUITransportRewind(wxUpdateUIEvent& event);
	void OnTransportForward(wxCommandEvent& WXUNUSED(event));
	void OnUpdateUITransportForward(wxUpdateUIEvent& event);
	void OnTransportStop(wxCommandEvent& WXUNUSED(event));
	void OnUpdateUITransportStop(wxUpdateUIEvent& event);
	void OnTransportTapeLifter(wxCommandEvent& WXUNUSED(event));
	void OnUpdateUITransportTapeLifter(wxUpdateUIEvent& event);
	void OnTransportLibraryWind(wxCommandEvent& WXUNUSED(event));
	void OnUpdateUITransportLibraryWind(wxUpdateUIEvent& event);
    void OnTapespeedHighspeed(wxCommandEvent& WXUNUSED(event));
    void OnUpdateTapespeedHighspeed(wxUpdateUIEvent& event);
    void OnTapespeedLowspeed(wxCommandEvent& WXUNUSED(event));
    void OnUpdateTapespeedLowspeed(wxUpdateUIEvent& event);
    void OnTransportToggleplay(wxCommandEvent& WXUNUSED(event));
    void OnUpdateTransportToggleplay(wxUpdateUIEvent& event);
    void OnTransportPunch(wxCommandEvent& WXUNUSED(event));
    void OnUpdateTransportPunch(wxUpdateUIEvent& event);

    void OnCuePointLoc0(wxCommandEvent& WXUNUSED(event));
    void OnUpdateCuePointLoc0(wxUpdateUIEvent& event);
    void OnCuePointLoc1(wxCommandEvent& WXUNUSED(event));
    void OnUpdateCuePointLoc1(wxUpdateUIEvent& event);
    void OnCuePointLoc2(wxCommandEvent& WXUNUSED(event));
    void OnUpdateCuePointLoc2(wxUpdateUIEvent& event);
    void OnCuePointLoc3(wxCommandEvent& WXUNUSED(event));
    void OnUpdateCuePointLoc3(wxUpdateUIEvent& event);
    void OnCuePointLoc4(wxCommandEvent& WXUNUSED(event));
    void OnUpdateCuePointLoc4(wxUpdateUIEvent& event);
    void OnCuePointLoc5(wxCommandEvent& WXUNUSED(event));
    void OnUpdateCuePointLoc5(wxUpdateUIEvent& event);
    void OnCuePointLoc6(wxCommandEvent& WXUNUSED(event));
    void OnUpdateCuePointLoc6(wxUpdateUIEvent& event);
    void OnCuePointLoc7(wxCommandEvent& WXUNUSED(event));
    void OnUpdateCuePointLoc7(wxUpdateUIEvent& event);
    void OnCuePointLoc8(wxCommandEvent& WXUNUSED(event));
    void OnUpdateCuePointLoc8(wxUpdateUIEvent& event);
    void OnCuePointLoc9(wxCommandEvent& WXUNUSED(event));
    void OnUpdateCuePointLoc9(wxUpdateUIEvent& event);
    void OnCueMarkIn(wxCommandEvent& WXUNUSED(event));
    void OnUpdateCueMarkIn(wxUpdateUIEvent& event);
    void OnCueMarkOut(wxCommandEvent& WXUNUSED(event));
    void OnUpdateCueMarkOut(wxUpdateUIEvent& event);
    void OnCuePunchIn(wxCommandEvent& WXUNUSED(event));
    void OnUpdateCuePunchIn(wxUpdateUIEvent& event);
    void OnCuePunchOut(wxCommandEvent& WXUNUSED(event));
    void OnUpdateCuePunchOut(wxUpdateUIEvent& event);
    void OnLocatorZeroreturn(wxCommandEvent& WXUNUSED(event));
    void OnUpdateLocatorZeroreturn(wxUpdateUIEvent& event);
    void OnLocatorCueMode(wxCommandEvent& WXUNUSED(event));
    void OnUpdateLocatorCueMode(wxUpdateUIEvent& event);
    void OnLocatorStoreMode(wxCommandEvent& WXUNUSED(event));
    void OnUpdateLocatorStoreMode(wxUpdateUIEvent& event);
    void OnLocatorResetCounter(wxCommandEvent& WXUNUSED(event));
    void OnUpdateLocatorResetCounter(wxUpdateUIEvent& event);
    void OnLocatorCancelsearch(wxCommandEvent& WXUNUSED(event));
    void OnUpdateLocatorCancelsearch(wxUpdateUIEvent& event);
    void OnLocatorAutoLoop(wxCommandEvent& WXUNUSED(event));
    void OnUpdateLocatorAutoLoop(wxUpdateUIEvent& event);
    void OnLocatorMarkIn(wxCommandEvent& WXUNUSED(event));
    void OnUpdateLocatorMarkIn(wxUpdateUIEvent& event);
    void OnLocatorMarkOut(wxCommandEvent& WXUNUSED(event));
    void OnUpdateLocatorMarkOut(wxUpdateUIEvent& event);
    void OnLocatorAutoPunch(wxCommandEvent& WXUNUSED(event));
    void OnUpdateLocatorAutoPunch(wxUpdateUIEvent& event);
    void OnLocatorPunchIn(wxCommandEvent& WXUNUSED(event));
    void OnUpdateLocatorPunchIn(wxUpdateUIEvent& event);
    void OnLocatorPunchOut(wxCommandEvent& WXUNUSED(event));
    void OnUpdateLocatorPunchOut(wxUpdateUIEvent& event);

    void OnGeneratorStop(wxCommandEvent& event);
    void OnUpdateGeneratorStop(wxUpdateUIEvent& event);
    void OnUpdateGeneratorStart(wxUpdateUIEvent& event);
    void OnGeneratorStart(wxCommandEvent& event);
    void OnGeneratorResume(wxCommandEvent& event);
    void OnUpdateGeneratorResume(wxUpdateUIEvent& event);
    void OnGeneratorEdittime(wxCommandEvent& event);
    void OnUpdateGeneratorEdittime(wxUpdateUIEvent& event);

	void OnAbout(wxCommandEvent& WXUNUSED(event));
};

// End-Of-File
