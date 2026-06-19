#pragma once

#define MAX_VEL_SAMPLES	500

///////////////////////////////////////////////////////////////////////////////
// Sub panels of tape transport panel

class VelocityPanel : public wxPanel
{
public:
	VelocityPanel(wxPanel* parent);

	void OnPaint(wxPaintEvent& WXUNUSED(event));
	void OnEraseBackground(wxEraseEvent& event);
	void OnSize(wxSizeEvent& event);
	void Draw(wxDC& dc);
	void DrawGrid(wxDC& dc);
	void PlotVelocity(wxDC& dc);

private:
	wxPanel* m_owner;
	wxColor m_colorPanel;
	wxColor m_colorGrid;
	wxColor m_colorPlot;
	wxColor m_colorLine;

	wxDECLARE_EVENT_TABLE();
};

// End-Of-File
