#include <random>
#include <chrono>
#include "stdafx.h"
#include "StunClientHelper.h"

CStunClientHelper::CStunClientHelper(sockaddr_in serverAddr): m_pClientTransaction (NULL), m_pBindingRequest (NULL),
	m_bInitialize (false)
{
	m_bInitialize = StunGlobals::Initialize ();
	m_serverAddr = serverAddr;
}

CStunClientHelper::CStunClientHelper (const char *pszServer): m_pClientTransaction (NULL), m_pBindingRequest (NULL),
	m_bInitialize (false)
{
    USHORT stunPort = 3478;

	m_bInitialize = StunGlobals::Initialize ();

    //Moved from GetRandomPort so that it can be called multiple times in a row
    SYSTEMTIME time;
    StunGetSystemTime (&time);
    srand (time.wMilliseconds);

	if (m_bInitialize == false)
	{
		return;
	}

    //3/19/2026 Adding the ability for the hostname to include the port by seperating it with a :
    char *hostnameWorkingString = _strdup(pszServer);
    char *justHostname = hostnameWorkingString;
    if (strchr(hostnameWorkingString, ':') != NULL)
    {
        //strtok modifies hostnameWorkingString, that's why we made a copy
        justHostname = strtok(hostnameWorkingString, ":");
        char* portstring = strtok(NULL, ":");
        stunPort = atoi(portstring);
    }

    hostent *pHostent = gethostbyname (justHostname);
	if (pHostent == NULL)
	{
		clog << endl << "gethostbyname returned an error: WSAGetLastError = " << WSAGetLastError ()
			<< ", line number = " << __LINE__  << ", in " << __FILE__ << endl;
		m_bInitialize = false;
        free(hostnameWorkingString);
		return;
	}

	memcpy_s (&m_serverAddr.sin_addr, sizeof (m_serverAddr.sin_addr), 
		pHostent->h_addr_list [0], sizeof (m_serverAddr.sin_addr));
	m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_port = htons (stunPort);
    free(hostnameWorkingString);
}

CStunClientHelper::~CStunClientHelper(void)
{
	if (m_pClientTransaction)
	{
		delete m_pClientTransaction;
	}

	if (m_pBindingRequest)
	{
		delete m_pBindingRequest;
	}

	StunGlobals::UnInitialize ();
}

bool CStunClientHelper::TestOne(sockaddr_in serverAddr, sockaddr_in sendFromAddr, CStunMessage **pResponseMessage)
{
	if (m_bInitialize == false)
	{
		clog << endl << "There was an error in initialization" << endl;
		return false;
	}

	if (m_pClientTransaction)
	{
		delete m_pClientTransaction;
		m_pClientTransaction = NULL;
	}

	if (m_pBindingRequest)
	{
		delete m_pBindingRequest;
		m_pBindingRequest = NULL;
	}
	
	m_pBindingRequest = new CStunBindingRequestMessage ();
	m_pClientTransaction = new CStunClientTransaction (serverAddr, m_pBindingRequest);
	m_pClientTransaction->BindTo (sendFromAddr);

	*pResponseMessage = NULL;
	int nResult = 0;

	//Send a binding request with no attributes
	if (m_pClientTransaction->SendRequest (nResult) == false)
	{
		return false;
	}
	
	//The message was sent successfully, so we now wait for a response
	if (m_pClientTransaction->ReceiveResponse (nResult) == false)
	{		
		return false;
	}

	//We got a response, so we now check if it was a binding response or an error response
	if (m_pClientTransaction->GetResponseType () == BINDING_RESPONSE)
	{
		*pResponseMessage = m_pClientTransaction->GetBindingResponse();
	}
	else if (m_pClientTransaction->GetResponseType () == BINDING_ERROR_RESPONSE ||
			 m_pClientTransaction->GetResponseType () == SHARED_SECRET_ERROR_RESPONSE)
	{
		*pResponseMessage = m_pClientTransaction->GetErrorResponse ();
	}

	return true;
}

