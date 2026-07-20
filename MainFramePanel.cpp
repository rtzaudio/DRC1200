#include "DRC1200.h"

///////////////////////////////////////////////////////////////////////////////
// Left Top Panel - Status pane

OptionPanel::OptionPanel(wxPanel* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(150, 50), wxBORDER_NONE)
{
	m_parent = parent;

	wxGetApp().m_panelOption = this;

	wxPanel* panel = new wxPanel(this, wxID_ANY);
	panel->SetBackgroundColour(wxGetApp().m_colorFrame);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(panel, 1, wxEXPAND | wxALL);

	m_checkAutoPlay = new wxCheckBox(panel, ID_CHECK_AUTOPLAY, wxT("Auto Play"));
	m_checkAutoPlay->SetForegroundColour(wxGetApp().m_colorText);

	m_checkLibWind = new wxCheckBox(panel, ID_CHECK_LIBRARYWIND, wxT("Lib Wind"));
	m_checkLibWind->SetForegroundColour(wxGetApp().m_colorText);

    m_checkStandbyMon = new wxCheckBox(panel, ID_CHECK_STANDBYMON, wxT("Standby Monitor"));
	m_checkStandbyMon->SetForegroundColour(wxGetApp().m_colorText);

	wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

	hbox->Add(m_checkAutoPlay, 0, wxEXPAND | wxALL, 5);
	hbox->Add(m_checkLibWind, 0, wxEXPAND | wxALL, 5);

	vbox->Add(hbox, 0, wxEXPAND | wxALL, 0);
    vbox->Add(m_checkStandbyMon, 0, wxEXPAND | wxALL, 5);

	panel->SetSizer(vbox);

	SetSizer(sizer);
}

CommandButton::CommandButton(wxPanel* mypanel, int id, const wxString& label)
	: wxButton(mypanel, id, label, wxDefaultPosition, wxSize(40, 20))
{
    SetBackgroundColour(wxGetApp().m_colorBtnDark);
	SetForegroundColour(wxGetApp().m_colorBtnText);

	Connect(id, wxEVT_BUTTON, wxCommandEventHandler(CommandButton::OnCommandButtonClicked));
}

void CommandButton::OnCommandButtonClicked(wxCommandEvent& event)
{
    // Get pointer to MainFrame (passed in constructor or via wxGetApp)
    wxWindow* mainFrame = wxGetApp().GetMainFrame();

    // Forward the event
    if (mainFrame)
    {
        wxCommandEvent* event = new wxCommandEvent(wxEVT_COMMAND_MENU_SELECTED, GetId());
        wxQueueEvent(mainFrame->GetEventHandler(), event);
    }
}

CommandPanel::CommandPanel(wxPanel* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(150, 50))
{
	m_parent = parent;

	wxPanel* panel = new wxPanel(this, wxID_ANY);

    panel->SetBackgroundColour(wxGetApp().m_colorPanel);

    m_cmdButton[0]  = new CommandButton(panel, ID_TRACK_ALLINPUT, wxT("All Tracks Input"));
	m_cmdButton[1]  = new CommandButton(panel, ID_TRACK_ALLREPRO, wxT("All Tracks Repro"));
	m_cmdButton[2]  = new CommandButton(panel, ID_TRACK_ALLSYNC, wxT("All Tracks Sync"));

	m_cmdButton[3]  = new CommandButton(panel, ID_TRACK_ALLSAFE, wxT("All Tracks Safe"));
    m_cmdButton[4]  = new CommandButton(panel, ID_TRACK_ALLREADY, wxT("All Tracks Ready"));
	m_cmdButton[5]  = new CommandButton(panel, ID_LOCATOR_RESETCOUNTER, wxT("Zero Reset"));

 	m_cmdButton[6]  = new CommandButton(panel, ID_TRACK_ALLMONITORON, wxT("All Monitor On"));
	m_cmdButton[7]  = new CommandButton(panel, ID_TRACK_ALLMONITOROFF, wxT("All Monitor Off"));
    m_cmdButton[8]  = new CommandButton(panel, ID_TRANSPORT_TOGGLEPLAY, wxT("Play/Stop"));

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	const int spacing = 0;

	wxGridSizer* gs = new wxGridSizer(3, 3, spacing, spacing);

	for (size_t i = 0; i <= 8; i++)
    {
        gs->Add(m_cmdButton[i], 1, wxEXPAND);
    }

	sizer->Add(panel, 1, wxEXPAND | wxALL);

	panel->SetSizer(gs);

	SetSizer(sizer);
}

///////////////////////////////////////////////////////////////////////////////
// Left Top Panel - Tape Time and Status pane

TopContainer::TopContainer(wxPanel* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(150, 50), wxBORDER_NONE)
{
	m_parent = parent;

	SetBackgroundColour(wxGetApp().m_colorPanel);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	sizer->Add(new TimePanel(this), 2, wxEXPAND | wxALL, 5);
	sizer->Add(new OptionPanel(this), 1, wxEXPAND | wxALL, 5);

	SetSizer(sizer);
}

