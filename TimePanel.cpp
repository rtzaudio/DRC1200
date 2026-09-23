#include "DRC1200.h"

#define DRAW_BUFFERED	        1
#define SHOW_TIMEDATE           1
#define SHOW_ENCODER_ERRORS     0

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

    wxString err;
	wxString str;
	wxColor colorText;

	wxSize size = GetClientSize();

	wxCoord center = (size.GetHeight() >> 1) + dc.FromDIP(10);

	// Create the various font sizes we'll need
	wxFont mono1(wxFontInfo(size.GetHeight() / 3).Family(wxFONTFAMILY_TELETYPE).Bold());
	wxFont mono2(wxFontInfo(size.GetHeight() / 8).Family(wxFONTFAMILY_TELETYPE).Bold());
    wxFont mono3(wxFontInfo(size.GetHeight() / 20).Family(wxFONTFAMILY_TELETYPE).Bold());
    wxFont mono4(wxFontInfo(size.GetHeight() / 12).Family(wxFONTFAMILY_TELETYPE).Bold());

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
    xpos += (width >> 1);

    xpos += (sizeText.GetWidth() >>  1);

    dc.DrawText(wxT("TEN"), xpos - 3, ypos);

    // ------------------------------------------------------
    // Now draw the extra mode display stuff
	// ------------------------------------------------------

   	dc.SetTextForeground(colorText);
	dc.SetFont(mono3);

	wxCoord xSpace = (wxCoord)((dc.GetCharWidth() * 4));

    str.Printf(wxT("%u-IPS %u\" %u-TRK"),
		state.tapeSpeed,
		state.tapeSize,
		state.trackCount);

    sizeText = dc.GetTextExtent(str);
    xpos = dc.FromDIP(15);
    ypos = sizeText.GetHeight() + dc.FromDIP(1);
	dc.DrawText(str, xpos, ypos);
    xpos += sizeText.GetWidth() + xSpace;

	if (state.hardwareFlags & STC_HF_SMPTE)
	{
	    wxString strMode;

		if (state.smpteMode == 1)
			strMode = _T("STRIPE");
		else if (state.smpteMode == 2)
			strMode = _T("SLAVE");
		else
			strMode = _T("OFF");

        str.Printf(wxT("SMPTE ") + strMode);
        sizeText = dc.GetTextExtent(str);
        dc.DrawText(str, xpos, ypos);
        xpos += sizeText.GetWidth() + xSpace;
	}

	// Display current transport mode string

    dc.SetFont(mono4);

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

    sizeText = dc.GetTextExtent(strMode);

    xpos = size.GetWidth() - (sizeText.GetWidth() + dc.FromDIP(10));

	dc.DrawText(strMode, xpos, ypos >> 1);

	// ------------------------------------------------------
	// Format and draw SMPTE time string as +h:mm:ss:fn
	// ------------------------------------------------------

	dc.SetTextForeground(colorText);

	if (state.hardwareFlags & STC_HF_SMPTE)
	{
        dc.SetFont(mono4);

		str.Printf(wxT("T/C %2.2u:%2.2u:%2.2u:%2.2u"),
			state.smpteTime.hour,
			state.smpteTime.mins,
			state.smpteTime.secs,
			state.smpteTime.frame);

        sizeText = dc.GetTextExtent(str);

		xpos = (size.GetWidth() >> 1) - (sizeText.GetWidth() >> 1);
        ypos = size.GetHeight() - (dc.GetCharHeight() + dc.FromDIP(10));

		dc.DrawText(str, xpos, ypos);
	}

    // ------------------------------------------------------
	// Draw the date and time from the machine
	// ------------------------------------------------------

#if (SHOW_TIMEDATE > 0)
    wxString buf;
    wxString ampm;

    dc.SetFont(mono3);

    uint8_t hour = state.dateTime.hour;

    ampm = wxT("AM");

    // Convert to 12 hour format
    if (hour >= 12)
    {
        ampm = wxT("PM");
        hour -= 12;
    }

    // Adjust if midnight hour.
    if (hour == 0)
        hour = 12;

    // Draw the time from the tape machine

    buf.Printf(wxT("%1u:%-2.2u %s"),
        hour,
        state.dateTime.min,
        ampm);

    sizeText = dc.GetTextExtent(buf);

    x = dc.FromDIP(10);
    y = size.GetHeight() - (sizeText.GetHeight() + dc.FromDIP(10));

    dc.DrawText(buf, x, y);

    // Draw the date from the tape machine

    buf.Printf(wxT("%u/%u/%4.4u"),
        state.dateTime.month + 1,
        state.dateTime.date + 1,
        state.dateTime.year + 2000);

    sizeText = dc.GetTextExtent(buf);

    x = size.GetWidth() - (sizeText.GetWidth() + dc.FromDIP(10));

    dc.DrawText(buf, x, y);
#endif

    // ------------------------------------------------------
	// Overlay any encoder error count over the tape time
	// ------------------------------------------------------

#if (SHOW_ENCODER_ERRORS > 0)
    if (state.errorCount > 0)
	{
        dc.SetFont(mono4);

        str.Printf(wxT("ENCODER ERRORS %u"), state.errorCount);

        sizeText = dc.GetTextExtent(str);

		xpos = (size.GetWidth() >> 1) - (sizeText.GetWidth() >> 1);
        ypos = (size.GetHeight() >> 1) - (sizeText.GetHeight() >> 2);

        dc.SetTextForeground(wxGetApp().m_colorError);

        dc.DrawText(str, xpos, ypos);
	}
#endif
}