bool CStunClientHelper::TestTwo(sockaddr_in serverAddr, sockaddr_in sendFromAddr, CStunMessage **pResponseMessage)
{
	if (m_bInitialize == false)
	{
		clog << endl << "There was an error in initialization" << endl;
		return false;
	}

	if (m_pClientTransaction)
	{
		delete m_pClientTransaction;
		m_pClientTransaction = NULL;
	}

	if (m_pBindingRequest)
	{
		delete m_pBindingRequest;
		m_pBindingRequest = NULL;
	}

	m_pBindingRequest = new CStunBindingRequestMessage ();
	m_pClientTransaction = new CStunClientTransaction (serverAddr, m_pBindingRequest);
	m_pClientTransaction->BindTo (sendFromAddr);

	*pResponseMessage = NULL;
	int nResult = 0;

	//Send a binding request with change IP and change port flag
	m_pBindingRequest->AddChangeRequestAttribute (CHANGE_PORT | CHANGE_IP);

	//Send the request
	if (m_pClientTransaction->SendRequest (nResult) == false)
	{
		return false;
	}
	
	//The message was sent successfully, so we now wait for a response
	if (m_pClientTransaction->ReceiveResponse (nResult) == false)
	{		
		return false;
	}

	//We got a response, so we now check if it was a binding response or an error response
	if (m_pClientTransaction->GetResponseType () == BINDING_RESPONSE)
	{
		*pResponseMessage = m_pClientTransaction->GetBindingResponse();
	}
	else if (m_pClientTransaction->GetResponseType () == BINDING_ERROR_RESPONSE ||
			 m_pClientTransaction->GetResponseType () == SHARED_SECRET_ERROR_RESPONSE)
	{
		*pResponseMessage = m_pClientTransaction->GetErrorResponse ();
	}

	return true;
}

bool CStunClientHelper::TestThree(sockaddr_in serverAddr, sockaddr_in sendFromAddr, CStunMessage **pResponseMessage)
{
	if (m_bInitialize == false)
	{
		clog << endl << "There was an error in initialization" << endl;
		return false;
	}

	if (m_pClientTransaction)
	{
		delete m_pClientTransaction;
		m_pClientTransaction = NULL;
	}

	if (m_pBindingRequest)
	{
		delete m_pBindingRequest;
		m_pBindingRequest = NULL;
	}

	m_pBindingRequest = new CStunBindingRequestMessage ();
	m_pClientTransaction = new CStunClientTransaction (serverAddr, m_pBindingRequest);
	m_pClientTransaction->BindTo (sendFromAddr);

	*pResponseMessage = NULL;
	int nResult = 0;

	//Send a binding request with change port attribute
	m_pBindingRequest->AddChangeRequestAttribute (CHANGE_PORT);

	//Send the request
	if (m_pClientTransaction->SendRequest (nResult) == false)
	{
		return false;
	}
	
	//The message was sent successfully, so we now wait for a response
	if (m_pClientTransaction->ReceiveResponse (nResult) == false)
	{		
		return false;
	}

	//We got a response, so we now check if it was a binding response or an error response
	if (m_pClientTransaction->GetResponseType () == BINDING_RESPONSE)
	{
		*pResponseMessage = m_pClientTransaction->GetBindingResponse();
	}
	else if (m_pClientTransaction->GetResponseType () == BINDING_ERROR_RESPONSE ||
			 m_pClientTransaction->GetResponseType () == SHARED_SECRET_ERROR_RESPONSE)
	{
		*pResponseMessage = m_pClientTransaction->GetErrorResponse ();
	}

	return true;
}

