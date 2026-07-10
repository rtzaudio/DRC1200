///////////////////////////////////////////////////////////////////////////////
// STC-1200 TCP/IP Client Commands
//
// This module provides a TCP/IP client to allow sending various transport
// and other commands to the STC-1200 controller.
//

#include "DRC1200.h"
#include "STCCommand.h"

///////////////////////////////////////////////////////////////////////////////
// STCSocketCommand Construction

STCCommand::STCCommand() :
	wxSocketClient(wxSOCKET_BLOCK)
{

}

STCCommand::~STCCommand()
{
	ConnectionClose();
}

///////////////////////////////////////////////////////////////////////////////
//

bool STCCommand::ConnectionOpen(wxString& host)
{
	bool connected = false;
	wxIPaddress* addr;
	wxIPV4address addr4;
#if wxUSE_IPV6
	//wxIPV6address addr6;
	//if (family == wxSockAddress::IPV6)
	//	addr = &addr6;
	//else
#endif
		addr = &addr4;

	addr->Hostname(host);
	addr->Service(STC_PORT_COMMAND);

	connected = Connect(*addr, true);

	return connected;
}

bool STCCommand::ConnectionClose(void)
{
	bool closed = false;

	if (IsConnected())
	{
		Close();
		closed = true;
	}

	return closed;
}

///////////////////////////////////////////////////////////////////////////////
// STCSocketCommand Helpers

void STCCommand::InitHeader(STC_COMMAND_HDR& hdr, uint16_t cmd, uint16_t len)
{
	memset(&hdr, 0, sizeof(STC_COMMAND_HDR));

	if (!len)
		len = sizeof(STC_COMMAND_HDR) + sizeof(STC_COMMAND_ARG);

	hdr.length  = len;
	hdr.command = cmd;
	hdr.index   = 0;
	hdr.status  = 0;
}

///////////////////////////////////////////////////////////////////////////////
// STCSocketCommand Transport Control and Command Methods

wxSocketError STCCommand::VersionGet(
	uint32_t* rev_stc, uint32_t* rev_dtc,
	uint8_t* sn_stc, uint8_t* sn_dtc,
	uint8_t* mac)
{
	STC_COMMAND_VERSION_GET msg;
    wxSocketError se = wxSOCKET_NOERROR;

	// First make sure socket is connected
	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_VERSION_GET, sizeof(msg));

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

	if (rev_stc)
		*rev_stc = msg.version_stc;
	if (rev_dtc)
		*rev_dtc = msg.version_dtc;

	// return MAC address
	if (mac)
		memcpy(mac, msg.macaddr, 6);
	// return STC serial number
	if (sn_stc)
		memcpy(sn_stc, msg.sernum_stc, 16);
	// return DTC serial number
	if (sn_dtc)
		memcpy(sn_dtc, msg.sernum_dtc, 16);

	return se;
}

wxSocketError STCCommand::TransportStop(uint32_t flags)
{
	STC_COMMAND_STOP msg;
    wxSocketError se = wxSOCKET_NOERROR;

	// First make sure socket is connected
	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_STOP, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = flags;
	msg.arg.param2.U = 0;

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

	return se;
}

wxSocketError STCCommand::TransportPlay(uint32_t flags)
{
	STC_COMMAND_PLAY msg;
    wxSocketError se = wxSOCKET_NOERROR;

	// First make sure socket is connected
	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_PLAY, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = flags;
	msg.arg.param2.U = 0;

	Write(&msg, msg.hdr.length);
	Read(&msg, msg.hdr.length);

	return se;
}

wxSocketError STCCommand::TransportRewind(uint32_t flags)
{
	STC_COMMAND_REW msg;
    wxSocketError se = wxSOCKET_NOERROR;

	// First make sure socket is connected
	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_REW, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = flags;
	msg.arg.param2.U = 0;

	Write(&msg, msg.hdr.length);
	Read(&msg, msg.hdr.length);

	return se;
}

wxSocketError STCCommand::TransportForward(uint32_t flags)
{
	STC_COMMAND_FWD msg;
    wxSocketError se = wxSOCKET_NOERROR;

	// First make sure socket is connected
	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_FWD, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = flags;
	msg.arg.param2.U = 0;

	Write(&msg, msg.hdr.length);
	Read(&msg, msg.hdr.length);

	return se;
}

