// Display .rdata
#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "netapi32.lib")

#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <lm.h>
#include <sddl.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

#define PASSWORD_SIZE 100
#define PASS  "Orpat21"

int runProgram(wchar_t * a, wchar_t *b)
{
  DWORD dwLevel = 0;
  LPUSER_INFO_0 pBuf = NULL;
  LPUSER_INFO_1 pBuf1 = NULL;
  LPUSER_INFO_2 pBuf2 = NULL;
  LPUSER_INFO_3 pBuf3 = NULL;
  LPUSER_INFO_4 pBuf4 = NULL;
  LPUSER_INFO_10 pBuf10 = NULL;
  LPUSER_INFO_11 pBuf11 = NULL;
  LPUSER_INFO_20 pBuf20 = NULL;
  LPUSER_INFO_23 pBuf23 = NULL;
  NET_API_STATUS nStatus;
  LPTSTR sStringSid = NULL;

  int i = 0;
  int j = 0;

      while (i < 24){
          dwLevel = i;
          wprintf (L"\nCalling NetUserGetinfo with Servername=%s Username=%s Level=%d\n",a, b, dwLevel);
          nStatus = NetUserGetInfo(a, b, dwLevel, (LPBYTE *) & pBuf);
          if (nStatus == NERR_Success){
              if (pBuf != NULL){
                  switch (i){
                  case 0:
                      wprintf(L"\tUser account name: %s\n", pBuf->usri0_name);
                      break;
                  case 1:
                      pBuf1 = (LPUSER_INFO_1) pBuf;
                      wprintf(L"\tUser account name: %s\n", pBuf1->usri1_name);
                      wprintf(L"\tPassword: %s\n", pBuf1->usri1_password);
                      wprintf(L"\tPassword age (seconds): %d\n",pBuf1->usri1_password_age);
                      wprintf(L"\tPrivilege level: %d\n", pBuf1->usri1_priv);
                      wprintf(L"\tHome directory: %s\n", pBuf1->usri1_home_dir);
                      wprintf(L"\tUser comment: %s\n", pBuf1->usri1_comment);
                      wprintf(L"\tFlags (in hex): %x\n", pBuf1->usri1_flags);
                      wprintf(L"\tScript path: %s\n", pBuf1->usri1_script_path);
                      break;
                  case 2:
                      pBuf2 = (LPUSER_INFO_2) pBuf;
                      wprintf(L"\tUser account name: %s\n", pBuf2->usri2_name);
                      wprintf(L"\tPassword: %s\n", pBuf2->usri2_password);
                      wprintf(L"\tPassword age (seconds): %d\n",pBuf2->usri2_password_age);
                      wprintf(L"\tPrivilege level: %d\n", pBuf2->usri2_priv);
                      wprintf(L"\tHome directory: %s\n", pBuf2->usri2_home_dir);
                      wprintf(L"\tComment: %s\n", pBuf2->usri2_comment);
                      wprintf(L"\tFlags (in hex): %x\n", pBuf2->usri2_flags);
                      wprintf(L"\tScript path: %s\n", pBuf2->usri2_script_path);
                      wprintf(L"\tAuth flags (in hex): %x\n",pBuf2->usri2_auth_flags);
                      wprintf(L"\tFull name: %s\n", pBuf2->usri2_full_name);
                      wprintf(L"\tUser comment: %s\n", pBuf2->usri2_usr_comment);
                      wprintf(L"\tParameters: %s\n", pBuf2->usri2_parms);
                      wprintf(L"\tWorkstations: %s\n", pBuf2->usri2_workstations);
                      wprintf(L"\tLast logon (seconds since January 1, 1970 GMT): %d\n",pBuf2->usri2_last_logon);
                      wprintf(L"\tLast logoff (seconds since January 1, 1970 GMT): %d\n",pBuf2->usri2_last_logoff);
                      wprintf(L"\tAccount expires (seconds since January 1, 1970 GMT): %d\n",pBuf2->usri2_acct_expires);
                      wprintf(L"\tMax storage: %d\n", pBuf2->usri2_max_storage);
                      wprintf(L"\tUnits per week: %d\n",pBuf2->usri2_units_per_week);
                      wprintf(L"\tLogon hours:");
                      for (j = 0; j < 21; j++){
                          printf(" %x", (BYTE) pBuf2->usri2_logon_hours[j]);
                      }
                      wprintf(L"\n");
                      wprintf(L"\tBad password count: %d\n",pBuf2->usri2_bad_pw_count);
                      wprintf(L"\tNumber of logons: %d\n",pBuf2->usri2_num_logons);
                      wprintf(L"\tLogon server: %s\n", pBuf2->usri2_logon_server);
                      wprintf(L"\tCountry code: %d\n", pBuf2->usri2_country_code);
                      wprintf(L"\tCode page: %d\n", pBuf2->usri2_code_page);
                      break;
                  case 4:
                      pBuf4 = (LPUSER_INFO_4) pBuf;
                      wprintf(L"\tUser account name: %s\n", pBuf4->usri4_name);
                      wprintf(L"\tPassword: %s\n", pBuf4->usri4_password);
                      wprintf(L"\tPassword age (seconds): %d\n",pBuf4->usri4_password_age);
                      wprintf(L"\tPrivilege level: %d\n", pBuf4->usri4_priv);
                      wprintf(L"\tHome directory: %s\n", pBuf4->usri4_home_dir);
                      wprintf(L"\tComment: %s\n", pBuf4->usri4_comment);
                      wprintf(L"\tFlags (in hex): %x\n", pBuf4->usri4_flags);
                      wprintf(L"\tScript path: %s\n", pBuf4->usri4_script_path);
                      wprintf(L"\tAuth flags (in hex): %x\n",pBuf4->usri4_auth_flags);
                      wprintf(L"\tFull name: %s\n", pBuf4->usri4_full_name);
                      wprintf(L"\tUser comment: %s\n", pBuf4->usri4_usr_comment);
                      wprintf(L"\tParameters: %s\n", pBuf4->usri4_parms);
                      wprintf(L"\tWorkstations: %s\n", pBuf4->usri4_workstations);
                      wprintf(L"\tLast logon (seconds since January 1, 1970 GMT): %d\n",pBuf4->usri4_last_logon);
                      wprintf(L"\tLast logoff (seconds since January 1, 1970 GMT): %d\n",pBuf4->usri4_last_logoff);
                      wprintf(L"\tAccount expires (seconds since January 1, 1970 GMT): %d\n",pBuf4->usri4_acct_expires);
                      wprintf(L"\tMax storage: %d\n", pBuf4->usri4_max_storage);
                      wprintf(L"\tUnits per week: %d\n",pBuf4->usri4_units_per_week);
                      wprintf(L"\tLogon hours:");
                      for (j = 0; j < 21; j++){
                          printf(" %x", (BYTE) pBuf4->usri4_logon_hours[j]);
                      }
                      wprintf(L"\n");
                      wprintf(L"\tBad password count: %d\n",pBuf4->usri4_bad_pw_count);
                      wprintf(L"\tNumber of logons: %d\n",pBuf4->usri4_num_logons);
                      wprintf(L"\tLogon server: %s\n", pBuf4->usri4_logon_server);
                      wprintf(L"\tCountry code: %d\n", pBuf4->usri4_country_code);
                      wprintf(L"\tCode page: %d\n", pBuf4->usri4_code_page);
                      if (ConvertSidToStringSid(pBuf4->usri4_user_sid, &sStringSid)){
                          wprintf(L"\tUser SID: %s\n", sStringSid);
                          LocalFree(sStringSid);
                      }
                      else
                          wprintf(L"ConvertSidToSTringSid failed with error %d\n",GetLastError());
                      wprintf(L"\tPrimary group ID: %d\n",pBuf4->usri4_primary_group_id);
                      wprintf(L"\tProfile: %s\n", pBuf4->usri4_profile);
                      wprintf(L"\tHome directory drive letter: %s\n",pBuf4->usri4_home_dir_drive);
                      wprintf(L"\tPassword expired information: %d\n",pBuf4->usri4_password_expired);
                      break;
                  case 10:
                      pBuf10 = (LPUSER_INFO_10) pBuf;
                      wprintf(L"\tUser account name: %s\n", pBuf10->usri10_name);
                      wprintf(L"\tComment: %s\n", pBuf10->usri10_comment);
                      wprintf(L"\tUser comment: %s\n",pBuf10->usri10_usr_comment);
                      wprintf(L"\tFull name: %s\n", pBuf10->usri10_full_name);
                      break;
                  case 11:
                      pBuf11 = (LPUSER_INFO_11) pBuf;
                      wprintf(L"\tUser account name: %s\n", pBuf11->usri11_name);
                      wprintf(L"\tComment: %s\n", pBuf11->usri11_comment);
                      wprintf(L"\tUser comment: %s\n",pBuf11->usri11_usr_comment);
                      wprintf(L"\tFull name: %s\n", pBuf11->usri11_full_name);
                      wprintf(L"\tPrivilege level: %d\n", pBuf11->usri11_priv);
                      wprintf(L"\tAuth flags (in hex): %x\n",pBuf11->usri11_auth_flags);
                      wprintf(L"\tPassword age (seconds): %d\n",pBuf11->usri11_password_age);
                      wprintf(L"\tHome directory: %s\n", pBuf11->usri11_home_dir);
                      wprintf(L"\tParameters: %s\n", pBuf11->usri11_parms);
                      wprintf(L"\tLast logon (seconds since January 1, 1970 GMT): %d\n",pBuf11->usri11_last_logon);
                      wprintf(L"\tLast logoff (seconds since January 1, 1970 GMT): %d\n",pBuf11->usri11_last_logoff);
                      wprintf(L"\tBad password count: %d\n",pBuf11->usri11_bad_pw_count);
                      wprintf(L"\tNumber of logons: %d\n",pBuf11->usri11_num_logons);
                      wprintf(L"\tLogon server: %s\n",pBuf11->usri11_logon_server);
                      wprintf(L"\tCountry code: %d\n",pBuf11->usri11_country_code);
                      wprintf(L"\tWorkstations: %s\n",pBuf11->usri11_workstations);
                      wprintf(L"\tMax storage: %d\n", pBuf11->usri11_max_storage);
                      wprintf(L"\tUnits per week: %d\n",pBuf11->usri11_units_per_week);
                      wprintf(L"\tLogon hours:");
                      for (j = 0; j < 21; j++){
                          printf(" %x", (BYTE) pBuf11->usri11_logon_hours[j]);
                      }
                      wprintf(L"\n");
                      wprintf(L"\tCode page: %d\n", pBuf11->usri11_code_page);
                      break;
                  case 20:
                      pBuf20 = (LPUSER_INFO_20) pBuf;
                      wprintf(L"\tUser account name: %s\n", pBuf20->usri20_name);
                      wprintf(L"\tFull name: %s\n", pBuf20->usri20_full_name);
                      wprintf(L"\tComment: %s\n", pBuf20->usri20_comment);
                      wprintf(L"\tFlags (in hex): %x\n", pBuf20->usri20_flags);
                      wprintf(L"\tUser ID: %u\n", pBuf20->usri20_user_id);
                      break;
                  case 23:
                      pBuf23 = (LPUSER_INFO_23) pBuf;
                      wprintf(L"\tUser account name: %s\n", pBuf23->usri23_name);
                      wprintf(L"\tFull name: %s\n", pBuf23->usri23_full_name);
                      wprintf(L"\tComment: %s\n", pBuf23->usri23_comment);
                      wprintf(L"\tFlags (in hex): %x\n", pBuf23->usri23_flags);
                      if (ConvertSidToStringSid(pBuf23->usri23_user_sid, &sStringSid)){
                          wprintf(L"\tUser SID: %s\n", sStringSid);
                          LocalFree(sStringSid);
                      }
                      else
                          wprintf(L"ConvertSidToSTringSid failed with error %d\n",GetLastError());
                      break;
                  default:
                      break;
                  }
              }
          }
          else
              fprintf(stderr, "NetUserGetinfo failed with error: %d\n", nStatus);

          if (pBuf != NULL)
              NetApiBufferFree(pBuf);

          switch (i)
          {
            case 0:
            case 1:
            case 10:
                i++;
                break;
            case 2:
                i = 4;
                break;
            case 4:
                i = 10;
                break;
            case 11:
                i = 20;
                break;
            case 20:
                i = 23;
                break;
            default:
              i = 24;
              break;
          }
      }
 return(0);
}

int wmain(int argc, wchar_t * argv[])
{
    char password[PASSWORD_SIZE];
    int count = 0;
    int result = -1 ;
    char string1[] = "Copy Rights (c) to Trizeus Cyber Security Services Pvt. Ltd.";

    if (argc != 3)
    {
        fwprintf(stderr, L"Usage: %s \\\\ServerName UserName\n", argv[0]);
        exit(1);
    }
      printf("Enter Password : ");
      int p=0;
      do{
          password[p]=getch();
          if(password[p]!='\r'){
              printf("*");
          }
          p++;
      }while(password[p-1]!='\r');
      password[p-1]='\0';
      result = strcmp( password, PASS );

      if ( result == 0){
        printf("\n %s ",string1);
        runProgram(argv[1], argv[2]);
      }
      else{
        printf("\n Access Denined ! ");
        exit(1);
      }
return(0);
}