NAT_TYPE CStunClientHelper::GetNatType()
{
	clog << endl << "Performing NAT detection" << endl;

	if (m_bInitialize == false)
	{
		clog << endl << "There was an error in initialization" << endl;
		return NAT_TYPE::ERROR_DETECTING_NAT;
	}

	bool bRet = false;
	
    sockaddr_in sendFromAddr;
/*** This code is breaking on machines with more than one IP. We should use INADDR_ANY instead
	char szHostName [MAX_PATH];
	if (gethostname (szHostName, MAX_PATH) == SOCKET_ERROR)
	{
		clog << endl << "gethostname returned an error: WSAGetLastError () = " << WSAGetLastError () 
			<< ", line number = " << __LINE__ << ", in " << __FILE__ << endl;
		return NAT_TYPE::ERROR_DETECTING_NAT;
	}

	hostent *pHostent = gethostbyname (szHostName);
	if (pHostent == 0)
	{
		clog << endl << "gethostbyname returned an error: WSAGetLastError () = " << WSAGetLastError () << endl;
		return NAT_TYPE::ERROR_DETECTING_NAT;
	}

	memcpy_s (&sendFromAddr.sin_addr, sizeof (sendFromAddr.sin_addr), 
		pHostent->h_addr_list [0], sizeof (sendFromAddr.sin_addr));
**/
	sendFromAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sendFromAddr.sin_family = AF_INET;
    sendFromAddr.sin_port = htons(GetRandomPort ());

	CStunMessage *pResponseMessage = NULL;

	clog << endl << "Performing test one" << endl;
	bRet = TestOne (m_serverAddr, sendFromAddr, &pResponseMessage);

	if (bRet == false)
	{
		//We didn't get any response so test one has failed
		//and the NAT type is UDP blocking firewall
		return NAT_TYPE::FIREWALL_BLOCKS_UDP;
	}

	if (pResponseMessage->GetMessageType () != BINDING_RESPONSE)
	{
		//We got an error response so just return -1
        clog << endl << "Received an error response:\n" << pResponseMessage->ToString ();
		return NAT_TYPE::ERROR_DETECTING_NAT;
	}

    //csb 5/11/2026 Google STUN servers don't return all of the attributes. Make sure all of
    //of the attributes are there before proceeding.
    if (!pResponseMessage->HasAttribute(MAPPED_ADDRESS) || !pResponseMessage->HasAttribute(SOURCE_ADDRESS) || !pResponseMessage->HasAttribute(CHANGED_ADDRESS))
    {
        clog << endl << "Received a response that is missing attributes:\n" << pResponseMessage->ToString ();
        return NAT_TYPE::ERROR_DETECTING_NAT;
    }

    sockaddr_in mappedAddress = {0}, sourceAddress = {0}, changedAddress = {0}, mappedAddress1 = {0};
	((CStunBindingResponseMessage *)pResponseMessage)->GetMappedAddress (&mappedAddress);
	((CStunBindingResponseMessage *)pResponseMessage)->GetSourceAddress (&sourceAddress);
	((CStunBindingResponseMessage *)pResponseMessage)->GetChangedAddress (&changedAddress);

	//Now match the mapped address with the source address
	if (mappedAddress.sin_port == sourceAddress.sin_port &&
		mappedAddress.sin_family == sourceAddress.sin_family &&
        mappedAddress.sin_addr.s_addr == sourceAddress.sin_addr.s_addr)
	{
		//We are not natted, now we try to determine whether we are behind
		//a symmetric UDP firewall or on open Internet

		clog << endl << "Performing test two" << endl;
		bRet = TestTwo (m_serverAddr, sendFromAddr, &pResponseMessage);

		if (bRet == true)
		{
			//We got a response
			return NAT_TYPE::OPEN_INTERNET;
		}
		else
		{
			return NAT_TYPE::SYMMETRIC_UDP_FIREWALL;
		}
	}
	else
	{
		//We are behind NAT
		bRet = TestTwo (m_serverAddr, sendFromAddr, &pResponseMessage);

		if (bRet == true)
		{
			//We got a response so we are behind full cone NAT
			return NAT_TYPE::FULL_CONE_NAT;
		}
		else
		{
			//No response
			//Perform test one with the changed address received in previous test one execution
			clog << endl << "Performing test one by sending request to the changed address which was sent in response to previous request to test one" << endl;
			bRet = TestOne (changedAddress, sendFromAddr, &pResponseMessage);
		
			if (bRet == false)
			{				
				return NAT_TYPE::ERROR_DETECTING_NAT;
			}

			if (pResponseMessage->GetMessageType () != BINDING_RESPONSE)
			{				
				clog << endl << "Received an error response:" << pResponseMessage->ToString ();
				return NAT_TYPE::ERROR_DETECTING_NAT;
			}

			//Got a response
			((CStunBindingResponseMessage *)pResponseMessage)->GetMappedAddress (&mappedAddress1);

			if (mappedAddress.sin_port != mappedAddress1.sin_port ||
				mappedAddress.sin_family != mappedAddress1.sin_family ||
                mappedAddress.sin_addr.s_addr != mappedAddress1.sin_addr.s_addr)
			{
				return NAT_TYPE::SYMMETRIC_NAT;
			}

			//IP's matched so perform test three
			clog << endl << "Performing test three" << endl;
			bRet = TestThree (m_serverAddr, sendFromAddr, &pResponseMessage);

			if (bRet == false)
			{
				//No response
				return NAT_TYPE::RESTRICTED_PORT_CONE_NAT;
			}
			else
			{
				return NAT_TYPE::RESTRICTED_CONE_NAT;
			}
		}
	}
}

