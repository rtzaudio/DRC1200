#include "DRC1200.h"
#include "ConnectDialog.h"
//#include "DRC1200.xpm"

///////////////////////////////////////////////////////////////////////////////
// Message Event Dispatch Table

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
// Socket Read Event
EVT_SOCKET(SOCKET_ID, MainFrame::OnSocketEvent)
EVT_MENU(CONNECT_ID, MainFrame::OnConnectEvent)
// Connect Menu
EVT_MENU(ID_CONNECT_TCP, MainFrame::OnConnectTcp)
EVT_UPDATE_UI(ID_CONNECT_TCP, MainFrame::OnUpdateUIConnectTcp)
EVT_MENU(ID_CONNECT_CLOSE, MainFrame::OnConnectClose)
EVT_UPDATE_UI(ID_CONNECT_CLOSE, MainFrame::OnUpdateUIConnectClose)
EVT_MENU(wxID_EXIT, MainFrame::OnExit)
// Track Range Menu
EVT_MENU_RANGE(ID_RECORD_TRACK1, ID_RECORD_TRACK24, MainFrame::OnRangeRecordTrack)
EVT_UPDATE_UI_RANGE(ID_RECORD_TRACK1, ID_RECORD_TRACK24, MainFrame::OnUpdateUIRangeRecordTrack)
EVT_MENU_RANGE(ID_INPUT_TRACK1, ID_INPUT_TRACK24, MainFrame::OnRangeInputTrack)
EVT_UPDATE_UI_RANGE(ID_INPUT_TRACK1, ID_INPUT_TRACK24, MainFrame::OnUpdateRangeInputTrack)
EVT_MENU_RANGE(ID_REPRO_TRACK1, ID_REPRO_TRACK24, MainFrame::OnRangeReproTrack)
EVT_UPDATE_UI_RANGE(ID_REPRO_TRACK1, ID_REPRO_TRACK24, MainFrame::OnUpdateRangeReproTrack)
EVT_MENU_RANGE(ID_SYNC_TRACK1, ID_SYNC_TRACK24, MainFrame::OnRangeSyncTrack)
EVT_UPDATE_UI_RANGE(ID_SYNC_TRACK1, ID_SYNC_TRACK24, MainFrame::OnUpdateRangeSyncTrack)
EVT_MENU_RANGE(ID_TOGGLEMODE_TRACK1, ID_TOGGLEMODE_TRACK24, MainFrame::OnRangeTrackToggleMode)
EVT_UPDATE_UI_RANGE(ID_TOGGLEMODE_TRACK1, ID_TOGGLEMODE_TRACK24, MainFrame::OnUpdateRangeTrackToggleMode)
EVT_MENU_RANGE(ID_MONITOR_TRACK1, ID_MONITOR_TRACK24, MainFrame::OnRangeMonitorTrack)
EVT_UPDATE_UI_RANGE(ID_MONITOR_TRACK1, ID_MONITOR_TRACK24, MainFrame::OnUpdateUIRangeMonitorTrack)
// Track Menu
EVT_MENU(ID_TRACK_ALLSAFE, MainFrame::OnTrackAllSafe)
EVT_UPDATE_UI(ID_TRACK_ALLSAFE, MainFrame::OnUpdateTrackAllSafe)
EVT_MENU(ID_TRACK_ALLREADY, MainFrame::OnTrackAllReady)
EVT_UPDATE_UI(ID_TRACK_ALLREADY, MainFrame::OnUpdateTrackAllReady)
EVT_MENU(ID_TRACK_ALLINPUT, MainFrame::OnTrackAllInput)
EVT_UPDATE_UI(ID_TRACK_ALLINPUT, MainFrame::OnUpdateTrackAllInput)
EVT_MENU(ID_TRACK_ALLREPRO, MainFrame::OnTrackAllRepro)
EVT_UPDATE_UI(ID_TRACK_ALLREPRO, MainFrame::OnUpdateTrackAllRepro)
EVT_MENU(ID_TRACK_ALLSYNC, MainFrame::OnTrackAllSync)
EVT_UPDATE_UI(ID_TRACK_ALLSYNC, MainFrame::OnUpdateTrackAllSync)
EVT_MENU(ID_TRACK_ALLMONITORON, MainFrame::OnTrackAllMonitorOn)
EVT_UPDATE_UI(ID_TRACK_ALLMONITORON, MainFrame::OnUpdateTrackAllMonitorOn)
EVT_MENU(ID_TRACK_ALLMONITOROFF, MainFrame::OnTrackAllMonitorOff)
EVT_UPDATE_UI(ID_TRACK_ALLMONITOROFF, MainFrame::OnUpdateTrackAllMonitorOff)
EVT_MENU(ID_TRACK_STANDBYMONITOR, MainFrame::OnTrackStandbymonitor)
EVT_UPDATE_UI(ID_TRACK_STANDBYMONITOR, MainFrame::OnUpdateTrackStandbymonitor)
EVT_MENU(ID_CONFIGURATION_RECALL, MainFrame::OnConfigurationRecall)
EVT_UPDATE_UI(ID_CONFIGURATION_RECALL, MainFrame::OnUpdateConfigurationRecall)
EVT_MENU(ID_CONFIGURATION_SAVE, MainFrame::OnConfigurationSave)
EVT_UPDATE_UI(ID_CONFIGURATION_SAVE, MainFrame::OnUpdateConfigurationSave)
EVT_MENU(ID_CONFIGURATION_RESET, MainFrame::OnConfigurationReset)
EVT_UPDATE_UI(ID_CONFIGURATION_RESET, MainFrame::OnUpdateConfigurationReset)
// Transport Menu
EVT_MENU(ID_TRANSPORT_RECORD, MainFrame::OnTransportRecord)
EVT_UPDATE_UI(ID_TRANSPORT_RECORD, MainFrame::OnUpdateUITransportRecord)
EVT_MENU(ID_TRANSPORT_PLAY, MainFrame::OnTransportPlay)
EVT_UPDATE_UI(ID_TRANSPORT_PLAY, MainFrame::OnUpdateUITransportPlay)
EVT_MENU(ID_TRANSPORT_REWIND, MainFrame::OnTransportRewind)
EVT_UPDATE_UI(ID_TRANSPORT_REWIND, MainFrame::OnUpdateUITransportRewind)
EVT_MENU(ID_TRANSPORT_FORWARD, MainFrame::OnTransportForward)
EVT_UPDATE_UI(ID_TRANSPORT_FORWARD, MainFrame::OnUpdateUITransportForward)
EVT_MENU(ID_TRANSPORT_STOP, MainFrame::OnTransportStop)
EVT_UPDATE_UI(ID_TRANSPORT_STOP, MainFrame::OnUpdateUITransportStop)
EVT_MENU(ID_TRANSPORT_TAPELIFTER, MainFrame::OnTransportTapeLifter)
EVT_UPDATE_UI(ID_TRANSPORT_TAPELIFTER, MainFrame::OnUpdateUITransportTapeLifter)
EVT_MENU(ID_TRANSPORT_LIBRARYWIND, MainFrame::OnTransportLibraryWind)
EVT_UPDATE_UI(ID_TRANSPORT_LIBRARYWIND, MainFrame::OnUpdateUITransportLibraryWind)
EVT_MENU(ID_TAPESPEED_HIGHSPEED, MainFrame::OnTapespeedHighspeed)
EVT_UPDATE_UI(ID_TAPESPEED_HIGHSPEED, MainFrame::OnUpdateTapespeedHighspeed)
EVT_MENU(ID_TAPESPEED_LOWSPEED, MainFrame::OnTapespeedLowspeed)
EVT_UPDATE_UI(ID_TAPESPEED_LOWSPEED, MainFrame::OnUpdateTapespeedLowspeed)
EVT_MENU(ID_TRANSPORT_PUNCH, MainFrame::OnTransportPunch)
EVT_UPDATE_UI(ID_TRANSPORT_PUNCH, MainFrame::OnUpdateTransportPunch)
EVT_MENU(ID_TRANSPORT_TOGGLEPLAY, MainFrame::OnTransportToggleplay)
EVT_UPDATE_UI(ID_TRANSPORT_TOGGLEPLAY, MainFrame::OnUpdateTransportToggleplay)
// Locator Menu
EVT_MENU(ID_LOCATOR_AUTOPLAY, MainFrame::OnLocatorAutoPlay)
EVT_UPDATE_UI(ID_LOCATOR_AUTOPLAY, MainFrame::OnUpdateUILocatorAutoPlay)
EVT_MENU(ID_LOCATOR_RESETCOUNTER, MainFrame::OnLocatorResetCounter)
EVT_UPDATE_UI(ID_LOCATOR_RESETCOUNTER, MainFrame::OnUpdateLocatorResetCounter)
EVT_MENU(ID_LOCATOR_CUEMODE, MainFrame::OnLocatorCueMode)
EVT_UPDATE_UI(ID_LOCATOR_CUEMODE, MainFrame::OnUpdateLocatorCueMode)
EVT_MENU(ID_LOCATOR_STOREMODE, MainFrame::OnLocatorStoreMode)
EVT_UPDATE_UI(ID_LOCATOR_STOREMODE, MainFrame::OnUpdateLocatorStoreMode)
EVT_MENU(ID_LOCATOR_CANCELSEARCH, MainFrame::OnLocatorCancelsearch)
EVT_UPDATE_UI(ID_LOCATOR_CANCELSEARCH, MainFrame::OnUpdateLocatorCancelsearch)
EVT_MENU(ID_LOCATOR_AUTOLOOP, MainFrame::OnLocatorAutoLoop)
EVT_UPDATE_UI(ID_LOCATOR_AUTOLOOP, MainFrame::OnUpdateLocatorAutoLoop)
EVT_MENU(ID_LOCATOR_MARKIN, MainFrame::OnLocatorMarkIn)
EVT_UPDATE_UI(ID_LOCATOR_MARKIN, MainFrame::OnUpdateLocatorMarkIn)
EVT_MENU(ID_LOCATOR_MARKOUT, MainFrame::OnLocatorMarkOut)
EVT_UPDATE_UI(ID_LOCATOR_MARKOUT, MainFrame::OnUpdateLocatorMarkOut)
EVT_MENU(ID_LOCATOR_PUNCHIN, MainFrame::OnLocatorPunchIn)
EVT_UPDATE_UI(ID_LOCATOR_PUNCHIN, MainFrame::OnUpdateLocatorPunchIn)
EVT_MENU(ID_LOCATOR_PUNCHOUT, MainFrame::OnLocatorPunchOut)
EVT_UPDATE_UI(ID_LOCATOR_PUNCHOUT, MainFrame::OnUpdateLocatorPunchOut)
EVT_MENU(ID_LOCATOR_AUTOPUNCH, MainFrame::OnLocatorAutoPunch)
EVT_UPDATE_UI(ID_LOCATOR_AUTOPUNCH, MainFrame::OnUpdateLocatorAutoPunch)
EVT_MENU(ID_CUEPOINT_LOC0, MainFrame::OnCuePointLoc0)
EVT_UPDATE_UI(ID_CUEPOINT_LOC0, MainFrame::OnUpdateCuePointLoc0)
EVT_MENU(ID_CUEPOINT_LOC1, MainFrame::OnCuePointLoc1)
EVT_UPDATE_UI(ID_CUEPOINT_LOC1, MainFrame::OnUpdateCuePointLoc1)
EVT_MENU(ID_CUEPOINT_LOC2, MainFrame::OnCuePointLoc2)
EVT_UPDATE_UI(ID_CUEPOINT_LOC2, MainFrame::OnUpdateCuePointLoc2)
EVT_MENU(ID_CUEPOINT_LOC3, MainFrame::OnCuePointLoc3)
EVT_UPDATE_UI(ID_CUEPOINT_LOC3, MainFrame::OnUpdateCuePointLoc3)
EVT_MENU(ID_CUEPOINT_LOC4, MainFrame::OnCuePointLoc4)
EVT_UPDATE_UI(ID_CUEPOINT_LOC4, MainFrame::OnUpdateCuePointLoc4)
EVT_MENU(ID_CUEPOINT_LOC5, MainFrame::OnCuePointLoc5)
EVT_UPDATE_UI(ID_CUEPOINT_LOC5, MainFrame::OnUpdateCuePointLoc5)
EVT_MENU(ID_CUEPOINT_LOC6, MainFrame::OnCuePointLoc6)
EVT_UPDATE_UI(ID_CUEPOINT_LOC6, MainFrame::OnUpdateCuePointLoc6)
EVT_MENU(ID_CUEPOINT_LOC7, MainFrame::OnCuePointLoc7)
EVT_UPDATE_UI(ID_CUEPOINT_LOC7, MainFrame::OnUpdateCuePointLoc7)
EVT_MENU(ID_CUEPOINT_LOC8, MainFrame::OnCuePointLoc8)
EVT_UPDATE_UI(ID_CUEPOINT_LOC8, MainFrame::OnUpdateCuePointLoc8)
EVT_MENU(ID_CUEPOINT_LOC9, MainFrame::OnCuePointLoc9)
EVT_UPDATE_UI(ID_CUEPOINT_LOC9, MainFrame::OnUpdateCuePointLoc9)
EVT_MENU(ID_CUEPOINT_MARKIN, MainFrame::OnCueMarkIn)
EVT_UPDATE_UI(ID_CUEPOINT_MARKIN, MainFrame::OnUpdateCueMarkIn)
EVT_MENU(ID_CUEPOINT_MARKOUT, MainFrame::OnCueMarkOut)
EVT_UPDATE_UI(ID_CUEPOINT_MARKOUT, MainFrame::OnUpdateCueMarkOut)
EVT_MENU(ID_CUEPOINT_PUNCHIN, MainFrame::OnCuePunchIn)
EVT_UPDATE_UI(ID_CUEPOINT_PUNCHIN, MainFrame::OnUpdateCuePunchIn)
EVT_MENU(ID_CUEPOINT_PUNCHOUT, MainFrame::OnCuePunchOut)
EVT_UPDATE_UI(ID_CUEPOINT_PUNCHOUT, MainFrame::OnUpdateCuePunchOut)
// Search Menu
EVT_MENU(ID_SEARCH_ZERORETURN, MainFrame::OnLocatorZeroreturn)
EVT_UPDATE_UI(ID_SEARCH_ZERORETURN, MainFrame::OnUpdateLocatorZeroreturn)
EVT_MENU(ID_LOCATOR_CANCELSEARCH, MainFrame::OnLocatorCancelsearch)
EVT_UPDATE_UI(ID_LOCATOR_CANCELSEARCH, MainFrame::OnUpdateLocatorCancelsearch)
// SMPTE Menu
EVT_MENU(ID_GENERATOR_START, MainFrame::OnGeneratorStart)
EVT_UPDATE_UI(ID_GENERATOR_START, MainFrame::OnUpdateGeneratorStart)
EVT_MENU(ID_GENERATOR_STOP, MainFrame::OnGeneratorStop)
EVT_UPDATE_UI(ID_GENERATOR_STOP, MainFrame::OnUpdateGeneratorStop)
EVT_MENU(ID_GENERATOR_RESUME, MainFrame::OnGeneratorResume)
EVT_UPDATE_UI(ID_GENERATOR_RESUME, MainFrame::OnUpdateGeneratorResume)
EVT_MENU(ID_GENERATOR_EDITTIME, MainFrame::OnGeneratorEdittime)
EVT_UPDATE_UI(ID_GENERATOR_EDITTIME, MainFrame::OnUpdateGeneratorEdittime)
// View Menu
EVT_MENU(ID_VIEW_SHOWTRACKS, MainFrame::OnViewShowTracks)
EVT_UPDATE_UI(ID_VIEW_SHOWTRACKS, MainFrame::OnUpdateUIViewShowTracks)
// Help Menu
EVT_MENU(ID_ABOUT, MainFrame::OnAbout)
// Controls in the MainFrame window
EVT_CHECKBOX(ID_CHECK_AUTOPLAY, MainFrame::OnCheckAutoPlay)
EVT_UPDATE_UI(ID_CHECK_AUTOPLAY, MainFrame::OnUpdateUICheckAutoPlay)
EVT_CHECKBOX(ID_CHECK_LIBRARYWIND, MainFrame::OnCheckLibraryWind)
EVT_UPDATE_UI(ID_CHECK_LIBRARYWIND, MainFrame::OnUpdateUICheckLibraryWind)
EVT_CHECKBOX(ID_CHECK_STANDBYMON, MainFrame::OnCheckStandbyMon)
EVT_UPDATE_UI(ID_CHECK_STANDBYMON, MainFrame::OnUpdateUICheckStandbyMon)
wxEND_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////
// The Main Application Frame Window

