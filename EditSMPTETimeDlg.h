#ifndef EDITSMPTETIMEDLG_H
#define EDITSMPTETIMEDLG_H

#include <wx/wx.h>

class CEditSMPTETimeDlg : public wxDialog
{
public:
    CEditSMPTETimeDlg(wxWindow* parent);

    void UpdateGroupboxCaption();

    // Dialog data (same semantics as the MFC members)
    int m_nHour;
    int m_nMins;
    int m_nSecs;
    int m_nFrame;

private:
    enum { ID_BUTTON_ZERO = wxID_HIGHEST + 1 };

    wxStaticBoxSizer* m_groupSizer;
    wxTextCtrl* m_ctlEdit[4];   // hour, mins, secs, frame

    int  GetFieldValue(int idx) const;
    void StepField(int idx, int delta);

    void OnButtonZero(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

#endif // EDITSMPTETIMEDLG_H