bool CStunClientHelper::GetStunMappedAddress (sockaddr_in *pAddr)
{
	clog << endl << "Getting STUN mapped IP" << endl;
	if (m_bInitialize == false)
	{
		clog << endl << "There was an error in initialization" << endl;
		return false;
	}

	if (m_pClientTransaction)
	{
		delete m_pClientTransaction;
		m_pClientTransaction = NULL;
	}

	if (m_pBindingRequest)
	{
		delete m_pBindingRequest;
		m_pBindingRequest = NULL;
	}
	
	m_pBindingRequest = new CStunBindingRequestMessage ();
	m_pClientTransaction = new CStunClientTransaction (m_serverAddr, m_pBindingRequest);

	int nResult = 0;

	//Send a binding request with no attributes
	if (m_pClientTransaction->SendRequest (nResult) == false)
	{
		return false;
	}
	
	//The message was sent successfully, so we now wait for a response
	if (m_pClientTransaction->ReceiveResponse (nResult) == false)
	{		
		return false;
	}

	//We got a response, so we now check if it was a binding response or an error response
	if (m_pClientTransaction->GetResponseType () == BINDING_RESPONSE)
	{
		if (m_pClientTransaction->GetBindingResponse ()->GetMappedAddress (pAddr) == false)
		{
			clog << endl << "A binding response was received, but there was an error in parsing the response." << endl;
			return false;
		}
		return true;
	}
	else if (m_pClientTransaction->GetResponseType () == BINDING_ERROR_RESPONSE ||
			 m_pClientTransaction->GetResponseType () == SHARED_SECRET_ERROR_RESPONSE)
	{
		unsigned short nErrorCode = 0;
		if (m_pClientTransaction->GetErrorResponse ()->GetErrorCode (nErrorCode) == false)
		{
			clog << endl << "An error response was received, but there was an error in parsing the response." << endl;
			return false;
		}

		clog << endl << "An error response was received: Error code = " << nErrorCode << endl;
		return false;
	}

	return true;
}

unsigned int CStunClientHelper::GetRandomPort ()
{
    return rand ()%30000 + 10000;

    // Use a random_device to seed the generator for better entropy
/*    static std::mt19937 generator(static_cast<unsigned int>(
        std::chrono::system_clock::now().time_since_epoch().count()
        ));

    // Define the range (rand() + 10000 typically goes up to ~42767 on Windows)
    // Here we define a standard range for ephemeral ports
    std::uniform_int_distribution<unsigned int> distribution(10000, 65535);

    return distribution(generator);*/
}