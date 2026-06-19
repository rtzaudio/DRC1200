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

void MainFrame::UpdateTransportButtonStates(void)
{
    STC_STATE_MSG& state = GetTransportState();

    BottomContainer* panel = m_panelLeft->m_panelBottom;

    if (m_transportMode_prev != m_state.transportMode)
    {
        /////////////////////////////////////////////////////
        // Set RECORD Button Color

        if (state.transportMode & STC_M_RECORD)
        {
            panel->m_btnRec->SetBackgroundColour(m_colorRecActive);
        }
        else
        {
            if (m_bRecord)
            {
                panel->m_btnRec->SetBackgroundColour(m_colorRecReady);
            }
            else
            {
                panel->m_btnRec->SetBackgroundColour(m_colorOFF);
            }
        }

        panel->m_btnRec->SetForegroundColour(m_colorText);

        /////////////////////////////////////////////////////
        // Set LIFTERS Button Color

        if (state.transportMode & STC_M_LIFTERS)
        {
            panel->m_btnLift->SetBackgroundColour(m_colorON);
        }
        else
        {
            panel->m_btnLift->SetBackgroundColour(m_colorOFF);
        }

        panel->m_btnLift->SetForegroundColour(m_colorText);
    }

    // Set Transport Button Colors

    if (m_ledMaskTransport_prev != m_state.ledMaskTransport)
    {
        UpdateTransportButton(panel->m_btnPlay, state, STC_L_PLAY);
        UpdateTransportButton(panel->m_btnRew, state, STC_L_REW);
        UpdateTransportButton(panel->m_btnFwd, state, STC_L_FWD);
        UpdateTransportButton(panel->m_btnStop, state, STC_L_STOP);
    }
}

void MainFrame::UpdateTransportButton(TransportButton* button, STC_STATE_MSG& state, uint32_t mask)
{
    if (state.ledMaskTransport & mask)
    {
        button->SetBackgroundColour(m_colorON);
    }
    else
    {
        button->SetBackgroundColour(m_colorOFF);
    }

    button->SetForegroundColour(m_colorText);
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
        button->SetBackgroundColour(m_colorON);
    }
    else
    {
        button->SetBackgroundColour(m_colorOFF);
    }

    button->SetForegroundColour(m_colorText);
}

///////////////////////////////////////////////////////////////////////////////
//

void MainFrame::UpdateLocatorButtonStates(void)
{
    STC_STATE_MSG& state = GetTransportState();

    wxColor color;

    RightContainer* panel = m_panelRight;

    if (m_ledMaskButton_prev != m_state.ledMaskButton)
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

    if (m_transportMode_prev != m_state.transportMode)
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

        color = m_colorOFF;

        if ((state.transportMode & STC_M_SEARCH) | (state.transportMode & STC_M_LOOP))
        {
            if (IsTransportSearching() || IsTransportLooping())
            {
                color = m_colorON;
            }
        }

        panel->m_btnCancel->SetBackgroundColour(color);
    }
}

void MainFrame::UpdateLocatorButton(LocatorButton* button, STC_STATE_MSG& state, uint32_t mask, int cueIndex)
{
    if (!IsConnected())
    {
        button->SetForegroundColour(m_colorGrey);
        button->SetBackgroundColour(m_colorDark);
    }
    else
    {
        if (cueIndex >= 0)
        {
            // If cue point button is active, draw it in white
            if (state.cueState[cueIndex] & STC_CF_ACTIVE)
            {
                button->SetForegroundColour(m_colorTextBright);
            }
            else
            {
                button->SetForegroundColour(m_colorTextDim);
            }

            if (state.ledMaskButton & mask)
                button->SetBackgroundColour(m_colorON);
            else
                button->SetBackgroundColour(m_colorOFF);
        }
        else
        {
            button->SetForegroundColour(m_colorText);

            if (state.ledMaskButton & mask)
                button->SetBackgroundColour(m_colorON);
            else
                button->SetBackgroundColour(m_colorOFF);
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
    STC_STATE_MSG& state = GetTransportState();

#if wxUSE_IPV6
    wxIPV6address addr;
#else
    wxIPV4address addr;
#endif
	wxString str;

	//STC_STATE_MSG& state = GetTransportState();

    // Display IP address connection state

	if (!m_sockCommand.IsConnected())
	{
        SetStatusText(wxT("(offline)"), 2);
        SetStatusText(wxT(""), 1);
	}
	else
	{
		wxString strMode;

		if (state.hardwareFlags & STC_HF_SMPTE)
        {
            if (state.smpteMode == 1)
                strMode = _T("STRIPE");
            else if (state.smpteMode == 2)
                strMode = _T("SLAVE");
            else
                strMode = _T("OFF");

            str.Printf(wxT("SMPTE: %s"), strMode);

            // Display current smpte mode
            SetStatusText(str, 1);
        }

        // Display the current host name
		m_sockState->GetPeer(addr);
        //str.Printf(wxT("%s : %s"), addr.Hostname(), m_strHostname);
		str.Printf(wxT("%s"), m_strHostname);
        SetStatusText(str, 2);
    }

    // Enable/disable connect menu options

	m_menuConnect->Enable(ID_CONNECT_TCP, !m_sockCommand.IsConnected() && !m_busy);
#if wxUSE_IPV6
	//m_menuSocket->Enable(CLIENT_OPENIPV6, !m_sock->IsConnected() && !m_busy);
#endif
	m_menuConnect->Enable(ID_CONNECT_CLOSE, m_sockCommand.IsConnected());
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
		str = wxT("TAPE OUT");
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