MainFrame::MainFrame() :
	wxFrame(nullptr, wxID_ANY, wxT("DRC1200 - Remote Control"), wxDefaultPosition, wxSize(925, 300))
{
	m_bLibWind = false;
	m_bAutoPlay = false;
	m_bAutoConnect = true;
	m_bRecord = false;
    m_bConfirmZeroReset = true;
    m_bShowTracks = true;
    m_bConnected = false;
	m_busy = false;
	m_nRxPacketCount = 0;
	m_velSampleIndex = 0;

	m_smpteHour = 0;
	m_smpteMins = 0;
	m_smpteSecs = 0;
	m_smpteFrame = 0;
    m_dlgProgress = nullptr;
    m_sockState = nullptr;

	m_strHostname = wxT("");
	m_nPortNumber = STC_PORT_STATE;

	for(int i=0; i < MAX_VEL_SAMPLES; i++)
		m_velSample[i] = 0.0f;

	memset(&m_state, 0, sizeof(STC_STATE_MSG));
	memset(&m_state_prev, -1, sizeof(STC_STATE_MSG));

	memset(&m_mac[0], 0, sizeof(m_mac));
	memset(&m_stcSN[0], 0, sizeof(m_stcSN));
	memset(&m_dtcSN[0], 0, sizeof(m_dtcSN));

    // Set a unique name
    SetName("DRC1200_MainFrame");

	wxConfig* config = wxGetApp().GetConfig();

    config->Read(_("AutoPlay"), &m_bAutoPlay);
    config->Read(_("AutoConnect"), &m_bAutoConnect);
    config->Read(_("LibWind"), &m_bLibWind);
    config->Read(_("ConfirmZeroReset"), &m_bConfirmZeroReset);
    config->Read(_("ShowTracks"), &m_bShowTracks);

    //SetIcon(wxICON(DRC1200));
	CreateMenuBar();

	// Create the main frame panel covering the entire client area
	m_panel = new wxPanel(this, wxID_ANY);

	m_panel->SetBackgroundColour(wxGetApp().m_colorFrame);

	wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

	m_panelLeft  = new LeftContainer(m_panel);
	m_panelRight = new RightContainer(m_panel);

	hbox->Add(m_panelLeft, 2, wxEXPAND | wxALL, 5);
	hbox->Add(m_panelRight, 1, wxEXPAND | wxUP | wxDOWN | wxRIGHT, 5);

	m_panel->SetSizer(hbox);

	SetMinSize(wxSize(760, 285));
	//SetMaxSize(wxSize(-1, 250));

	m_trackFrame = new TrackFrame(this);

	// Restore the previously saved geometry, if any, and register this frame
	// for its geometry to be saved when it is closed using the given wxConfig
	// key name.

	if (!wxPersistentRegisterAndRestore(this, "DRC1200_MainFrame"))
	{
		// Choose some custom default size for the first run -- or don't do
		// anything at all and let the system use the default initial size.
		SetClientSize(FromDIP(wxSize(925, 300)));
	}

	int widths[] = { -1, 175, 130, 130 };

	const size_t numWidths =  sizeof(widths) / sizeof(int);

    // Status bar
	CreateStatusBar(numWidths);
	SetStatusWidths(numWidths, widths);
	SetStatusText(wxT("Ready"), 0);

    // Hide or show the child track assign window
    m_trackFrame->Show(m_bShowTracks);

	// Set check boxes in frame window to initial state
	wxGetApp().m_panelOption->m_checkLibWind->SetValue(m_bLibWind);
	wxGetApp().m_panelOption->m_checkAutoPlay->SetValue(m_bAutoPlay);

    // If a host name was provided on the application command line
    // at startup, then queue up a message to open this connection
    // after this function returns.

    if (m_bAutoConnect)
    {
        wxString hostname = wxGetApp().m_strHostArg;
        if (hostname.Length() > 0)
        {
            m_strHostname = hostname;
            wxCommandEvent* event = new wxCommandEvent(wxEVT_COMMAND_MENU_SELECTED, CONNECT_ID);
            wxQueueEvent(GetEventHandler(), event);
        }
    }

	UpdateStatusBar();
}