///////////////////////////////////////////////////////////////////////////////
// Left Middle Panel - Velocity Graph Panel

MiddleContainer::MiddleContainer(wxPanel* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(150, 50), wxBORDER_NONE)
{
	m_parent = parent;

	SetBackgroundColour(wxGetApp().m_colorPanel);

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	sizer->Add(new CommandPanel(this), 2, wxEXPAND | wxALL, 5);
	sizer->Add(new VelocityPanel(this), 1, wxEXPAND | wxALL, 5);

	SetSizer(sizer);
}

///////////////////////////////////////////////////////////////////////////////
// Left Bottom Panel - Transport Control Buttons

TransportButton::TransportButton(wxPanel* mypanel, int id, const wxString& label)
	: wxButton(mypanel, id, label, wxDefaultPosition, wxSize(50, 30))
{
    SetBackgroundColour(wxGetApp().m_colorBtnDark);
	SetForegroundColour(wxGetApp().m_colorBtnText);

	Connect(id, wxEVT_BUTTON, wxCommandEventHandler(TransportButton::OnTransportButtonClicked));
}

void TransportButton::OnTransportButtonClicked(wxCommandEvent& event)
{
    // Get pointer to MainFrame (passed in constructor or via wxGetApp)
    wxWindow* mainFrame = wxGetApp().GetMainFrame();

    // Forward the event
    if (mainFrame)
    {
        wxCommandEvent* event = new wxCommandEvent(wxEVT_COMMAND_MENU_SELECTED, GetId());
        wxQueueEvent(mainFrame->GetEventHandler(), event);
    }
}

BottomContainer::BottomContainer(wxPanel* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(270, 150), wxBORDER_NONE)
{
	m_parent = parent;

	SetBackgroundColour(wxGetApp().m_colorPanel);

	wxPanel* panel = new wxPanel(this, -1);
    //panel->SetBackgroundColour(wxGetApp().m_colorPanel);

	m_btnRec  = new TransportButton(panel, ID_TRANSPORT_RECORD, wxT("REC"));
	m_btnPlay = new TransportButton(panel, ID_TRANSPORT_PLAY, wxT("PLAY"));
	m_btnRew  = new TransportButton(panel, ID_TRANSPORT_REWIND, wxT("REW"));
	m_btnFwd  = new TransportButton(panel, ID_TRANSPORT_FORWARD, wxT("FWD"));
	m_btnStop = new TransportButton(panel, ID_TRANSPORT_STOP, wxT("STOP"));
	m_btnLift = new TransportButton(panel, ID_TRANSPORT_TAPELIFTER, wxT("LIFT"));

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

	wxGridSizer* gs = new wxGridSizer(1, 6, 10, 10);

	gs->Add(m_btnRec, 1, wxEXPAND);
	gs->Add(m_btnPlay, 1, wxEXPAND);
	gs->Add(m_btnRew, 1, wxEXPAND);
	gs->Add(m_btnFwd, 1, wxEXPAND);
	gs->Add(m_btnStop, 1, wxEXPAND);
	gs->Add(m_btnLift, 1, wxSHRINK);

	sizer->Add(panel, 1, wxEXPAND | wxALL, 10);

	panel->SetSizer(gs);

	SetSizer(sizer);
}

///////////////////////////////////////////////////////////////////////////////
// Left Hand View Frame Container Panel

LeftContainer::LeftContainer(wxWindow* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(150, 50), wxBORDER_NONE)
{
	m_parent = parent;

	SetBackgroundColour(wxGetApp().m_colorPanel);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxPanel* panel = new wxPanel(this, -1);
    //panel->SetBackgroundColour(wxGetApp().m_colorPanel);

	m_panelTop    = new TopContainer(panel);
	m_panelMiddle = new MiddleContainer(panel);
	m_panelBottom = new BottomContainer(panel);

	wxGridSizer* gs = new wxGridSizer(3, 1, 5, 5);

	gs->Add(m_panelTop, 1, wxEXPAND);
	gs->Add(m_panelMiddle, 1, wxEXPAND);
	gs->Add(m_panelBottom, 1, wxEXPAND);

	sizer->Add(panel, 1, wxEXPAND | wxALL, 5);

	panel->SetSizer(gs);

	SetSizer(sizer);
}

///////////////////////////////////////////////////////////////////////////////
// Right Hand View Frame Container Panel

LocatorButton::LocatorButton(wxPanel* mypanel, int id, const wxString& label)
	: wxButton(mypanel, id, label, wxDefaultPosition, wxSize(40, 40))
{
    SetBackgroundColour(wxGetApp().m_colorBtnDark);
	SetForegroundColour(wxGetApp().m_colorBtnText);

	Connect(id, wxEVT_BUTTON, wxCommandEventHandler(LocatorButton::OnLocatorButtonClicked));
}

