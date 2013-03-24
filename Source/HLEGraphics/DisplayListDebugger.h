/*

  Copyright (C) 2006 StrmnNrmn

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifndef __DAEDALUS_DISPLAYLISTDEBUGGER_H__
#define __DAEDALUS_DISPLAYLISTDEBUGGER_H__

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef DAEDALUS_DEBUG_DISPLAYLIST
bool DLDebugger_IsDebugging();
void DLDebugger_RequestDebug();
bool DLDebugger_Process();

bool DLDebugger_RegisterWebDebug();
#endif

#endif // __DAEDALUS_DISPLAYLISTDEBUGGER_H__