MainFrame::~MainFrame()
{
	wxConfig* config = wxGetApp().GetConfig();

    config->Write(_("ShowTracks"), m_bShowTracks);
    config->Write(_("AutoPlay"), m_bAutoPlay);
    config->Write(_("AutoConnect"), m_bAutoConnect);
    config->Write(_("LibWind"), m_bLibWind);
    config->Write(_("ConfirmZeroReset"), m_bConfirmZeroReset);
}

bool MainFrame::Destroy()
{
    // Close any connection open
    ConnectionClose();
    // Close the track assign window
    //m_trackFrame->Close(true);
    m_trackFrame->Destroy();
    // Continue destroying this frame window
    return wxFrame::Destroy();
}

///////////////////////////////////////////////////////////////////////////////
// Network Event Handlers

// Connection open event posted from main application task

void MainFrame::OnConnectEvent(wxCommandEvent& WXUNUSED(event))
{
    if (m_strHostname.Length() > 0)
    {
        ConnectionOpen(wxSockAddress::IPV4, m_strHostname);
    }
}

// Connection Open Handler

bool MainFrame::ConnectionOpen(wxSockAddress::Family family, wxString hostname)
{
	wxUnusedVar(family);			// unused in !wxUSE_IPV6 case
	wxIPaddress* addr;
	wxIPV4address addr4;

	if (m_sockState)
		m_sockState->Destroy();

	// Create the socket
	m_sockState = new wxSocketClient(wxSOCKET_NOWAIT);

	// Setup the event handler and subscribe to most events
	m_sockState->SetEventHandler(*this, SOCKET_ID);
	m_sockState->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	m_sockState->Notify(true);

	addr = &addr4;

	m_menuConnect->Enable(ID_CONNECT_TCP, false);
	m_menuConnect->Enable(ID_CONNECT_CLOSE, false);

	addr->Hostname(hostname);
	addr->Service(m_nPortNumber);

	// we connect asynchronously and will get a wxSOCKET_CONNECTION event when
	// the connection is really established

	wxString msg;
	msg.Printf(wxT("Connecting..."), hostname, addr->Service());
	SetStatusText(msg, 2);

    m_dlgProgress = new wxProgressDialog(
                         wxT("Progress dialog"),
                         wxT("Connecting to MM1200"),
                         100,
                         this,
                         wxPD_CAN_ABORT | wxPD_APP_MODAL);

	memset(&m_state_prev, -1, sizeof(STC_STATE_MSG));

	m_trackFrame->ResetTrackButtonStates(true);

	// Connect to streaming STC machine status socket

	m_bConnecting = true;
	m_bConnected  = false;

	m_sockState->Connect(*addr, false);

	return true;
}

void MainFrame::ConnectionClose(void)
{
	if (m_dlgProgress)
	{
		m_dlgProgress->Destroy();
		m_dlgProgress = nullptr;
	}

    m_sockState->Notify(false);

    if (m_sockState->IsConnected())
    {
        m_sockState->Close();
        m_sockCommand.ConnectionClose();
    }

    UpdateAllControls();

	Refresh();
}

void MainFrame::UpdateAllControls()
{
    // Update all buttons
    UpdateTimePanel();

    // Update velocity control
	UpdateVelocityPanel();

	// Update any Command buttons
	UpdateCommandButtonStates();

    // Update any Transport buttons
	UpdateTransportButtonStates(true);

	// Update any Locator buttons
	UpdateLocateButtonStates(true);

	// Update status bar connection status
	UpdateStatusBar();

	// Update any track assignment buttons
	m_trackFrame->ResetTrackButtonStates(true);

	bool standby = IsStandbyMonitor();

	wxGetApp().m_panelOption->m_checkStandbyMon->SetValue(standby);
	wxGetApp().m_panelOption->m_checkLibWind->SetValue(m_bLibWind);
	wxGetApp().m_panelOption->m_checkAutoPlay->SetValue(m_bAutoPlay);
}

///////////////////////////////////////////////////////////////////////////////
// Handle Socket Events for receive, connect and disconnect

void MainFrame::OnSocketEvent(wxSocketEvent& event)
{
	switch (event.GetSocketEvent())
	{
	case wxSOCKET_INPUT:
		HandleReceiveData();
		break;
	case wxSOCKET_LOST:
		HandleDisconnect();
		break;
	case wxSOCKET_CONNECTION:
		HandleConnect();
		break;
	default:
		wxLogMessage("Unknown socket event!!!");
		break;
	}
}