void LocatorButton::OnLocatorButtonClicked(wxCommandEvent& event)
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
}

RightContainer::RightContainer(wxWindow* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxBORDER_NONE)
{
	m_parent = parent;

	SetBackgroundColour(wxGetApp().m_colorPanel);

	wxPanel* panel = new wxPanel(this, -1);

    panel->SetBackgroundColour(wxGetApp().m_colorPanel);

	m_btnDigit0     = new LocatorButton(panel, ID_CUEPOINT_LOC0, wxT("0"));
	m_btnDigit1     = new LocatorButton(panel, ID_CUEPOINT_LOC1, wxT("1"));
	m_btnDigit2     = new LocatorButton(panel, ID_CUEPOINT_LOC2, wxT("2"));
	m_btnDigit3     = new LocatorButton(panel, ID_CUEPOINT_LOC3, wxT("3"));
	m_btnDigit4     = new LocatorButton(panel, ID_CUEPOINT_LOC4, wxT("4"));
	m_btnDigit5     = new LocatorButton(panel, ID_CUEPOINT_LOC5, wxT("5"));
	m_btnDigit6     = new LocatorButton(panel, ID_CUEPOINT_LOC6, wxT("6"));
	m_btnDigit7     = new LocatorButton(panel, ID_CUEPOINT_LOC7, wxT("7"));
	m_btnDigit8     = new LocatorButton(panel, ID_CUEPOINT_LOC8, wxT("8"));
	m_btnDigit9     = new LocatorButton(panel, ID_CUEPOINT_LOC9, wxT("9"));
    m_btnZeroReturn = new LocatorButton(panel, ID_SEARCH_ZERORETURN, wxT("RTZ"));
	m_btnCueMode    = new LocatorButton(panel, ID_LOCATOR_CUEMODE, wxT("CUE"));
	m_btnStoreMode  = new LocatorButton(panel, ID_LOCATOR_STOREMODE, wxT("STO"));
	m_btnAutoPunch  = new LocatorButton(panel, ID_LOCATOR_AUTOPUNCH, wxT("Auto\nPunch"));
	m_btnPunchIn    = new LocatorButton(panel, ID_LOCATOR_PUNCHIN, wxT("Punch\nIn"));
	m_btnPunchOut   = new LocatorButton(panel, ID_LOCATOR_PUNCHOUT, wxT("Punch\nOut"));
	m_btnAutoLoop   = new LocatorButton(panel, ID_LOCATOR_AUTOLOOP, wxT("Auto\nLoop"));
	m_btnMarkIn     = new LocatorButton(panel, ID_LOCATOR_MARKIN, wxT("Mark\nIn"));
	m_btnMarkOut    = new LocatorButton(panel, ID_LOCATOR_MARKOUT, wxT("Mark\nOut"));
	m_btnCancel     = new LocatorButton(panel, ID_LOCATOR_CANCELSEARCH, wxT("CAN"));

	wxGridSizer* gs = new wxGridSizer(4, 5, 5, 5);

	gs->Add(m_btnDigit1, 1, wxEXPAND);
	gs->Add(m_btnDigit2, 1, wxEXPAND);
	gs->Add(m_btnDigit3, 1, wxEXPAND);
	gs->Add(m_btnAutoPunch, 1, wxEXPAND);
	gs->Add(m_btnAutoLoop, 1, wxEXPAND);

	gs->Add(m_btnDigit4, 1, wxEXPAND);
	gs->Add(m_btnDigit5, 1, wxEXPAND);
	gs->Add(m_btnDigit6, 1, wxEXPAND);
	gs->Add(m_btnPunchIn, 1, wxEXPAND);
	gs->Add(m_btnMarkIn, 1, wxEXPAND);

	gs->Add(m_btnDigit7, 1, wxEXPAND);
	gs->Add(m_btnDigit8, 1, wxEXPAND);
	gs->Add(m_btnDigit9, 1, wxEXPAND);

    gs->Add(m_btnPunchOut, 1, wxEXPAND);
	gs->Add(m_btnMarkOut, 1, wxEXPAND);

	gs->Add(m_btnCueMode, 1, wxEXPAND);
	gs->Add(m_btnDigit0, 1, wxEXPAND);
	gs->Add(m_btnStoreMode, 1, wxEXPAND);

	gs->Add(m_btnZeroReturn, 1, wxEXPAND);
	gs->Add(m_btnCancel, 1, wxEXPAND);

    wxBoxSizer* sizerMain = new wxBoxSizer(wxVERTICAL);

	sizerMain->Add(panel, 1, wxEXPAND | wxALL, 10);

	panel->SetSizer(gs);

	SetSizer(sizerMain);
}

