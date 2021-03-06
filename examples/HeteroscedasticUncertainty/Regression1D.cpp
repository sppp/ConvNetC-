#include "Regression1D.h"

#define IMAGECLASS Regression1DImg
#define IMAGEFILE <Regression1D/Regression1D.iml>
#include <Draw/iml_source.h>

Regression1D::Regression1D() {
	Title("Heteroscedastic regression with dropout uncertainty demo");
	Icon(Regression1DImg::icon());
	Sizeable().MaximizeBox().MinimizeBox().Zoomable();
	
	function = 0;
	
	t =		"[\n"
			"\t{\"type\":\"input\", \"input_width\":1, \"input_height\":1, \"input_depth\":1},\n"
			"\t{\"type\":\"fc\", \"neuron_count\":20, \"activation\": \"tanh\"},\n"
			"\t{\"type\":\"dropout\", \"drop_prob\":" + DblStr(drop_prob) + "},\n"
			"\t{\"type\":\"fc\", \"neuron_count\":20, \"activation\": \"tanh\"},\n"
			"\t{\"type\":\"dropout\", \"drop_prob\":" + DblStr(drop_prob) + "},\n"
			"\t{\"type\":\"heteroscedastic_regression\", \"neuron_count\":2},\n"
			"\t{\"type\":\"adadelta\", \"learning_rate\":0.01, \"momentum\":0, \"batch_size\":1, \"l2_decay\":0.001}\n"
			"]\n";
	ses.SetTestPredict(true);
	ses.SetPredictInterval(10);
	
	net_edit.SetData(t);
	net_ctrl.Add(net_edit.HSizePos().VSizePos(0,30));
	net_ctrl.Add(reload_btn.HSizePos().BottomPos(0,30));
	reload_btn.SetLabel("Reload Network");
	reload_btn <<= THISBACK(Reload);
	
	Add(layer_ctrl.HSizePos().VSizePos(0,30));
	Add(lbl_pointcount.BottomPos(3,24).LeftPos(4, 250-4));
	Add(pointcount.BottomPos(3,24).LeftPos(250,100));
	Add(funcs.BottomPos(3,24).LeftPos(350,200));
	Add(regen.BottomPos(3,24).LeftPos(550,200));
	
	funcs.Add("-x * sin(x)");
	funcs.Add("sin(x)");
	funcs.Add("x**2");
	funcs.SetIndex(0);
	
	lbl_pointcount.SetLabel("Number of points to generate:");
	regen.SetLabel("Regenerate data");
	regen <<= THISBACK(Regenerate);
	funcs <<= THISBACK(Regenerate);
	
	pointcount.SetData(100);
	
	layer_ctrl.SetSession(ses);
	
	Regenerate();
	Reload();
	
	network_view.SetSession(ses);
	
	PostCallback(THISBACK(Refresher));
}

void Regression1D::DockInit() {
	AutoHide(DOCK_LEFT, Dockable(net_ctrl, "Edit Network").SizeHint(Size(640, 320)));
	DockRight(Dockable(network_view, "Network View").SizeHint(Size(320, 480)));
}

void Regression1D::Refresher() {
	layer_ctrl.Refresh();
	network_view.Refresh();
	
	PostCallback(THISBACK(Refresher));
}

void Regression1D::Reload() {
	ses.StopTraining();
	
	String net_str = net_edit.GetData();
	
	bool success = ses.MakeLayers(net_str);
	
	layer_ctrl.Layout();
	
	if (success) {
		ses.StartTraining();
	}
}

void Regression1D::Regenerate() {
	ses.StopTraining();
	
	int N = min(1000, max(2, (int)pointcount.GetData()));
	
	SessionData& d = ses.Data();
	d.BeginDataResult(2, N+1, 1, 0);
	
	function = min(2, max(0, funcs.GetIndex()));
	
	for (int i = 0; i < N; i++) {
		double x = Randomf() * 10 - 5;
		double y;
		if (function == 0)
			y = -x * sin(x);
		else if (function == 1)
			y = sin(x);
		else if (function == 2)
			y = x * x;
		d.SetData(i, 0, x);
		d.SetResult(i, 0, y);
		d.SetResult(i, 1, y);
	}
	
	d.SetData(N, 0, 0);
	d.SetResult(N, 0, -1);
	d.SetResult(N, 1, 0);
	
	d.EndData();
	
	ses.StartTraining();
}
