#include "DRC1200.h"

#define DRAW_BUFFERED	1

// Standard DC supports drawing with alpha on OSX and GTK3.
#if defined(__WXOSX__) || defined(__WXGTK3__)
#define wxDRAWING_DC_SUPPORTS_ALPHA 1
#else
#define wxDRAWING_DC_SUPPORTS_ALPHA 0
#endif // __WXOSX__ || __WXGTK3__

wxBEGIN_EVENT_TABLE(VelocityPanel, wxPanel)
EVT_PAINT(VelocityPanel::OnPaint)
EVT_ERASE_BACKGROUND(VelocityPanel::OnEraseBackground)
EVT_SIZE(VelocityPanel::OnSize)
wxEND_EVENT_TABLE()

VelocityPanel::VelocityPanel(wxPanel* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE)
{
	m_owner = parent;

	wxGetApp().m_panelVelocity = this;

#if DRAW_BUFFERED > 0
    // 1. Set background style to PAINT to prevent default erasing
    SetBackgroundStyle(wxBG_STYLE_PAINT);
#endif
	m_colorPanel = wxColor(wxT("#444444"));
	m_colorGrid	 = wxColor(wxT("#707070"));
	m_colorPlot	 = wxColor(wxT("#3CB371"));
    m_colorLine  = wxColor(wxT("#000000"));

    m_colorPlot.SetRGBA(0x2071B33C);
}

void VelocityPanel::OnSize(wxSizeEvent& event)
{
	Refresh();
	Layout();
}

void VelocityPanel::OnPaint(wxPaintEvent& event)
{
#if DRAW_BUFFERED > 0
    // 2. Create the DC. It automatically handles buffering.
    wxAutoBufferedPaintDC dc(this);
#else
	wxPaintDC dc(this);
#endif
	PrepareDC(dc);
	Draw(dc);

	event.Skip();
}

void VelocityPanel::OnEraseBackground(wxEraseEvent& event)
{
}

void VelocityPanel::Draw(wxDC& dc)
{
	wxSize size = GetClientSize();

	MainFrame* mainframe = wxGetApp().GetMainFrame();

	wxBrush brush(m_colorPanel);

	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.SetBrush(brush);
	dc.DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());

	DrawGrid(dc);

	if (mainframe->IsConnected())
        PlotVelocity(dc);
}

void VelocityPanel::DrawGrid(wxDC& dc)
{
	wxSize size = GetClientSize();

	wxPen pen(m_colorGrid);

    dc.SetPen(pen);
	dc.SetBrush(*wxTRANSPARENT_BRUSH);

	dc.DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());

	float h = size.GetHeight();
	float w = size.GetWidth();

	// draw vertical grid lines
	for (int i = 0; i < 10; i++)
	{
		int x = (w / 11) * (i + 1);
		dc.DrawLine(x, 0, x, h);
	}

	// draw horizontal grid lines
	for (int i = 0; i < 3; i++)
	{
		int y = (h / 4) * (i + 1);
		dc.DrawLine(0, y, w, y);
	}
}

void VelocityPanel::PlotVelocity(wxDC& dc)
{
	MainFrame* mainframe = wxGetApp().GetMainFrame();

	STC_STATE_MSG& state = mainframe->GetTransportState();

	if (state.tapeVelocity == 0)
        return;

    //OptionPanel* optpanel = mainframe->GetOptionPanel();
    //if (optpanel != nullptr)
    //{
        //optpanel->SetProgress(state.searchProgress);
    //}

	wxRect rect = GetClientRect();

	// Now plot the velocity graph data

	rect.x += 1;
	rect.y += 1;
	rect.width -= 2;
	rect.height -= 2;

	float step = (float)rect.width / (float)MAX_VEL_SAMPLES;

    wxPen penPlot(m_colorPlot);
    wxPen penLine(m_colorLine, 3);

    dc.SetBrush(*wxTRANSPARENT_BRUSH);

	for (size_t i = 0; i < MAX_VEL_SAMPLES; i++)
	{
		float x = (float)i * step;

		float v = mainframe->GetVelocitySample(i);

		if (v <= 0.0f)
			continue;

		if (v > 100.0f)
			v = 100.0f;

		float y2 = (float)rect.GetTop();
		float y1 = (float)rect.GetBottom() - 1;

		float val = (float)v * 0.01f;

		float y = ((y2 - y1) * val) + y1;

        wxPoint pt1(rect.GetLeft() + x, rect.GetBottom());
        wxPoint pt2(rect.GetLeft() + x, y);

        dc.SetPen(penPlot);
		dc.DrawLine(pt1, pt2);

		// Now draw black line indicating progress

		if (state.searching)
		{
			int x1 = rect.GetLeft();
			int x2 = rect.GetRight();

			float progress = (float)state.searchProgress * 0.01f;

			float x = ((x2 - x1) * progress) + x1;

			if (x > x2)
				x = x2;

			if (x < x1)
				x = x1;

            dc.SetPen(penLine);
            dc.DrawLine(x, rect.GetTop(), x, rect.GetBottom());
		}
	}
}