wxSocketError STCCommand::TransportLifter(uint32_t flags)
{
	STC_COMMAND_LIFTER msg;
    wxSocketError se = wxSOCKET_NOERROR;

	// First make sure socket is connected
	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_LIFTER, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = flags;
	msg.arg.param2.U = 0;

	Write(&msg, msg.hdr.length);
	Read(&msg, msg.hdr.length);

	return se;
}

// STCSocketCommand Cue Point Methods

wxSocketError STCCommand::CuePointClear(uint32_t index, uint32_t flags)
{
	STC_COMMAND_CUEPOINT_CLEAR msg;
    wxSocketError se = wxSOCKET_NOERROR;

	// First make sure socket is connected
	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_CUEPOINT_CLEAR, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = index;
	msg.arg.param2.U = flags;

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

	return se;
}

wxSocketError STCCommand::CuePointStore(uint32_t index, int32_t cuepoint, uint32_t flags)
{
	STC_COMMAND_CUEPOINT_STORE msg;
    wxSocketError se = wxSOCKET_NOERROR;

	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_CUEPOINT_STORE, sizeof(msg));

	msg.hdr.index    = (uint16_t)index;

	msg.arg.bitflags = 0;
	msg.arg.param1.I = cuepoint;
	msg.arg.param2.U = flags;

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

	return se;
}

wxSocketError STCCommand::CuePointSet(uint32_t index, int32_t cuepoint, uint32_t flags)
{
	STC_COMMAND_CUEPOINT_SET msg;
    wxSocketError se = wxSOCKET_NOERROR;

	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_CUEPOINT_SET, sizeof(msg));

	msg.hdr.index    = (uint16_t)index;

	msg.arg.bitflags = 0;
	msg.arg.param1.I = cuepoint;
	msg.arg.param2.U = flags;

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

	return se;
}

wxSocketError STCCommand::CuePointGet(uint32_t index, int32_t* cuepoint, uint32_t* flags)
{
	STC_COMMAND_CUEPOINT_GET msg;
    wxSocketError se = wxSOCKET_NOERROR;

	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_CUEPOINT_GET, sizeof(msg));

	msg.hdr.index    = (uint16_t)index;

	msg.arg.bitflags = 0;
	msg.arg.param1.U = index;
	msg.arg.param2.U = 0;

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

    *cuepoint = msg.arg.param1.I;		// return cue point position
    *flags    = msg.arg.param2.U;		// return cue point flags (STC_CF_ACTIVE, etc)

	return se;
}

// STCSocketCommand Locator Methods

wxSocketError STCCommand::LocatorSetMode(uint32_t mode)
{
	STC_COMMAND_LOCATE_MODE_SET msg;
    wxSocketError se = wxSOCKET_NOERROR;

	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_LOCATE_MODE_SET, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = mode;
	msg.arg.param2.U = 0;

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

	return se;
}

wxSocketError STCCommand::LocatorZeroReturn(uint32_t flags)
{
	STC_COMMAND_LOCATE msg;
    wxSocketError se = wxSOCKET_NOERROR;

	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_LOCATE, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = STC_CUE_POINT_HOME;
	msg.arg.param2.U = flags;

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

	return se;
}

wxSocketError STCCommand::LocatorZeroReset(void)
{
	STC_COMMAND_ZERO_RESET msg;
    wxSocketError se = wxSOCKET_NOERROR;

	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_ZERO_RESET, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = 0;
	msg.arg.param2.U = 0;

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

	return se;
}

wxSocketError STCCommand::LocatorSearch(uint32_t index, uint32_t flags)
{
	STC_COMMAND_LOCATE msg;
    wxSocketError se = wxSOCKET_NOERROR;

	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_LOCATE, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = index;
	msg.arg.param2.U = flags;

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

	return se;
}

wxSocketError STCCommand::LocatorAutoLoop(uint32_t flags)
{
	STC_COMMAND_LOCATE_AUTO_LOOP msg;
    wxSocketError se = wxSOCKET_NOERROR;

	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_LOCATE_AUTO_LOOP, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = flags;
	msg.arg.param2.U = 0;

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

	return se;
}

wxSocketError STCCommand::LocatorAutoPunchSet(uint32_t flags)
{
	STC_COMMAND_AUTO_PUNCH_SET msg;
    wxSocketError se = wxSOCKET_NOERROR;

	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_AUTO_PUNCH_SET, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = flags;
	msg.arg.param2.U = 0;

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

	return se;
}

