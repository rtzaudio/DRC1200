#include "DRC1200.h"
#include "ConnectDialog.h"

///////////////////////////////////////////////////////////////////////////////
//helper functions

enum wxbuildinfoformat {
    short_f, long_f
};

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif
#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

///////////////////////////////////////////////////////////////////////////////
// Helpers

bool MainFrame::IsTrackControllerReady(void)
{
	if (IsConnected())
	{
		if (IS_DCS_FOUND())
			return true;
	}

	return false;
}

bool MainFrame::IsAnyTracksArmed(void)
{
	size_t i;

	for (i=0; i < GET_MAX_TRACKS(); i++)
	{
		if (m_state.trackState[i] & STC_T_READY)
			return true;
	}

	return false;
}

void MainFrame::UpdateTransportButtonStates(bool repaint)
{
    STC_STATE_MSG& state = GetTransportState();

    BottomContainer* panel = m_panelLeft->m_panelBottom;

    if ((m_transportMode_prev != m_state.transportMode) || repaint)
    {
        /////////////////////////////////////////////////////
        // Set RECORD Button Color

        if (!IsConnected())
        {
            panel->m_btnRec->SetForegroundColour(wxGetApp().m_colorBtnGrey);
            panel->m_btnRec->SetBackgroundColour(wxGetApp().m_colorBtnDark);
        }
        else
        {
            if (state.transportMode & STC_M_RECORD)
            {
                panel->m_btnRec->SetBackgroundColour(wxGetApp().m_colorBtnRecActive);
            }
            else
            {
                if (m_bRecord)
                {
                    panel->m_btnRec->SetBackgroundColour(wxGetApp().m_colorBtnRecReady);
                }
                else
                {
                    panel->m_btnRec->SetBackgroundColour(wxGetApp().m_colorBtnFace);
                }
            }

            panel->m_btnRec->SetForegroundColour(wxGetApp().m_colorBtnText);
        }

        /////////////////////////////////////////////////////
        // Set LIFTERS Button Color

        if (!IsConnected())
        {
            panel->m_btnLift->SetForegroundColour(wxGetApp().m_colorBtnGrey);
            panel->m_btnLift->SetBackgroundColour(wxGetApp().m_colorBtnDark);
        }
        else
        {
            if (state.transportMode & STC_M_LIFTERS)
            {
                panel->m_btnLift->SetBackgroundColour(wxGetApp().m_colorBtnActive);
            }
            else
            {
                panel->m_btnLift->SetBackgroundColour(wxGetApp().m_colorBtnFace);
            }

            panel->m_btnLift->SetForegroundColour(wxGetApp().m_colorBtnText);


            bool checked = (state.transportMode & STC_M_LIBWIND) ? true : false;

            wxGetApp().m_panelOption->m_checkLibWind->SetValue(checked);
        }
    }

    // Set Transport Button Colors

    if ((m_ledMaskTransport_prev != m_state.ledMaskTransport) || repaint)
    {
        UpdateTransportButton(panel->m_btnPlay, state, STC_L_PLAY);
        UpdateTransportButton(panel->m_btnRew, state, STC_L_REW);
        UpdateTransportButton(panel->m_btnFwd, state, STC_L_FWD);
        UpdateTransportButton(panel->m_btnStop, state, STC_L_STOP);
    }
}

void MainFrame::UpdateTransportButton(TransportButton* button, STC_STATE_MSG& state, uint32_t mask)
{
    if (!IsConnected())
    {
        button->SetForegroundColour(wxGetApp().m_colorBtnGrey);
        button->SetBackgroundColour(wxGetApp().m_colorBtnDark);
    }
    else
    {
        if (state.ledMaskTransport & mask)
        {
            button->SetBackgroundColour(wxGetApp().m_colorBtnActive);
        }
        else
        {
            button->SetBackgroundColour(wxGetApp().m_colorBtnFace);
        }

        button->SetForegroundColour(wxGetApp().m_colorBtnText);
    }
}


void MainFrame::UpdateCommandButtonStates(void)
{
    //STC_STATE_MSG& state = GetTransportState();

    //BottomContainer* panel = m_panelLeft->m_panelBottom;
}

void MainFrame::UpdateCommandButton(CommandButton* button, STC_STATE_MSG& state, uint32_t mask)
{
    if (IsConnected() && IsHardwareDCS())
    {
        button->SetBackgroundColour(wxGetApp().m_colorBtnActive);
    }
    else
    {
        button->SetBackgroundColour(wxGetApp().m_colorBtnFace);
    }

    button->SetForegroundColour(wxGetApp().m_colorBtnText);
}

///////////////////////////////////////////////////////////////////////////////
//

