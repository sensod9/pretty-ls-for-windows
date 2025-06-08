# pretty-ls-for-windows
> Implementation of "ls" command for Windows written in C. Provides correct word alignment. Supports pattern search and common options

![image](https://github.com/user-attachments/assets/2ae1a162-43d7-473d-81d0-eed746e844df)

### `ls`: Sort entries alphabetically if none of -rtS is specified.
### `lsd`: Sort entries alphabetically, but directories are always first, if none of -rtS is specified.

## Usage:
```
> ls [OPTION]... [FILE]...
```
## Options:
```
  -a, --all                  do not ignore entries starting with .
  -A, --almost-all           do not list implied . and ..
  -l                         use a long listing format
  -r, --reverse              reverse order while sorting
  -S                         sort by file size, largest first
  -t                         sort by time, newest first; see --time
  -1                         list one file per line
      --help        display this help and exit
      --version     output version information and exit
```
## Pattern search syntax:
```
> ls '[PATTERN]' (e.g. '*.exe' prints all files that names ends with ".exe").
```

## Installation from [releases](https://github.com/sensod9/pretty-ls-for-windows/releases/):
### You can run `.msi` files for quick installation or manually copy `.exe` files to `Windows/System32`
After that you should be able to use respective programs by typing `ls`/`lsd` in Cmd 