void MainFrame::HandleConnect(void)
{
	if (m_dlgProgress)
	{
		m_dlgProgress->Destroy();
		m_dlgProgress = nullptr;
	}

	m_bConnecting = false;

	m_bConnected = m_sockCommand.ConnectionOpen(m_strHostname);

	if (m_bConnected)
	{
		// Get and check the STC and DTC versions to make sure we are compatible

		wxSocketError err;

		uint32_t rev_stc, rev_dtc;

		rev_stc = rev_dtc = 0;

		err = GetSocketCommand().VersionGet(&rev_stc, &rev_dtc, m_stcSN, m_dtcSN, m_mac);

		if (err != wxSOCKET_NOERROR)
		{
			rev_stc = rev_dtc = 0;
			wxLogMessage(_("Error getting version info - DTC may not be present!"));
		}
		else
		{
			m_stcVersion = rev_stc;
			m_dtcVersion = rev_dtc;

			// Requires STC version 3.0 or greater!
			if (((rev_stc >> 16) >= 3) && (((rev_stc & 0xFF) >= 0) || (rev_stc == 0)))
			{
				// Query the STC to see how many tracks it's configured to support
				// if the DCS channel controller is installed in the machine.

				bool dcsFound;
				uint32_t numTracks;

				err = GetSocketCommand().TrackGetCount(&numTracks, &dcsFound);

				if (err == wxSOCKET_NOERROR)
				{
					if (numTracks != wxGetApp().m_trackCount)
					{
						if ((numTracks >= 8) && (numTracks <= 24))
							wxGetApp().m_trackCount = numTracks;
						else
							wxGetApp().m_trackCount = numTracks = 24;

						// Save the real number of tracks found
						wxGetApp().GetConfig()->Write(_("NumTracks"), wxGetApp().m_trackCount);
					}

					// Indicates if the STC has a DCS track controller available
					wxGetApp().m_dcsFound = dcsFound;
				}

				m_trackFrame->SetTrackConfig(numTracks);
			}
		}
	}

    UpdateAllControls();
}

void MainFrame::HandleDisconnect(void)
{
	ConnectionClose();
}

void MainFrame::HandleReceiveData(void)
{
	m_sockState->Read(&m_state, sizeof(STC_STATE_MSG));

	++m_nRxPacketCount;

	size_t index = m_velSampleIndex;

	if (index >= MAX_VEL_SAMPLES)
	{
		memmove(&m_velSample[0], &m_velSample[1], sizeof(float) * (MAX_VEL_SAMPLES - 1));

		m_velSample[MAX_VEL_SAMPLES - 1] = (float)m_state.tapeVelocity;
	}
	else
	{
		m_velSample[index++] = (float)m_state.tapeVelocity;

		m_velSampleIndex = index;
	}

	// Update the time display if tape is moving
	UpdateTimePanel();
	// Update the velocity panel control if tape is moving
	UpdateVelocityPanel();
	// Update any Transport buttons
	UpdateTransportButtonStates();
	// Update any Locator buttons
	UpdateLocateButtonStates();
	// Update any track assignment buttons
	m_trackFrame->UpdateTrackButtonStates();

	// Save previous states for selective updates. We only want
	// to redraw the buttons that have changed states.

	memcpy(&m_state_prev, &m_state, sizeof(STC_STATE_MSG));

	wxString str;
	str.Printf(_T("%d"), m_nRxPacketCount);
	SetStatusText(str, 3);
}

///////////////////////////////////////////////////////////////////////////////
// CONNECT Menu Handlers

void MainFrame::OnConnectTcp(wxCommandEvent& WXUNUSED(event))
{
	wxConfig* config = wxGetApp().GetConfig();

	wxString str;
	wxString key = _("hostname");

    if (!config->Read(key, &str))
    {
        str = _("192.168.1.143");
    }

    // Ask user for server address
    wxString hostname = wxGetTextFromUser(
            _("Enter the TCP/IP address of the MM1200 to connect with"),
            _("Connect..."), str);

	if (hostname.empty())
	{
		UpdateStatusBar();
		return;
	}

	m_strHostname = hostname;

    config->Write(key, hostname);
    config->Flush();

	ConnectionOpen(wxSockAddress::IPV4, hostname);
}

void MainFrame::OnUpdateUIConnectTcp(wxUpdateUIEvent& event)
{
	if (IsConnected())
	{
		event.Enable(false);
		return;
	}

	event.Enable(true);
}

void MainFrame::OnConnectClose(wxCommandEvent& WXUNUSED(event))
{
	ConnectionClose();
}

void MainFrame::OnUpdateUIConnectClose(wxUpdateUIEvent& event)
{
	if (IsConnected())
	{
		event.Enable(true);
		return;
	}

	event.Enable(false);
}

void MainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

///////////////////////////////////////////////////////////////////////////////
// TRACK - RECORD READY HANDLERS

void MainFrame::OnRangeRecordTrack(wxCommandEvent& event)
{
    uint32_t track = (uint32_t)(event.GetId() - ID_RECORD_TRACK1);
    wxASSERT(track < 24);

    // Toggle track arm state for the channel
    uint8_t mask = GetTransportState().trackState[track];
    mask ^= STC_T_READY;
    GetSocketCommand().TrackSetState(track, mask);
}

void MainFrame::OnUpdateUIRangeRecordTrack(wxUpdateUIEvent& event)
{
	if (!IsConnected())
	{
		event.Enable(false);
		return;
	}

	int check = 0;
	bool enable = IsTrackControllerReady();

    uint32_t track = (uint32_t)(event.GetId() - ID_RECORD_TRACK1);
    wxASSERT(track < 24);

    if (track >= GET_MAX_TRACKS())
    {
        enable = FALSE;
    }
    else
    {
        uint8_t flags = GetTransportState().trackState[track];

        if (flags & STC_T_READY)
            check = 1;
    }

	event.Check((check) ? true : false);
	event.Enable(enable);
}

///////////////////////////////////////////////////////////////////////////////
// TRACK - INPUT MODE

void MainFrame::OnRangeInputTrack(wxCommandEvent& event)
{
	STC_STATE_MSG& state = GetTransportState();

    uint32_t track = (uint32_t)(event.GetId() - ID_INPUT_TRACK1);
    wxASSERT(track < 24);

    // Preserve upper flag bits
    uint8_t flags = state.trackState[track] & ~(STC_TRACK_MASK);
    uint8_t mode  = STC_TRACK_INPUT | flags;

    GetSocketCommand().TrackSetState(track, (uint32_t)mode);
}

void MainFrame::OnUpdateRangeInputTrack(wxUpdateUIEvent& event)
{
	STC_STATE_MSG& state = GetTransportState();

	int check = 0;
	bool enable = IsTrackControllerReady();

    uint32_t track = (uint32_t)(event.GetId() - ID_INPUT_TRACK1);
    wxASSERT(track < 24);

    if (track >= GET_MAX_TRACKS())
    {
        enable = FALSE;
    }
    else
    {
        if ((state.trackState[track] & STC_TRACK_MASK) == STC_TRACK_INPUT)
            check = 1;
    }

	event.Check(check);
	event.Enable(enable);
}

///////////////////////////////////////////////////////////////////////////////
// TRACK - REPRO MODE

void MainFrame::OnRangeReproTrack(wxCommandEvent& event)
{
	STC_STATE_MSG& state = GetTransportState();

    uint32_t track = (uint32_t)(event.GetId() - ID_REPRO_TRACK1);
    wxASSERT(track < 24);

    // Preserve upper flag bits
    uint8_t flags = state.trackState[track] & ~(STC_TRACK_MASK);
    uint8_t mode = STC_TRACK_REPRO | flags;

    GetSocketCommand().TrackSetState(track, (uint32_t)mode);
}

void MainFrame::OnUpdateRangeReproTrack(wxUpdateUIEvent& event)
{
    STC_STATE_MSG& state = GetTransportState();

	int check = 0;
	bool enable = IsTrackControllerReady();

    uint32_t track = (uint32_t)(event.GetId() - ID_REPRO_TRACK1);
    wxASSERT(track < 24);

    if (track >= GET_MAX_TRACKS())
    {
        enable = FALSE;
    }
    else
    {
        if ((state.trackState[track] & STC_TRACK_MASK) == STC_TRACK_REPRO)
            check = 1;
	}

	event.Check(check);
    event.Enable(enable);
}

///////////////////////////////////////////////////////////////////////////////
// TRACK - SYNC MODE

void MainFrame::OnRangeSyncTrack(wxCommandEvent& event)
{
	STC_STATE_MSG& state = GetTransportState();

    uint32_t track = (uint32_t)(event.GetId() - ID_SYNC_TRACK1);
    wxASSERT(track < 24);

    // Preserve upper flag bits
    uint8_t flags = state.trackState[track] & ~(STC_TRACK_MASK);
    uint8_t mode = STC_TRACK_SYNC | flags;

    GetSocketCommand().TrackSetState(track, (uint32_t)mode);
}

void MainFrame::OnUpdateRangeSyncTrack(wxUpdateUIEvent& event)
{
    STC_STATE_MSG& state = GetTransportState();

	int check = 0;
	bool enable = IsTrackControllerReady();

    uint32_t track = (uint32_t)(event.GetId() - ID_SYNC_TRACK1);
    wxASSERT(track < 24);

    if (track >= GET_MAX_TRACKS())
    {
        enable = FALSE;
    }
    else
    {
        if ((state.trackState[track] & STC_TRACK_MASK) == STC_TRACK_SYNC)
            check = 1;
    }

	event.Check(check);
    event.Enable(enable);
}

///////////////////////////////////////////////////////////////////////////////
// TRACK - MONITOR INPUT UI HANDLERS

