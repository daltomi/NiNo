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

#include "MainWindow.hpp"


/* DEFINE */

FXDEFMAP( MainWindow ) MainWindowMap [] = {
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_QUIT,		MainWindow::OnCmdQuit),
	FXMAPFUNC(SEL_SIGNAL,		MainWindow::ID_QUIT,		MainWindow::OnCmdQuit),
	FXMAPFUNC(SEL_CLOSE,		MainWindow::ID_TITLE,		MainWindow::OnCmdQuit),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_NEXT,		MainWindow::OnCmdNext),
	FXMAPFUNC(SEL_UPDATE,		MainWindow::ID_NEXT,		MainWindow::OnCmdNext),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_BACK,		MainWindow::OnCmdBack),
	FXMAPFUNC(SEL_UPDATE,		MainWindow::ID_BACK,		MainWindow::OnCmdBack),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_INSTALL,		MainWindow::OnCmdInstall),
	FXMAPFUNC(SEL_UPDATE,		MainWindow::ID_INSTALL,		MainWindow::OnCmdInstall),
	FXMAPFUNC(SEL_COMMAND,		MainWindow::ID_CBOX,		MainWindow::OnCmdCBox),
};

FXIMPLEMENT( MainWindow, FXMainWindow, MainWindowMap, ARRAYNUMBER(MainWindowMap) )


/* Constructores */

MainWindow::MainWindow ()
{ }

MainWindow::MainWindow (FXApp * theApp) :

	FXMainWindow (theApp, "NiNo", nullptr, nullptr, FX::DECOR_MINIMIZE | FX::DECOR_CLOSE | FX::DECOR_BORDER, 0, 0, 500, 400)

