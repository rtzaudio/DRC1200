
class ConnectDialog : public wxDialog
{
public:
	ConnectDialog(wxWindow* parent, wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxASCII_STR(wxDialogNameStr));
	
	wxTextCtrl* m_ctlAddressTxt;
	wxTextCtrl* m_ctlPortTxt;

	void OnOk(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()
};