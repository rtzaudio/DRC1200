#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include "wx/sizer.h"
#include "wx/gbsizer.h"
#include "wx/minifram.h"
#include "wx/statline.h"
#include "wx/notebook.h"
#include "wx/spinctrl.h"
#include "wx/wrapsizer.h"
#include "wx/generic/stattextg.h"
#include "wx/html/htmlwin.h"
#include "wx/persist/toplevel.h"
#include "wx/aboutdlg.h"
#include "wx/socket.h"
#include "wx/url.h"
#include "wx/sstream.h"
#include "wx/thread.h"
#include "wx/dcbuffer.h"
#include "wx/dcgraph.h"
#include "wx/overlay.h"
#include "wx/graphics.h"
#include "wx/minifram.h"

#include "DRC1200.h"
#include "TrackFrame.h"

#define WIN_STYLE	(wxRESIZE_BORDER | wxCAPTION | wxPOPUP_WINDOW | wxFRAME_TOOL_WINDOW)

wxBEGIN_EVENT_TABLE(TrackFrame, wxMiniFrame)
EVT_CLOSE(TrackFrame::OnClose)
wxEND_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////
// TrackFrame Buttons

TrackButton::TrackButton(wxPanel* panel, int id, const wxString& label)
	: wxButton(panel, id, label, wxDefaultPosition, wxSize(10, 10))
{
	Connect(id, wxEVT_BUTTON, wxCommandEventHandler(TrackButton::OnTrackButtonClicked));

//  SetBackgroundColour(wxColor(*wxBLACK));
//	SetForegroundColour(wxColor(*wxLIGHT_GREY));
//	Enable(false);
}

void TrackButton::OnTrackButtonClicked(wxCommandEvent& event)
{
    // Get pointer to MainFrame (passed in constructor or via wxGetApp)
    wxWindow* mainFrame = wxGetApp().GetMainFrame();
    // Forward the event
    if (mainFrame)
    {
        wxCommandEvent* event = new wxCommandEvent(wxEVT_COMMAND_MENU_SELECTED, GetId());
        //event->SetEventObject(this);
        wxQueueEvent(mainFrame->GetEventHandler(), event);
    }
	//event.Skip();
}

///////////////////////////////////////////////////////////////////////////////
// TrackFrame

