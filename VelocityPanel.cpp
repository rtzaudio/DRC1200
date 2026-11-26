#include "DRC1200.h"

#include <memory>
#include <cmath>
#include <algorithm>
#include <wx/dcbuffer.h>
#include <wx/graphics.h>

#define DRAW_BUFFERED	1

// Alpha (0-255) of the velocity plot line. Lower = more transparent.
#define PLOT_ALPHA		0x60

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
	// Set background style to PAINT to prevent default erasing
	SetBackgroundStyle(wxBG_STYLE_PAINT);
#endif
	m_colorPanel = wxColour(0x44, 0x44, 0x44);
	m_colorGrid  = wxColour(0x70, 0x70, 0x70);
	m_colorPlot  = wxColour(0x3C, 0xB3, 0x71, PLOT_ALPHA);	// medium sea green w/ alpha
	m_colorLine  = wxColour(0x00, 0x00, 0x00);
}

void VelocityPanel::OnSize(wxSizeEvent& event)
{
	Refresh();
	Layout();
}

void VelocityPanel::OnPaint(wxPaintEvent& event)
{
#if DRAW_BUFFERED > 0
	wxAutoBufferedPaintDC dc(this);
#else
	wxPaintDC dc(this);
#endif
	PrepareDC(dc);

	// The graphics context must be destroyed before the DC so that
	// everything is flushed into the (buffered) DC. unique_ptr declared
	// after dc guarantees that order.
	std::unique_ptr<wxGraphicsContext> gc(wxGraphicsContext::Create(dc));

	if (gc)
	{
		gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
		gc->SetInterpolationQuality(wxINTERPOLATION_DEFAULT);
		gc->SetCompositionMode(wxCOMPOSITION_OVER);	// normal alpha blending

		Draw(gc.get());
	}

	event.Skip();
}

void VelocityPanel::OnEraseBackground(wxEraseEvent& event)
{
}

void VelocityPanel::Draw(wxGraphicsContext* gc)
{
	wxSize size = GetClientSize();

	MainFrame* mainframe = wxGetApp().GetMainFrame();

	// Opaque panel background
	gc->SetPen(*wxTRANSPARENT_PEN);
	gc->SetBrush(wxBrush(m_colorPanel));
	gc->DrawRectangle(0, 0, size.GetWidth(), size.GetHeight());

	DrawGrid(gc);

	if (mainframe->IsConnected())
		PlotVelocity(gc);
}

void VelocityPanel::DrawGrid(wxGraphicsContext* gc)
{
	wxSize size = GetClientSize();

	const double h = size.GetHeight();
	const double w = size.GetWidth();

	gc->SetPen(wxPen(m_colorGrid, 1));
	gc->SetBrush(*wxTRANSPARENT_BRUSH);

	// Snap lines to pixel centers (+0.5) so 1px lines stay crisp
	// with antialiasing enabled.

	wxGraphicsPath path = gc->CreatePath();

	// outer border
	path.AddRectangle(0.5, 0.5, w - 1.0, h - 1.0);

	// vertical grid lines
	for (int i = 0; i < 10; i++)
	{
		double x = std::floor((w / 11.0) * (i + 1)) + 0.5;
		path.MoveToPoint(x, 0.0);
		path.AddLineToPoint(x, h);
	}

	// horizontal grid lines
	for (int i = 0; i < 3; i++)
	{
		double y = std::floor((h / 4.0) * (i + 1)) + 0.5;
		path.MoveToPoint(0.0, y);
		path.AddLineToPoint(w, y);
	}

	gc->StrokePath(path);
}

void VelocityPanel::PlotVelocity(wxGraphicsContext* gc)
{
	MainFrame* mainframe = wxGetApp().GetMainFrame();

	STC_STATE_MSG& state = mainframe->GetTransportState();

	if (state.tapeVelocity == 0)
		return;

	wxRect rect = GetClientRect();

	// Inset by one pixel so we don't overwrite the grid border

	rect.x += 1;
	rect.y += 1;
	rect.width -= 2;
	rect.height -= 2;

	const double step    = (double)rect.width / (double)MAX_VEL_SAMPLES;
	const double yBottom = (double)rect.GetBottom() + 1.0;
	const double yTop    = (double)rect.GetTop();
	const double yRange  = (yBottom - 1.0) - yTop;

	// Build every velocity spike into ONE path and stroke it once. A single
	// stroke is blended as a unit, so neighboring/overlapping spikes do not
	// double-darken where they touch. Pen width equals the sample pitch
	// (min 1px) with butt caps so the spikes tile into a continuous plot.

	wxGraphicsPath path = gc->CreatePath();

	for (size_t i = 0; i < MAX_VEL_SAMPLES; i++)
	{
		float v = mainframe->GetVelocitySample(i);

		if (v <= 0.0f)
			continue;

		if (v > 100.0f)
			v = 100.0f;

		const double x = rect.GetLeft() + ((double)i + 0.5) * step;
		const double y = (yBottom - 1.0) - (yRange * (double)v * 0.01);

		path.MoveToPoint(x, yBottom);
		path.AddLineToPoint(x, y);
	}

	wxGraphicsPenInfo penInfo(m_colorPlot);
	penInfo.Width(std::max(step, 1.0)).Cap(wxCAP_BUTT);

	gc->SetPen(gc->CreatePen(penInfo));
	gc->SetBrush(*wxTRANSPARENT_BRUSH);
	gc->StrokePath(path);

	// Now draw black line indicating search progress. (Drawn once, after the
	// plot, rather than once per sample inside the loop.)

	if (state.searching)
	{
		const double x1 = rect.GetLeft();
		const double x2 = rect.GetRight();

		double progress = (double)state.searchProgress * 0.01;
		double x = ((x2 - x1) * progress) + x1;

		if (x > x2)
			x = x2;

		if (x < x1)
			x = x1;

		gc->SetPen(gc->CreatePen(wxGraphicsPenInfo(m_colorLine).Width(3.0)));
		gc->StrokeLine(x, rect.GetTop(), x, rect.GetBottom());
	}

	// Display any quadrature encoder errors.

	if (state.errorCount > 0)
	{
		const wxString line1 = wxT("ENCODER");
		const wxString line2 = wxString::Format(wxT("ERRORS %u"), (unsigned)state.errorCount);

		gc->SetFont(GetFont(), wxGetApp().m_colorError);

		double w1, h1, w2, h2;
		gc->GetTextExtent(line1, &w1, &h1);
		gc->GetTextExtent(line2, &w2, &h2);

		const double cx = rect.GetWidth() / 2.0;
		const double y0 = (rect.GetHeight() - (h1 + h2)) / 2.0;

		gc->DrawText(line1, cx - w1 / 2.0, y0);
		gc->DrawText(line2, cx - w2 / 2.0, y0 + h1);
	}
}
