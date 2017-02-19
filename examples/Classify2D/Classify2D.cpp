#include "Classify2D.h"

Classify2D::Classify2D() : pctrl(session), lctrl(session) {
	Sizeable().MaximizeBox().MinimizeBox();
	Title("Classify2D");
	
	running = false;
	stopped = true;
	
	t = "[\n"
		"\t{\"type\":\"input\", \"input_width\":1, \"input_height\":1, \"input_depth\":2},\n"
		"\t{\"type\":\"fc\", \"neuron_count\":6, \"activation\": \"tanh\"},\n"
		"\t{\"type\":\"fc\", \"neuron_count\":2, \"activation\": \"tanh\"},\n"
		"\t{\"type\":\"softmax\", \"class_count\":2},\n"
		"\t{\"type\":\"sgd\", \"learning_rate\":0.01, \"momentum\":0.1, \"batch_size\":10, \"l2_decay\":0.001}\n"
		"]\n";
	
	net_ctrl.Add( net_edit.HSizePos().VSizePos(0, 30) );
	net_ctrl.Add( btn.LeftPos(5, 200).BottomPos(2, 26) );
	
	btn.SetLabel("Change network");
	btn <<= THISBACK(Reload);
	
	v_split << net_ctrl << h_split;
	v_split.Vert();
	v_split.SetPos(2500);
	
	h_split.Horz();
	h_split << parent_pctrl << lctrl;
	
	parent_pctrl.Add(pctrl.VSizePos(0,60).HSizePos());
	parent_pctrl.Add(pctrl_btns.BottomPos(0,60).HSizePos());
	pctrl_btns.Horz();
	pctrl_btns << btn_simple << btn_circle << btn_spiral << btn_random;
	btn_simple.SetLabel("Simple data");
	btn_circle.SetLabel("Circle data");
	btn_spiral.SetLabel("Spiral data");
	btn_random.SetLabel("Random data");
	btn_simple <<= THISBACK1(RandomData, 12);
	btn_circle <<= THISBACK1(CircleData, 100);
	btn_spiral <<= THISBACK1(SpiralData, 100);
	btn_random <<= THISBACK1(RandomData, 40);
	
	Add(v_split.SizePos());
	
	net_edit.SetData(t);
    
    PostCallback(THISBACK(OriginalData));
}

void Classify2D::ViewLayer(int i) {
	lctrl.ViewLayer(i);
}

void Classify2D::Reload() {
	String net_str = net_edit.GetData();
	session.LoadJSON(net_str);
	session.StartTraining();
	
	if (stopped) {
		running = true;
		stopped = false;
		PostCallback(THISBACK(Refresher));
	}
}

void Classify2D::RandomData(int count) {
	session.StopTraining();
	session.BeginData(count, 2);
	for(int k=0; k < count; k++) {
		session.SetData(k, 0, Randomf() * 10.0 - 5.0);
		session.SetData(k, 1, Randomf() * 10.0 - 5.0);
		session.SetLabel(k, Randomf() > 0.5 ? 1 : 0);
	}
	session.EndData();
	Reload();
}

void Classify2D::OriginalData() {
	session.StopTraining();
	session.BeginData(13, 2);
	session.SetData(0, 0, -0.4326)	.SetData(0, 1, 1.1909)	.SetLabel(0, 1);
	session.SetData(1, 0, 3.0)		.SetData(1, 1, 4.0)		.SetLabel(1, 1);
	session.SetData(2, 0, 0.1253)	.SetData(2, 1, -0.0376)	.SetLabel(2, 1);
	session.SetData(3, 0, 0.2877)	.SetData(3, 1, 0.3273)	.SetLabel(3, 1);
	session.SetData(4, 0, -1.1465)	.SetData(4, 1, 0.1746)	.SetLabel(4, 1);
	session.SetData(5, 0, 1.8133)	.SetData(5, 1, 1.0139)	.SetLabel(5, 0);
	session.SetData(6, 0, 2.7258)	.SetData(6, 1, 1.0668)	.SetLabel(6, 0);
	session.SetData(7, 0, 1.4117)	.SetData(7, 1, 0.5593)	.SetLabel(7, 0);
	session.SetData(8, 0, 4.1832)	.SetData(8, 1, 0.3044)	.SetLabel(8, 0);
	session.SetData(9, 0, 1.8636)	.SetData(9, 1, 0.1677)	.SetLabel(9, 0);
	session.SetData(10, 0, 0.5)		.SetData(10, 1, 3.2)	.SetLabel(10, 1);
	session.SetData(11, 0, 0.8)		.SetData(11, 1, 3.2)	.SetLabel(11, 1);
	session.SetData(12, 0, 1.0)		.SetData(12, 1, -2.2)	.SetLabel(12, 1);
	session.EndData();
	Reload();
}

void Classify2D::CircleData(int count) {
	session.StopTraining();
	session.BeginData(count, 2);
	int count_a = count / 2;
	int count_b = count - count_a;
	int j = 0;
	for(int i = 0; i < count_a; i++) {
		double r = Randomf() * 2.0;
		double t = Randomf() * 2.0*M_PI;
		session.SetData(j, 0, r*sin(t));
		session.SetData(j, 1, r*cos(t));
		session.SetLabel(j, 1);
		j++;
	}
	for(int i=0; i < count_b; i++) {
		double r = Randomf() * 2.0 + 3.0;
		double t = 2.0*M_PI*i/50.0;
		session.SetData(j, 0, r*sin(t));
		session.SetData(j, 1, r*cos(t));
		session.SetLabel(j, 0);
		j++;
	}
	session.EndData();
	Reload();
}

void Classify2D::SpiralData(int count) {
	session.StopTraining();
	session.BeginData(count, 2);
	int count_a = count / 2;
	int count_b = count - count_a;
	int j = 0;
	for(int i = 0; i < count_a; i++) {
		double r = (double)i / count_a * 5.0 + Randomf() * 0.2 - 0.1;
		double t = 1.25*(double)i/count_a*2*M_PI + Randomf() * 0.2 - 0.1;
		session.SetData(j, 0, r*sin(t));
		session.SetData(j, 1, r*cos(t));
		session.SetLabel(j, 1);
		j++;
	}
	for(int i=0; i < count_b; i++) {
		double r = (double)i/count_b*5.0 + Randomf() * 0.2 - 0.1;
		double t = 1.25*(double)i/count_b*2*M_PI + M_PI + Randomf() * 0.2 - 0.1;
		session.SetData(j, 0, r*sin(t));
		session.SetData(j, 1, r*cos(t));
		session.SetLabel(j, 0);
		j++;
	}
	session.EndData();
	Reload();
}




void Classify2D::Refresher() {
	if (session.IsTraining()) {
		#ifndef flagMT
		session.TrainIteration();
		#endif
		pctrl.Refresh();
		lctrl.Refresh();
	}
	if (running) PostCallback(THISBACK(Refresher));
	else stopped = true;
}



