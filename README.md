# Akiner

Cross-platform Qt-based application which helps you to delete, move or copy *akin files*.

> *Akin files* (in this program context) – files with the same name except full path and file extension.

## For what is it?

This is useful for photographers, whose camera takes **two** files at a time:

* One is a basic *.jpg* file
* And one is a file in high resolution

So photographers must first choose good files in *.jpg* format and then choose the same photos but in high resolution.

> This programs automates the process of finding the same files but with another file extension and doing something base operations like deleting, copying or moving.

## How to use it?

First, you should provide a directory with files for which program will search and akin files.

Then, you should provide one or more directories which you want to scan.

At last, you should select and action to do with similar files founded in directories from step before.

## Demo

![](demo.gif)

## Building from source

On *Linux* or *Mac OS* you can build it from source by using the next commands:

```bash
git clone https://github.com/Arjentix/Akiner
cd Akiner
mkdir build
cd build
qmake ..
make
```

You can run `make` command with `-j4` option to run building on 4 threads.

If you don't have a `qmake` program, then you have to install it on your system.

## Running

On *Linux* or *Mac OS* you can run it with:

```bash
./Akiner
```

## Windows

To get it on *Windows* simply download a *.zip* file from the [release page](https://github.com/Arjentix/Akiner/releases), unpack it and create a shortcut for an *.exe* file.