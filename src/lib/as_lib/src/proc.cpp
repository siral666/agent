/*
 * Copyright (C) 2020-2025 ASHINi corp. 
 * 
 * This library is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU Lesser General Public 
 * License as published by the Free Software Foundation; either 
 * version 2.1 of the License, or (at your option) any later version. 
 * 
 * This library is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * Lesser General Public License for more details. 
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library; if not, write to the Free Software 
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA 
 * 
 */

#include "stdafx.h"
#include "as_parse.h"
#include "as_proc.h"
#include "as_util.h"


/*
 * 해당 프로세스의 이름과 부모 프로세스 ID를 얻는다
 */
int get_proc_info_by_pid(int pid, proc_info_t *pProcInfo)
{
	FILE *fd = NULL;
	char *pTok, *pLast;
	char acBuf[256] = {0,};
	char acDelim[2] = {0x09, 0x00 };
	char acStatPath[MAX_PROC_PATH] = {0,};

	if(pProcInfo == NULL)
	{
		return -1;
	}

	snprintf(acStatPath, MAX_PROC_PATH-1, "/proc/%d/status", pid);
	acStatPath[MAX_PROC_PATH] = 0;
	
	fd = fopen(acStatPath, "r");
	if (fd == NULL)
	{
		return -2;
	}

	memset(acBuf, 0x00, sizeof(acBuf));

	while (fgets(acBuf, sizeof(acBuf), fd))
	{
		trim_line_feed(acBuf);
		pTok = strtok_r(acBuf, acDelim, &pLast);
		if(pTok == NULL)
			continue;

		if (!_stricmp(pTok, "Name:"))
		{
			pTok = strtok_r(NULL, acDelim, &pLast);
			if(pTok != NULL)
			{
				snprintf(pProcInfo->name, MAX_PROC_PATH-1, "%s", pTok);
				pProcInfo->name[MAX_PROC_PATH-1] = 0;
			}
			memset(acBuf, 0x00, sizeof(acBuf));
			continue;

		}
		else if (!_stricmp(pTok, "PPid:"))
		{
			pTok = strtok_r(NULL, acDelim, &pLast);
			if(pTok != NULL)
				pProcInfo->ppid = atoi(pTok);
			memset(acBuf, 0x00, sizeof(acBuf));
			break;
		}
		else
		{
			memset(acBuf, 0x00, sizeof(acBuf));
			continue;
		}
	}

	fclose(fd);
	return 0;
}


/* PID 저장 파일에 저장된 PID를 이용해 프로세스 정보를 얻어와서
 * 해당 PID가 현재 실행중인지 체크하여 결과를 리턴
 *
 * @return
 *
 * -1 : 프로세스 정보를 얻는데 실패함
 * ASI_PROC_NOT_EXIST : 해당 프로세스 존재하지 않음
 * asi_PROC_EXIST : 해당 프로세스 존재
 */
int check_proc_exist_by_pid_file(const char *acPidPath, const char *acProcName)
{
	int fd = -1;
	char acPid[32] = {0,};
	char acPidFile[MAX_PROC_PATH]= {0,};
	struct stat	f_stat;
	proc_info_t stProcInfo;

	if (acPidPath == NULL || acProcName == NULL)
	{
		return -1;
	}

	/* PID 저장 파일 */
	snprintf(acPidFile, MAX_PROC_PATH-1, "%s/%s.pid", acPidPath, acProcName);
	acPidFile[MAX_PROC_PATH-1] = 0;

	if (stat(acPidFile, &f_stat) != 0)
	{
		return ASI_PROC_NOT_EXIST;
	}

	if ( (fd = open(acPidFile, O_RDONLY)) == -1)
	{
		return -2;
	}

	memset(acPid, 0x00, sizeof(acPid));

	if (read(fd, acPid, sizeof(acPid)-1) <= 0)
	{
		close(fd);
		return -3;
	}
	else
	{
		close(fd);
	}

	trim_space(acPid);
	if (is_valid_num(acPid) == -1)
	{
		return -4;
	}

	memset(&stProcInfo, 0x00, sizeof(stProcInfo));
	if (get_proc_info_by_pid(atoi(acPid), &stProcInfo) == -1)
	{
		return ASI_PROC_NOT_EXIST;
	}

	if (!_stricmp(acProcName, stProcInfo.name))
	{
		return ASI_PROC_EXIST;
	}
	else
	{
		return ASI_PROC_NOT_EXIST;
	}
}


