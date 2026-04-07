#include "stdafx.h"
#include "StunTransaction.h"
#include "StunClientTransaction.h"
#include "StunGlobals.h"
#include "StunOS.h"

CStunTransaction::CStunTransaction(sockaddr_in sendToAddr, CStunMessage *pMessageToSend):
	m_SendToAddr (sendToAddr), m_pMessageToSend (pMessageToSend)
{
	Initialize ();
}

CStunTransaction::~CStunTransaction(void)
{
	if (m_pMessageReceived)
	{
		delete m_pMessageReceived;
	}
#ifdef _WIN32
	closesocket (m_SendSock);
#else
    close(m_SendSock);
#endif
}

bool CStunTransaction::SendStunMessage (int nResult)
{
	nResult = 0;
	
	int nRetries = 0;
	fd_set fdWrite;
	timeval timeInterval = {0, 10000};
	
	while (nRetries < 5)
	{
		FD_ZERO (&fdWrite);
		FD_SET (m_SendSock, &fdWrite);

        if ((nResult = select (m_SendSock+1, NULL, &fdWrite, NULL, &timeInterval))
			== SOCKET_ERROR)
		{
			nResult = WSAGetLastError ();
			clog << endl << "An error occured in select operation: " << "WSAGetLastError () = " << 
				nResult << ", line number = " << __LINE__  << ", in " << __FILE__ << endl;
			return false;		
		}

		if (nResult > 0 && FD_ISSET (m_SendSock, &fdWrite))
		{
			clog << endl << "Sending request: " << endl;
			clog << m_pMessageToSend->ToString ();

			if (sendto (m_SendSock, m_pMessageToSend->GetBuffer (), m_pMessageToSend->GetTotalLength (),
                0, (sockaddr*)&m_SendToAddr, sizeof (m_SendToAddr)) == SOCKET_ERROR)
			{
				nResult = WSAGetLastError ();
				clog << endl << "An error occured in sendto operation: " << "WSAGetLastError () = " << 
					nResult << endl;
				return false;
			}
			return true;
		}

		clog << endl << "Socket is not ready to send. Will now try after ten milliseconds." << endl;
		--nRetries;
	}

	clog << endl << "Unable to send STUN message. Aborting transaction." << endl;
	return false;
}


//Checks if the message is correcrly formed, looks for message integrity and 
//other attributes to detect an attack
bool CStunTransaction::ValidateMessage()
{	
	if (m_pMessageReceived->IsMessageValid () == false &&
		m_pMessageReceived->HasAttribute (MESSAGE_INTEGRITY) != 
		m_pMessageToSend->HasAttribute (MESSAGE_INTEGRITY))
	{
		clog << endl << "Invalid message received, discarding the response." << endl;

		//The message is not valid so we discard this response
		delete m_pMessageReceived;
		m_pMessageReceived = NULL;
		return false;
	}

	if (m_pMessageReceived->HasAttribute (MESSAGE_INTEGRITY) == true)
	{
		//TODO: Compute HMAC over the response and make sure it matches
		//the message integrity attribute
	}

	return true;
}

bool CStunTransaction::ReceiveStunMessage(CStunMessage **pMessageReceived, int nResult)
{
    unsigned int nSizeFromAddr = sizeof (m_SendToAddr);
	
	char pBuffer [2400];
	int nBufferSize = 2400;

    if (nResult = recvfrom (m_SendSock, pBuffer, nBufferSize, 0, (sockaddr *)&m_SendToAddr,
		&nSizeFromAddr) == SOCKET_ERROR)
	{
		nResult = WSAGetLastError ();
		clog << endl << "An error occured in recvfrom operation: " << "WSAGetLastError () = " << 
			nResult << endl;
		return false;
	}

	*pMessageReceived = StunGlobals::CreateMessage (pBuffer);

	clog << endl << "Received response: " << endl;
	clog << (*pMessageReceived)->ToString ();

	return true;
}

bool CStunTransaction::Initialize()
{
	m_SendSock = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	m_pMessageReceived = NULL;

	return true;
}

bool CStunTransaction::BindTo (sockaddr_in sendFromAddr)
{
    if (bind (m_SendSock, (sockaddr *)&sendFromAddr, sizeof (sockaddr)) ==
		SOCKET_ERROR)
	{
		clog << endl << "An error occured in bind operation: " << "WSAGetLastError () = " << 
			WSAGetLastError () << ", line number = " << __LINE__  << ", in " << __FILE__ << endl;
		return false;
	}

	return true;
}