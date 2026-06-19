/***************************************************************
 * Name:      DRC1200App.h
 * Purpose:   Defines Application Class
 * Author:    Robert Starr (rtzaudio@comcast.net)
 * Created:   2019-12-12
 * Copyright: Robert Starr (rtzaudio.com)
 * License:
 **************************************************************/

#include "DRC1200.h"

wxIMPLEMENT_APP(DRC1200App);

///////////////////////////////////////////////////////////////////////////////
// DRC1200App

DRC1200App::DRC1200App()
{
	m_pMainFrame    = nullptr;
	m_panelTime     = nullptr;
	m_panelOption   = nullptr;
	m_panelVelocity = nullptr;
    m_trackCount    = STC_MAX_TRACKS;
    m_dcsFound      = false;
}

DRC1200App::~DRC1200App()
{

}

///////////////////////////////////////////////////////////////////////////////
//

bool DRC1200App::OnInit()
{
    // Create the configuration file manager object

    wxFileName fname(argv[0]);
    wxString ini_filename;

    ini_filename = fname.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR) + _("DRC1200.ini");

	m_pConfig = new wxConfig(_("DRC1200"), wxEmptyString, ini_filename);

    // Read the track count configuration parameter if found, otherwise
    // assume 24-track. This value may get changed later when the user
    // connects to the machine and reads it supported track count.

    m_trackCount = 24;

    m_pConfig->Read(_("NumTracks"), &m_trackCount);

    if (m_trackCount > 24) {
        m_trackCount = 24;
    }

    // If host name was passed on command line at startup,
    // save the name and the mainframe will attempt to connect
    // after initializing the main frame.

    if (argc > 1)
    {
        m_strHostArg = argv[1];
    }

	// Create and show the main application frame window

	m_pMainFrame = new MainFrame();

	m_pMainFrame->Show();

	return true;
}

int DRC1200App::OnExit()
{
    if (m_pConfig)
    {
        m_pConfig->Flush();

        delete m_pConfig;
    }

    return 0;
}

// End-Of-File
