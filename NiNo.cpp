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


#include "NiNo.hpp"

FXApp * gApp = nullptr;



int main (int theArgc, char * theArgv [])
{
    setsid();

    FXApp aApp("NiNo","nino");

    gApp = & aApp;

    aApp.init (theArgc, theArgv);

    SetDefaultColors (aApp);
    
    MainWindow aWindow (& aApp);

    // Agregar aquí los programas a la lista de Programas:"
    aWindow.AddProgramToList ("xscreensaver-video", new xscr_video_program);

    aApp.addSignal(SIGINT, & aWindow, MainWindow::ID_QUIT);

    aApp.create();

    return aApp.run ();
}


void SetDefaultColors (FXApp & theApp)
{
    // "Desert FOX" - ControlPanel
    theApp.setBaseColor		(FXRGB (214, 205, 187));
    theApp.setBorderColor 	(FXColors::Black);
    theApp.setBackColor 	(FXColors::Gray100);
    theApp.setForeColor 	(FXColors::Black);
    theApp.setHiliteColor 	(FXRGB (250, 250, 250));
    theApp.setShadowColor 	(FXRGB (143, 137, 125));
    theApp.setSelforeColor 	(FXColors::Gray100);
    theApp.setSelbackColor 	(FXRGB (128,0,0));
    theApp.setTipforeColor 	(FXColors::Black);
    theApp.setTipbackColor 	(FXRGB (255, 255, 225));
    theApp.setSelMenuTextColor 	(FXColors::Gray100);
    theApp.setSelMenuBackColor 	(FXRGB (128,0,0));
}

