#include "DRC1200.h"

///////////////////////////////////////////////////////////////////////////////
//

void MainFrame::CreateMenuBar(void)
{
	// Create CONNECT menu items

	m_menuConnect = new wxMenu;

	m_menuConnect->Append(ID_CONNECT_TCP,
		wxT("&TCP/IP\tF1"),
		wxT("Connect to server"));
	m_menuConnect->Append(ID_CONNECT_CLOSE,
		wxT("&Disconnect\tF2"),
		wxT("Disconnect from server"));
	m_menuConnect->AppendSeparator();
	m_menuConnect->Append(wxID_EXIT,
		wxT("E&xit"),
		wxT("Close and exit the program"));

	// Create TRACK menu items

	wxMenu* menuTrack = new wxMenu;
	wxMenu* menuRecord = new wxMenu;
	wxMenu* menuMonitor = new wxMenu;

	wxMenu* submenuInput = new wxMenu;
	wxMenu* submenuRepro = new wxMenu;
	wxMenu* submenuSync = new wxMenu;

	for (int i=0; i < STC_MAX_TRACKS; i++)
	{
		wxString strTrack;
		wxString strDesc;

		strTrack.Printf(wxT("Track %u"), i + 1);

		strDesc.Printf(wxT("Toggle track %u record enable state"), i + 1);
		menuRecord->AppendCheckItem(ID_RECORD_TRACK1+i, strTrack, strDesc);

		strDesc.Printf(wxT("Toggle track %u input mode state"), i + 1);
		submenuInput->AppendCheckItem(ID_INPUT_TRACK1 + i, strTrack, strDesc);

		strDesc.Printf(wxT("Toggle track %u repro mode state"), i + 1);
		submenuRepro->AppendCheckItem(ID_REPRO_TRACK1 + i, strTrack, strDesc);

		strDesc.Printf(wxT("Toggle track %u sync mode state"), i + 1);
		submenuSync->AppendCheckItem(ID_SYNC_TRACK1 + i, strTrack, strDesc);

		strDesc.Printf(wxT("Toggle track %u monitor mode state"), i + 1);
		menuMonitor->AppendCheckItem(ID_MONITOR_TRACK1 + i, strTrack, strDesc);
	}

	wxMenu* submenuSource = new wxMenu;

	submenuSource->Append(ID_SOURCE_INPUT, wxT("Input"), submenuInput);
	submenuSource->Append(ID_SOURCE_REPRO, wxT("Repro"), submenuRepro);
	submenuSource->Append(ID_SOURCE_SYNC, wxT("Sync"), submenuSync);

	menuTrack->Append(ID_TRACK_SOURCE_MODE,
		wxT("Mode"), submenuSource,
		wxT("Set the track source mode to input, repro or sync"));
	menuTrack->Append(ID_TRACK_RECORD_READY,
		wxT("Ready"), menuRecord,
		wxT("Toggle the track record enable state"));
	menuTrack->Append(ID_TRACK_MONITOR_MODE,
		wxT("Monitor"), menuMonitor,
		wxT("Set the track monitor mode to input or tape"));
	menuTrack->AppendSeparator();
	menuTrack->Append(ID_TRACK_ALLSAFE,
		wxT("All Safe"),
		wxT("Set all tracks to safe mode"));
	menuTrack->Append(ID_TRACK_ALLREADY,
		wxT("All Ready"),
		wxT("Set all tracks to ready mode"));
	menuTrack->AppendSeparator();
	menuTrack->Append(ID_TRACK_ALLINPUT,
		wxT("All Input"),
		wxT("Set all tracks to input mode"));
	menuTrack->Append(ID_TRACK_ALLREPRO,
		wxT("All Repro"),
		wxT("Set all tracks to reproduce mode"));
	menuTrack->Append(ID_TRACK_ALLSYNC,
		wxT("All Sync"),
		wxT("Set all tracks to sync mode"));
	menuTrack->AppendSeparator();
	menuTrack->Append(ID_TRACK_ALLMONITORON,
		wxT("All Monitor On"),
		wxT("Enable standby monitor on all tracks"));
	menuTrack->Append(ID_TRACK_ALLMONITOROFF,
		wxT("All Monitor Off"),
		wxT("Disable standby monitor on all tracks"));
	menuTrack->AppendCheckItem(ID_TRACK_STANDBYMONITOR,
		wxT("Standby Monitor"),
		wxT("Toggle global standby monitor mode on/off"));
	menuTrack->AppendSeparator();

	wxMenu* submenuConfig = new wxMenu;

	submenuConfig->Append(ID_CONFIGURATION_SAVE,
        wxT("Store Configuration"),
        wxT("Store track configuration to EPROM"));
	submenuConfig->Append(ID_CONFIGURATION_RECALL,
        wxT("Recall Configuration"),
        wxT("Recall track configuration from EPROM"));
	submenuConfig->Append(ID_CONFIGURATION_RESET,
        wxT("Reset to Defaults"),
        wxT("Reset all tracks to default configuration"));

	menuTrack->Append(ID_TRACK_CONFIGURATION, wxT("Configuration"), submenuConfig);

	// Create TRANSPORT menu items

	wxMenu* menuTransport = new wxMenu;

	menuTransport->Append(ID_TRANSPORT_RECORD,
		wxT("Record &Enable\tAlt-E"),
		wxT("Arm record when play mode initiates"));
	menuTransport->AppendSeparator();
	menuTransport->Append(ID_TRANSPORT_PLAY,
		wxT("&Play\tAlt-P"),
		wxT("Set transport to play mode"));
	menuTransport->Append(ID_TRANSPORT_REWIND,
		wxT("&Rewind\tAlt-R"),
		wxT("Set transport to rewind shuttle mode"));
	menuTransport->Append(ID_TRANSPORT_FORWARD,
		wxT("&Forward\tAlt-F"),
		wxT("Set transport to forward shuttle mode"));
	menuTransport->Append(ID_TRANSPORT_STOP,
		wxT("&Stop\tAlt-S"),
		wxT("Set transport to stop mode"));
	menuTransport->AppendSeparator();
	menuTransport->Append(ID_TRANSPORT_PUNCH,
		wxT("&Punch\t*"),
		wxT("Toggle the tape lifters"));
	menuTransport->Append(ID_TRANSPORT_TOGGLEPLAY,
		wxT("Toggle Play/Stop"),
		wxT("Toggle transport between play and stop"));
	menuTransport->AppendSeparator();
	menuTransport->Append(ID_TRANSPORT_TAPELIFTER,
		wxT("&Tape Lifter\tAlt-L"),
		wxT("Toggle the tape lifters"));

	wxMenu* submenuSpeed = new wxMenu;

	submenuSpeed->AppendCheckItem(ID_TAPESPEED_HIGHSPEED,
        wxT("30 IPS"),
        wxT("Set transport to high speed"));
	submenuSpeed->AppendCheckItem(ID_TAPESPEED_LOWSPEED,
        wxT("15 IPS"),
        wxT("Set transport to low speed"));

	menuTransport->AppendSeparator();

    menuTransport->Append(ID_TRACK_CONFIGURATION, wxT("Tape Speed"), submenuSpeed);

	menuTransport->AppendCheckItem(ID_TRANSPORT_LIBRARYWIND,
		wxT("&Library Wind"),
		wxT("Toggle library wind mode for shuttle commands"));

	// Create LOCATE menu items

	wxMenu* menuLocate = new wxMenu;
	wxMenu* submenuSearch = new wxMenu;

	submenuSearch->Append(ID_CUEPOINT_LOC0, wxT("LOC &0"));
	submenuSearch->Append(ID_CUEPOINT_LOC1, wxT("LOC &1"));
	submenuSearch->Append(ID_CUEPOINT_LOC2, wxT("LOC &2"));
	submenuSearch->Append(ID_CUEPOINT_LOC3, wxT("LOC &3"));
	submenuSearch->Append(ID_CUEPOINT_LOC4, wxT("LOC &4"));
	submenuSearch->Append(ID_CUEPOINT_LOC5, wxT("LOC &5"));
	submenuSearch->Append(ID_CUEPOINT_LOC6, wxT("LOC &6"));
	submenuSearch->Append(ID_CUEPOINT_LOC7, wxT("LOC &7"));
	submenuSearch->Append(ID_CUEPOINT_LOC8, wxT("LOC &8"));
	submenuSearch->Append(ID_CUEPOINT_LOC9, wxT("LOC &9"));
	submenuSearch->AppendSeparator();
	submenuSearch->Append(ID_SEARCH_MARKIN, wxT("Mark In"));
	submenuSearch->Append(ID_SEARCH_MARKOUT, wxT("Mark Out"));
	submenuSearch->AppendSeparator();
	submenuSearch->Append(ID_SEARCH_PUNCHIN, wxT("Punch In"));
	submenuSearch->Append(ID_SEARCH_PUNCHOUT, wxT("Punch Out"));
	submenuSearch->AppendSeparator();
	submenuSearch->Append(ID_SEARCH_ZERORETURN, wxT("Zero Return"));

	menuLocate->Append(ID_LOCATOR_CUEMODE, wxT("&Cue Mode"));
	menuLocate->Append(ID_LOCATOR_STOREMODE, wxT("&Store Mode"));
	menuLocate->AppendSeparator();
	menuLocate->Append(ID_LOCATOR_SEARCH, wxT("Search"), submenuSearch);
	menuLocate->Append(ID_LOCATOR_CANCELSEARCH, wxT("Cancel Search"));
	menuLocate->AppendSeparator();
	menuLocate->Append(ID_LOCATOR_AUTOLOOP, wxT("Auto-Loop"));
	menuLocate->Append(ID_LOCATOR_MARKIN, wxT("Mark In"));
	menuLocate->Append(ID_LOCATOR_MARKOUT, wxT("Mark Out"));
	menuLocate->AppendSeparator();
	menuLocate->Append(ID_LOCATOR_AUTOPUNCH, wxT("Auto-Punch"));
	menuLocate->Append(ID_LOCATOR_PUNCHIN, wxT("Punch In"));
	menuLocate->Append(ID_LOCATOR_PUNCHOUT, wxT("Punch Out"));
	menuLocate->AppendSeparator();
	menuLocate->Append(ID_LOCATOR_RESETCOUNTER, wxT("&Zero Reset Counter"));
	menuLocate->AppendSeparator();
	menuLocate->AppendCheckItem(ID_LOCATOR_AUTOPLAY, wxT("Auto Play"));

	// Create MEMORY menu items

	wxMenu* menuMemory = new wxMenu;
	wxMenu* submenuEdit = new wxMenu;

	submenuEdit->Append(ID_EDITTIME_LOC0, wxT("LOC &0"));
	submenuEdit->Append(ID_EDITTIME_LOC1, wxT("LOC &1"));
	submenuEdit->Append(ID_EDITTIME_LOC2, wxT("LOC &2"));
	submenuEdit->Append(ID_EDITTIME_LOC3, wxT("LOC &3"));
	submenuEdit->Append(ID_EDITTIME_LOC4, wxT("LOC &4"));
	submenuEdit->Append(ID_EDITTIME_LOC5, wxT("LOC &5"));
	submenuEdit->Append(ID_EDITTIME_LOC6, wxT("LOC &6"));
	submenuEdit->Append(ID_EDITTIME_LOC7, wxT("LOC &7"));
	submenuEdit->Append(ID_EDITTIME_LOC8, wxT("LOC &8"));
	submenuEdit->Append(ID_EDITTIME_LOC9, wxT("LOC &9"));
	submenuEdit->AppendSeparator();
	submenuEdit->Append(ID_EDITTIME_MARKIN, wxT("Mark In"));
	submenuEdit->Append(ID_EDITTIME_MARKOUT, wxT("Mark Out"));
	submenuEdit->AppendSeparator();
	submenuEdit->Append(ID_EDITTIME_PUNCHIN, wxT("Punch In"));
	submenuEdit->Append(ID_EDITTIME_PUNCHOUT, wxT("Punch Out"));
	submenuEdit->AppendSeparator();
	submenuEdit->Append(ID_EDITTIME_RTZ, wxT("Zero Return"));

	menuMemory->Append(ID_MEMORY, wxT("Edit Time"), submenuEdit);
	menuMemory->AppendSeparator();
	menuMemory->Append(ID_MEMORY_CLEARALL, wxT("Clear &All"));
	menuMemory->AppendSeparator();
	menuMemory->Append(ID_MEMORY_SAVETODISK, wxT("Save to disk..."));
	menuMemory->Append(ID_MEMORY_LOADFROMDISK, wxT("Load from disk..."));

	// Create SMPTE menu items

	wxMenu* menuSMPTE = new wxMenu;
    wxMenu* submenuGenerator = new wxMenu;

	submenuGenerator->Append(ID_GENERATOR_STOP, wxT("Stop"));
	submenuGenerator->Append(ID_GENERATOR_START, wxT("Start"));
	submenuGenerator->Append(ID_GENERATOR_RESUME, wxT("Resume"));
    submenuGenerator->AppendSeparator();
	submenuGenerator->Append(ID_GENERATOR_EDITTIME, wxT("Edit Start Time..."));

	menuSMPTE->Append(ID_SOURCE_INPUT, wxT("Generator"), submenuGenerator);

	// Create VIEW menu items

	wxMenu* menuView = new wxMenu;

	menuView->AppendCheckItem(ID_VIEW_SHOWTRACKS,
		wxT("&Track Window"),
		wxT("Hide/Show Track Assign Window"));

	// Create HELP menu items

	wxMenu* helpMenu = new wxMenu;

	helpMenu->Append(ID_ABOUT,
		wxT("About"),
		wxT("Display program information"));

	// Now add the menu bar items

	wxMenuBar* menuBar = new wxMenuBar;

	menuBar->Append(m_menuConnect, wxT("&Connect"));
	menuBar->Append(menuTrack, wxT("T&rack"));
	menuBar->Append(menuTransport, wxT("&Transport"));
	menuBar->Append(menuLocate, wxT("&Locator"));
	menuBar->Append(menuMemory, wxT("&Memory"));
    menuBar->Append(menuSMPTE, wxT("&SMPTE"));
	menuBar->Append(menuView, wxT("&View"));
	menuBar->Append(helpMenu, wxT("&Help"));

	// Associate the menu bar with the frame
	SetMenuBar(menuBar);
}