wxSocketError STCCommand::LocatorCancel(uint32_t flags)
{
	STC_COMMAND_CANCEL msg;
    wxSocketError se = wxSOCKET_NOERROR;

	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_CANCEL, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = flags;
	msg.arg.param2.U = 0;

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

	return se;
}

// Track/Channel State Methods

wxSocketError STCCommand::TrackGetCount(uint32_t* numTracks, bool* dcsFound)
{
	STC_COMMAND_TRACK_GET_COUNT msg;
    wxSocketError se = wxSOCKET_NOERROR;

	// First make sure socket is connected
	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_TRACK_GET_COUNT, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = 0;
	msg.arg.param2.U = 0;

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

	*numTracks = (uint32_t)msg.arg.param1.U;
	*dcsFound  = (bool)msg.arg.param2.U;

	return se;
}

wxSocketError STCCommand::TrackSetState(uint32_t index, uint32_t mode)
{
	STC_COMMAND_TRACK_SET_STATE msg;
    wxSocketError se = wxSOCKET_NOERROR;

	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_TRACK_SET_STATE, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = index & 0xFF;
	msg.arg.param2.U = mode & 0xFF;

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

	return se;
}

wxSocketError STCCommand::TrackMaskAll(uint32_t setmask, uint32_t clearmask)
{
	STC_COMMAND_TRACK_MASK_ALL msg;
    wxSocketError se = wxSOCKET_NOERROR;

	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_TRACK_MASK_ALL, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = setmask;
	msg.arg.param2.U = clearmask;

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

	return se;
}

wxSocketError STCCommand::TrackMaskToggleAll(uint32_t mask)
{
	STC_COMMAND_TRACK_TOGGLE_ALL msg;
    wxSocketError se = wxSOCKET_NOERROR;

	// First make sure socket is connected
	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_TRACK_TOGGLE_ALL, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = mask;
	msg.arg.param2.U = 0;

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

	return se;
}

wxSocketError STCCommand::TrackModeAll(uint32_t mode)
{
	STC_COMMAND_TRACK_MODE_ALL msg;
    wxSocketError se = wxSOCKET_NOERROR;

	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_TRACK_MODE_ALL, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = mode;
	msg.arg.param2.U = 0;

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

	return se;
}

// DTC Load/Store/Reset track confg to EPROM: 0=load, 1=store, 2=reset
wxSocketError STCCommand::ConfigEPROM(uint32_t opcode)
{
	STC_COMMAND_CONFIG_EPROM msg;
    wxSocketError se = wxSOCKET_NOERROR;

	// First make sure socket is connected
	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_CONFIG_EPROM, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = opcode;
	msg.arg.param2.U = 0;

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

	return se;
}

wxSocketError STCCommand::MonitorSet(bool standby)
{
	STC_COMMAND_MONITOR msg;
    wxSocketError se = wxSOCKET_NOERROR;

	// First make sure socket is connected
	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_MONITOR, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = standby;
	msg.arg.param2.U = 0;

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

	return se;
}

wxSocketError STCCommand::TapeSpeedSet(uint32_t speed)
{
	STC_COMMAND_TAPE_SPEED_SET msg;
    wxSocketError se = wxSOCKET_NOERROR;

	// First make sure socket is connected
	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_TAPE_SPEED_SET, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = speed;
	msg.arg.param2.U = 0;

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

	return se;
}

wxSocketError STCCommand::MachineConfig(uint32_t opcode)
{
	STC_COMMAND_MACHINE_CONFIG msg;
    wxSocketError se = wxSOCKET_NOERROR;

	// First make sure socket is connected
	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_MACHINE_CONFIG, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = opcode;		/* 0=load, 1=store, 2=defaults */
	msg.arg.param2.U = 0;

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

	return se;
}

wxSocketError STCCommand::MachineConfigGet(STC_CONFIG_DATA& stc, DTC_CONFIG_DATA& dtc)
{
	STC_COMMAND_MACHINE_CONFIG_GET msg;
    wxSocketError se = wxSOCKET_NOERROR;

	// First make sure socket is connected
	if (!IsConnected())
        return wxSOCKET_NOHOST;

    memset(&stc, 0, sizeof(STC_CONFIG_DATA));
	memset(&dtc, 0, sizeof(DTC_CONFIG_DATA));

	InitHeader(msg.hdr, STC_CMD_MACHINE_CONFIG_GET, 0);

	msg.arg.bitflags = 0;
	msg.arg.param1.U = 0;
	msg.arg.param2.U = 0;

	Write(&msg, msg.hdr.length);

	msg.hdr.length = sizeof(msg);

	Read(&msg, msg.hdr.length);

    memcpy(&stc, &msg.stc, sizeof(STC_CONFIG_DATA));
    memcpy(&dtc, &msg.dtc, sizeof(DTC_CONFIG_DATA));

	return se;
}

