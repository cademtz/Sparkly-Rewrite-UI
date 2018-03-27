#include "Aimbot.h"

Aimbot gAim;

// Do your coding shenanigans here

// This is purely example code. Reference this to see how you can use controls
void Aimbot::DoAimbot()
{
	if (!enabled.value)
		return; // Stop right here if aimbot is off

	int HitboxIndex = 0;
	
	/*	The value of our listbox is the index of our selected options
	*	Example: { "Head", "Body", "Auto" }... Head is index 0, Body is index 1, Auto is 2, etc...
	*	We can use a switch or if statement to get the desired hitbox based on the listbox
	*/

	if (hitbox.value == 0) // "Head" option
		HitboxIndex = 0; // Head hitbox index for TF2
	else if (hitbox.value == 1) // "Body" option
		HitboxIndex = 2; // Some random torso bone
	else if (hitbox.value == 2)
		/* HitboxIndex = AutoSelect() */; // You can do it yourself

	bool ShootBullet = false;
	if (shoot.KeyDown() && ShootBullet)
		/* MakeAimbotShoot() */; // KeyDown() will always return according to keybind settings
}