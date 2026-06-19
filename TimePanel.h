#pragma once

///////////////////////////////////////////////////////////////////////////////
// Sub panels of tape transport panel

class TimePanel : public wxPanel
{
public:
	TimePanel(wxPanel* parent);

	void OnPaint(wxPaintEvent& WXUNUSED(event));
	void OnEraseBackground(wxEraseEvent& event);
	void OnSize(wxSizeEvent& event);
	void Draw(wxDC& dc);

private:
	wxPanel* m_owner;

	wxDECLARE_EVENT_TABLE();
};

// End-Of-File