wxSocketError STCCommand::MachineConfigSet(STC_CONFIG_DATA& stc, DTC_CONFIG_DATA& dtc)
{
	STC_COMMAND_MACHINE_CONFIG_SET msg;
    wxSocketError se = wxSOCKET_NOERROR;

	// First make sure socket is connected
	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_MACHINE_CONFIG_SET, sizeof(msg));

	msg.arg.bitflags = 0;
	msg.arg.param1.U = 0;
	msg.arg.param2.U = 0;

	memcpy(&msg.stc, &stc, sizeof(STC_CONFIG_DATA));
	memcpy(&msg.dtc, &dtc, sizeof(DTC_CONFIG_DATA));

	Write(&msg, msg.hdr.length);
	Read(&msg, msg.hdr.length);

	return se;
}

wxSocketError STCCommand::RTCTimeDateGet(DATETIME& datetime)
{
	STC_COMMAND_RTC_TIMEDATE_GET msg;
    wxSocketError se = wxSOCKET_NOERROR;

	// First make sure socket is connected
	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_RTC_TIMEDATE_GET, sizeof(msg));

	Write(&msg, msg.hdr.length);
	Read(&msg, msg.hdr.length);

    // Copy time/date return to callers buffer
    memcpy(&datetime, &msg.datetime, sizeof(DATETIME));

	return se;
}

wxSocketError STCCommand::RTCTimeDateSet(DATETIME& datetime)
{
	STC_COMMAND_RTC_TIMEDATE_SET msg;
    wxSocketError se = wxSOCKET_NOERROR;

	// First make sure socket is connected
	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_RTC_TIMEDATE_SET, sizeof(msg));

	Write(&msg, msg.hdr.length);
	Read(&msg, msg.hdr.length);

	return se;
}

wxSocketError STCCommand::MACAddrGet(uint8_t* mac, uint8_t* sn_stc, uint8_t* sn_dtc)
{
	STC_COMMAND_MACADDR_GET msg;
    wxSocketError se = wxSOCKET_NOERROR;

	// First make sure socket is connected
	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_MACADDR_GET, sizeof(msg));

	Write(&msg, msg.hdr.length);
	Read(&msg, msg.hdr.length);

	// Check if the STC IPC to the DTC failed
	//if ((dwError = GetLastError()) == ERROR_SUCCESS)
	{
		// return MAC address
		if (mac)
			memcpy(mac, msg.macaddr, 6);

		// return STC serial number
		if (sn_stc)
			memcpy(sn_stc, msg.sernum_stc, 16);

		// return DTC serial number
		if (sn_dtc)
			memcpy(sn_dtc, msg.sernum_dtc, 16);
	}

	return se;
}

wxSocketError STCCommand::SMPTEEncoderCtrl(uint32_t ctrl, uint32_t flags)
{
	STC_COMMAND_SMPTE_ENCODER_CTRL msg;
    wxSocketError se = wxSOCKET_NOERROR;

	// First make sure socket is connected
	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_SMPTE_ENCODER_CTRL, sizeof(msg));

	msg.ctrl  = (uint16_t)ctrl;
	msg.flags = (uint16_t)flags;

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

	return se;
}

wxSocketError STCCommand::SMPTEEncoderSetTime(uint32_t hours, uint32_t mins, uint32_t secs, uint32_t frame)
{
    wxSocketError se = wxSOCKET_NOERROR;

	STC_COMMAND_SMPTE_TIME_SET msg;

	// First make sure socket is connected
	if (!IsConnected())
        return wxSOCKET_NOHOST;

	InitHeader(msg.hdr, STC_CMD_SMPTE_TIME_SET, sizeof(msg));

	msg.hours = (uint8_t)hours;
	msg.mins  = (uint8_t)mins;
	msg.secs  = (uint8_t)secs;
	msg.frame = (uint8_t)frame;

	Write(&msg, sizeof(msg));
	Read(&msg, sizeof(msg));

	return se;
}


// End-Of-File