void MainFrame::OnRangeMonitorTrack(wxCommandEvent& event)
{
	STC_STATE_MSG& state = GetTransportState();

    uint32_t track = (uint32_t)(event.GetId() - ID_MONITOR_TRACK1);
    wxASSERT(track < 24);

    // Toggle track monitor state for the channel
    uint8_t mask = state.trackState[track];

    // Toggle monitor enable flag for the track
    mask ^= STC_T_MONITOR;

    // If not in monitor mode, clear standby bit!
    if (!(mask & STC_T_MONITOR))
    {
        mask &= ~(STC_T_STANDBY);
    }
    else
    {
        // If master monitor enabled, enable standby mode for
        // the track so it will switch to standby input mode.
        if (state.monitorFlags)
            mask |= STC_T_STANDBY;
    }

    GetSocketCommand().TrackSetState(track, mask);
}

void MainFrame::OnUpdateUIRangeMonitorTrack(wxUpdateUIEvent& event)
{
	int check = 0;
	bool enable = IsTrackControllerReady();

    uint32_t track = (uint32_t)(event.GetId() - ID_MONITOR_TRACK1);
    wxASSERT(track < 24);

    if (track >= GET_MAX_TRACKS())
    {
        enable = FALSE;
    }
    else
    {
        uint8_t flags = GetTransportState().trackState[track];

        if (flags & STC_T_MONITOR)
            check = 1;
    }

	event.Check(check);
	event.Enable(enable);
}

///////////////////////////////////////////////////////////////////////////////
// TRACK - TOGGLE TRACK MODE

void MainFrame::OnRangeTrackToggleMode(wxCommandEvent& event)
{
	STC_STATE_MSG& state = GetTransportState();

    uint32_t track = (uint32_t)(event.GetId() - ID_TOGGLEMODE_TRACK1);
    wxASSERT(track < 24);

    // Toggle track mode between input, sync and repro
    uint8_t flags = state.trackState[track] & ~(STC_TRACK_MASK);
    uint8_t mode  = (state.trackState[track] & STC_TRACK_MASK);

    // If they are trying to change the channel assignment while
    // it's already in standby mode, then just disable standby
    // monitor and go back to the default channel state.
    if (flags & STC_T_STANDBY)
    {
        flags &= ~(STC_T_STANDBY | STC_T_MONITOR);
    }
    else
    {
        // Otherwise, advance to the next track mode.
        ++mode;
    }

    // Check for wrap around to next track state
    if (mode > STC_TRACK_INPUT)
        mode = STC_TRACK_REPRO;

    // Combine the new mode with track state flags and set it.
    mode |= flags;

    // Set the new track state
    GetSocketCommand().TrackSetState(track, (uint32_t)mode);
}

void MainFrame::OnUpdateRangeTrackToggleMode(wxUpdateUIEvent& event)
{
	bool enable = IsTrackControllerReady();

    event.Enable(enable);
}

///////////////////////////////////////////////////////////////////////////////
// TRACK - SET ALL TRACKS TO SAFE

void MainFrame::OnTrackAllSafe(wxCommandEvent& event)
{
	GetSocketCommand().TrackMaskAll(0, STC_T_READY);
}

void MainFrame::OnUpdateTrackAllSafe(wxUpdateUIEvent& event)
{
    event.Enable(IsTrackControllerReady());
}

///////////////////////////////////////////////////////////////////////////////
// TRACK - SET ALL TRACKS TO REC READY

void MainFrame::OnTrackAllReady(wxCommandEvent& event)
{
    GetSocketCommand().TrackMaskAll(STC_T_READY, 0);
}

void MainFrame::OnUpdateTrackAllReady(wxUpdateUIEvent& event)
{
    event.Enable(IsTrackControllerReady());
}

///////////////////////////////////////////////////////////////////////////////
// TRACK - SET ALL TRACKS TO INPUT MODE

void MainFrame::OnTrackAllInput(wxCommandEvent& event)
{
    GetSocketCommand().TrackModeAll(STC_TRACK_INPUT);
}

void MainFrame::OnUpdateTrackAllInput(wxUpdateUIEvent& event)
{
    event.Enable(IsTrackControllerReady());
}

///////////////////////////////////////////////////////////////////////////////
// TRACK - SET ALL TRACKS TO REPRO MODE

void MainFrame::OnTrackAllRepro(wxCommandEvent& event)
{
    GetSocketCommand().TrackModeAll(STC_TRACK_REPRO);
}

void MainFrame::OnUpdateTrackAllRepro(wxUpdateUIEvent& event)
{
    event.Enable(IsTrackControllerReady());
}

///////////////////////////////////////////////////////////////////////////////
// TRACK - SET ALL TRACKS TO SYNC MODE

void MainFrame::OnTrackAllSync(wxCommandEvent& event)
{
    GetSocketCommand().TrackModeAll(STC_TRACK_SYNC);
}

void MainFrame::OnUpdateTrackAllSync(wxUpdateUIEvent& event)
{
    event.Enable(IsTrackControllerReady());
}

// =========================================================================
// TRACK - SET ALL TO MONITOR INPUT MODE

void MainFrame::OnTrackAllMonitorOn(wxCommandEvent& event)
{
	GetSocketCommand().TrackMaskAll(STC_T_MONITOR, 0);
}

void MainFrame::OnUpdateTrackAllMonitorOn(wxUpdateUIEvent& event)
{
	event.Enable(IsTrackControllerReady());
}

///////////////////////////////////////////////////////////////////////////////
// TRACK - SET ALL TRACKS STANDBY MONITOR MODE OFF

void MainFrame::OnTrackAllMonitorOff(wxCommandEvent& event)
{
	GetSocketCommand().TrackMaskAll(0, STC_T_MONITOR | STC_T_STANDBY);
}

void MainFrame::OnUpdateTrackAllMonitorOff(wxUpdateUIEvent& event)
{
    event.Enable(IsTrackControllerReady());
}

// =========================================================================
// TRACK - TOGGLE STANDBY MONITOR MODE

void MainFrame::OnTrackStandbymonitor(wxCommandEvent& event)
{
	bool state = IsStandbyMonitor() ? false : true;

	GetSocketCommand().MonitorSet(state);
	
	wxGetApp().m_panelOption->m_checkStandbyMon->SetValue(state);
}

void MainFrame::OnUpdateTrackStandbymonitor(wxUpdateUIEvent& event)
{
	if (!IsTrackControllerReady())
	{
		event.Enable(false);
		return;
	}

	event.Enable(true);
	event.Check(IsStandbyMonitor());
}

// =========================================================================
// TRACK CONFIGURATION - RECALL DEFAULTS FROM EPROM

void MainFrame::OnConfigurationRecall(wxCommandEvent& event)
{
    wxString str("Recall default track configuration parameters from EEPROM memory?\n\nAll track configuration parameters currently in memory will be replaced!");

    int response = wxMessageBox(str, wxT("Confirm Action"), wxYES_NO | wxICON_QUESTION);

	if (response == wxYES)
    {
		GetSocketCommand().ConfigEPROM(0);
	}
}

void MainFrame::OnUpdateConfigurationRecall(wxUpdateUIEvent& event)
{
	if (!IsTrackControllerReady())
	{
		event.Enable(false);
		return;
	}

	event.Enable(IsConnected());
}

// =========================================================================
// TRACK CONFIGURATION - SAVE TO EPROM AS DEFAULT

void MainFrame::OnConfigurationSave(wxCommandEvent& event)
{
    wxString str("Save all current track configuration parameters in memory to EEPROM memory?\n\nThe current parameters will be stored in EPROM and used as the default values next power-up!");

    int response = wxMessageBox(str, "Confirm Action", wxYES_NO | wxICON_QUESTION);

	if (response == wxYES)
	{
		GetSocketCommand().ConfigEPROM(1);
	}
}

void MainFrame::OnUpdateConfigurationSave(wxUpdateUIEvent& event)
{
	if (!IsTrackControllerReady())
	{
		event.Enable(false);
		return;
	}

	event.Enable(IsConnected());
}

// =========================================================================
// TRACK CONFIGURATION - RESET ALL TO DEFAULT VALUES

void MainFrame::OnConfigurationReset(wxCommandEvent& event)
{
    wxString str("Recall default machine configuration parameters from EEPROM memory?\n\nAll machine configuration parameters currently in memory will be replaced!");

    int response = wxMessageBox(str, "Confirm Action", wxYES_NO | wxICON_QUESTION);

	if (response == wxYES)
	{
		// Reset to defaults
		GetSocketCommand().ConfigEPROM(2);

		// Recall the new default values into memory
		GetSocketCommand().ConfigEPROM(0);
	}
}

void MainFrame::OnUpdateConfigurationReset(wxUpdateUIEvent& event)
{
	if (!IsTrackControllerReady())
	{
		event.Enable(false);
		return;
	}

	event.Enable(IsConnected());
}

