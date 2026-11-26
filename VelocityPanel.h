#pragma once

#define MAX_VEL_SAMPLES	500

class wxGraphicsContext;

///////////////////////////////////////////////////////////////////////////////
// Sub panels of tape transport panel

class VelocityPanel : public wxPanel
{
public:
	VelocityPanel(wxPanel* parent);

	void OnPaint(wxPaintEvent& WXUNUSED(event));
	void OnEraseBackground(wxEraseEvent& event);
	void OnSize(wxSizeEvent& event);
	void Draw(wxGraphicsContext* gc);
	void DrawGrid(wxGraphicsContext* gc);
	void PlotVelocity(wxGraphicsContext* gc);

private:
	wxPanel* m_owner;
	wxColor m_colorPanel;
	wxColor m_colorGrid;
	wxColor m_colorPlot;
	wxColor m_colorLine;

	wxDECLARE_EVENT_TABLE();
};

// End-Of-File
