#pragma once

#include "StunMessage.h"

class CStunTransaction
{
protected:
	bool SendStunMessage (int nResult);
	bool ReceiveStunMessage (CStunMessage **pMessageReceived, int nResult);
	bool Initialize ();

	CStunMessage *m_pMessageToSend;
	CStunMessage *m_pMessageReceived;

    SOCKET m_SendSock;
    sockaddr_in m_SendToAddr;
	int m_nPortToSendMessageFrom;

	//Checks if the message is correcrly formed, looks for message integrity and 
	//other attributes to detect an attack
	bool ValidateMessage();

public:
    CStunTransaction(sockaddr_in sendToAddr, CStunMessage *pMessageToSend);
    CStunTransaction(sockaddr_in sendToAddr, sockaddr_in sendFromAddr, CStunMessage *pMessageToSend);
	virtual ~CStunTransaction(void) = 0;
    bool BindTo (sockaddr_in sendFromAddr);
};
