	.set noreorder

#include "pspstub.s"

	STUB_START "getmodel",0x400A0000,0x00010005
	STUB_FUNC  0x828D268F,GetPSPModel
	STUB_END
