#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "ConnectDialog.h"

wxBEGIN_EVENT_TABLE(ConnectDialog, wxDialog)
EVT_BUTTON(wxID_OK, ConnectDialog::OnOk)
EVT_BUTTON(wxID_CANCEL, ConnectDialog::OnCancel)
wxEND_EVENT_TABLE()

ConnectDialog::ConnectDialog(wxWindow* parent, wxWindowID id, const wxString& title,
		const wxPoint& position, const wxSize& size, long style)
	: wxDialog(parent, id, title, position, size, style)
{
	wxFlexGridSizer* flexSizer = new wxFlexGridSizer(2);

	flexSizer->Add(new wxStaticText(this, wxID_ANY, _("&Host:")), 0, wxALL | wxALIGN_RIGHT, 5);
	m_ctlAddressTxt = new wxTextCtrl(this, wxID_ANY, wxT("192.168.1.143"));
	flexSizer->Add(m_ctlAddressTxt, 0, wxALL, 5);

	flexSizer->Add(new wxStaticText(this, wxID_ANY, _("&Port:")), 0, wxALL | wxALIGN_RIGHT, 5);
	m_ctlPortTxt = new wxTextCtrl(this, wxID_ANY, wxT("1200"));
	flexSizer->Add(m_ctlPortTxt, 0, wxALL, 5);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* buttonSizer = new wxBoxSizer(wxVERTICAL);

	buttonSizer->Add(CreateButtonSizer(wxOK), 0, wxALL, 5);
	buttonSizer->Add(CreateButtonSizer(wxCANCEL), 0, wxLEFT|wxRIGHT, 5);
	
	mainSizer->Add(flexSizer, 0, wxALL, 5);
	mainSizer->Add(buttonSizer, 0, wxALL, 5);

	SetSizerAndFit(mainSizer);

	Centre();
}

void ConnectDialog::OnOk(wxCommandEvent& WXUNUSED(event))
{
	EndModal(wxID_OK);
}

void ConnectDialog::OnCancel(wxCommandEvent& WXUNUSED(event))
{
	EndModal(wxID_CANCEL);
}