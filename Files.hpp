/*
   Copyright (c) 2016, daltomi <daltomi@disroot.org>

   This file is part of NiNo.

    NiNo is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    NiNo is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with NiNo.  If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "Log.hpp"

static short const 	gBufferSize = 500 * sizeof(char);
static char 		gFifoName []  = "/tmp/NiNoFifoXXXXXX";
static char * 		gBuffer;
static int * 		gFifoIn;
static int * 		gFifoOut;
static FILE * 		gFilePipe;


static inline void MapSharedMemory ()
{
    gFifoIn  	= (int*)mmap(NULL, sizeof( int*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    gFifoOut 	= (int*)mmap(NULL, sizeof (int*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    gFilePipe 	= (FILE*)mmap(NULL, sizeof (FILE*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    gBuffer 	= (char*) mmap(NULL, gBufferSize, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
}


static void inline UnmapSharedMemory ()
{
    munmap (gFifoIn, sizeof(int* ));
    munmap (gFifoOut, sizeof(int* ));
    munmap (gFilePipe, sizeof(FILE* ));
    munmap (gBuffer, gBufferSize);

    gFifoIn 	= nullptr;
    gFifoOut 	= nullptr;
    gFilePipe 	= nullptr;
    gBuffer 	= nullptr;
}


static void RemoveFile (char const * theName)
{
    errno = 0;

    int aReturn = unlink (theName);

    if (-1 == aReturn)
    {
	Log(eLogIds::SHOW_ERROR,"Failed unlink: %s", theName);
    }
}


static void MakeFifo (char const * theName)
{
    errno = 0;

    int aReturn = mkfifo (theName, S_IRUSR | S_IWUSR);

    if (-1 == aReturn)
    {
	Log(eLogIds::SHOW_ERROR, "Error MakeFifo: %s\n%s", theName, strerror (errno));
    }
}


static int FifoOpen (char const * theName, int const theFlags)
{
    errno = 0;

    int aFifo = open (theName, theFlags);

    if (-1 == aFifo)
    {
	Log(eLogIds::SHOW_ERROR, "Error FifoOpen: %s\n%s", theName, strerror (errno));
    }

    return aFifo;
}


static int FifoOpenForRead  (char const * theName)
{
    return FifoOpen (theName, O_RDONLY | O_NONBLOCK);
}


static int FifoOpenForWrite (char const * theName)
{
    return FifoOpen (theName, O_WRONLY);// | O_NONBLOCK);
}


static FILE * PipeOpen (char const * theCmd)
{
    errno = 0;

    FILE * aPipe = popen (theCmd, "r");

    if (NULL == aPipe)
    {
	Log(eLogIds::SHOW_ERROR, "Error PipeOpen: %s\n%s", theCmd, strerror (errno));
    }

    return aPipe;
}


static void PipeClose ()
{
    if (gFilePipe)
    {
	pclose (gFilePipe);
    }
}


static void FiFoClose ()
{
    close (* gFifoIn);
    close (* gFifoOut);
}


static void CloseAllFiles ()
{
    PipeClose ();
    FiFoClose ();
    fcloseall ();
}


static void FifoWrite (char const * theData, size_t theSize)
{
    errno = 0;

    ssize_t aSize = write (*gFifoOut, (void *)theData, theSize);

    syncfs (*gFifoOut);

    CheckIf(-1 != aSize, return, eLogIds::SHOW_ERROR, "Error FifoWrite: %s", strerror (errno));
}


static ssize_t FifoRead ()
{
    syncfs (*gFifoIn);

    ssize_t aLen = read (*gFifoIn, gBuffer, gBufferSize);

    return aLen;
}


static void MakeTempFile (char * theName)
{
    errno = 0;

    int aFile = mkstemp (theName);

    CheckIf(-1 != aFile, return, eLogIds::SHOW_ERROR, "Error MakeTempFile: %s\n%s", theName, strerror (errno));

    close (aFile);

    RemoveFile (theName);
}


static int WriteFile (char const * theName, char const * theData)
{
    errno = 0;

    FILE * aFile = fopen (theName, "w");

    CheckIf(NULL != aFile, return -1, eLogIds::SHOW_ERROR, "Error WriteFile: %s\n%s", theName, strerror (errno));

    fwrite (theData, 1, strlen (theData), aFile);

    fclose (aFile);

    return 0;
}
