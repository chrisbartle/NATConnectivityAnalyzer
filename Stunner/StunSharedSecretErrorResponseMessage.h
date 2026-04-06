#pragma once
#include "StunErrorResponseMessage.h"

class CStunSharedSecretErrorResponseMessage :
	public CStunErrorResponseMessage
{
public:
	CStunSharedSecretErrorResponseMessage(unsigned short nErrorCode);
	CStunSharedSecretErrorResponseMessage(char *pBuffer);
	~CStunSharedSecretErrorResponseMessage(void);
};
