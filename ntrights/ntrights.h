// ntrights.h

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

BOOL CheckError(const _TCHAR* functionName, NTSTATUS ntStatus);
NTSTATUS CreateLsaHandle(PLSA_HANDLE pHandle);
LSA_UNICODE_STRING StringToLsaUnicodeString(LPCTSTR string);
PLSA_TRANSLATED_SID2 GetSidByUserName (PLSA_HANDLE pHandle, const LPCTSTR userName);