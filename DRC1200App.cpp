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

	// Generic colors
	m_colorBlack.SetRGB(0x000000);
	m_colorWhite.SetRGB(0xFFFFFF);

	// Frame, panel and basic text colors
	m_colorFrame.SetRGB(0x484848);				// panel frame color
	m_colorPanel.SetRGB(0x333333);				// background panel color
	m_colorText.SetRGB(0xE0E0E0);

	m_colorTimeBkgnd = *wxBLACK;
	m_colorTime = *wxYELLOW;
	m_colorError = *wxRED;

	// Button text colors
	m_colorBtnText.SetRGB(0xFFFFFF);
	m_colorBtnTextDim.SetRGB(0x222222);
	m_colorBtnTextActive.SetRGB(0xFFFFFF);

	// Button face colors
	m_colorBtnFace.SetRGB(0x585858);
	m_colorBtnGrey.SetRGB(0xC9C9C9);
	m_colorBtnDark.SetRGB(0x484848);
	m_colorBtnActive.SetRGB(0x54A835);
	m_colorBtnRecReady.SetRGB(0x000055);
	m_colorBtnRecActive.SetRGB(0x0000FF);
	m_colorBtnModeInput.SetRGB(0x04B2FE);
	m_colorBtnModeRepro.SetRGB(0x54A835);
	m_colorBtnModeSync.SetRGB(0xEEA200);
	m_colorBtnModeMon.SetRGB(0xA0A0A0);
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

    // Load formats like PNG or JPEG directly from a file
    wxInitAllImageHandlers();

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
