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

void TimePanel::OnPaint(wxPaintEvent& WXUNUSED(event))
{
#if DRAW_BUFFERED > 0
    wxAutoBufferedPaintDC dc(this);
#else
	wxPaintDC dc(this);
#endif
    PrepareDC(dc);
	Draw(dc);
}

void TimePanel::OnEraseBackground(wxEraseEvent& event)
{
}

void TimePanel::Draw(wxDC& dc)
{
	wxSize size = GetClientSize();

	MainFrame* mainframe = wxGetApp().GetMainFrame();

	STC_STATE_MSG& state = mainframe->GetTransportState();

	wxFont mono1(wxFontInfo(size.GetHeight() >> 1).Family(wxFONTFAMILY_TELETYPE).Bold());
	wxFont mono2(wxFontInfo(size.GetHeight() >> 2).Family(wxFONTFAMILY_TELETYPE).Bold());
    wxFont mono3(wxFontInfo(size.GetHeight() / 10).Family(wxFONTFAMILY_TELETYPE).Bold());

    // Fill the background with black

	dc.SetPen(*wxBLACK_PEN);
	dc.SetBrush(*wxBLACK_BRUSH);
	dc.DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());

	// Draw the tape time in hh:mm:ss format

	wxColor colorText;

	if (mainframe->IsConnected())
		colorText = wxColor(*wxYELLOW);
	else
		colorText = wxColor(wxT("#333333"));

	char sign = (state.tapeTime.flags & F_TAPETIME_PLUS) ? wxT('+') : wxT('-');

	wxString str;
	str.Printf(wxT("%c%1u:%2.2u:%2.2u"),
		sign,
		state.tapeTime.hour,
		state.tapeTime.mins,
		state.tapeTime.secs);

	dc.SetTextForeground(colorText);
	dc.SetFont(mono1);
	dc.DrawText(str, 10, dc.FromDIP(10));

    wxCoord width = dc.GetCharWidth();

	wxSize sizeText = dc.GetTextExtent(str);

    // Append tens after seconds in smaller font

	str.Printf(wxT(":%1u"), state.tapeTime.tens);
	dc.SetTextForeground(colorText);
	dc.SetFont(mono2);
	dc.DrawText(str, 15 + sizeText.GetWidth(), (sizeText.GetHeight() >> 2) + 5);

	// Draw the headings hours, mins, seconds and tens over time digits

    wxCoord ypos = 5;

    dc.SetTextForeground(colorText);
	dc.SetFont(mono3);

	sizeText = dc.GetTextExtent(str);

	wxCoord xpos = width + (width >> 1) + 4;

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

    xpos += sizeText.GetWidth();

    dc.DrawText(wxT("TEN"), xpos, ypos);

    //
    // Now draw the extra mode display stuff
    //

    wxCoord vspace = sizeText.GetHeight() + dc.FromDIP(1);

    xpos += dc.FromDIP(40);
    //ypos += dc.FromDIP(3);

    wxString strMode;
    mainframe->GetModeText(state, strMode);

    if (mainframe->IsConnected())
    {
        if (mainframe->IsTransportModeFlags(STC_M_RECORD) ||
            mainframe->IsTransportMode(STC_MODE_HALT))
        {
            dc.SetTextForeground(*wxRED);
        }
    }

	dc.DrawText(strMode, xpos, ypos);

	dc.SetTextForeground(colorText);

	ypos += vspace; // + dc.FromDIP(3);

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
        dc.SetTextForeground(*wxRED);

        str.Printf(wxT("ERRORS %u"), state.errorCount);
        dc.DrawText(str, xpos, ypos);
    }
}
