// ntrights.cpp

/*    Copyright 2012 Justin Dearing.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "stdafx.h"
#include "ntrights.h"

BOOL CheckError(const _TCHAR* functionName, NTSTATUS ntStatus) {
	if (ntStatus != ERROR_SUCCESS) {
		LPVOID  message;
		ULONG winError = LsaNtStatusToWinError(ntStatus);
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			winError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &message,
			0,
			NULL 
		);

		// An error occurred. Display it as a win32 error code.
		_tprintf(_T("%s() returned Ox%lX: %s\n"), functionName, winError, (_TCHAR*)message);
		LocalFree(message);
	}
	return (ntStatus != ERROR_SUCCESS);
}

NTSTATUS CreateLsaHandle(PLSA_HANDLE pHandle) {
	LSA_OBJECT_ATTRIBUTES attributes;
	ACCESS_MASK necessaryAccess = POLICY_LOOKUP_NAMES | POLICY_CREATE_ACCOUNT;
	//ACCESS_MASK necessaryAccess = POLICY_ALL_ACCESS; //TODO: Apply minimal necessary access

	ZeroMemory(&attributes, sizeof(attributes));
	return LsaOpenPolicy(NULL, &attributes, necessaryAccess, pHandle);
}

PLSA_TRANSLATED_SID2 GetSidByUserName (PLSA_HANDLE pHandle, const LPCTSTR userName) {
	NTSTATUS ntStatus;
	LSA_UNICODE_STRING lsaName;
	PLSA_REFERENCED_DOMAIN_LIST referencedDomain;
	PLSA_TRANSLATED_SID2 translatedSid;

	lsaName = StringToLsaUnicodeString(userName);
	ntStatus = LsaLookupNames2(*pHandle, LSA_LOOKUP_ISOLATED_AS_LOCAL, 1, &lsaName, &referencedDomain, &translatedSid);
	
	if (ntStatus == ERROR_SUCCESS || ntStatus == STATUS_NONE_MAPPED || ntStatus == STATUS_SOME_NOT_MAPPED) {
		LsaFreeMemory(referencedDomain);
		if (ntStatus != ERROR_SUCCESS) {
			LsaFreeMemory(translatedSid);
			translatedSid = NULL;
		}
	}
	else { translatedSid = NULL; }
	CheckError(_T("LsaLookupNames2"), ntStatus);
	return translatedSid;
}

LSA_UNICODE_STRING StringToLsaUnicodeString(LPCTSTR string) {
	LSA_UNICODE_STRING lsaString;
	DWORD dwLen = 0;
	
	dwLen = wcslen(string);
	lsaString.Buffer = (LPWSTR) string;
	lsaString.Length = (USHORT)((dwLen) * sizeof(WCHAR));
	lsaString.MaximumLength = (USHORT)((dwLen + 1) * sizeof(WCHAR));
	return lsaString;
}

int _tmain(int argc, _TCHAR* argv[]) {
	NTSTATUS ntStatus;
	LSA_HANDLE lsaHandle;
	PLSA_TRANSLATED_SID2 translatedSid;

	ntStatus = CreateLsaHandle(&lsaHandle);
	if (!CheckError(_T("CreateLsaHandle"), ntStatus)) {
		translatedSid = GetSidByUserName(&lsaHandle, _T("zippy"));
		if (translatedSid != NULL) {
			LPTSTR szSid;
			ConvertSidToStringSid(translatedSid->Sid, &szSid);
			_tprintf(_T("Sid: %s\n"), szSid);
			LocalFree(&szSid);
			ntStatus = LsaAddAccountRights
				(lsaHandle, translatedSid->Sid, &StringToLsaUnicodeString(_T("SeServiceLogonRight")), 1);
			CheckError(_T("LsaAddAccountRights"), ntStatus);
			LsaFreeMemory(translatedSid);			
		}

		ntStatus = LsaClose(lsaHandle);
		CheckError(_T("LsaClose"), ntStatus);
	}
	_tprintf(_T("Press any key to continue . . ."));
	_gettch();
	return 0;
}