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

#if 0
class TCPConnectThread : public wxThread
{
public:
    TCPConnectThread(wxEvtHandler* parent, const wxString& host, int port)
    : wxThread(wxTHREAD_JOINABLE)
    , m_pParent(parent)
    , m_sHost(host)
    , m_iPort(port)

protected:
    ExitCode Entry() override {

        // Create the socket and perform connection here
        wxIPV4address addr;
        addr.Hostname(m_sHost);
        addr.Service(m_iPort);

        m_pSocket = new wxSocketClient();

        // Connect asynchronously or use a timeout so you can check TestDestroy()
        m_pSocket->Connect(addr, false); // false = don't block main loop

        // Wait for connection or cancellation
        while (!TestDestroy() && !m_pSocket->WaitOnConnect(100)) {
            // Wait loop
        }

        if (TestDestroy()) {
            if (m_pSocket) {
                m_pSocket->Destroy();
                m_pSocket = nullptr;
            }
            return (ExitCode)0;
        }

        // Notify parent on completion
        wxThreadEvent event(wxEVT_THREAD, wxID_OK);
        wxQueueEvent(m_pParent, event.Clone());

        return (ExitCode)0;
    }

    void CancelConnect() {
        if (m_pSocket) {
            m_pSocket->Close();
        }
    }

private:
    int m_iPort;
    wxString m_sHost;
    wxEvtHandler* m_pParent;
    wxSocketClient* m_pSocket = nullptr;
};
#endif
#endif // TCPCONNECTTHREAD_H_INCLUDED
