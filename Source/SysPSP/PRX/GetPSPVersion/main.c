/*
Copyright (C) 2010 Salvy6735

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

#include <pspsdk.h>
#include <pspkernel.h>
#include <pspsysmem_kernel.h>


PSP_MODULE_INFO("get model", 0x1099, 1, 1);
PSP_MAIN_THREAD_ATTR(0);

//*************************************************************************************
//
//*************************************************************************************
void GetPSPModel()
{
    int sceKernelGetModel(void);
}

//*************************************************************************************
//
//*************************************************************************************
u32 module_start(SceSize args, void *argp)
{
	return 0;
}

//*************************************************************************************
//
//*************************************************************************************
u32 module_stop()
{
	return 0;
}
