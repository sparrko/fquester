#include <iostream>
#include "fdpPacker.h"


int main(int argc, char** argv) {
	FDP_Packer fdpz;

	if (!fdpz.addResource("res0/default.png", "default.png")) std::cout << "Error: " + fdpz.getError() << std::endl;
	if (!fdpz.addResource("res0/1402.png", "1402.png")) std::cout << "Error: " + fdpz.getError() << std::endl;
	if (!fdpz.addResource("res0/frame.png", "frame.png")) std::cout << "Error: " + fdpz.getError() << std::endl;
	
	if (!fdpz.build("result/res0.fdp", "FQuaster Logo. ")) std::cout << "Error: " + fdpz.getError() << std::endl;


	FDP_Packer fdpo;

	for (int a = 0; a < 62; a++) {
		std::string z = "res1/anim/"; z += std::to_string((long long)a); z += ".png";
		std::string x = "load_anim/"; x += std::to_string((long long)a); x += ".png";
		if (!fdpo.addResource(z, x)) std::cout << "Error: " + fdpo.getError() << std::endl;
	}

	if (!fdpo.addResource("res1/limitTextBox.ogg", "audio/limitTextBox.ogg")) std::cout << "Error: " + fdpo.getError() << std::endl;
	if (!fdpo.addResource("res1/shutdown.ogg", "audio/shutdown.ogg")) std::cout << "Error: " + fdpo.getError() << std::endl;
	if (!fdpo.addResource("res1/startup.ogg", "audio/startup.ogg")) std::cout << "Error: " + fdpo.getError() << std::endl;
	
	if (!fdpo.addResource("res1/messageBox.ogg", "audio/messageBox.ogg")) std::cout << "Error: " + fdpo.getError() << std::endl;
	if (!fdpo.addResource("res1/warning.ogg", "audio/warning.ogg")) std::cout << "Error: " + fdpo.getError() << std::endl;
	if (!fdpo.addResource("res1/completed.ogg", "audio/completed.ogg")) std::cout << "Error: " + fdpo.getError() << std::endl;

	if (!fdpo.addResource("res1/screenshot.ogg", "audio/screenshot.ogg")) std::cout << "Error: " + fdpo.getError() << std::endl;

	if (!fdpo.build("result/res1.fdp", "FQuaster. First resources. ")) std::cout << "Error: " + fdpo.getError() << std::endl;


	FDP_Packer fdpo2;

	if (!fdpo2.addResource("res2/audio/auction.ogg", "audio/auction.ogg")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/audio/categories.ogg", "audio/categories.ogg")) std::cout << "Error: " + fdpo2.getError() << std::endl;	
	if (!fdpo2.addResource("res2/audio/end.ogg", "audio/end.ogg")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/audio/haha.ogg", "audio/haha.ogg")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/audio/menu.ogg", "audio/menu.ogg")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/audio/part_one.ogg", "audio/part_one.ogg")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/audio/part_three.ogg", "audio/part_three.ogg")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/audio/part_two.ogg", "audio/part_two.ogg")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/audio/path_four.ogg", "audio/path_four.ogg")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/audio/time.ogg", "audio/time.ogg")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/audio/sb_press.ogg", "audio/sb_press.ogg")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/audio/serf.ogg", "audio/serf.ogg")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/audio/tick.ogg", "audio/tick.ogg")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/audio/tack.ogg", "audio/tack.ogg")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/audio/plus.ogg", "audio/plus.ogg")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/audio/high.ogg", "audio/high.ogg")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/audio/result.ogg", "audio/result.ogg")) std::cout << "Error: " + fdpo2.getError() << std::endl;

	if (!fdpo2.addResource("res2/texture/bg_menu.png", "texture/bg_menu.png")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/texture/gui_menu.png", "texture/gui_menu.png")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/texture/frame.png", "texture/frame.png")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/texture/light.png", "texture/light.png")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/texture/logo.png", "texture/logo.png")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/texture/cursor.png", "texture/cursor.png")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/texture/select_select.png", "texture/select_select.png")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/texture/select_static.png", "texture/select_static.png")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/texture/floatButton.png", "texture/floatButton.png")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/texture/floatBar.png", "texture/floatBar.png")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/texture/cmdPointsFrame.png", "texture/cmdPointsFrame.png")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/texture/cmdPointsBg.png", "texture/cmdPointsBg.png")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/texture/bg_game.png", "texture/bg_game.png")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/texture/ask_frame.png", "texture/ask_frame.png")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	if (!fdpo2.addResource("res2/texture/scmd.png", "texture/scmd.png")) std::cout << "Error: " + fdpo2.getError() << std::endl;
	
	for (int a = 0; a < 80; a++) {
		std::string z = "res2/anim_music/"; z += std::to_string((long long)a); z += ".png";
		std::string x = "anim_music/"; x += std::to_string((long long)a); x += ".png";
		if (!fdpo2.addResource(z, x)) std::cout << "Error: " + fdpo2.getError() << std::endl;
	}

	if (!fdpo2.build("result/res2.fdp", "FQuaster. Second resources. ")) std::cout << "Error: " + fdpo2.getError() << std::endl;

	std::cout << "End" << std::endl;

	system("PAUSE");

	return 0;
}