void MainFrame::UpdateLocatorButtonStates(bool repaint)
{
    STC_STATE_MSG& state = GetTransportState();

    wxColor color;

    RightContainer* panel = m_panelRight;

    if ((m_ledMaskButton_prev != m_state.ledMaskButton) || repaint)
    {
        // Locate Buttons
        UpdateLocatorButton(panel->m_btnDigit0, state, STC_L_LOC0, 0);
        UpdateLocatorButton(panel->m_btnDigit1, state, STC_L_LOC1, 1);
        UpdateLocatorButton(panel->m_btnDigit2, state, STC_L_LOC2, 2);
        UpdateLocatorButton(panel->m_btnDigit3, state, STC_L_LOC3, 3);
        UpdateLocatorButton(panel->m_btnDigit4, state, STC_L_LOC4, 4);
        UpdateLocatorButton(panel->m_btnDigit5, state, STC_L_LOC5, 5);
        UpdateLocatorButton(panel->m_btnDigit6, state, STC_L_LOC6, 6);
        UpdateLocatorButton(panel->m_btnDigit7, state, STC_L_LOC7, 7);
        UpdateLocatorButton(panel->m_btnDigit8, state, STC_L_LOC8, 8);
        UpdateLocatorButton(panel->m_btnDigit9, state, STC_L_LOC9, 9);
        UpdateLocatorButton(panel->m_btnCueMode, state, STC_L_CUE);
        UpdateLocatorButton(panel->m_btnStoreMode, state, STC_L_STORE);
        // Auto Punch Buttons
        UpdateLocatorButton(panel->m_btnAutoPunch, state, STC_L_AUTO_LOOP);
        UpdateLocatorButton(panel->m_btnPunchIn, state, STC_L_PUNCH_IN);
        UpdateLocatorButton(panel->m_btnPunchOut, state, STC_L_PUNCH_OUT);
        // Auto Loop Buttons
        UpdateLocatorButton(panel->m_btnAutoLoop, state, STC_L_AUTO_LOOP);
        UpdateLocatorButton(panel->m_btnMarkIn, state, STC_L_MARK_IN);
        UpdateLocatorButton(panel->m_btnMarkOut, state, STC_L_MARK_OUT);
    }

    if ((m_transportMode_prev != m_state.transportMode) || repaint)
    {
        // Auto Punch Buttons
/*
        if (state.transportMode & STC_M_PUNCH) {
            panel->m_btnAutoPunch->SetBackgroundColour(m_colorON);
        } else {
            panel->m_btnAutoPunch->SetBackgroundColour(m_colorOFF);
        }
*/
        // For CANCEL button we need to check if the transport is
        // searching or looping before drawing solid background.

        if (!IsConnected())
        {
            panel->m_btnCancel->SetForegroundColour(wxGetApp().m_colorBtnGrey);
            panel->m_btnCancel->SetBackgroundColour(wxGetApp().m_colorBtnDark);
        }
        else
        {
            color = wxGetApp().m_colorBtnFace;

            if ((state.transportMode & STC_M_SEARCH) | (state.transportMode & STC_M_LOOP))
            {
                if (IsTransportSearching() || IsTransportLooping())
                {
                    color = wxGetApp().m_colorBtnActive;
                }
            }

            panel->m_btnCancel->SetBackgroundColour(color);
        }
    }

    if (!IsConnected())
    {
        panel->m_btnZeroReturn->SetForegroundColour(wxGetApp().m_colorBtnGrey);
        panel->m_btnZeroReturn->SetBackgroundColour(wxGetApp().m_colorBtnDark);
    }
    else
    {
        panel->m_btnZeroReturn->SetForegroundColour(wxGetApp().m_colorBtnText);
        panel->m_btnZeroReturn->SetBackgroundColour(wxGetApp().m_colorBtnFace);
    }
}