TrackFrame::TrackFrame(wxFrame* parent) :
	wxMiniFrame(parent, wxID_ANY, wxT("DRC1200 - Track Assign"),
    wxDefaultPosition, wxSize(915, 120), WIN_STYLE)
{
	wxString str;

	m_colorBlack.SetRGB(0x000000);
    m_colorWhite.SetRGB(0xFFFFFF);

	m_colorFrame.SetRGB(0x484848);
	m_colorPanel.SetRGB(0x333333);

	m_colorBtnText.SetRGB(0xFFFFFF);
	m_colorBtnFace.SetRGB(0x585858);
    m_colorBtnGrey.SetRGB(0xC9C9C9);
    m_colorBtnDark.SetRGB(0x484848);

    m_colorBtnRecReady.SetRGB(0x000055);
    m_colorBtnRecActive.SetRGB(0x0000FF);

    m_colorBtnModeInput.SetRGB(0x04B2FE);       // 0xFFC200
    m_colorBtnModeRepro.SetRGB(0x54A835);       // 0x00FF00
    m_colorBtnModeSync.SetRGB(0xEEA200);        // 0x2280FF
    m_colorBtnModeMon.SetRGB(0xA0A0A0);         // 0xFFFFFF

    SetName("DRC1200_TrackFrame");

    // Zero out track/cue state shadow buffers
    memset(m_trackState, 0xFF, sizeof(m_trackState));

    // Create the background panel
	wxPanel* panel = new wxPanel(this, wxID_ANY);
	panel->SetBackgroundColour(m_colorFrame);

	// Create panel to hold all the button controls
	m_midPan = new wxPanel(panel, wxID_ANY);
	m_midPan->SetBackgroundColour(m_colorPanel);

	// Create and add box sizer in panel with some padding margin
	m_hbox = new wxBoxSizer(wxHORIZONTAL);
	m_hbox->Add(m_midPan, 1, wxEXPAND | wxALL, 5);
	panel->SetSizer(m_hbox);

	wxFlexGridSizer* gs = new wxFlexGridSizer(4, STC_MAX_TRACKS, 2, 2);

    gs->SetFlexibleDirection(wxBOTH);
    gs->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_ALL);

    gs->AddGrowableRow(0);
    gs->AddGrowableRow(1);
    gs->AddGrowableRow(2);
    gs->AddGrowableRow(3);

    for (int i=0; i < STC_MAX_TRACKS; i++)
    {
        gs->AddGrowableCol(i);
    }

	// Create track number buttons in disabled state

	for (int i=0; i < STC_MAX_TRACKS; i++)
	{
		str.Printf(wxT("%u"), i + 1);

		m_btnTrackNum[i] = new TrackButton(m_midPan, wxID_ANY, str);

		m_btnTrackNum[i]->SetBackgroundColour(wxColor(*wxBLACK));
		m_btnTrackNum[i]->SetForegroundColour(m_colorBtnGrey);
		m_btnTrackNum[i]->Enable(false);

		gs->Add(m_btnTrackNum[i], 2, wxEXPAND | wxTOP | wxLEFT, 2);
	}

	// Create track arm status buttons

	for (int i=0; i < STC_MAX_TRACKS; i++)
	{
		str.Printf(wxT("%s"), wxT("SAFE"));

		m_btnTrackArm[i] = new TrackButton(m_midPan, ID_RECORD_TRACK1+i, str);

		m_btnTrackArm[i]->SetBackgroundColour(m_colorBtnDark);
		m_btnTrackArm[i]->SetForegroundColour(m_colorBtnGrey);
		m_btnTrackArm[i]->Enable(false);

        gs->Add(m_btnTrackArm[i], 2, wxEXPAND | wxTOP | wxLEFT, 2);
	}

	// Create track mode buttons

	for (int i=0; i < STC_MAX_TRACKS; i++)
	{
		str.Printf(wxT("%s"), wxT("SYN"));

		m_btnTrackMode[i] = new TrackButton(m_midPan, ID_TOGGLEMODE_TRACK1+i, str);

		m_btnTrackMode[i]->SetBackgroundColour(m_colorBtnDark);
		m_btnTrackMode[i]->SetForegroundColour(m_colorBtnGrey);
		m_btnTrackMode[i]->Enable(false);

        gs->Add(m_btnTrackMode[i], 2, wxEXPAND | wxTOP | wxBOTTOM | wxLEFT, 2);
	}

	// Create track monitor mode buttons

    for (int i=0; i < STC_MAX_TRACKS; i++)
	{
		str.Printf(wxT("%s"), wxT("MON"));

		m_btnTrackMon[i] = new TrackButton(m_midPan, ID_MONITOR_TRACK1+i, str);

		m_btnTrackMon[i]->SetBackgroundColour(m_colorBtnDark);
		m_btnTrackMon[i]->SetForegroundColour(m_colorBtnGrey);
		m_btnTrackMon[i]->Enable(false);

        gs->Add(m_btnTrackMon[i], 2, wxEXPAND | wxTOP | wxBOTTOM | wxLEFT, 2);
	}

	m_midPan->SetSizer(gs);

	SetMinSize(wxSize(200, 100));
	//SetMaxSize(wxSize(-1, 250));

    wxPersistentRegisterAndRestore(this, "DRC1200_TrackFrame");
}

TrackFrame::~TrackFrame()
{

}

void TrackFrame::SetTrackConfig(int numtracks)
{
    for (int i=0; i < STC_MAX_TRACKS; i++)
    {
        if (i >= numtracks)
        {
            m_btnTrackNum[i]->Hide();
            m_btnTrackArm[i]->Hide();
            m_btnTrackMode[i]->Hide();
            m_btnTrackMon[i]->Hide();
        }
        else
        {
            m_btnTrackNum[i]->Show();
            m_btnTrackArm[i]->Show();
            m_btnTrackMode[i]->Show();
            m_btnTrackMon[i]->Show();
        }
    }

    m_midPan->Layout();
}

///////////////////////////////////////////////////////////////////////////////
// Override Handlers

void TrackFrame::OnClose(wxCloseEvent& event)
{
	//Hide();
	//event.CanVeto();
	//Close();
}

///////////////////////////////////////////////////////////////////////////////
// Helpers

void TrackFrame::ResetTrackButtonStates(bool repaint)
{
    memset(m_trackState, 0xFF, sizeof(m_trackState));

    if (repaint)
    {
        UpdateTrackButtonStates();
    }
}

