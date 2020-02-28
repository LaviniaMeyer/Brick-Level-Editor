#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
using namespace sf;

class app
{

private:
	Event				event;				// The event
	View				View;				// The view
	RenderWindow		window;				// The render window
	Clock				clock;

	//Brick

	const int			n = 15;
	const int			m = 15;
	int					n2;
	int					m2;
	int					width;
	int					height;
	float				gap;
	float				space;
	float				space2;
	int					hits_selected;
	Texture				cracked1;
	Texture				cracked2;
	RectangleShape**	brick;
	bool**				collide;
	int**				hits_current;
	int**				hits_original;

	//color selection
	Color				newC;
	Color				current;
	Image				image;
	const int			o = 13;
	const int			p = 14;
	int					Cwidth;
	int					Cheight;
	float				Cgap;
	float				Cspace;
	float				Cspace2;
	int					brush_size;
	RectangleShape		brush_UI[3];
	Texture				brush_txtr[3];
	RectangleShape**	Cbrick;

	//Audio
	SoundBuffer			startB;
	Sound				startS;
	SoundBuffer			hitB;
	Sound				hitS;
	SoundBuffer			breakB;
	Sound				breakS;
	SoundBuffer			clickB;
	Sound				clickS;
	SoundBuffer			endB;
	Sound				endS;
	Music				music;

	//circle
	float				radius;
	int					signx;
	int					signy;
	float				rxspeed;
	float				ryspeed;
	Font				font;
	CircleShape			circle;

	//Buttons
	RectangleShape		play;
	RectangleShape		clear;
	RectangleShape		hits_UI[3];
	Texture				hits_txtr[3];
	RectangleShape		templates_UI[3];
	Image				templates[3];
	Texture				templates_image[3];
	bool				gameplay;
	RectangleShape*		grid_UI;
	RectangleShape*		grid_UI2;

	//Text
	Text				play_text;
	Text				clear_text;
	Text				back_text;
	Text				gridW_text;
	Text				gridL_text;
	Text				gridW_label;
	Text				gridL_label;
	Text				brush_label;
	Text				hits_label;
	Text				colour_label;

	//Time
	float				deltaTime;

public:

	app(char* Title,						// The constructor
		int   ScreenWidth,
		int   ScreenHeight,
		int   ScreenBpp);

	~app();									// The destructor

	bool Init();							// Initializes everything not in the constructor
	void HandleEvents();					// Handles input from the player
	void Draw();							// Draws the scene
	void Update();							// Updates variables, game object, etc.
	void Run();
};