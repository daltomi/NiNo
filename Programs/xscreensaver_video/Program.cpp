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

#include "Program.hpp"

///////////////////////////////////////////////////////////

FXDEFMAP( xscr_video_program ) xscr_video_program_map [] = {
    FXMAPFUNC(SEL_COMMAND,	xscr_video_program::ID_OPEN,  xscr_video_program::OnCmdOpenDir),
    FXMAPFUNC(SEL_IO_READ,	xscr_video_program::ID_PIPE_EVENT,  xscr_video_program::IProgram::OnPipeEvent)

};

FXIMPLEMENT( xscr_video_program, FXVerticalFrame, xscr_video_program_map, ARRAYNUMBER(xscr_video_program_map) )
///////////////////////////////////////////////////////////


xscr_video_program::xscr_video_program () :

    mCheckAudio (false),
    mTargetAudio (mCheckAudio),

    mCheckVideo (false),
    mTargetVideo (mCheckVideo),

    mDir(nullptr),
    mTitle(nullptr)
{ }


xscr_video_program::~xscr_video_program ()
{ }


void xscr_video_program::BuildLayout (FXComposite * theParent)
{
    static FXVerticalFrame * aStaticVFrame = nullptr;

    if (aStaticVFrame != nullptr)
    {
	return;
    }


    FXHorizontalFrame * aHFrame = nullptr;
    FXVerticalFrame * 	aVFrame = nullptr;
    FXMatrix * 		aMatrix = nullptr;

    aStaticVFrame = new FXVerticalFrame (theParent,LAYOUT_LEFT | LAYOUT_FILL_X | LAYOUT_FILL_Y);

    BuildCommonLayout (aStaticVFrame);

    aHFrame = new FXHorizontalFrame (aStaticVFrame, LAYOUT_FILL_X ,0,0,0,0,28,0,8,8);

    aMatrix = new FXMatrix (aHFrame,2, MATRIX_BY_COLUMNS | LAYOUT_FILL_Y | LAYOUT_FILL_X | LAYOUT_TOP,0,0,0,0,0,0,0,0);

    new FXLabel (aMatrix, "Título :", nullptr, LAYOUT_CENTER_Y | LAYOUT_RIGHT,0,0,0,0,0,0,0,0);
    mTitle = new FXTextField (aMatrix, 2, nullptr, 0, FRAME_SUNKEN|FRAME_THICK | LAYOUT_FILL_X|LAYOUT_CENTER_Y |LAYOUT_FILL_COLUMN);


    new FXLabel (aMatrix, "Directorio de videos :", nullptr, LAYOUT_CENTER_Y);
    mDir = new FXTextField (aMatrix, 2, nullptr, 0, TEXTFIELD_READONLY|FRAME_SUNKEN|FRAME_THICK | LAYOUT_FILL_X | LAYOUT_CENTER_Y| LAYOUT_FILL_COLUMN);
    mDir->setBackColor (FXRGB (228,228,228));

    new FXButton (aHFrame, "...", nullptr, this, ID_OPEN,FRAME_SUNKEN|FRAME_THICK | LAYOUT_BOTTOM,0,0,0,0,2,2,2,3);

    aVFrame = new FXVerticalFrame (aStaticVFrame, LAYOUT_FILL_X | PACK_UNIFORM_WIDTH,0,0,0,0,30,5,5,3);
    new FXCheckButton (aVFrame,"¿Reproducir audio?",& mTargetAudio, FXDataTarget::ID_VALUE,ICON_AFTER_TEXT | JUSTIFY_RIGHT);
    new FXCheckButton (aVFrame,"¿Reproducir todos los videos?",& mTargetVideo, FXDataTarget::ID_VALUE,ICON_AFTER_TEXT | JUSTIFY_RIGHT);

    aStaticVFrame->create ();
}


void xscr_video_program::Install ()
{
    char aCmd [80];
    char const * aAudio 	= (mCheckAudio == true) ? "" : "--no-audio";
    char const * aScript	= (mCheckVideo == true) ? xscr_videos_sh : xscr_video_sh;
    char * 	aVideoId	= strdup (xscr_video_id);
    char * 	aInstallId	= strdup (xscr_instalar_id);
    char * 	aFifoName	= strdup (gFifoName);


    MakeTempFile (aFifoName);

    MakeFifo (aFifoName);

    WriteFileScript (aVideoId, aScript, mDir->getText().text(), aAudio);

    WriteFileScript (aInstallId,
		     xscr_instalar_sh,
		     gShellReply [eShellReply::YES].mMsg,
		     gShellReply [eShellReply::NO].mMsg,
		     gShellProto [eShellProto::PROTO_ERR_Q].mMsg,
		     gShellProto [eShellProto::PROTO_ERR_I].mMsg,
		     gShellProto [eShellProto::PROTO_ERR_W].mMsg,
		     gShellProto [eShellProto::PROTO_INFO].mMsg,
		     gShellProto [eShellProto::PROTO_WARN].mMsg,
		     gShellProto [eShellProto::PROTO_QUIT].mMsg,
		     gShellToken,
		     aVideoId,
		     mTitle->getText().text(),
		     aFifoName,
		     "%s",
		     (void*)0);

    snprintf (aCmd, 80, "/usr/bin/bash %s", aInstallId);

    *gFifoIn = FifoOpenForRead (aFifoName);

    *gFifoOut = FifoOpenForWrite (aFifoName);

    gFilePipe = PipeOpen (aCmd);

    free (aVideoId);
    free (aInstallId);
    free (aFifoName);

    gApp->addInput(this, ID_PIPE_EVENT, *gFifoIn, INPUT_READ, NULL);
}


char const * const xscr_video_program::GetHelp () const
{
    return xscr_help;
}


bool xscr_video_program::IsReadyForInstall () const
{
    if (mDir == nullptr || mTitle == nullptr)
    {
	return false;
    }

    return (mDir->getText ().count() > 0) && (mTitle->getText ().count () > 0);
 }


long xscr_video_program::OnCmdOpenDir(FXObject* theObj, FXSelector theSelector, void* theData)
{
    static FXString aLastDirectory = FXSystem::getHomeDirectory();

    FXDirDialog aFileDlg (gApp, "NiNo - Abrir ubicación de videos");

    aFileDlg.setDirectory (aLastDirectory);

    if ( aFileDlg.execute ())
    {
	aLastDirectory = aFileDlg.getDirectory ();
	mDir->setText (aLastDirectory);
    }

    return 1;
}


void xscr_video_program::Quit ()
{
    gApp->exit (0);
}
