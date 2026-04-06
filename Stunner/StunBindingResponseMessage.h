#pragma once
#include "StunBindingMessage.h"

class CStunBindingResponseMessage :
	public CStunBindingMessage
{
public:
	CStunBindingResponseMessage(CStunAddressAttribute *pMappedAddressAttribute, 
								CStunAddressAttribute *pSourceAddressAttribute,
								CStunAddressAttribute *pChangedAddressAttribute);
	CStunBindingResponseMessage (char *pBuffer);

	~CStunBindingResponseMessage(void);

	void AddReflectedFromAddressAttribute(unsigned short nPort, const char *pszAddress);

    bool GetMappedAddress(sockaddr_in *pAddr);
    bool GetSourceAddress(sockaddr_in *pAddr);
    bool GetChangedAddress(sockaddr_in *pAddr);
    bool GetReflectedFromAddress(sockaddr_in *pAddr);
};
