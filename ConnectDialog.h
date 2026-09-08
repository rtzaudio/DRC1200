
class ConnectDialog : public wxDialog
{
public:
	ConnectDialog(wxWindow* parent, wxWindowID id, const wxString& title);

	wxTextCtrl* m_ctlAddressTxt;
	wxTextCtrl* m_ctlPortTxt;

	void OnOk(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()
};
