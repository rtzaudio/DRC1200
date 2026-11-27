#include "EditSMPTETimeDlg.h"
#include <wx/valnum.h>

wxBEGIN_EVENT_TABLE(CEditSMPTETimeDlg, wxDialog)
    EVT_BUTTON(ID_BUTTON_ZERO, CEditSMPTETimeDlg::OnButtonZero)
wxEND_EVENT_TABLE()

CEditSMPTETimeDlg::CEditSMPTETimeDlg(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, wxT("SMPTE Time"),
               wxDefaultPosition, wxDefaultSize,
               wxDEFAULT_DIALOG_STYLE)
    , m_nHour(0)
    , m_nMins(0)
    , m_nSecs(0)
    , m_nFrame(0)
{
    struct Field { const wxChar* label; int* value; int maxVal; };
    const Field fields[4] =
    {
        { wxT("Hour"),  &m_nHour,  23 },
        { wxT("Mins"),  &m_nMins,  59 },
        { wxT("Secs"),  &m_nSecs,  59 },
        { wxT("Frame"), &m_nFrame, 29 }
    };

    m_groupSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("0:00:00.00"));
    wxStaticBox* box = m_groupSizer->GetStaticBox();

    wxFlexGridSizer* grid = new wxFlexGridSizer(2, 4, 4, 8);

    // Row 1: edit + up/down arrow buttons (like MFC buddy pair)
    for (int i = 0; i < 4; i++)
    {
        const int maxVal = fields[i].maxVal;

        wxTextCtrl* edit = new wxTextCtrl(box, wxID_ANY, wxT("0"),
                                wxDefaultPosition, FromDIP(wxSize(40, -1)),
                                wxTE_RIGHT);

        // DDX/DDV equivalent: transfers to the int member and range-checks
        wxIntegerValidator<int> val(fields[i].value);
        val.SetRange(0, maxVal);
        edit->SetValidator(val);

        m_ctlEdit[i] = edit;

        // Typed/programmatic text -> caption
        edit->Bind(wxEVT_TEXT, [this](wxCommandEvent& e)
        {
            UpdateGroupboxCaption();
            e.Skip();
        });

        // Up/down arrow buttons (stand-in for the MFC up-down control)
        const wxSize btnSize(FromDIP(wxSize(20, 14)));
        wxButton* up   = new wxButton(box, wxID_ANY, wxT("\u25B2"),
                                wxDefaultPosition, btnSize, wxBU_EXACTFIT);
        wxButton* down = new wxButton(box, wxID_ANY, wxT("\u25BC"),
                                wxDefaultPosition, btnSize, wxBU_EXACTFIT);
        up->SetFont(up->GetFont().Smaller());
        down->SetFont(down->GetFont().Smaller());

        up->Bind(wxEVT_BUTTON,   [this, i](wxCommandEvent&) { StepField(i, +1); });
        down->Bind(wxEVT_BUTTON, [this, i](wxCommandEvent&) { StepField(i, -1); });

        wxBoxSizer* arrows = new wxBoxSizer(wxVERTICAL);
        arrows->Add(up);
        arrows->Add(down);

        wxBoxSizer* pair = new wxBoxSizer(wxHORIZONTAL);
        pair->Add(edit, 1, wxALIGN_CENTER_VERTICAL);
        pair->Add(arrows, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 2);
        grid->Add(pair, 0, wxALIGN_CENTER);
    }

    // Row 2: labels
    for (int i = 0; i < 4; i++)
        grid->Add(new wxStaticText(box, wxID_ANY, fields[i].label),
                  0, wxALIGN_CENTER);

    m_groupSizer->Add(grid, 1, wxALL | wxEXPAND, 8);

    // Buttons: OK / Zero / Cancel stacked on the right
    wxBoxSizer* btnSizer = new wxBoxSizer(wxVERTICAL);
    wxButton* okBtn = new wxButton(this, wxID_OK, wxT("OK"));
    okBtn->SetDefault();
    btnSizer->Add(okBtn, 0, wxEXPAND | wxBOTTOM, 4);
    btnSizer->Add(new wxButton(this, ID_BUTTON_ZERO, wxT("Zero")),
                  0, wxEXPAND | wxBOTTOM, 4);
    btnSizer->Add(new wxButton(this, wxID_CANCEL, wxT("Cancel")), 0, wxEXPAND);

    wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
    topSizer->Add(m_groupSizer, 1, wxALL | wxEXPAND, 8);
    topSizer->Add(btnSizer, 0, wxTOP | wxRIGHT | wxBOTTOM, 8);

    SetSizerAndFit(topSizer);
    CentreOnParent();
}

static const int kMaxVal[4] = { 23, 59, 59, 29 };

int CEditSMPTETimeDlg::GetFieldValue(int idx) const
{
    long v = 0;
    if (!m_ctlEdit[idx]->GetValue().ToLong(&v) || v < 0)
        v = 0;
    if (v > kMaxVal[idx])
        v = kMaxVal[idx];
    return (int)v;
}

void CEditSMPTETimeDlg::StepField(int idx, int delta)
{
    int v = GetFieldValue(idx) + delta;
    if (v < 0) v = 0;                    // clamp, no wrap (as MFC default)
    if (v > kMaxVal[idx]) v = kMaxVal[idx];
    m_ctlEdit[idx]->SetValue(wxString::Format(wxT("%d"), v));
}

void CEditSMPTETimeDlg::UpdateGroupboxCaption()
{
    wxString str;
    str.Printf(wxT("%2u:%02u:%02u.%02u"),
               (unsigned)GetFieldValue(0), (unsigned)GetFieldValue(1),
               (unsigned)GetFieldValue(2), (unsigned)GetFieldValue(3));
    m_groupSizer->GetStaticBox()->SetLabel(str);
}

void CEditSMPTETimeDlg::OnButtonZero(wxCommandEvent& WXUNUSED(event))
{
    m_nHour = m_nMins = m_nSecs = m_nFrame = 0;

    TransferDataToWindow();      // equivalent of UpdateData(FALSE)
    UpdateGroupboxCaption();
}
