#include "EditTapeTimeDlg.h"
#include <wx/valnum.h>
#include <wx/valgen.h>

// Field order: hour, mins, secs, tens
static const int kMaxVal[4] = { 1, 59, 59, 9 };

CEditTapeTimeDlg::CEditTapeTimeDlg(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, wxT("Edit Time"),
               wxDefaultPosition, wxDefaultSize,
               wxDEFAULT_DIALOG_STYLE)
    , m_nMemIndex(0)
    , m_nHour(0)
    , m_nMins(0)
    , m_nSecs(0)
    , m_nTens(0)
    , m_bActive(false)
{
    struct Field { const wxChar* label; int* value; };
    const Field fields[4] =
    {
        { wxT("Hour"), &m_nHour },
        { wxT("Mins"), &m_nMins },
        { wxT("Secs"), &m_nSecs },
        { wxT("Tens"), &m_nTens }
    };

    m_groupSizer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("+0:00:00:0"));
    wxStaticBox* box = m_groupSizer->GetStaticBox();

    wxFlexGridSizer* grid = new wxFlexGridSizer(2, 4, 4, 8);

    // Row 1: edit + up/down arrow buttons (like MFC buddy pair)
    for (int i = 0; i < 4; i++)
    {
        wxTextCtrl* edit = new wxTextCtrl(box, wxID_ANY, wxT("0"),
                                wxDefaultPosition, FromDIP(wxSize(40, -1)),
                                wxTE_RIGHT);

        // DDX/DDV equivalent: transfers to the int member and range-checks
        wxIntegerValidator<int> val(fields[i].value);
        val.SetRange(0, kMaxVal[i]);
        edit->SetValidator(val);

        m_ctlEdit[i] = edit;

        edit->Bind(wxEVT_TEXT, [this](wxCommandEvent& e)
        {
            UpdateGroupboxCaption();
            e.Skip();
        });

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

    // Right column: OK / Cancel, then the Active checkbox
    wxCheckBox* chkActive = new wxCheckBox(this, wxID_ANY, wxT("Active"));
    chkActive->SetValidator(wxGenericValidator(&m_bActive));

    wxBoxSizer* btnSizer = new wxBoxSizer(wxVERTICAL);
    wxButton* okBtn = new wxButton(this, wxID_OK, wxT("OK"));
    okBtn->SetDefault();
    btnSizer->Add(okBtn, 0, wxEXPAND | wxBOTTOM, 4);
    btnSizer->Add(new wxButton(this, wxID_CANCEL, wxT("Cancel")),
                  0, wxEXPAND | wxBOTTOM, 12);
    btnSizer->Add(chkActive, 0, wxLEFT, 4);

    wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
    topSizer->Add(m_groupSizer, 1, wxALL | wxEXPAND, 8);
    topSizer->Add(btnSizer, 0, wxTOP | wxRIGHT | wxBOTTOM, 8);

    SetSizerAndFit(topSizer);
    CentreOnParent();
}

bool CEditTapeTimeDlg::TransferDataToWindow()
{
    // Runs when the dialog is shown, i.e. after the caller has filled in
    // m_nMemIndex and the time fields (the MFC OnInitDialog equivalent).
    bool ok = wxDialog::TransferDataToWindow();
    UpdateGroupboxCaption();
    return ok;
}

int CEditTapeTimeDlg::GetFieldValue(int idx) const
{
    long v = 0;
    if (!m_ctlEdit[idx]->GetValue().ToLong(&v) || v < 0)
        v = 0;
    if (v > kMaxVal[idx])
        v = kMaxVal[idx];
    return (int)v;
}

void CEditTapeTimeDlg::StepField(int idx, int delta)
{
    int v = GetFieldValue(idx) + delta;
    if (v < 0) v = 0;                    // clamp, no wrap (as MFC default)
    if (v > kMaxVal[idx]) v = kMaxVal[idx];
    m_ctlEdit[idx]->SetValue(wxString::Format(wxT("%d"), v));
}

void CEditTapeTimeDlg::UpdateGroupboxCaption()
{
    wxString str;
    str.Printf(wxT("[%u] - %1u:%02u:%02u:%1u"),
               (unsigned)m_nMemIndex,
               (unsigned)GetFieldValue(0), (unsigned)GetFieldValue(1),
               (unsigned)GetFieldValue(2), (unsigned)GetFieldValue(3));
    m_groupSizer->GetStaticBox()->SetLabel(str);
}