void MainFrame::UpdateLocatorButton(LocatorButton* button, STC_STATE_MSG& state, uint32_t mask, int cueIndex)
{
    if (!IsConnected())
    {
        button->SetForegroundColour(wxGetApp().m_colorBtnGrey);
        button->SetBackgroundColour(wxGetApp().m_colorBtnDark);
    }
    else
    {
        if (cueIndex >= 0)
        {
            // If cue point button is active, draw it in active color
            if (state.cueState[cueIndex] & STC_CF_ACTIVE)
            {
                button->SetForegroundColour(wxGetApp().m_colorBtnTextActive);
            }
            else
            {
                button->SetForegroundColour(wxGetApp().m_colorBtnTextDim);
            }

            if (state.ledMaskButton & mask)
                button->SetBackgroundColour(wxGetApp().m_colorBtnActive);
            else
                button->SetBackgroundColour(wxGetApp().m_colorBtnFace);
        }
        else
        {
            button->SetForegroundColour(wxGetApp().m_colorBtnText);

            if (state.ledMaskButton & mask)
                button->SetBackgroundColour(wxGetApp().m_colorBtnActive);
            else
                button->SetBackgroundColour(wxGetApp().m_colorBtnFace);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
//

void MainFrame::UpdateTimePanel(void)
{
	if (wxGetApp().m_panelTime)
		wxGetApp().m_panelTime->Refresh(false);
}

///////////////////////////////////////////////////////////////////////////////
//

void MainFrame::UpdateVelocityPanel(void)
{
	if (wxGetApp().m_panelVelocity)
		wxGetApp().m_panelVelocity->Refresh(false);
}

///////////////////////////////////////////////////////////////////////////////
//

void MainFrame::UpdateStatusBar(void)
{
    wxIPV4address addr;

    // Display IP address connection state
	if (!m_sockCommand.IsConnected())
	{
        SetStatusText(wxT(""), 1);
        SetStatusText(wxT("(offline)"), 2);
    }
	else
	{
        // Display the current host name
		m_sockState->GetPeer(addr);
        SetStatusText(addr.Hostname(), 1);
        SetStatusText(m_strHostname, 2);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Return the current transport mode as a string

void MainFrame::GetModeText(STC_STATE_MSG& msg, wxString& str)
{
	uint16_t mode = (msg.transportMode & STC_MODE_MASK);

	if (msg.transportMode & STC_M_SEARCH)
	{
		str = wxT("SEARCH...");
		return;
	}
	else if (msg.transportMode & STC_M_LOOP)
	{
		str = wxT("LOOP");
		return;
	}

	switch (mode)
	{
	case STC_MODE_HALT:
		str = wxT("TAPE OUT HALT");
		break;

	case STC_MODE_THREAD:
		str = wxT("THREAD");
		break;

	case STC_MODE_STOP:
		str = wxT("STOP");
		break;

	case STC_MODE_PLAY:
		str = (msg.transportMode & STC_M_RECORD) ? wxT("RECORD") : wxT("PLAY");
		break;

	case STC_MODE_FWD:
		str = wxT("FWD");
		if (msg.transportMode & STC_M_LIBWIND)
			str += wxT(" (LIB)");
		break;

	case STC_MODE_REW:
		str = wxT("REW");
		if (msg.transportMode & STC_M_LIBWIND)
			str += wxT(" (LIB)");
		break;

	default:
		str = wxT("???");
		break;
	}
}

#if 0

class TCPConnectThread : public wxThread {
public:
    TCPConnectThread(wxEvtHandler* parent, const wxString& host, int port)
        : wxThread(wxTHREAD_JOINABLE), m_pParent(parent), m_sHost(host), m_iPort(port) {}

protected:
    ExitCode Entry() override {
        // Create the socket and perform connection here
        wxIPV4address addr;
        addr.hostname(m_sHost);
        addr.service(m_iPort);

        m_pSocket = new wxSocketClient();

        // Connect asynchronously or use a timeout so you can check TestDestroy()
        m_pSocket->Connect(addr, false); // false = don't block main loop

        // Wait for connection or cancellation
        while (!TestDestroy() && !m_pSocket->WaitOnConnect(100)) {
            // Wait loop
        }class TCPConnectThread : public wxThread {
public:
    TCPConnectThread(wxEvtHandler* parent, const wxString& host, int port)
        : wxThread(wxTHREAD_JOINABLE), m_pParent(parent), m_sHost(host), m_iPort(port) {}

protected:
    ExitCode Entry() override {
        // Create the socket and perform connection here
        wxIPV4address addr;
        addr.hostname(m_sHost);
        addr.service(m_iPort);

        m_pSocket = new wxSocketClient();
class TCPConnectThread : public wxThread {
public:
    TCPConnectThread(wxEvtHandler* parent, const wxString& host, int port)
        : wxThread(wxTHREAD_JOINABLE), m_pParent(parent), m_sHost(host), m_iPort(port) {}

protected:
    ExitCode Entry() override {
        // Create the socket and perform connection here
        wxIPV4address addr;
        addr.hostname(m_sHost);
        addr.service(m_iPort);

        m_pSocket = new wxSocketClient();

        // Connect asynchronously or use a timeout so you can check TestDestroy()
        m_pSocket->Connect(addr, false); // false = don't block main loop

        // Wait for connection or cancellation
        while (!TestDestroy() && !m_pSocket->WaitOnConnect(100)) {
            // Wait loop
        }

        if (TestDestroy()) {
            if (m_pSocket) {
                m_pSocket->Destroy();
                m_pSocket = nullptr;
            }
            return (ExitCode)0;
        }

        // Notify parent on completion
        wxThreadEvent event(wxEVT_THREAD, wxID_OK);
        wxQueueEvent(m_pParent, event.Clone());

        return (ExitCode)0;
    }

    void CancelConnect() {
        if (m_pSocket) {
            m_pSocket->Close();
        }
    }

private:
    wxEvtHandler* m_pParent;
    wxString m_sHost;
    int m_iPort;
    wxSocketClient* m_pSocket = nullptr;
};

Use code with caution.
2. Manage the Dialog and Thread
In your main window or dialog initiating the connection, hook the thread up to the wxProgressDialog.
cpp

void MyClass::ConnectToServer() {
    // 1. Start background thread
    m_pThread = new TCPConnectThread(this, "192.168.1.100", 80);
    if (m_pThread->Run() != wxTHREAD_NO_ERROR) {
        wxLogError("Can't create the thread!");
        return;
    }

    // 2. Show Progress Dialog
    wxProgressDialog progressDialog(
        "Connecting",
        "Attempting to connect to server...",
        100,
        this,
        wxPD_CAN_ABORT | wxPD_APP_MODAL | wxPD_AUTO_HIDE
    );

    // 3. Keep UI responsive and check for abort
    bool continueLoop = true;
    while (m_pThread && m_pThread->IsRunning()) {
        if (!progressDialog.Pulse()) {
            // User pressed Cancel!
            m_pThread->CancelConnect();
            m_pThread->Delete(); // Request thread to exit
            continueLoop = false;
            break;
        }
        wxMilliSleep(100); // Give CPU some rest, or use events
    }

    // 4. Clean up
    progressDialog.Destroy();

    if (!continueLoop) {
        wxMessageBox("Connection cancelled.");
    } else {
        wxMessageBox("Successfully connected!");
    }
}

        // Connect asynchronously or use a timeout so you can check TestDestroy()
        m_pSocket->Connect(addr, false); // false = don't block main loop

        // Wait for connection or cancellation
        while (!TestDestroy() && !m_pSocket->WaitOnConnect(100)) {
            // Wait loop
        }

        if (TestDestroy()) {
            if (m_pSocket) {
                m_pSocket->Destroy();
                m_pSocket = nullptr;
            }
            return (ExitCode)0;
        }

        // Notify parent on completion
        wxThreadEvent event(wxEVT_THREAD, wxID_OK);
        wxQueueEvent(m_pParent, event.Clone());

        return (ExitCode)0;
    }

    void CancelConnect() {
        if (m_pSocket) {
            m_pSocket->Close();
        }
    }

private:
    wxEvtHandler* m_pParent;
    wxString m_sHost;
    int m_iPort;
    wxSocketClient* m_pSocket = nullptr;
};



Use code with caution.
2. Manage the Dialog and Thread
In your main window or dialog initiating the connection, hook the thread up to the wxProgressDialog.
cpp

void MyClass::ConnectToServer()
{
    // 1. Start background thread
    m_pThread = new TCPConnectThread(this, "192.168.1.100", 80);

    if (m_pThread->Run() != wxTHREAD_NO_ERROR)
    {
        wxLogError("Can't create the thread!");
        return;
    }

    // 2. Show Progress Dialog
    wxProgressDialog progressDialog(
        "Connecting",
        "Attempting to connect to server...",
        100,
        this,
        wxPD_CAN_ABORT | wxPD_APP_MODAL | wxPD_AUTO_HIDE
    );

    // 3. Keep UI responsive and check for abort
    bool continueLoop = true;

    while (m_pThread && m_pThread->IsRunning())
    {
        if (!progressDialog.Pulse())
        {
            // User pressed Cancel!
            m_pThread->CancelConnect();
            m_pThread->Delete(); // Request thread to exit
            continueLoop = false;
            break;
        }

        wxMilliSleep(100); // Give CPU some rest, or use events
    }

    // 4. Clean up
    progressDialog.Destroy();

    if (!continueLoop) {
        wxMessageBox("Connection cancelled.");
    } else {
        wxMessageBox("Successfully connected!");
    }
}



        if (TestDestroy()) {
            if (m_pSocket) {
                m_pSocket->Destroy();
                m_pSocket = nullptr;
            }
            return (ExitCode)0;
        }

        // Notify parent on completion
        wxThreadEvent event(wxEVT_THREAD, wxID_OK);
        wxQueueEvent(m_pParent, event.Clone());

        return (ExitCode)0;
    }

    void CancelConnect() {
        if (m_pSocket) {
            m_pSocket->Close();
        }
    }

private:
    wxEvtHandler* m_pParent;
    wxString m_sHost;
    int m_iPort;
    wxSocketClient* m_pSocket = nullptr;
};

#endif
