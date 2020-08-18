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

#include <fx.h>
#include <limits.h>

#include "../Files.hpp"
#include "../FileScript.hpp"
#include "../Shell.hpp"
#include "../Log.hpp"


class IProgram
{
    public:

	IProgram () {
	    MapSharedMemory ();
	}

	virtual ~ IProgram () {
	    UnmapSharedMemory ();
	}

	virtual void Install () = 0;

	virtual bool IsReadyForInstall () const = 0;

	virtual void BuildLayout (FXComposite * theParent) = 0;

	virtual char const * const GetHelp () const = 0;

	virtual void Quit () = 0;


	long OnPipeEvent (FXObject* theObj, FXSelector theSelector, void* theData)
	{
	    ProcessProtocolResponse aResponse;

	    bzero (gBuffer, gBufferSize);

	    ssize_t aLen = FifoRead ();

	    CheckIf((aLen > 0 && aLen < SSIZE_MAX), goto END, eLogIds::LOG_ERROR, "Error OnPipeEvent, falló read");

	    {
		gBuffer [aLen] = '\0';

		ShellFormat const aFormat (gBuffer);

		 aResponse = ProcessProtocol (aFormat);

		if (aResponse.mWait == true)
		{
		    // No cerrar la comunicación todavía.
		    return 1;
		}
	    }

	END:

	    gApp->removeInput(*gFifoIn, INPUT_READ);

	    CloseAllFiles ();

	    if (aResponse.mQuit == true)
	    {
		    Quit ();
	    }

	    return 1;
	}


    protected:

	static void BuildCommonLayout (FXComposite * theParent)
	{
	    new FXLabel (theParent, "A continuación configure el programa.", nullptr, LAYOUT_LEFT | JUSTIFY_LEFT,0,0,0,0,30,5,5,3);
	    new FXLabel (theParent, "Luego utilice el botón Instalar para continuar, o Cancelar para salir.", nullptr, LAYOUT_LEFT | JUSTIFY_LEFT,0,0,0,0,30,5,5,3);
	}
};
