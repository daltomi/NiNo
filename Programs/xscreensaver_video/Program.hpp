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

#include "../IProgram.hpp"
#include "Scripts.hpp"


class xscr_video_program : public IProgram, public FXVerticalFrame
{
    FXDECLARE(xscr_video_program)

    public:

	xscr_video_program ();
	~ xscr_video_program ();

	void Install ();
	char const * const GetHelp () const;
	bool IsReadyForInstall () const;
	void BuildLayout (FXComposite * theParent);
	void Quit ();

	enum {
	    ID_OPEN = FXVerticalFrame::ID_LAST,
	    ID_PIPE_EVENT
	};

	long OnCmdOpenDir(FXObject* theObj, FXSelector theSelector, void* theData);


    private:

	FXDataTarget mTargetAudio;
	FXbool 	mCheckAudio;

	FXDataTarget mTargetVideo;
	FXbool 	mCheckVideo;

	FXTextField * mDir;
	FXTextField * mTitle;
};