///////////////////////////////////////////////////////////////////////////////
// TRANSPORT - RECORD ARM TOGGLE

void MainFrame::OnTransportRecord(wxCommandEvent& WXUNUSED(event))
{
	m_bRecord = (m_bRecord) ? false : true;
}

void MainFrame::OnUpdateUITransportRecord(wxUpdateUIEvent& event)
{
	if (!IsConnected() || !IsTransportReady())
	{
		event.Enable(false);
		return;
	}

	//event.Check((m_bRecord) ? true : false);
	event.Enable(true);
}

///////////////////////////////////////////////////////////////////////////////
// TRANSPORT - STOP

void MainFrame::OnTransportStop(wxCommandEvent& WXUNUSED(event))
{
	m_bRecord = FALSE;
	GetSocketCommand().TransportStop();
}

void MainFrame::OnUpdateUITransportStop(wxUpdateUIEvent& event)
{
	if (!IsConnected())
	{
		event.Enable(false);
		return;
	}

	//event.Enable(IsTransportMode(STC_MODE_STOP) ? true : false);
	event.Enable(true);
}

///////////////////////////////////////////////////////////////////////////////
// TRANSPORT - PLAY

void MainFrame::OnTransportPlay(wxCommandEvent& WXUNUSED(event))
{
	GetSocketCommand().TransportPlay((m_bRecord) ? 1 : 0);
}

void MainFrame::OnUpdateUITransportPlay(wxUpdateUIEvent& event)
{
	if (!IsConnected())
	{
		event.Enable(false);
		return;
	}

	//event.Check(IsTransportMode(STC_MODE_PLAY));
	event.Enable(IsTransportReady());
}

///////////////////////////////////////////////////////////////////////////////
// TRANSPORT - REWIND

void MainFrame::OnTransportRewind(wxCommandEvent& WXUNUSED(event))
{
	m_bRecord = FALSE;
	GetSocketCommand().TransportRewind(IsLibWind() ? STC_M_LIBWIND : 0);
}

void MainFrame::OnUpdateUITransportRewind(wxUpdateUIEvent& event)
{
	if (!IsConnected())
	{
		event.Enable(false);
		return;
	}

	//event.Check(IsTransportMode(STC_MODE_REW));
	event.Enable(IsTransportReady());
}

///////////////////////////////////////////////////////////////////////////////
// TRANSPORT - FORWARD

void MainFrame::OnTransportForward(wxCommandEvent& WXUNUSED(event))
{
	m_bRecord = FALSE;
	GetSocketCommand().TransportForward(IsLibWind() ? STC_M_LIBWIND : 0);
}

void MainFrame::OnUpdateUITransportForward(wxUpdateUIEvent& event)
{
	if (!IsConnected())
	{
		event.Enable(false);
		return;
	}

	//event.Check(IsTransportMode(STC_MODE_FWD));
	event.Enable(IsTransportReady());
}

// =========================================================================
// TRANSPORT - TOGGLE PLAY/STOP

void MainFrame::OnTransportToggleplay(wxCommandEvent& WXUNUSED(event))
{
	if (IsTransportMode(STC_MODE_STOP))
	{
		GetSocketCommand().TransportPlay((m_bRecord) ? 1 : 0);
	}
	else if (IsTransportMode(STC_MODE_PLAY))
	{
		GetSocketCommand().TransportStop();
	}
}

void MainFrame::OnUpdateTransportToggleplay(wxUpdateUIEvent& event)
{
	if (!IsConnected() || !IsTransportReady())
	{
		event.Enable(false);
		return;
	}

	event.Enable(IsTransportMode(STC_MODE_STOP) || IsTransportMode(STC_MODE_PLAY));
}

// =========================================================================
// TRANSPORT - PUNCH IN/OUT IN PLAY MODE

void MainFrame::OnTransportPunch(wxCommandEvent& WXUNUSED(event))
{
	GetSocketCommand().TransportPlay(1);
}

void MainFrame::OnUpdateTransportPunch(wxUpdateUIEvent& event)
{
	if (!IsConnected() || !IsTransportReady())
	{
		event.Enable(false);
		return;
	}

	event.Enable(IsTransportMode(STC_MODE_PLAY));
}

///////////////////////////////////////////////////////////////////////////////
// TRANSPORT - TAPE LIFTER

void MainFrame::OnTransportTapeLifter(wxCommandEvent& WXUNUSED(event))
{
	m_bRecord = FALSE;
	GetSocketCommand().TransportLifter();
}

void MainFrame::OnUpdateUITransportTapeLifter(wxUpdateUIEvent& event)
{
	if (!IsConnected())
	{
		event.Enable(false);
		return;
	}

	//event.Check(IsTransportMode(STC_M_LIFTERS) ? true : false);
	event.Enable(true);
}

///////////////////////////////////////////////////////////////////////////////
// TRANSPORT - LIBRARY WIND

void MainFrame::OnTransportLibraryWind(wxCommandEvent& WXUNUSED(event))
{
	m_bLibWind = (m_bLibWind) ? false : true;

    wxGetApp().m_panelOption->m_checkLibWind->SetValue(m_bLibWind);
}

void MainFrame::OnUpdateUITransportLibraryWind(wxUpdateUIEvent& event)
{
	if (!IsConnected())
	{
		event.Enable(false);
		return;
	}

	event.Enable(true);
    event.Check(m_bLibWind);
}

///////////////////////////////////////////////////////////////////////////////
// TRANSPORT - SPEED SELECT

void MainFrame::OnTapespeedHighspeed(wxCommandEvent& WXUNUSED(event))
{
	GetSocketCommand().TapeSpeedSet(30);
}

void MainFrame::OnUpdateTapespeedHighspeed(wxUpdateUIEvent& event)
{
	if (!IsTrackControllerReady())
	{
		event.Enable(false);
		return;
	}

    event.Enable(true);
	event.Check(IsTapeSpeed(30));
}

void MainFrame::OnTapespeedLowspeed(wxCommandEvent& WXUNUSED(event))
{
	GetSocketCommand().TapeSpeedSet(15);
}

void MainFrame::OnUpdateTapespeedLowspeed(wxUpdateUIEvent& event)
{
	if (!IsTrackControllerReady())
	{
		event.Enable(false);
		return;
	}

    event.Enable(true);
	event.Check(IsTapeSpeed(15));
}

///////////////////////////////////////////////////////////////////////////////
// LOCATOR Menu Handlers

void MainFrame::OnLocatorAutoPlay(wxCommandEvent& event)
{
	m_bAutoPlay = (event.IsChecked()) ? true : false;

	wxGetApp().m_panelOption->m_checkAutoPlay->SetValue(m_bAutoPlay);
}

void MainFrame::OnUpdateUILocatorAutoPlay(wxUpdateUIEvent& event)
{
	if (!IsConnected())
	{
		event.Enable(false);
		return;
	}

	event.Enable(true);
	event.Check(m_bAutoPlay);
}

// =========================================================================
// LOCATOR HELPER METHODS

void MainFrame::SearchOrStore(uint32_t cueIndex)
{
	uint32_t flags = 0;

	if (IsLocatorButtonMask(STC_L_STORE))
	{
		GetSocketCommand().CuePointStore(cueIndex, -1, STC_CF_ACTIVE);
	}
	else
	{
		if (IsAutoPlay())
			flags |= STC_CF_AUTO_PLAY;

		if (IsRecord())
			flags |= STC_CF_AUTO_REC;

		GetSocketCommand().LocatorSearch(cueIndex, flags);
	}
}

void MainFrame::UpdateLocatorButtonUI(wxUpdateUIEvent& event, uint32_t ledmask)
{
	if (!IsConnected())	// || !IsTransportReady())
	{
		event.Enable(false);
		return;
	}

	event.Enable(true);
}

// =========================================================================
// LOCATOR - CUE POINT 0

void MainFrame::OnCuePointLoc0(wxCommandEvent& WXUNUSED(event))
{
	SearchOrStore(0);
}

void MainFrame::OnUpdateCuePointLoc0(wxUpdateUIEvent& event)
{
	UpdateLocatorButtonUI(event, STC_L_LOC0);
}

// =========================================================================
// LOCATOR - CUE POINT 1

void MainFrame::OnCuePointLoc1(wxCommandEvent& WXUNUSED(event))
{
	SearchOrStore(1);
}

void MainFrame::OnUpdateCuePointLoc1(wxUpdateUIEvent& event)
{
	UpdateLocatorButtonUI(event, STC_L_LOC1);
}

// =========================================================================
// LOCATOR - CUE POINT 2

void MainFrame::OnCuePointLoc2(wxCommandEvent& WXUNUSED(event))
{
	SearchOrStore(2);
}

void MainFrame::OnUpdateCuePointLoc2(wxUpdateUIEvent& event)
{
	UpdateLocatorButtonUI(event, STC_L_LOC2);
}

