	.set noreorder

#include "pspstub.s"

	STUB_START "pspmodel",0x40090000,0x00010005
	STUB_FUNC  0x828D268F,GetPSPModel
	STUB_END
