#ifndef EDITTAPETIMEDLG_H
#define EDITTAPETIMEDLG_H

#include <wx/wx.h>

class CEditTapeTimeDlg : public wxDialog
{
public:
    CEditTapeTimeDlg(wxWindow* parent);

    virtual bool TransferDataToWindow() wxOVERRIDE;

    void UpdateGroupboxCaption();

    // Dialog data (same semantics as the MFC members).
    // Set these before calling ShowModal().
    int  m_nMemIndex;
    int  m_nHour;
    int  m_nMins;
    int  m_nSecs;
    int  m_nTens;
    bool m_bActive;

private:
    wxStaticBoxSizer* m_groupSizer;
    wxTextCtrl* m_ctlEdit[4];   // hour, mins, secs, tens

    int  GetFieldValue(int idx) const;
    void StepField(int idx, int delta);
};

#endif // EDITTAPETIMEDLG_H
