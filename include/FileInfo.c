#include "FileInfo.h"

void ConstructFileInfo(FileInfo *Destination, WCHAR *FileName, WIN32_FILE_ATTRIBUTE_DATA FileExInfo)
{
  DWORD FileAttributes = FileExInfo.dwFileAttributes;
  wcscpy(Destination->FileName, FileName);

  Destination->SymbolicAttributes[0] = (FileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? 'd' : (FileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) ? 'l' : '-';
  Destination->SymbolicAttributes[1] = (FileAttributes & FILE_ATTRIBUTE_ARCHIVE) ? 'a' : '-';
  Destination->SymbolicAttributes[2] = (FileAttributes & FILE_ATTRIBUTE_HIDDEN) ? 'h' : '-';
  Destination->SymbolicAttributes[3] = (FileAttributes & FILE_ATTRIBUTE_READONLY) ? 'r' : '-';
  Destination->SymbolicAttributes[4] = (FileAttributes & FILE_ATTRIBUTE_SYSTEM) ? 's' : '-';
  Destination->SymbolicAttributes[5] = (FileAttributes & FILE_ATTRIBUTE_ENCRYPTED) ? 'e' : '-';
  Destination->SymbolicAttributes[6] = '\0';

  UINT64 iFileSize = ((UINT64)FileExInfo.nFileSizeHigh << 32) | FileExInfo.nFileSizeLow;
  Destination->iFileSize = iFileSize;
  if (iFileSize >= TB) {
    sprintf(Destination->FileSize, "%lluTB", iFileSize / TB);
  }
  else if (iFileSize >= GB) {
    sprintf(Destination->FileSize, "%lluGB", iFileSize / GB);
  }
  else if (iFileSize >= MB) {
    sprintf(Destination->FileSize, "%lluMB", iFileSize / MB);
  }
  else if (iFileSize >= KB) {
    sprintf(Destination->FileSize, "%lluKB", iFileSize / KB);
  }
  else {
    sprintf(Destination->FileSize, "%lluB", iFileSize);
  }

  Destination->iWriteTime = ((UINT64)FileExInfo.ftLastWriteTime.dwHighDateTime << 32) | FileExInfo.ftLastWriteTime.dwLowDateTime;

  SYSTEMTIME SystemTime;
  FileTimeToSystemTime(&FileExInfo.ftLastWriteTime, &SystemTime);

  sprintf(Destination->WriteDate, "%04d-%02d-%02d", SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay);
  sprintf(Destination->WriteTime, "%02d:%02d", SystemTime.wHour, SystemTime.wMinute);
}

int fiCompBySize(const void *a, const void *b)
{
  return (((FileInfo *)a)->iFileSize < ((FileInfo *)b)->iFileSize) - (((FileInfo *)a)->iFileSize > ((FileInfo *)b)->iFileSize);
}

int fiCompByTime(const void *a, const void *b)
{
  UINT64 sTime_a = ((FileInfo *)a)->iWriteTime;
  UINT64 sTime_b = ((FileInfo *)b)->iWriteTime;
  return (sTime_a < sTime_b) - (sTime_a > sTime_b);
}
