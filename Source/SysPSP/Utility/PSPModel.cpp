
#include "stdafx.h"
#include "PSPModel.h"
#include "SysPSP/Utility/ModulePSP.h"

extern "C"
{
	void GetPSPModel();
}


bool pspModelCheck()
{
  // Start the model check stack
  int model {CModule::Load("pspmodel.prx")};

// Initialize

  if (model > 0)
  {
    GetPSPModel();
		//    std::cout << "PSP Model is: " << model << endl;

    CModule::Unload (model );

    return true;
  }

  // If this failed for some reason return false

  return false;

}

    /*
1 - Fat PSP
2 - Slim PSP
3 - Slim & Brite
5 - PSP Go
6 - ???
7 - ???
8 - ???
    */