{
	//////////////////////////////////////////////////////
	FXHorizontalFrame * aHFrame 	= nullptr;
	FXVerticalFrame * aVFrame 	= nullptr;
	FXLabel * aLabel  		= nullptr;
	FXFont  * aFont10Bold		= nullptr;
	FXFont  * aFont8		= nullptr;
	FXButton * aButton		= nullptr;
	FXScrollWindow * aScroll	= nullptr;
	//////////////////////////////////////////////////////

	setTarget (this);
	setSelector (ID_TITLE);

	NewIcon (nino_png, mIconNino);
	NewIcon (listado_png, mIconList);
	NewIcon (miniapp_png, mIconApp);
	NewIcon (siguiente_gif, mIconNext);
	NewIcon (previo_gif, mIconPrev);
	NewIcon (cancelar_gif, mIconCancel);
	NewIcon (aceptar_gif, mIconOk);
	
	aFont8 	= MakeFont ("Sans", 8);
	getApp ()->setNormalFont (aFont8);
	aFont10Bold	= MakeFont (10, FXFont::Bold);


	// Título / Descripción
	{
	    aHFrame = new FXHorizontalFrame (this, LAYOUT_LEFT | LAYOUT_FILL_X ,0,0,0,0,15,5,5,5,25,0);
	    SetBackColor (aHFrame, FXColors::Gray99);

	    {
		mLabelTitle = new FXLabel (aHFrame, * mStrTitle [eTitleId::MAIN], nullptr, LAYOUT_LEFT | LAYOUT_CENTER_Y | JUSTIFY_LEFT);
		mLabelTitle->setBackColor (FXRGB (255,255,255));
		mLabelTitle->setFont (aFont10Bold);

		aLabel 	= new FXLabel (aHFrame, FXString::null, mIconNino ,LAYOUT_RIGHT | LAYOUT_CENTER_Y);
		aLabel->setBackColor (FXRGB (255,255,255));
	    }
	    new FXSeparator (this, SEPARATOR_GROOVE | LAYOUT_FILL_X);
	}


	{ // Panel contenedor
	    mLayoutSwitch = new FXSwitcher (this, LAYOUT_FILL_X | LAYOUT_FILL_Y);
	    aVFrame = new FXVerticalFrame (mLayoutSwitch, LAYOUT_FILL_X);
	}

	{ // Panel principal
	    new FXLabel (aVFrame, "Por favor, seleccione un programa de la lista.", nullptr, LAYOUT_LEFT | JUSTIFY_LEFT,0,0,0,0,30,5,5,3);
	    new FXLabel (aVFrame, "Luego utilice el botón Siguiente para continuar, o Cancelar para salir.", nullptr, LAYOUT_LEFT | JUSTIFY_LEFT,0,0,0,0,30,5,5,3);

	    aHFrame = new FXHorizontalFrame (aVFrame, LAYOUT_LEFT ,0,0,0,0,30,5,15,8);

	    new FXLabel (aHFrame, "Programas :", mIconApp, TEXT_AFTER_ICON | LAYOUT_LEFT | JUSTIFY_LEFT);

	    mCBoxPrograms = new FXComboBox (aHFrame, 15, this, ID_CBOX, COMBOBOX_STATIC|FRAME_SUNKEN|FRAME_THICK|LAYOUT_RIGHT);
	    mCBoxPrograms->setNumVisible (4);

	    aScroll = new FXScrollWindow (aVFrame,LAYOUT_FILL_X | LAYOUT_FILL_Y);

	    mLabelDesc = new FXLabel (aScroll, "Descripción :", mIconList,
			    TEXT_AFTER_ICON | JUSTIFY_LEFT | JUSTIFY_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y,0,0,0,0,33,5,0,0);

	    // Mantiene al scroll en  posición.
	    new FXFrame (aVFrame, LAYOUT_BOTTOM | LAYOUT_FILL_Y,0,0,0,0,3,3,3,3);
	}

	{ // statico

	    aVFrame = new FXVerticalFrame (this,  LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X ,0,0,0,0,30,30,15,10);

	    new FXSeparator (aVFrame, SEPARATOR_GROOVE | LAYOUT_FILL_X);

	    aHFrame = new FXHorizontalFrame (aVFrame, LAYOUT_BOTTOM|LAYOUT_FILL_X | LAYOUT_FILL_Y | PACK_UNIFORM_HEIGHT,0,0,0,0,8,8,8,4);

	    new FXButton (aHFrame, "&Cancelar", mIconCancel, this, ID_QUIT, TEXT_AFTER_ICON|FRAME_RAISED|FRAME_THICK|LAYOUT_FILL_Y|LAYOUT_RIGHT,0,0,0,0,12,12,2,2);
	    new FXFrame (aHFrame,LAYOUT_FIX_WIDTH|LAYOUT_RIGHT,0,0,10,0);
	    new FXButton (aHFrame, "&Siguiente", mIconNext, this, ID_NEXT, TEXT_AFTER_ICON|FRAME_RAISED|FRAME_THICK|LAYOUT_FILL_Y|LAYOUT_RIGHT,0,0,0,0,12,12,2,2);
	    aButton = new FXButton (aHFrame, "&Instalar", mIconOk, this, ID_INSTALL, TEXT_AFTER_ICON|FRAME_RAISED|FRAME_THICK|LAYOUT_FILL_Y|LAYOUT_RIGHT,0,0,0,0,12,12,2,2);
	    aButton->hide ();
	    aButton->disable ();
	    aButton = new FXButton (aHFrame, "&Atrás", mIconPrev, this , ID_BACK,  TEXT_AFTER_ICON|FRAME_RAISED|FRAME_THICK|LAYOUT_FILL_Y|LAYOUT_RIGHT,0,0,0,0,12,12,2,2);
	    aButton->hide();

	}
};


MainWindow::~MainWindow ()
{
    delete mIconNino;
    delete mIconList;
    delete mIconApp;
    delete mIconCancel;
    delete mIconPrev;
    delete mIconNext;
    delete mIconOk;
}



void MainWindow::create ()
{
    FXMainWindow::create ();

    mIconNino->create ();
    mIconNext->create ();
    mIconPrev->create ();
    mIconCancel->create ();
    mIconOk->create ();

    show (PLACEMENT_SCREEN);
}


long MainWindow::OnCmdCBox (FXObject* theObj, FXSelector theSelector, void* theData)
{
    IProgram * aProgram  = (IProgram*) mCBoxPrograms->getItemData ( mCBoxPrograms->getCurrentItem ());

    CheckIf(aProgram != nullptr, return 0, eLogIds::LOG_ERROR, "OnCmdCBox: aProgram == NULL");

    FXString aHelp = aProgram->GetHelp ();
    mLabelDesc->setText (aHelp);
}


long MainWindow::OnCmdQuit(FXObject* theObj, FXSelector theSelector, void* theData)
{
    if ( MBOX_CLICKED_YES == FXMessageBox::question (getApp (), MBOX_YES_NO, "NiNo", "¿Desea salir?"))
    {
	getApp ()->exit (0);
    }
    return 1;
}


