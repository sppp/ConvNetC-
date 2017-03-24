#include "CharGen.h"

#define IMAGECLASS CharGenImg
#define IMAGEFILE <CharGen/CharGen.iml>
#include <Draw/iml_source.h>

GUI_APP_MAIN {
	if (0) {
		CharGen cg;
		cg.Reload();
		cg.Start();
		
		TimeStop ts;
		while (ts.Elapsed() < 300*1000) Sleep(1000);
	} else {
		CharGen().Run();
	}
}
