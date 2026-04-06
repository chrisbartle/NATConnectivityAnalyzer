#pragma once
#include "StunBindingMessage.h"

class CStunBindingRequestMessage :
	public CStunBindingMessage
{
public:
	CStunBindingRequestMessage(void);
	CStunBindingRequestMessage(char *pBuffer);
	~CStunBindingRequestMessage(void);

	void AddResponseAddressAttribute(unsigned short nPort, const char *pszAddress);
	void AddChangeRequestAttribute(unsigned int nChangeFlag);
	void AddUsernameAttribute(const char* pszUsername);

    bool GetResponseAddress(sockaddr_in *addr);
	bool GetUsername (const char** pszUsername);
	bool GetChangeRequest (unsigned int nChangeRequest);
};
