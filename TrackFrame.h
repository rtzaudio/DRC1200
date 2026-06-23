#pragma once

#define STC_MAX_TRACKS	24

class DRC1200App;

class TrackButton : public wxButton
{
public:
    TrackButton(wxPanel* mypanel, int id, const wxString& label);

	void OnTrackButtonClicked(wxCommandEvent& event);
};

class TrackFrame : public wxMiniFrame
{
public:
	TrackFrame(wxFrame* parent);
	~TrackFrame();

    void UpdateTrackButtonStates(void);
    void ResetTrackButtonStates(bool repaint=true);
    void SetTrackConfig(int numtracks);

private:
    void SetTrackButtonStates(void);

public:
	wxBoxSizer* m_hbox;
	//wxGridSizer* m_gs;
    wxPanel* m_midPan;

	TrackButton* m_btnTrackNum[STC_MAX_TRACKS];
	TrackButton* m_btnTrackArm[STC_MAX_TRACKS];
	TrackButton* m_btnTrackMode[STC_MAX_TRACKS];
	TrackButton* m_btnTrackMon[STC_MAX_TRACKS];

    uint8_t m_trackState[STC_MAX_TRACKS];

private:
	wxDECLARE_EVENT_TABLE();

	void OnClose(wxCloseEvent& WXUNUSED(event));
};

