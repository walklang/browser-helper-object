/***************************************************************************
 *   Copyright (C) 2005 by Neil Graham   *
 *   lerc@screamingduck.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
typedef void* I2W_OutputHandle;


#ifdef __cplusplus
extern "C" {
#endif

#ifdef OUTPUT_X
#include <X11/Xlib.h>
I2W_OutputHandle I2W_CreateOutput(Window XWindow,Display* XDisplay);
//I2W_OutputHandle I2W_CreateOutput_UsingProvidedSurface(Window XWindow,Display* XDisplay, void* Surface);
int I2W_GetSharingID(I2W_OutputHandle Handle);
#endif

#ifdef OUTPUT_WINDOWS
#include <windows.h>
I2W_OutputHandle I2W_CreateOutput(HWND Window);
I2W_OutputHandle I2W_CreateOutput_UsingProvidedSurface(HWND Window, void* Surface);
#endif 

void I2W_FreeOutput(I2W_OutputHandle Handle);

void* I2W_GetOutputSurface(I2W_OutputHandle Handle);
int I2W_GetOutputWidth(I2W_OutputHandle Handle);
int I2W_GetOutputHeight(I2W_OutputHandle Handle);

void I2W_Draw(I2W_OutputHandle Handle);

void I2W_DrawSomething(I2W_OutputHandle Handle);

#ifdef __cplusplus
}  /* end extern "C" */
#endif