void TrackFrame::UpdateTrackButtonStates(void)
{
	uint8_t mode;
	wxString str;
    wxColor colorBtn;
    wxColor colorTxt;

    MainFrame* pMainFrame = wxGetApp().GetMainFrame();

	STC_STATE_MSG& state = pMainFrame->GetTransportState();

	bool enabled = pMainFrame->IsTrackControllerReady();

	size_t numTracks = state.trackCount;

	for (size_t i = 0; i < numTracks; i++)
	{
		//-----------------------------------------------------------
		// ROW-1: Track# heading buttons
		//-----------------------------------------------------------

		// Update only if track state has changed

        if (m_trackState[i] == state.trackState[i])
            continue;

        m_trackState[i] = state.trackState[i];

		//-----------------------------------------------------------
		// ROW-2: Track ready/safe record status button
		//-----------------------------------------------------------

		if (!enabled || (i >= numTracks))
		{
			colorBtn = m_colorBtnDark;
			colorTxt = m_colorBtnGrey;
		}
		else
        {
            colorBtn = m_colorBtnFace;
            colorTxt = m_colorBtnText;

            if (state.trackState[i] & STC_T_READY)
            {
                if ((state.trackState[i] & STC_T_RECORD) && (state.transportMode & STC_M_RECORD))
                {
                    str = wxT("REC");
                    colorBtn = m_colorBtnRecActive;
                }
                else
                {
                    str = wxT("RDY");
                    colorBtn = m_colorBtnRecReady;
                }
            }
            else
            {
                str = wxT("SAFE");
            }

            m_btnTrackArm[i]->SetLabelText(str);
        }

        m_btnTrackArm[i]->SetBackgroundColour(colorBtn);
        m_btnTrackArm[i]->SetForegroundColour(colorTxt);

        m_btnTrackArm[i]->Enable(enabled);

		//-----------------------------------------------------------
		// ROW-3: Track mode status button
        //-----------------------------------------------------------

		if (!enabled || (i >= numTracks))
		{
			colorBtn = m_colorBtnDark;
			colorTxt = m_colorBtnGrey;
		}
		else
		{
            mode = (state.trackState[i] & STC_TRACK_MASK);

            if (state.trackState[i] & STC_T_STANDBY)
            {
                if (state.trackState[i] & STC_T_MONITOR)
                    mode = STC_TRACK_INPUT;
            }

			colorTxt = m_colorBtnText;
            colorBtn = m_colorBtnFace;

			switch (mode)
            {
            case STC_TRACK_INPUT:
                colorBtn = m_colorBtnModeInput;
                str = wxString("INP");
                break;
            case STC_TRACK_REPRO:
                colorBtn = m_colorBtnModeRepro;
                str = wxString("REPR");
                break;
            case STC_TRACK_SYNC:
                colorBtn = m_colorBtnModeSync;
                str = wxString("SYNC");
                break;
            default:
                str = wxString(" ");
                break;
            }

            m_btnTrackMode[i]->SetLabelText(str);
		}

        m_btnTrackMode[i]->SetBackgroundColour(colorBtn);
        m_btnTrackMode[i]->SetForegroundColour(colorTxt);

        m_btnTrackMode[i]->Enable(enabled);

		//-----------------------------------------------------------
		// ROW-4: Draw the monitor mode button and text
		//-----------------------------------------------------------

		if (!enabled || (i >= numTracks))
		{
			colorBtn = m_colorBtnDark;
			colorTxt = m_colorBtnGrey;
		}
		else
        {
            // Draw input monitor mode according to it's state
            if (state.trackState[i] & STC_T_MONITOR)
            {
                if (state.trackState[i] & STC_T_STANDBY)
                {
                    colorBtn = m_colorBtnModeInput;
                    colorTxt = m_colorBtnText;
                }
                else
                {
                    colorBtn = m_colorBtnModeMon;
                    colorTxt = m_colorBtnText;
                }
            }
            else
            {
                colorBtn = m_colorBtnFace;
                colorTxt = m_colorBtnText;
            }
		}

        m_btnTrackMon[i]->SetBackgroundColour(colorBtn);
        m_btnTrackMon[i]->SetForegroundColour(colorTxt);

        m_btnTrackMon[i]->Enable(enabled);
	}
}

// End-Of-File
