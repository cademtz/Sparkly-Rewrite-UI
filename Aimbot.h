#pragma once
#include "Controls.h"

class Aimbot
{
public:
	// ======= Declaring some menu variables here ===== //

	Checkbox enabled = Checkbox("Enabled");
	KeyBind shoot = KeyBind("Auto shoot", VK_SHIFT);
	Checkbox hitscan = Checkbox("Hitscan");
	Listbox hitbox = Listbox("Hitbox", { "Head", "Body", "Auto" });

	Checkbox silent = Checkbox("Silent");
	Checkbox backtrack = Checkbox("Backtrack");
	Slider tick = Slider("Backtrack tick", 13, 0, 13, 1);

	// More coding stuff here
	void DoAimbot();
};
extern Aimbot gAim;