/* 프로세스명을 이용해 해당 프로세스가 존재하는지
 * 체크하여 결과를 리턴
 *
 * @return
 *
 * -1 : 프로세스 정보를 얻는데 실패함
 * ASI_PROC_NOT_EXIST : 해당 프로세스 존재하지 않음
 * asi_PROC_EXIST : 해당 프로세스 존재
 */
int check_proc_exist_by_name(const char *acProcName, int nExceptPid)
{
	FILE *fp = NULL;
	DIR *dir = NULL;
	char *tok, *last;
	char buf[256] = { 0,};
	char acFileName[MAX_PROC_PATH] = {0,};
	char *pcPath = NULL;
	char delim[3] = { 0x09, 0x20, 0x00 }; /* tab, space */
	struct dirent entry, *result = NULL;

	pcPath = (char *)malloc(MAX_PROC_PATH);
	if(pcPath == NULL)
	{
		return -1;
	}
	memset(pcPath, 0, MAX_PROC_PATH);

	if ( (dir = opendir("/proc")) == NULL)
	{
		safe_free(pcPath);
		return -2;
	}

	while (readdir_r(dir, &entry, &result) == 0 && result != NULL)
	{
		if (entry.d_name[0] < '0' || entry.d_name[0] > '9')
		{
			continue;
		}
		strncpy(acFileName, entry.d_name, MAX_PROC_PATH-1);
		acFileName[MAX_PROC_PATH-1] = 0;
		snprintf(pcPath, MAX_PROC_PATH-1, "/proc/%s/status", acFileName);
		pcPath[MAX_PROC_PATH-1] = 0;

		if ( (fp = fopen(pcPath, "r")) == NULL)
		{
			continue;
		}

		while (fgets(buf, sizeof(buf), fp))
		{
			trim_line_feed(buf);
			tok = strtok_r(buf, delim, &last);
			if (tok == NULL)
			{
				memset(buf, 0x00, sizeof(buf));
				continue;
			}

			if (strcmp(tok, "Name:") != 0)
			{
				memset(buf, 0x00, sizeof(buf));
				continue;
			}

			tok = strtok_r(NULL, "", &last);
			if (tok == NULL)
			{
				memset(buf, 0x00, sizeof(buf));
				continue;
			}

			if (strcmp(tok, acProcName) != 0)
			{
				memset(buf, 0x00, sizeof(buf));
				break;
			}
			else
			{
				if ( nExceptPid != 0 && nExceptPid == atoi(entry.d_name))
				{
					break;
				}
				fclose(fp);
				closedir(dir);
				safe_free(pcPath);
				return ASI_PROC_EXIST;
			}
		}

		fclose(fp);
	}

	closedir(dir);
	safe_free(pcPath);

	return ASI_PROC_NOT_EXIST;
}


int make_pid_file(const char *acPidPath, const char *acProcName, pid_t pid)
{
	FILE *fp = NULL;
	char acPidFile[MAX_PROC_PATH] = { 0,};
	struct stat  f_stat;

	/* PID 파일 저장 경로가 없으면 생성 */
	if (stat(acPidFile, &f_stat) != 0)
	{
		if (make_dir(acPidFile) == -1)
		{
			return -1;
		}
	}

	/* PID 저장 파일 생성 */
	snprintf(acPidFile, MAX_PROC_PATH-1, "%s/%s.pid", acPidPath, acProcName);
	acPidFile[MAX_PROC_PATH-1] = 0;
	
	fp = fopen(acPidFile, "w");
	if (fp == NULL)
	{
		return -2;
	}
	fprintf(fp, "%d", pid);
	fclose(fp);
	return 0;
}