// =========================================================================
// LOCATOR - CUE POINT 3

void MainFrame::OnCuePointLoc3(wxCommandEvent& WXUNUSED(event))
{
	SearchOrStore(3);
}

void MainFrame::OnUpdateCuePointLoc3(wxUpdateUIEvent& event)
{
	UpdateLocatorButtonUI(event, STC_L_LOC3);
}

// =========================================================================
// LOCATOR - CUE POINT 4

void MainFrame::OnCuePointLoc4(wxCommandEvent& WXUNUSED(event))
{
	SearchOrStore(4);
}

void MainFrame::OnUpdateCuePointLoc4(wxUpdateUIEvent& event)
{
	UpdateLocatorButtonUI(event, STC_L_LOC4);
}

// =========================================================================
// LOCATOR - CUE POINT 5

void MainFrame::OnCuePointLoc5(wxCommandEvent& WXUNUSED(event))
{
	SearchOrStore(5);
}

void MainFrame::OnUpdateCuePointLoc5(wxUpdateUIEvent& event)
{
	UpdateLocatorButtonUI(event, STC_L_LOC5);
}

// =========================================================================
// LOCATOR - CUE POINT 6

void MainFrame::OnCuePointLoc6(wxCommandEvent& WXUNUSED(event))
{
	SearchOrStore(6);
}

void MainFrame::OnUpdateCuePointLoc6(wxUpdateUIEvent& event)
{
	UpdateLocatorButtonUI(event, STC_L_LOC6);
}

// =========================================================================
// LOCATOR - CUE POINT 7

void MainFrame::OnCuePointLoc7(wxCommandEvent& WXUNUSED(event))
{
	SearchOrStore(7);
}

void MainFrame::OnUpdateCuePointLoc7(wxUpdateUIEvent& event)
{
	UpdateLocatorButtonUI(event, STC_L_LOC7);
}

// =========================================================================
// LOCATOR - CUE POINT 8

void MainFrame::OnCuePointLoc8(wxCommandEvent& WXUNUSED(event))
{
	SearchOrStore(8);
}

void MainFrame::OnUpdateCuePointLoc8(wxUpdateUIEvent& event)
{
	UpdateLocatorButtonUI(event, STC_L_LOC8);
}

// =========================================================================
// LOCATOR - CUE POINT 9

void MainFrame::OnCuePointLoc9(wxCommandEvent& WXUNUSED(event))
{
	SearchOrStore(9);
}

void MainFrame::OnUpdateCuePointLoc9(wxUpdateUIEvent& event)
{
	UpdateLocatorButtonUI(event, STC_L_LOC9);
}

// =========================================================================
// SEARCH - TO AUTO-LOOP MARK IN TIME

void MainFrame::OnCueMarkIn(wxCommandEvent& WXUNUSED(event))
{
	uint32_t flags = 0;

	if (IsAutoPlay())
		flags |= STC_CF_AUTO_PLAY;

	if (IsRecord())
		flags |= STC_CF_AUTO_REC;

	GetSocketCommand().LocatorSearch(STC_CUE_POINT_MARK_IN, flags);
}

void MainFrame::OnUpdateCueMarkIn(wxUpdateUIEvent& event)
{
	if (!IsConnected() || !IsTransportReady())
	{
		event.Enable(false);
		return;
	}

	event.Enable(true);
}

// =========================================================================
// SEARCH - TO AUTO-LOOP MARK OUT TIME

void MainFrame::OnCueMarkOut(wxCommandEvent& WXUNUSED(event))
{
	uint32_t flags = 0;

	if (IsAutoPlay())
		flags |= STC_CF_AUTO_PLAY;

	if (IsRecord())
		flags |= STC_CF_AUTO_REC;

	GetSocketCommand().LocatorSearch(STC_CUE_POINT_MARK_OUT, flags);
}

void MainFrame::OnUpdateCueMarkOut(wxUpdateUIEvent& event)
{
	if (!IsConnected() || !IsTransportReady())
	{
		event.Enable(false);
		return;
	}

	event.Enable(true);
}

// =========================================================================
// SEARCH - TO PUNCH IN TIME

void MainFrame::OnCuePunchIn(wxCommandEvent& WXUNUSED(event))
{
	uint32_t flags = 0;

	if (IsAutoPlay())
		flags |= STC_CF_AUTO_PLAY;

	if (IsRecord())
		flags |= STC_CF_AUTO_REC;

	GetSocketCommand().LocatorSearch(STC_CUE_POINT_PUNCH_IN, flags);
}

void MainFrame::OnUpdateCuePunchIn(wxUpdateUIEvent& event)
{
	if (!IsConnected() || !IsTransportReady())
	{
		event.Enable(false);
		return;
	}

	event.Enable(true);
}

// =========================================================================
// SEARCH - TO PUNCH OUT TIME

void MainFrame::OnCuePunchOut(wxCommandEvent& WXUNUSED(event))
{
	uint32_t flags = 0;

	if (IsAutoPlay())
		flags |= STC_CF_AUTO_PLAY;

	if (IsRecord())
		flags |= STC_CF_AUTO_REC;

	GetSocketCommand().LocatorSearch(STC_CUE_POINT_PUNCH_OUT, flags);
}

void MainFrame::OnUpdateCuePunchOut(wxUpdateUIEvent& event)
{
	if (!IsConnected() || !IsTransportReady())
	{
		event.Enable(false);
		return;
	}

	event.Enable(true);
}

// =========================================================================
// SEARCH - RETURN TO ZERO HOME

void MainFrame::OnLocatorZeroreturn(wxCommandEvent& WXUNUSED(event))
{
	GetSocketCommand().LocatorZeroReturn();
}

void MainFrame::OnUpdateLocatorZeroreturn(wxUpdateUIEvent& event)
{
	if (!IsConnected() || !IsTransportReady())
	{
		event.Enable(false);
		return;
	}

	event.Enable(true);
}

// =========================================================================
// LOCATOR - TOGGLE BETWEEN CUE/STORE MODE

void MainFrame::OnLocatorCueMode(wxCommandEvent& WXUNUSED(event))
{
	GetSocketCommand().LocatorSetMode(0);
}

void MainFrame::OnUpdateLocatorCueMode(wxUpdateUIEvent& event)
{
	if (!IsConnected())
	{
		event.Enable(false);
		return;
	}

	//event.Check((GetTransportState().ledMaskButton & STC_L_CUE) ? 1 : 0);
	event.Enable(true);
}

void MainFrame::OnLocatorStoreMode(wxCommandEvent& WXUNUSED(event))
{
	GetSocketCommand().LocatorSetMode(1);
}

void MainFrame::OnUpdateLocatorStoreMode(wxUpdateUIEvent& event)
{
	if (!IsConnected())
	{
		event.Enable(false);
		return;
	}

	//event.Check((GetTransportState().ledMaskButton & STC_L_STORE) ? 1 : 0);
	event.Enable(true);
}

// =========================================================================
// LOCATOR - RESET COUNTER TO ZERO

void MainFrame::OnLocatorResetCounter(wxCommandEvent& WXUNUSED(event))
{
	bool reset = false;

	if (!m_bConfirmZeroReset)
	{
		reset = TRUE;
	}
	else
	{
	    wxString str("Reset the tape time counter to zero?");

        int response = wxMessageBox(str, wxT("DRC1200"), wxYES_NO | wxICON_QUESTION);

        if (response == wxYES)
        {
			reset = TRUE;
        }
	}

	if (reset)
		GetSocketCommand().LocatorZeroReset();
}

void MainFrame::OnUpdateLocatorResetCounter(wxUpdateUIEvent& event)
{
	event.Enable(IsConnected());
}

// =========================================================================
// LOCATOR - CANCEL SEARCH

void MainFrame::OnLocatorCancelsearch(wxCommandEvent& WXUNUSED(event))
{
	GetSocketCommand().LocatorCancel();
}

void MainFrame::OnUpdateLocatorCancelsearch(wxUpdateUIEvent& event)
{
	if (!IsConnected())
	{
		event.Enable(false);
		return;
	}

    event.Enable(IsTransportSearching() || IsTransportLooping());
}

// =========================================================================
// LOCATOR - AUTO LOOP MODE

void MainFrame::OnLocatorAutoLoop(wxCommandEvent& WXUNUSED(event))
{
	uint32_t cue_flags = IsRecord() ? STC_CF_AUTO_REC : 0;

	GetSocketCommand().LocatorAutoLoop(cue_flags);
}

void MainFrame::OnUpdateLocatorAutoLoop(wxUpdateUIEvent& event)
{
	if (!IsConnected())
	{
		event.Enable(false);
		return;
	}

    event.Enable(IsTransportReady());
}

// =========================================================================
// LOCATOR - MARK IN

