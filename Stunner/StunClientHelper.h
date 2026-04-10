#pragma once
#include "StunMessage.h"
#include "StunClientTransaction.h"
#include "StunBindingRequestMessage.h"
#include "StunClientTransaction.h"
#include "StunGlobals.h"
#include "StunOS.h"

enum NAT_TYPE
{
	ERROR_DETECTING_NAT = -1,
	OPEN_INTERNET,
	FIREWALL_BLOCKS_UDP,
	SYMMETRIC_UDP_FIREWALL,
	FULL_CONE_NAT,
	SYMMETRIC_NAT,
	RESTRICTED_CONE_NAT,
	RESTRICTED_PORT_CONE_NAT,
};

class CStunClientHelper
{
public:
    CStunClientHelper(sockaddr_in serverAddr);
	CStunClientHelper(const char *pszServer);

	~CStunClientHelper(void);

    bool TestOne (sockaddr_in serverAddr, sockaddr_in sendFromAddr, CStunMessage **pMessage);
    bool TestTwo (sockaddr_in serverAddr, sockaddr_in sendFromAddr, CStunMessage **pMessage);
    bool TestThree (sockaddr_in serverAddr, sockaddr_in sendFromAddr, CStunMessage **pMessage);

	NAT_TYPE GetNatType ();

    bool GetStunMappedAddress (sockaddr_in *pAddr);
	
private:
    sockaddr_in m_serverAddr;
	CStunBindingRequestMessage *m_pBindingRequest;
	CStunClientTransaction *m_pClientTransaction;
	bool m_bInitialize;

	unsigned int GetRandomPort ();
};
