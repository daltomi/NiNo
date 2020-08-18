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
#include <FXAutoPtr.h>


#include "Resources/Resources.h"
#include "Programs/Programs.hpp"



class MainWindow : public FXMainWindow
{
    FXDECLARE(MainWindow)

    public: // Funciones

	MainWindow ();
	MainWindow (FXApp *theApp);
	virtual ~MainWindow ();

	FXFont * MakeFont(const FXival theSize, const FXival theWeight);
	FXFont * MakeFont(const FXString & theFontName, FXival theSize);
	FXFont * MakeFont(const FXFontDesc & theFontDesc);

	void AddProgramToList (char const * const theName, IProgram * theProgram);

	inline void SetBackColor (FXWindow * theWindow, FXival theColor)
	{
	    theWindow->setBackColor (theColor);
	}

	inline void NewIcon (unsigned char const * theData, FXPNGIcon * & theIcon)
	{
	    theIcon = new FXPNGIcon (getApp(), theData);
	}
	
	inline void NewIcon (unsigned char const * theData, FXGIFIcon * & theIcon)
	{
	    theIcon = new FXGIFIcon (getApp(), theData);
	}
	

	// Eventos
	long OnCmdQuit		(FXObject* theObj, FXSelector theSelector, void* theData);
	long OnPipeEvent 	(FXObject* theObj, FXSelector theSelector, void* theData);
	long OnCmdBack 		(FXObject* theObj, FXSelector theSelector, void* theData);
	long OnCmdNext 		(FXObject* theObj, FXSelector theSelector, void* theData);
	long OnCmdInstall 	(FXObject* theObj, FXSelector theSelector, void* theData);
	long OnCmdCBox 		(FXObject* theObj, FXSelector theSelector, void* theData);

    protected: // Funciones

	virtual void create ();
	FXint GetNumPanels () const;
	void  SetCurrentPanel (FXint theIndex);
	FXint GetCurrentPanel () const;

    public: // variables


	enum {
	    ID_TITLE = FXMainWindow::ID_LAST,
	    ID_QUIT,
	    ID_BACK,
	    ID_NEXT,
	    ID_INSTALL,
	    ID_CBOX
	};



    private: // variables

	FXPNGIcon * 	mIconNino;
	FXPNGIcon * 	mIconList;
	FXPNGIcon * 	mIconApp;
	FXGIFIcon *	mIconNext;
	FXGIFIcon *	mIconPrev;
	FXGIFIcon *	mIconCancel;
	FXGIFIcon * 	mIconOk;
	
	FXSwitcher *	mLayoutSwitch;
	FXLabel *	mLabelTitle;
	FXLabel *	mLabelDesc;
	FXComboBox *	mCBoxPrograms;



	enum eTitleId {
		MAIN = 0,
		CONF,
		LEN
	};

	static constexpr char const * const mStrTitle [][eTitleId::LEN] = {
	    {
		"Bienvenido.\n"
		"NiNo le ayudará a configurar e instalar sus programas."
	    },
	    {
		"Se instalará '%s'"
	    }
	};
};