void MainFrame::OnLocatorMarkIn(wxCommandEvent& WXUNUSED(event))
{
	GetSocketCommand().CuePointStore(STC_CUE_POINT_MARK_IN, -1, STC_CF_ACTIVE);
}

void MainFrame::OnUpdateLocatorMarkIn(wxUpdateUIEvent& event)
{
 	if (!IsConnected())
	{
		event.Enable(false);
		return;
	}

    event.Enable(IsTransportReady());
}

// =========================================================================
// LOCATOR - MARK OUT

void MainFrame::OnLocatorMarkOut(wxCommandEvent& WXUNUSED(event))
{
	GetSocketCommand().CuePointStore(STC_CUE_POINT_MARK_OUT, -1, STC_CF_ACTIVE);
}

void MainFrame::OnUpdateLocatorMarkOut(wxUpdateUIEvent& event)
{
 	if (!IsConnected())
	{
		event.Enable(false);
		return;
	}

    event.Enable(IsTransportReady());
}

// =========================================================================
// LOCATOR - AUTO PUNCH MODE

void MainFrame::OnLocatorAutoPunch(wxCommandEvent& WXUNUSED(event))
{
	STC_STATE_MSG& state = GetTransportState();

	// toggle auto-punch mode flag
	if (state.transportMode & STC_M_PUNCH)
		GetSocketCommand().LocatorAutoPunchSet(0);
	else
		GetSocketCommand().LocatorAutoPunchSet(1);
}

void MainFrame::OnUpdateLocatorAutoPunch(wxUpdateUIEvent& event)
{
 	if (!IsConnected())
	{
		event.Enable(false);
		return;
	}

    event.Enable(IsTransportReady());
}

void MainFrame::OnLocatorPunchIn(wxCommandEvent& WXUNUSED(event))
{
	GetSocketCommand().CuePointStore(STC_CUE_POINT_PUNCH_IN, -1, STC_CF_ACTIVE);
}

void MainFrame::OnUpdateLocatorPunchIn(wxUpdateUIEvent& event)
{
 	if (!IsConnected())
	{
		event.Enable(false);
		return;
	}

    event.Enable(IsTransportReady());
}

void MainFrame::OnLocatorPunchOut(wxCommandEvent& WXUNUSED(event))
{
	GetSocketCommand().CuePointStore(STC_CUE_POINT_PUNCH_OUT, -1, STC_CF_ACTIVE);
}

void MainFrame::OnUpdateLocatorPunchOut(wxUpdateUIEvent& event)
{
    if (!IsConnected())
	{
		event.Enable(false);
		return;
	}

    event.Enable(IsTransportReady());
}

///////////////////////////////////////////////////////////////////////////////
// MEMORY Menu Handlers


///////////////////////////////////////////////////////////////////////////////
// SMPTE - START SMPTE GENERATOR

void MainFrame::OnGeneratorStop(wxCommandEvent& event)
{
	wxSocketError dwError;

	dwError = GetSocketCommand().SMPTEEncoderCtrl(STC_SMPTE_ENCODER_CTRL_STOP);

	if (dwError != wxSOCKET_NOERROR)
        wxLogMessage("OnGeneratorStop failed");
}

void MainFrame::OnUpdateGeneratorStop(wxUpdateUIEvent& event)
{
	bool enable = false;

	if (IsHardwareSMPTE() && IsConnected())
	{
		if (m_state.smpteMode == STC_SMPTE_ENCODER)
			enable = true;
	}

	event.Enable(enable);
}

void MainFrame::OnGeneratorStart(wxCommandEvent& event)
{
	wxSocketError dwError;

	// Set the starting time for the encoder
	dwError = GetSocketCommand().SMPTEEncoderSetTime(
		m_smpteHour, m_smpteMins,
		m_smpteSecs, m_smpteFrame);

	if (dwError == wxSOCKET_NOERROR)
	{
		// Start the encoder at the time set per above
		dwError = GetSocketCommand().SMPTEEncoderCtrl(STC_SMPTE_ENCODER_CTRL_START, 0);
	}

	if (dwError)
        wxLogMessage("OnGeneratorStop failed");
}

void MainFrame::OnUpdateGeneratorStart(wxUpdateUIEvent& event)
{
	bool enable = false;

	if (IsHardwareSMPTE() && IsConnected())
	{
		if (m_state.smpteMode == STC_SMPTE_OFF)
			enable = true;
	}

	event.Enable(enable);
}

// =========================================================================
// SMPTE - RESUME SMPTE GENERATOR

void MainFrame::OnGeneratorResume(wxCommandEvent& event)
{
	wxSocketError dwError;

	dwError = GetSocketCommand().SMPTEEncoderCtrl(STC_SMPTE_ENCODER_CTRL_START, 0);

	if (dwError)
        wxLogMessage("OnGeneratorResume failed");
}

void MainFrame::OnUpdateGeneratorResume(wxUpdateUIEvent& event)
{
	bool enable = FALSE;

	if (IsHardwareSMPTE() && IsConnected())
	{
		if (m_state.smpteMode == STC_SMPTE_OFF)
			enable = TRUE;
	}

	event.Enable(enable);
}

// =========================================================================
// SMPTE - EDIT TIME

void MainFrame::OnGeneratorEdittime(wxCommandEvent& event)
{
#if 0
	INT_PTR rc;
	CEditSMPTETimeDlg dlg;

	dlg.m_nHour = m_smpteHour;
	dlg.m_nMins = m_smpteMins;
	dlg.m_nSecs = m_smpteSecs;
	dlg.m_nFrame = m_smpteFrame;

	// Prompt the edit time dialog
	if ((rc = dlg.DoModal()) == IDOK)
	{
		m_smpteHour = dlg.m_nHour;
		m_smpteMins = dlg.m_nMins;
		m_smpteSecs = dlg.m_nSecs;
		m_smpteFrame = dlg.m_nFrame;
	}
#endif
}

void MainFrame::OnUpdateGeneratorEdittime(wxUpdateUIEvent& event)
{
	bool enable = false;

	if (IsHardwareSMPTE() && IsConnected())
		enable = true;

	event.Enable(enable);
}

///////////////////////////////////////////////////////////////////////////////
// VIEW Menu Handlers

void MainFrame::OnViewShowTracks(wxCommandEvent& event)
{
	if (event.IsChecked())
	{
		m_bShowTracks = true;
		m_trackFrame->Show();
	}
	else
	{
		m_bShowTracks = false;
		m_trackFrame->Hide();
	}
}

void MainFrame::OnUpdateUIViewShowTracks(wxUpdateUIEvent& event)
{
	event.Enable(true);
	event.Check(m_trackFrame->IsVisible());
}

///////////////////////////////////////////////////////////////////////////////
// Frame Control Handlers

void MainFrame::OnCheckLibraryWind(wxCommandEvent& event)
{
	m_bLibWind = event.IsChecked() ? true : false;
}

void MainFrame::OnUpdateUICheckLibraryWind(wxUpdateUIEvent& event)
{
	if (!IsConnected())
	{
		event.Enable(false);
		return;
	}

	event.Enable(true);
	//event.Check(m_bLibWind);
}

void MainFrame::OnCheckAutoPlay(wxCommandEvent& event)
{
	m_bAutoPlay = event.IsChecked() ? true : false;
}

void MainFrame::OnUpdateUICheckAutoPlay(wxUpdateUIEvent& event)
{
	if (!IsConnected())
	{
		event.Enable(false);
		return;
	}

	event.Enable(true);

	//event.Check(m_bAutoPlay);
}

void MainFrame::OnCheckStandbyMon(wxCommandEvent& event)
{
	bool state = IsStandbyMonitor() ? false : true;

	GetSocketCommand().MonitorSet(state);
}

void MainFrame::OnUpdateUICheckStandbyMon(wxUpdateUIEvent& event)
{
	if (!IsTrackControllerReady())
	{
		event.Enable(FALSE);
		return;
	}

	event.Enable(TRUE);
	//event.Check(IsStandbyMonitor());
}

///////////////////////////////////////////////////////////////////////////////
// HELP Menu Handlers

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxAboutDialogInfo info;

	info.SetName(wxT("DRC1200"));
	info.SetIcon(wxIcon(wxT("DRC1200.png")));
	//info.SetIcon(wxIcon(wxT("AAAA_DRC1200")));
	info.SetVersion(wxT("1.04"), wxT("Version 1.05"));
	info.SetDescription(wxT("TCP/IP Remote Control for Ampex MM1200"));
	info.SetCopyright(wxT("Copyright (C) 2026, RTZ Professional Audio"));
	info.AddDeveloper(wxT("Robert E Starr, Jr."));
	info.SetWebSite(wxT("rtzaudio.com"), wxT("visit us at rtzaudio.com"));

	wxAboutBox(info, this);
}
