#ifndef _FILEINFO_H_INCLUDED
#define _FILEINFO_H_INCLUDED

#include <windows.h>
#include <stdio.h>

#define TB 1000000000000
#define GB 1000000000
#define MB 1000000
#define KB 1000

typedef struct {
#define FI_MAX_ATTRIBUTES_WIDTH 6
#define FI_MAX_SIZE_WIDTH 6
#define FI_MAX_DATE_WIDTH 10
#define FI_MAX_TIME_WIDTH 5
  WCHAR  FileName[MAX_PATH];
  char   SymbolicAttributes[FI_MAX_ATTRIBUTES_WIDTH + 1];
  char   FileSize[FI_MAX_SIZE_WIDTH + 1];
  char   WriteDate[FI_MAX_DATE_WIDTH + 1];
  char   WriteTime[FI_MAX_TIME_WIDTH + 1];
  UINT64 iFileSize;
  UINT64 iWriteTime;
} FileInfo;

void ConstructFileInfo(FileInfo *Destination, WCHAR *FileName, WIN32_FILE_ATTRIBUTE_DATA FileExInfo);

int fiCompBySize(const void *a, const void *b);

int fiCompByTime(const void *a, const void *b);

#endif // !_FILEINFO_H_INCLUDED