long MainWindow::OnCmdBack(FXObject* theObj, FXSelector theSelector, void* theData)
{
    switch (FXSELTYPE (theSelector))
    {
	case SEL_COMMAND:
	    mLabelTitle->setText (* mStrTitle [eTitleId::MAIN]);
	    SetCurrentPanel (GetCurrentPanel () - 1);
	    break;
	case SEL_UPDATE:
	    theObj->handle (this, GetCurrentPanel () ?
		    FXSEL(SEL_COMMAND,ID_SHOW) : FXSEL(SEL_COMMAND,ID_HIDE), NULL);
	    break;
    };

    return 1;
}


long MainWindow::OnCmdNext(FXObject* theObj, FXSelector theSelector, void* theData)
{

    switch (FXSELTYPE (theSelector))
    {
	case SEL_COMMAND:
	{
	    IProgram * aProgram  = (IProgram*) mCBoxPrograms->getItemData ( mCBoxPrograms->getCurrentItem ());

	    CheckIf(aProgram != nullptr, return 0, eLogIds::LOG_ERROR, "OnCmdNext: aProgram == NULL");

	    FXString aTitle;

	    aTitle.format (* mStrTitle [eTitleId::CONF], mCBoxPrograms->getItem (mCBoxPrograms->getCurrentItem ()).text() );

	    mLabelTitle->setText (aTitle);

	    aProgram->BuildLayout (mLayoutSwitch);

	    SetCurrentPanel (GetCurrentPanel () + 1);
	}
	break;
	case SEL_UPDATE:
	    theObj->handle (this, (GetCurrentPanel () == 1) ?
		    FXSEL(SEL_COMMAND,ID_HIDE) : FXSEL(SEL_COMMAND,ID_SHOW), NULL);
	    break;
    };

    return 1;
}


long MainWindow::OnCmdInstall (FXObject* theObj, FXSelector theSelector, void* theData)
{
    switch (FXSELTYPE (theSelector))
    {
	case SEL_COMMAND:
	{
	    IProgram * aProgram  = (IProgram*) mCBoxPrograms->getItemData ( mCBoxPrograms->getCurrentItem ());
	    aProgram->Install ();
	    break;
	}

	case SEL_UPDATE:
	{
	    IProgram * aProgram  = (IProgram*) mCBoxPrograms->getItemData ( mCBoxPrograms->getCurrentItem ());

	    CheckIf(aProgram != nullptr, return 0, eLogIds::LOG_ERROR, "OnCmdInstall: aProgram == NULL");

	    bool aIsReadyForInstall = (aProgram != nullptr) ? aProgram->IsReadyForInstall () : false;
	    bool aIsPanels = (GetCurrentPanel () == 1);

	    theObj->handle (this, (aIsPanels) ? FXSEL(SEL_COMMAND,ID_SHOW) : FXSEL(SEL_COMMAND,ID_HIDE), NULL);
	    theObj->handle (this, (aIsReadyForInstall) ? FXSEL(SEL_COMMAND,ID_ENABLE) : FXSEL(SEL_COMMAND,ID_DISABLE), NULL);
	    break;
	}
    };

    return 1;
}


// Crea una fuente basada en el tipo de fuente normal.
FXFont * MainWindow::MakeFont (FXival const theSize, FXival const theWeight)
{
    FXFontDesc aFontDesc = getApp ()->getNormalFont ()->getFontDesc ();
    aFontDesc.size = (aFontDesc.size * theSize)/10;
    aFontDesc.weight = theWeight;
    return new FXFont (getApp (), aFontDesc);
}


FXFont * MainWindow::MakeFont (FXString const & theFontName, FXival theSize)
{
    FXFont * aFont  = new FXFont (getApp(), theFontName,theSize,0);
    aFont->create ();
    return aFont;
}


FXFont * MainWindow::MakeFont (FXFontDesc const & theFontDesc)
{
    FXFont * aFont = new FXFont (getApp(), theFontDesc);
    aFont->create ();
    return aFont;
}


FXint MainWindow::GetNumPanels () const
{
    return mLayoutSwitch->numChildren ();
}



void MainWindow::SetCurrentPanel (FXint theIndex)
{
    mLayoutSwitch->setCurrent (theIndex);
}



FXint MainWindow::GetCurrentPanel () const
{
    return mLayoutSwitch->getCurrent ();
}



void MainWindow::AddProgramToList (char const * const theName, IProgram * theProgram)
{
    mCBoxPrograms->appendItem (theName, (FXptr)theProgram);

    OnCmdCBox(nullptr, 0, nullptr);
}

