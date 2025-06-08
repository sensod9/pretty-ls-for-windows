#include "../include/FileInfo.h"

#include <shlwapi.h>
#include <locale.h>

// cmd text colors
#define COLOR_BLACK         0
#define COLOR_BLUE          1
#define COLOR_GREEN         2
#define COLOR_CYAN          3
#define COLOR_RED           4
#define COLOR_MAGENTA       5
#define COLOR_YELLOW        6
#define COLOR_WHITE         7
#define COLOR_GRAY          8
#define COLOR_LIGHT_BLUE    9
#define COLOR_LIGHT_GREEN   10
#define COLOR_LIGHT_CYAN    11
#define COLOR_LIGHT_RED     12
#define COLOR_LIGHT_MAGENTA 13
#define COLOR_LIGHT_YELLOW  14
#define COLOR_BRIGHT_WHITE  15

#define MAX_FILES 1700 // Windows buffer size is only 1MB (!)

void SetMatchingColor(HANDLE hConsoleOutput, WCHAR *CurrentDirectory, WCHAR *FileName)
{
  WCHAR FilePath[MAX_PATH];
  wcscpy(FilePath, CurrentDirectory);
  wcscat(FilePath, FileName);

  DWORD FileAttributes = GetFileAttributesW(FilePath);

  if (FileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
    SetConsoleTextAttribute(hConsoleOutput, COLOR_LIGHT_BLUE);
  }
  else if (FileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
    SetConsoleTextAttribute(hConsoleOutput, COLOR_LIGHT_CYAN);

  else {
    WCHAR *wFileFormat = wcsrchr(FileName, '.');
    if (wFileFormat != NULL) {
      wFileFormat++;
      char FileFormat[32];
      wcstombs(FileFormat, wFileFormat, 32);

      if (!strcmp(FileFormat, "jpg") || !strcmp(FileFormat, "xbm") ||
          !strcmp(FileFormat, "pcx") || !strcmp(FileFormat, "mp4") ||
          !strcmp(FileFormat, "rm") || !strcmp(FileFormat, "xcf") ||
          !strcmp(FileFormat, "ogx") || !strcmp(FileFormat, "jpeg") ||
          !strcmp(FileFormat, "xpm") || !strcmp(FileFormat, "now") ||
          !strcmp(FileFormat, "m4v") || !strcmp(FileFormat, "ico") ||
          !strcmp(FileFormat, "mkv") || !strcmp(FileFormat, "gif") ||
          !strcmp(FileFormat, "tiff") || !strcmp(FileFormat, "mpg") ||
          !strcmp(FileFormat, "mp4v") || !strcmp(FileFormat, "flc") ||
          !strcmp(FileFormat, "guv") || !strcmp(FileFormat, "bmp") ||
          !strcmp(FileFormat, "tif") || !strcmp(FileFormat, "mpeg") ||
          !strcmp(FileFormat, "wob") || !strcmp(FileFormat, "avi") ||
          !strcmp(FileFormat, "psd") || !strcmp(FileFormat, "svg") ||
          !strcmp(FileFormat, "flv") || !strcmp(FileFormat, "axv") ||
          !strcmp(FileFormat, "webm") || !strcmp(FileFormat, "umv") ||
          !strcmp(FileFormat, "png") || !strcmp(FileFormat, "asf") ||
          !strcmp(FileFormat, "dl") || !strcmp(FileFormat, "ogv"))
        SetConsoleTextAttribute(hConsoleOutput, COLOR_LIGHT_MAGENTA);

      else if (!strcmp(FileFormat, "tar") || !strcmp(FileFormat, "tgz") ||
              !strcmp(FileFormat, "arj") || !strcmp(FileFormat, "taz") ||
              !strcmp(FileFormat, "lzh") || !strcmp(FileFormat, "lzna") ||
              !strcmp(FileFormat, "tlz") || !strcmp(FileFormat, "txz") ||
              !strcmp(FileFormat, "zip") || !strcmp(FileFormat, "z") ||
              !strcmp(FileFormat, "Z") || !strcmp(FileFormat, "dz") ||
              !strcmp(FileFormat, "gz") || !strcmp(FileFormat, "lz") ||
              !strcmp(FileFormat, "xz") || !strcmp(FileFormat, "bz2") ||
              !strcmp(FileFormat, "bz") || !strcmp(FileFormat, "tbz") ||
              !strcmp(FileFormat, "tbz2") || !strcmp(FileFormat, "tz") ||
              !strcmp(FileFormat, "deb") || !strcmp(FileFormat, "rpm") ||
              !strcmp(FileFormat, "jar") || !strcmp(FileFormat, "uar") ||
              !strcmp(FileFormat, "rar") || !strcmp(FileFormat, "ace") ||
              !strcmp(FileFormat, "7z") || !strcmp(FileFormat, "rz"))
        SetConsoleTextAttribute(hConsoleOutput, COLOR_RED);

      else if (!strcmp(FileFormat, "aac") || !strcmp(FileFormat, "au") ||
              !strcmp(FileFormat, "flac") || !strcmp(FileFormat, "mid") ||
              !strcmp(FileFormat, "midi") || !strcmp(FileFormat, "mka") ||
              !strcmp(FileFormat, "mp3") || !strcmp(FileFormat, "mpc") ||
              !strcmp(FileFormat, "ogg") || !strcmp(FileFormat, "ra") ||
              !strcmp(FileFormat, "uav") || !strcmp(FileFormat, "axa") ||
              !strcmp(FileFormat, "oga") || !strcmp(FileFormat, "spx") ||
              !strcmp(FileFormat, "xspf"))
        SetConsoleTextAttribute(hConsoleOutput, COLOR_CYAN);
      
      else if (!strcmp(FileFormat, "vbs") || !strcmp(FileFormat, "sh") ||
              !strcmp(FileFormat, "bash") || !strcmp(FileFormat, "zsh") ||
              !strcmp(FileFormat, "py") || !strcmp(FileFormat, "pl") ||
              !strcmp(FileFormat, "rb") || !strcmp(FileFormat, "run") ||
              !strcmp(FileFormat, "bin") || !strcmp(FileFormat, "out") ||
              !strcmp(FileFormat, "exe") || !strcmp(FileFormat, "AppImage") ||
              !strcmp(FileFormat, "elf") || !strcmp(FileFormat, "so") ||
              !strcmp(FileFormat, "ko") || !strcmp(FileFormat, "bat") ||
              !strcmp(FileFormat, "cmd") || !strcmp(FileFormat, "ps1") ||
              !strcmp(FileFormat, "msi") || !strcmp(FileFormat, "com"))
        SetConsoleTextAttribute(hConsoleOutput, COLOR_GREEN);

      else if (!strcmp(FileFormat, "url") || !strcmp(FileFormat, "lnk"))
        SetConsoleTextAttribute(hConsoleOutput, COLOR_LIGHT_CYAN);

      else
        SetConsoleTextAttribute(hConsoleOutput, COLOR_WHITE);
    }
    else {
      SetConsoleTextAttribute(hConsoleOutput, COLOR_GREEN);
    }
  }
}

void CalculateLongListingWidths(FileInfo *FileInfoArray, UINT16 FilesAmount, UINT16 NameWidths[], UINT16 *MaxNameWidth, UINT16 *MaxSizeWidth)
{
  for (UINT16 index = 0; index < FilesAmount; index++) {
    UINT16 CurNameWidth = wcslen(FileInfoArray[index].FileName);
    UINT16 CurSizeWidth = strlen(FileInfoArray[index].FileSize);
    NameWidths[index] = CurNameWidth;
    if (CurNameWidth > *MaxNameWidth) *MaxNameWidth = CurNameWidth;
    if (CurSizeWidth > *MaxSizeWidth) *MaxSizeWidth = CurSizeWidth;
  }
}

void PrintLongListing(HANDLE hConsoleOutput, CONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo,
                      WCHAR *CurrentDirectory, UINT16 FilesAmount, FileInfo *FileInfoArray)
{
  UINT16 ConsoleWidth = lpConsoleScreenBufferInfo.dwSize.X;

  UINT16 NameWidths[FilesAmount];
  UINT16 MaxNameWidth = 0, MaxSizeWidth = 0;
  CalculateLongListingWidths(FileInfoArray, FilesAmount, NameWidths, &MaxNameWidth, &MaxSizeWidth);
  
  UINT8 format = (ConsoleWidth >  FI_MAX_ATTRIBUTES_WIDTH + MaxSizeWidth + FI_MAX_DATE_WIDTH + FI_MAX_TIME_WIDTH + MaxNameWidth + 4) ? 4 : 
                 (ConsoleWidth == FI_MAX_ATTRIBUTES_WIDTH + MaxSizeWidth + FI_MAX_DATE_WIDTH + FI_MAX_TIME_WIDTH + MaxNameWidth + 4) ? 3 : 
                 (ConsoleWidth >  FI_MAX_ATTRIBUTES_WIDTH + MaxSizeWidth + FI_MAX_DATE_WIDTH + FI_MAX_TIME_WIDTH + 3) ? 2 : 
                 (ConsoleWidth == FI_MAX_ATTRIBUTES_WIDTH + MaxSizeWidth + FI_MAX_DATE_WIDTH + FI_MAX_TIME_WIDTH + 3) ? 1 : 0;
;

  for (UINT16 index = 0; index < FilesAmount; index++) {
    printf("%s %*s %s %s", FileInfoArray[index].SymbolicAttributes, MaxSizeWidth, FileInfoArray[index].FileSize, FileInfoArray[index].WriteDate, FileInfoArray[index].WriteTime);
    if (format == 4 || format == 3) {
      SetMatchingColor(hConsoleOutput, CurrentDirectory, FileInfoArray[index].FileName);
      wprintf(L" %s", FileInfoArray[index].FileName);
      SetConsoleTextAttribute(hConsoleOutput, COLOR_WHITE);
      if (format == 4 || NameWidths[index] < MaxNameWidth)
        putchar('\n');
    }
    else {
      if (format == 2)
        putchar('\n');
      else if (format == 0)
        putchar(' ');
      SetMatchingColor(hConsoleOutput, CurrentDirectory, FileInfoArray[index].FileName);
      wprintf(L"%s\n", FileInfoArray[index].FileName);
      SetConsoleTextAttribute(hConsoleOutput, COLOR_WHITE);
    }
  }
  printf("%u files\n", FilesAmount);
}

void CalculateShortListingWidths(FileInfo *FileInfoArray, UINT16 FilesAmount, UINT16 NameWidths[])
{
  for (UINT16 index = 0; index < FilesAmount; index++) {
    UINT16 CurNameWidth = wcslen(FileInfoArray[index].FileName);
    NameWidths[index] = CurNameWidth;
  }
}

void PrintShortListing(HANDLE hConsoleOutput, CONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo,
                       WCHAR *CurrentDirectory, UINT16 FilesAmount,
                       FileInfo FileInfoArray[FilesAmount], BOOL flag_1)
{
  UINT16 NameWidths[FilesAmount];
  CalculateShortListingWidths(FileInfoArray, FilesAmount, NameWidths);

  UINT16 ConsoleWidth = lpConsoleScreenBufferInfo.dwSize.X;
  UINT16 MaxColumns = 1;

  UINT16 MaxColsWidths[ConsoleWidth / 3 + ConsoleWidth % 3];
  for (UINT16 cols = (!flag_1) ? (ConsoleWidth / 3 + ConsoleWidth % 3) : 1; cols > 0; cols--) {
    for (UINT16 i = 0; i < cols; i++) {
      MaxColsWidths[i] = 0;
    }

    if (cols < FilesAmount) {
      UINT16 AllIndex = 0;
      int LastRowSize = FilesAmount % cols;
      for (UINT16 col = 0; col < cols; col++) {
        UINT16 rows = FilesAmount / cols + (LastRowSize-- > 0 ? 1 : 0);
        for (UINT16 index = 0; index < rows; index++) {
          UINT16 CurWidth = NameWidths[AllIndex++];
          if (CurWidth > MaxColsWidths[col]) {
            MaxColsWidths[col] = CurWidth;
          }
        }
      }
    } else if (cols < ConsoleWidth / 3 + ConsoleWidth % 3)
      continue;
    else {
      UINT16 ColsWidthSum = 0;
      for (UINT16 index = 0; index < FilesAmount; index++) {
        MaxColsWidths[index] = NameWidths[index];
        ColsWidthSum += NameWidths[index];
      }

      if (ColsWidthSum + ((FilesAmount - 1) * 2) <= ConsoleWidth) {
        MaxColumns = FilesAmount;
        break;
      }
    }

    UINT16 ColsWidthSum = 0;
    for (UINT16 i = 0; i < cols; i++) {
      ColsWidthSum += MaxColsWidths[i];
    }
    if (ColsWidthSum + ((cols - 1) * 2) <= ConsoleWidth) {
      MaxColumns = cols;
      break;
    }
  }
  const UINT16 MaxRows = FilesAmount / MaxColumns + (FilesAmount % MaxColumns ? 1 : 0);
  UINT16 ConstLastRowSize = FilesAmount % MaxColumns;
  if (!ConstLastRowSize) ConstLastRowSize = MaxColumns;

  if (MaxRows != 1) {
    for (UINT16 row = 0; row < MaxRows; row++) {
      UINT16 LastCharIndex = 0;
      INT16 LastRowSize = FilesAmount % MaxColumns;
      if (!LastRowSize) LastRowSize = FilesAmount / MaxColumns;
      for (UINT16 col = 0; col < ((row != MaxRows - 1) ? MaxColumns : ConstLastRowSize); col++) {
        UINT16 NameIndex = row + (col * (FilesAmount / MaxColumns + ((FilesAmount % MaxColumns) ? 1 : 0))) + (LastRowSize < 0 ? LastRowSize : 0);
        LastRowSize--;

        SetMatchingColor(hConsoleOutput, CurrentDirectory, FileInfoArray[NameIndex].FileName);
        wprintf(L"%s", FileInfoArray[NameIndex].FileName);
        LastCharIndex += NameWidths[NameIndex];
        if (col != MaxColumns - 1) {
          for (UINT16 k = 0; k < (MaxColsWidths[col] + 2) - NameWidths[NameIndex]; k++) {
            putchar(' ');
            LastCharIndex++;
          }
        }
      }
      if (LastCharIndex != ConsoleWidth && row != MaxRows - 1) {
        putchar('\n');
      }
    }
  }
  else {
    for (UINT16 index = 0; index < FilesAmount - 1; index++) {
      SetMatchingColor(hConsoleOutput, CurrentDirectory, FileInfoArray[index].FileName);
      wprintf(L"%s  ", FileInfoArray[index].FileName);
    }
    SetMatchingColor(hConsoleOutput, CurrentDirectory, FileInfoArray[FilesAmount - 1].FileName);
    wprintf(L"%s", FileInfoArray[FilesAmount - 1].FileName);
  }
}

void ReverseFileInfoArray(UINT16 FilesAmount, FileInfo FileInfoArray[])
{
  FileInfo TempFileInfoArray[FilesAmount];
  for (UINT16 index = 0; index < FilesAmount; index++) {
    TempFileInfoArray[index] = FileInfoArray[FilesAmount - 1 - index];
  }

  for (UINT16 index = 0; index < FilesAmount; index++) {
    FileInfoArray[index] = TempFileInfoArray[index];
  }
}

void ReverseWCharArray(UINT16 FilesAmount, WCHAR WCharArray[][MAX_PATH])
{
  WCHAR TempWCharArray[FilesAmount][MAX_PATH];
  for (UINT16 index = 0; index < FilesAmount; index++) {
    wcscpy(TempWCharArray[index], WCharArray[FilesAmount - 1 - index]);
  }

  for (UINT16 index = 0; index < FilesAmount; index++) {
    wcscpy(WCharArray[index], TempWCharArray[index]);
  }
}

void ReverseUINT16Array(UINT16 FilesAmount, UINT16 UINT16Array[])
{
  UINT16 TempUINT16Array[MAX_FILES];
  for (UINT16 index = 0; index < FilesAmount; index++) {
    TempUINT16Array[index] = UINT16Array[FilesAmount - 1 - index];
  }

  for (UINT16 index = 0; index < FilesAmount; index++) {
    UINT16Array[index] = TempUINT16Array[index];
  }
}

int BasicLs(HANDLE hConsoleOutput, CONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo, int argc, WCHAR *argv[])
{
  BOOL flag_A = FALSE, flag_a = FALSE, flag_l = FALSE, flag_r = FALSE, flag_S = FALSE, flag_t = FALSE, flag_1 = FALSE;
  BOOL has_pattern = FALSE;
  WCHAR temp_pattern[MAX_PATH];
  WCHAR *pattern;
  UINT8 custom_directory_type = 0;
  WCHAR CustomCurrentDirectory[MAX_PATH];

  for (UINT8 index = 1; index < argc; index++) {
    if (argv[index][0] == L'-') {
      WCHAR *cur_arg = argv[index] + 1;
      while (*cur_arg != L'\0') {
        switch (*cur_arg) {
          case 'a':
            flag_a = TRUE;
            flag_A = FALSE;
            break;
          case 'A':
            flag_A = TRUE;
            flag_a = FALSE;
            break;
          case 'l':
            flag_l = TRUE;
            break;
          case 'r':
            flag_r = TRUE;
            break;
          case 'S':
            flag_S = TRUE;
            flag_t = FALSE;
            break;
          case 't':
            flag_t = TRUE;
            flag_S = FALSE;
            break;
          case '1':
            flag_1 = TRUE;
            break;
          case '-':
            if (wcslen(argv[index]) > 2) {
              WCHAR *str_arg = argv[index] + 2;
              if (!wcscmp(str_arg, L"all")) {
                flag_a = TRUE;
                flag_A = FALSE;
              }
              else if (!wcscmp(str_arg, L"almost-all")) {
                flag_A = TRUE;
                flag_a = FALSE;
              }
              else if (!wcscmp(str_arg, L"reverse")) {
                flag_r = TRUE;
              }
              else if (!wcscmp(str_arg, L"help")) {
                puts(
                    "Usage: lsd [OPTION]... [FILE]...\n"
                    "List information about the FILEs (the current directory by default).\n"
                    "Sort entries alphabetically, but directories are always first, if none of -rtS is specified.\n"
                    "\n"
                    "  -a, --all                  do not ignore entries starting with .\n"
                    "  -A, --almost-all           do not list implied . and ..\n"
                    "  -l                         use a long listing format\n"
                    "  -r, --reverse              reverse order while sorting\n"
                    "  -S                         sort by file size, largest first\n"
                    "  -t                         sort by time, newest first; see --time\n"
                    "  -1                         list one file per line\n"
                    "      --help        display this help and exit\n"
                    "      --version     output version information and exit\n"
                    "\n"
                    "Pattern search syntax: '[PATTERN]' (e.g. '*.exe' prints all files that names ends with \".exe\")."
                );
                return (0);
              }
              else if (!wcscmp(str_arg, L"version")) {
                puts(
                    "lsd (sensod9/pretty-ls-for-windows) 1.0\n"
                    "Copyright (C) 2025 MIT.\n"
                    "This is free software: you are free to change and redistribute it.\n"
                    "There is NO WARRANTY, to the extent permitted by law.\n"
                    "\n"
                    "Written by Roman Izosimov (sensod9).\n"
                    "Inspired by ls written by Richard M. Stallman and David MacKenzie."
                );
                return (0);
              }
            }
        } cur_arg++;
      }
    }
    else {
      if (wcslen(argv[index]) > 2) {
        if (argv[index][0] == '\'') {
          wcscpy(temp_pattern, argv[index]);
          temp_pattern[wcslen(temp_pattern) - 1] = '\0';
          pattern = temp_pattern + 1;
          has_pattern = TRUE;
        }
        else {
          wcscpy(CustomCurrentDirectory, argv[index]);
          if (argv[index][1] == ':' && (argv[index][2] == '\\' || argv[index][2] == '/'))
            custom_directory_type = 2; // absolute path
          else 
            custom_directory_type = 1; // relative path
        }
      }
      else {
        wcscpy(CustomCurrentDirectory, argv[index]);
        custom_directory_type = 1;
      }
    }
  }

  WORD DefaultColor = lpConsoleScreenBufferInfo.wAttributes;

  WCHAR CurrentDirectory[MAX_PATH];
  if (custom_directory_type == 2) {
    wcscpy(CurrentDirectory, CustomCurrentDirectory);
  }
  else if (custom_directory_type == 1) {
    GetCurrentDirectoryW(MAX_PATH, CurrentDirectory);
    wcscat(CurrentDirectory, L"\\");
    wcscat(CurrentDirectory, CustomCurrentDirectory);
  }
  else {
    GetCurrentDirectoryW(MAX_PATH, CurrentDirectory);
  }

  CurrentDirectory[wcslen(CurrentDirectory) + 1] = L'\0'; // common mistake: try to swap these 2 lines of code
  CurrentDirectory[wcslen(CurrentDirectory)] = L'\\';

  WCHAR CurrentSelector[MAX_PATH];
  wcscpy(CurrentSelector, CurrentDirectory);
  CurrentSelector[wcslen(CurrentSelector) + 1] = L'\0';
  CurrentSelector[wcslen(CurrentSelector)] = L'*';

  WCHAR *FileName;
  WCHAR FilePath[MAX_PATH];

  WIN32_FIND_DATAW FindFileData;
  HANDLE hFind;
  hFind = FindFirstFileW(CurrentSelector, &FindFileData);

  if (hFind == INVALID_HANDLE_VALUE) {
    GetCurrentDirectoryW(MAX_PATH, CurrentDirectory);
    CurrentDirectory[wcslen(CurrentDirectory) + 1] = L'\0';
    CurrentDirectory[wcslen(CurrentDirectory)] = L'\\';
    wcscpy(CurrentSelector, CurrentDirectory);
    CurrentSelector[wcslen(CurrentSelector) + 1] = L'\0';
    CurrentSelector[wcslen(CurrentSelector)] = L'*';
    hFind = FindFirstFileW(CurrentSelector, &FindFileData);
  }

  FileInfo DirectoryInfoArray[MAX_FILES];
  FileInfo FileInfoArray[MAX_FILES];

  UINT16 d_index = 0;
  UINT16 f_index = 0;

  if (hFind != INVALID_HANDLE_VALUE) {
    if (!flag_a)
      FindNextFileW(hFind, &FindFileData);
    FileName = FindFileData.cFileName;
    if ((wcscmp(FileName, L"..") != 0 || flag_a) && 
        (!has_pattern || PathMatchSpecW(FileName, pattern))) {
      wcscpy(FilePath, CurrentDirectory);
      wcscat(FilePath, FileName);

      WIN32_FILE_ATTRIBUTE_DATA FileExInfo;
      GetFileAttributesExW(FilePath, GetFileExInfoStandard, &FileExInfo);
      DWORD FileAttributes = FileExInfo.dwFileAttributes;

      if (!(FileAttributes & FILE_ATTRIBUTE_HIDDEN) || flag_A || flag_a) {
        FileInfo CurrentFileInfo;
        ConstructFileInfo(&CurrentFileInfo, FileName, FileExInfo);

        if (!(FileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
          FileInfoArray[f_index++] = CurrentFileInfo;
        }
        else {
          DirectoryInfoArray[d_index++] = CurrentFileInfo;
        }
      }
    }

    while (FindNextFileW(hFind, &FindFileData)) {
      FileName = FindFileData.cFileName;

      if (!has_pattern || PathMatchSpecW(FileName, pattern)) {
        wcscpy(FilePath, CurrentDirectory);
        wcscat(FilePath, FileName);

        WIN32_FILE_ATTRIBUTE_DATA FileExInfo;
        GetFileAttributesExW(FilePath, GetFileExInfoStandard, &FileExInfo);
        DWORD FileAttributes = FileExInfo.dwFileAttributes;

        if (!(FileAttributes & FILE_ATTRIBUTE_HIDDEN) || flag_A || flag_a) {
          FileInfo CurrentFileInfo;
          ConstructFileInfo(&CurrentFileInfo, FileName, FileExInfo);

          if (!(FileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            FileInfoArray[f_index++] = CurrentFileInfo;
          }
          else {
            DirectoryInfoArray[d_index++] = CurrentFileInfo;
          }
        }
      }
    }
  }

  else {
    printf("Fatal error occured: %lu.\nType \"lsd --help\" for usage.", GetLastError());
    return (1);
  }

  CloseHandle(hFind);

  for (UINT16 index = 0; index < f_index; index++) {
    DirectoryInfoArray[index + d_index] = FileInfoArray[index];
  }

  if (d_index + f_index != 0) {
    if (flag_S)
      qsort(DirectoryInfoArray, d_index + f_index, sizeof(FileInfo), fiCompBySize);
    else if (flag_t)
      qsort(DirectoryInfoArray, d_index + f_index, sizeof(FileInfo), fiCompByTime);
    if (flag_r)
      ReverseFileInfoArray(d_index + f_index, DirectoryInfoArray);

    if (flag_l)
      PrintLongListing(hConsoleOutput, lpConsoleScreenBufferInfo, CurrentDirectory, d_index + f_index, DirectoryInfoArray);
    else
      PrintShortListing(hConsoleOutput, lpConsoleScreenBufferInfo, CurrentDirectory, d_index + f_index, DirectoryInfoArray, flag_1);
  }

  SetConsoleTextAttribute(hConsoleOutput, DefaultColor);
  return (0);
}

int wmain(int argc, WCHAR *argv[]) // (!!!) COMPILE WITH -municode -lshlwapi
{
  setlocale(LC_ALL, "");
  HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

  CONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo;
  GetConsoleScreenBufferInfo(hConsoleOutput, &lpConsoleScreenBufferInfo);
  BasicLs(hConsoleOutput, lpConsoleScreenBufferInfo, argc, argv);

  return (0);
}
