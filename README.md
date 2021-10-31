# cponto - Command line task timer

This utility let's you time you tasks and save them in a csv file for later processing in a spreadsheet software.

The time is taken when you start a task and the end time when you initiate a new task, and the last task can be terminated by stopping.

The program only writes to a single file, save it before making a new one, the [makefile](./Makefile) `OUTPUT_CSV_FILENAME_(YOUR_OS)` variable defines at compile time the path to the file.

## Table of contents
- [cponto - Command line task timer](#cponto---command-line-task-timer)
  - [Table of contents](#table-of-contents)
  - [Usage](#usage)
  - [Building and Installing](#building-and-installing)
    - [Tested on:](#tested-on)
    - [Dependencies](#dependencies)
    - [make](#make)

## Usage

You can create the first task using `start`.

```console
    $ cponto start "New task"
```

Then you can add tasks as you go trought your day using `next`.

```console
    $ cponto next "Another task"
```

And finally, `stop`.

```console
    $ cponto stop
```

There are macros for this options with other names to use, check them out with the ´--help´ option.
Also you can create a new file using:

```console
    $ cponto new
```

## Building and Installing

### Tested on:
- Window 10 64bits (MinGW64).
- Archlinux (gcc).

### Dependencies

- [c_doc](https://github.com/Joao-Peterson/c_doc)
- [cmdf](https://github.com/Joao-Peterson/CMD-Friend)

### make

First read and alter the [makefile](./Makefile) variables as you want, mainly `OUTPUT_CSV_FILENAME_(YOUR_OS)` and `INSTALL_BIN_DIR_(YOUR_OS)`, alter them before building. 

Build using the command:

```console
    $ make release
```

And installing by:

```console
    $ make install
```

*Obs.:* do `make release` as user on linux, do not `sudo` it, `make install` on the otherhand can be ran as sudo if necessary, see `INSTALL_BIN_DIR_(YOUR_OS)` on the [makefile](./Makefile).  