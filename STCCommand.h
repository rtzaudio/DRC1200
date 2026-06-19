///////////////////////////////////////////////////////////////////////////////
//

#pragma once

class STCCommand : public wxSocketClient
{
// Construction
public:
	STCCommand();

// Attributes
public:

// Operations
public:
	bool ConnectionOpen(wxString& host);
	bool ConnectionClose(void);

    wxSocketError VersionGet(uint32_t* rev_stc, uint32_t* rev_dtc,
        uint8_t* sn_stc, uint8_t* sn_dtc, uint8_t* mac);

	wxSocketError TransportStop(uint32_t flags=0);
	wxSocketError TransportPlay(uint32_t flags=0);
	wxSocketError TransportRewind(uint32_t flags=0);
	wxSocketError TransportForward(uint32_t flags=0);
	wxSocketError TransportLifter(uint32_t flags=0);

	wxSocketError LocatorSetMode(uint32_t mode);
	wxSocketError LocatorZeroReset(void);
	wxSocketError LocatorZeroReturn(uint32_t flags=0);
	wxSocketError LocatorSearch(uint32_t index, uint32_t flags=0);
	wxSocketError LocatorAutoLoop(uint32_t flags=0);
	wxSocketError LocatorAutoPunchSet(uint32_t flags=0);
	wxSocketError LocatorCancel(uint32_t flags=0);

	wxSocketError CuePointClear(uint32_t index, uint32_t flags=0);
	wxSocketError CuePointStore(uint32_t index, int32_t cuepoint, uint32_t flags=0);
	wxSocketError CuePointSet(uint32_t index, int32_t cuepoint, uint32_t flags=0);
	wxSocketError CuePointGet(uint32_t index, int32_t* cuepoint, uint32_t *flags);

    wxSocketError TrackGetCount(uint32_t* numTracks, bool* dcsFound);
	wxSocketError TrackSetState(uint32_t index, uint32_t mode);
	wxSocketError TrackMaskAll(uint32_t setmask, uint32_t clearmask);
	wxSocketError TrackMaskToggleAll(uint32_t mask);
	wxSocketError TrackModeAll(uint32_t newmode);

	wxSocketError ConfigEPROM(uint32_t opcode);
    wxSocketError MonitorSet(bool standby);
    wxSocketError TapeSpeedSet(uint32_t speed);
    wxSocketError MachineConfig(uint32_t opcode);
    wxSocketError MachineConfigGet(STC_CONFIG_DATA& stc, DTC_CONFIG_DATA& dtc);
    wxSocketError MachineConfigSet(STC_CONFIG_DATA& stc, DTC_CONFIG_DATA& dtc);

    wxSocketError RTCTimeDateGet(DATETIME& datetime);
    wxSocketError RTCTimeDateSet(DATETIME& datetime);
    wxSocketError MACAddrGet(uint8_t* mac, uint8_t* sn_stc, uint8_t* sn_dtc);
    wxSocketError SMPTEEncoderCtrl(uint32_t ctrl, uint32_t flags=0);
    wxSocketError SMPTEEncoderSetTime(uint32_t hours=0, uint32_t mins=0, uint32_t secs=0, uint32_t frame=0);

// Implementation
public:

// Overrides
protected:

// Implementation
public:
	void InitHeader(STC_COMMAND_HDR& hdr, uint16_t cmd, uint16_t len=0);

	virtual ~STCCommand();
};

// End-Of-File
