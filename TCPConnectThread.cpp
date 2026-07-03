#include "TCPConnectThread.h"

// Define the event
wxDEFINE_EVENT(wxEVT_TCP_CONNECT, wxThreadEvent);

TCPConnectThread::TCPConnectThread(wxEvtHandler* parent, const wxString& host, int port)
    : wxThread(wxTHREAD_DETACHED), m_pParent(parent), m_host(host), m_port(port)
{
    m_pSocket = new wxSocketClient();
}

TCPConnectThread::~TCPConnectThread()
{
    delete m_pSocket;
}

wxThread::ExitCode TCPConnectThread::Entry()
{
    // Create the socket and perform connection here
    wxIPV4address addr;
    addr.Hostname(m_host);
    addr.Service(m_port);

    m_pSocket = new wxSocketClient();

    // Connect asynchronously or use a timeout so you can check TestDestroy()
    m_pSocket->Connect(addr, false); // false = don't block main loop

    // Wait for connection or cancellation
    while (!TestDestroy() && !m_pSocket->WaitOnConnect(100))
    {
        // Wait loop
    }

    if (TestDestroy())
    {
        if (m_pSocket)
        {
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


#if 0
wxThread::ExitCode TCPConnectThread::Entry()
{
    wxIPV4address addr;
    addr.Hostname(m_host);
    addr.Service(m_port);

    // Attempt to connect (this blocks until it succeeds or times out)
    m_pSocket->Connect(addr, false); // false = non-blocking connect call

    // Wait for the connection, or handle errors here
    bool bConnected = m_pSocket->WaitOnConnect(5); // Wait up to 5 seconds

    // Package the result and send it to the GUI thread
    wxThreadEvent event(wxEVT_TCP_CONNECT);
    event.SetPayload(bConnected); // true if connected, false otherwise

    // wxQueueEvent is MT-safe for passing events to the main UI
    wxQueueEvent(m_pParent, event.Clone());

    return (ExitCode)0;
}
#endif

