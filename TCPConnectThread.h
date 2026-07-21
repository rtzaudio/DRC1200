#ifndef TCPCONNECTTHREAD_H_INCLUDED
#define TCPCONNECTTHREAD_H_INCLUDED

#include <wx/thread.h>
#include <wx/event.h>
#include <wx/socket.h>

// Define a custom thread event type
wxDECLARE_EVENT(wxEVT_TCP_CONNECT, wxThreadEvent);

class TCPConnectThread : public wxThread
{
public:
    TCPConnectThread(wxEvtHandler* parent, const wxString& host, int port);
    virtual ~TCPConnectThread();

    void CancelConnect()
    {
        if (m_pSocket) {
            m_pSocket->Close();
        }
    }

protected:
    virtual ExitCode Entry() override;

private:
    wxEvtHandler* m_pParent;
    wxSocketClient* m_pSocket;
    wxString m_host;
    int m_port;
};

#endif // TCPCONNECTTHREAD_H_INCLUDED
