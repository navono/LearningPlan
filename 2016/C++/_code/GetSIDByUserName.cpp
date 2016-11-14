BOOL GetSIDByUserName( const char* userName, LPTSTR szSid, DWORD dwSize )
{
	BOOL bRet = FALSE;

	PSID	pUserSID = nullptr;
	LPTSTR	pUserDomain = nullptr;
	SID_NAME_USE snu;

	DWORD cbUserSID = 0;
	DWORD cbDomain = 0;

	lstrcpy(szSid, "");

	__try
	{
		if (!LookupAccountName(nullptr, userName, nullptr, &cbUserSID, nullptr, &cbDomain, &snu))
		{
			pUserSID = (PSID)malloc(cbUserSID);
			pUserDomain = (LPTSTR)malloc(cbDomain * sizeof(TCHAR));
			if (pUserSID == nullptr || pUserDomain == nullptr)
			{
				__leave;
			}

			if (!LookupAccountName(nullptr, userName, pUserSID, &cbUserSID, pUserDomain, &cbDomain, &snu))
			{
				__leave;
			}

			if (IsValidSid(pUserSID))
			{
				if (_ConvertSid(pUserSID, szSid, &dwSize))
				{
					bRet = TRUE;
				}
			}
		}
	}
	__finally
	{
		if (pUserDomain != nullptr)
		{
			free(pUserDomain);
		}

		if (pUserSID != nullptr)
		{
			free(pUserSID);
		}
	}
	return  bRet;
}

BOOL _ConvertSid( const PSID pSid, LPTSTR textualSid, LPDWORD lpdwBufferLen )
{
	if(!IsValidSid(pSid))
	{
		return FALSE;
	}

	DWORD dwSidRev = SID_REVISION;

	PSID_IDENTIFIER_AUTHORITY psia = GetSidIdentifierAuthority(pSid);
	DWORD dwSubAuthorities = *GetSidSubAuthorityCount(pSid);
	DWORD dwSidSize = (15 + 12 + (12 * dwSubAuthorities) + 1) * sizeof(TCHAR);
	if (*lpdwBufferLen < dwSidSize)
	{
		*lpdwBufferLen = dwSidSize;
		SetLastError(ERROR_INSUFFICIENT_BUFFER);
		return FALSE;
	}
	dwSidSize = wsprintf(textualSid, TEXT("S-%lu-"), dwSidRev);
	if ((psia->Value[0] != 0) || (psia->Value[1] != 0))
	{
		dwSidSize += wsprintf(textualSid + lstrlen(textualSid), TEXT("0x%02hx%02hx%02hx%02hx%02hx%02hx"),
			(USHORT)psia->Value[0],
			(USHORT)psia->Value[1],
			(USHORT)psia->Value[2],
			(USHORT)psia->Value[3],
			(USHORT)psia->Value[4],
			(USHORT)psia->Value[5]);
	}
	else
	{
		dwSidSize += wsprintf(textualSid + lstrlen(textualSid), TEXT("%lu"),
			(ULONG)(psia->Value[5])   +
			(ULONG)(psia->Value[4] <<  8)   +
			(ULONG)(psia->Value[3] << 16)   +
			(ULONG)(psia->Value[2] << 24));
	}

	for (auto dwCounter = 0U; dwCounter < dwSubAuthorities; ++dwCounter)
	{
		dwSidSize += wsprintf(textualSid + dwSidSize, TEXT("-%lu"), *GetSidSubAuthority(pSid, dwCounter));
	}

	return TRUE;
}