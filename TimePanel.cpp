#include "DRC1200.h"

#define DRAW_BUFFERED	1

// Standard DC supports drawing with alpha on OSX and GTK3.
#if defined(__WXOSX__) || defined(__WXGTK3__)
#define wxDRAWING_DC_SUPPORTS_ALPHA 1
#else
#define wxDRAWING_DC_SUPPORTS_ALPHA 0
#endif // __WXOSX__ || __WXGTK3__

wxBEGIN_EVENT_TABLE(TimePanel, wxPanel)
EVT_PAINT(TimePanel::OnPaint)
EVT_ERASE_BACKGROUND(TimePanel::OnEraseBackground)
EVT_SIZE(TimePanel::OnSize)
wxEND_EVENT_TABLE()

TimePanel::TimePanel(wxPanel* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE)
{
	m_owner = parent;

	wxGetApp().m_panelTime = this;

#if DRAW_BUFFERED > 0
    SetBackgroundStyle(wxBG_STYLE_PAINT);
#endif
}

void TimePanel::OnSize(wxSizeEvent& event)
{
	Refresh();
	Layout();
}

void TimePanel::OnPaint(wxPaintEvent& event)
{
#if DRAW_BUFFERED > 0
    wxAutoBufferedPaintDC dc(this);
#else
	wxPaintDC dc(this);
#endif
    PrepareDC(dc);
	Draw(dc);

	event.Skip();
}

void TimePanel::OnEraseBackground(wxEraseEvent& event)
{
}

void TimePanel::Draw(wxDC& dc)
{
	MainFrame* mainframe = wxGetApp().GetMainFrame();

	STC_STATE_MSG& state = mainframe->GetTransportState();

	wxString str;
	wxColor colorText;
	wxSize size = GetClientSize();
	wxCoord center = size.GetHeight() >> 1;

	// Create the various font sizes we'll need
	wxFont mono1(wxFontInfo(size.GetHeight() >> 1).Family(wxFONTFAMILY_TELETYPE).Bold());
	wxFont mono2(wxFontInfo(size.GetHeight() >> 2).Family(wxFONTFAMILY_TELETYPE).Bold());
    wxFont mono3(wxFontInfo(size.GetHeight() / 10).Family(wxFONTFAMILY_TELETYPE).Bold());

    // Fill the background with black

	dc.SetPen(*wxBLACK_PEN);
	dc.SetBrush(wxGetApp().m_colorTimeBkgnd);
	dc.DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());

	// ------------------------------------------------------
	// Format & draw signed tape time as +HH:MM:SS:TENS
	// ------------------------------------------------------

	if (mainframe->IsConnected())
		colorText = wxGetApp().m_colorTime;
	else
		colorText = wxGetApp().m_colorBtnTextDim;

	char sign = (state.tapeTime.flags & F_TAPETIME_PLUS) ? wxT('+') : wxT('-');

	str.Printf(wxT("%c%1u:%2.2u:%2.2u"),
		sign,
		state.tapeTime.hour,
		state.tapeTime.mins,
		state.tapeTime.secs);

	// Setup the large mono font to draw the tape time with

	dc.SetFont(mono1);
	dc.SetTextForeground(colorText);

	// Draw the tape time vertically centered

	wxSize sizeText = dc.GetTextExtent(str);

	wxCoord width = dc.GetCharWidth();

	wxCoord x = dc.FromDIP(10);
	wxCoord y = center - (sizeText.GetHeight() >> 1);

	dc.DrawText(str, x, y);

    // Append tens after seconds in smaller font

	str.Printf(wxT(":%1u"), state.tapeTime.tens);
	dc.SetTextForeground(colorText);
	dc.SetFont(mono2);

	wxCoord yTop = y + (wxCoord)((dc.GetCharHeight() * 0.33));

	dc.DrawText(str, 15 + sizeText.GetWidth(), yTop);

	// ------------------------------------------------------
	// Draw the headings hours, mins, secs and tens
	// ------------------------------------------------------

    dc.SetTextForeground(colorText);
	dc.SetFont(mono3);

	sizeText = dc.GetTextExtent(str);

	wxCoord xpos = width + (width >> 1) + 4;
	wxCoord ypos = yTop - (sizeText.GetHeight() + 5);

	dc.DrawText(wxT("HR"), xpos, ypos);

    xpos += width;
    xpos += width;
    xpos += (width >> 1);

    dc.DrawText(wxT("MIN"), xpos, ypos);

    xpos += width;
    xpos += width;
    xpos += width;

    dc.DrawText(wxT("SEC"), xpos, ypos);

    xpos += width;
    xpos += (width / 2);

    xpos += (sizeText.GetWidth() >>  1);

    dc.DrawText(wxT("TEN"), xpos, ypos);

    // ------------------------------------------------------
    // Now draw the extra mode display stuff
	// ------------------------------------------------------

    wxCoord vspace = sizeText.GetHeight() + dc.FromDIP(1);

	xpos += (sizeText.GetWidth() << 1) + sizeText.GetWidth();

	// Get current transport mode string
    wxString strMode;
    mainframe->GetModeText(state, strMode);

    if (mainframe->IsConnected())
    {
        if (mainframe->IsTransportModeFlags(STC_M_RECORD) ||
            mainframe->IsTransportMode(STC_MODE_HALT))
        {
            dc.SetTextForeground(wxGetApp().m_colorError);
        }
    }

	dc.DrawText(strMode, xpos, ypos);

	dc.SetTextForeground(colorText);

	ypos += vspace;

	if (state.hardwareFlags & STC_HF_SMPTE)
	{
		if (state.smpteMode == 1)
			strMode = _T("STRIPE");
		else if (state.smpteMode == 2)
			strMode = _T("SLAVE");
		else
			strMode = _T("OFF");

        str.Printf(wxT("SMPTE ") + strMode);
        dc.DrawText(str, xpos, ypos);
	}

    ypos += vspace;

    str.Printf(wxT("SPEED %u IPS"), state.tapeSpeed);
	dc.DrawText(str, xpos, ypos);

    ypos += vspace;

    str.Printf(wxT("TAPE %u\" %u-TRK"), state.tapeSize, state.trackCount);
	dc.DrawText(str, xpos, ypos);

    ypos += vspace;

    if (state.errorCount)
    {
		if (mainframe->IsConnected())
			colorText = wxGetApp().m_colorError;
		else
			colorText = wxGetApp().m_colorBtnTextDim;

        dc.SetTextForeground(colorText);

        str.Printf(wxT("QE ERRORS %u"), state.errorCount);
        dc.DrawText(str, xpos, ypos);
    }
}
