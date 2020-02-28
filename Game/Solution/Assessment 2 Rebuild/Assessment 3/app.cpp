#include "app.h"
#include <iostream>
using namespace std;

//Constructor
app::app(char *Title, int ScreenWidth, int ScreenHeight, int ScreenBpp)
{
	window.create(VideoMode(1920, 1080, ScreenBpp), "Arkanoid Level Editor");
	window.setFramerateLimit(0);
	View = window.getDefaultView();
}

//Initialisation
bool app::Init()
{
	// Initialize stuff

	//Bricks
	n2 = 10;
	m2 = 10;
	width = window.getSize().y / n2;
	height = window.getSize().x / (m2 * 6);
	gap = 10;
	space = (window.getSize().x - (n2*width + (n2 - 1) * gap)) / 2.0f;
	space2 = (window.getSize().y - (m2*height + (m2 - 1) * gap)) / 5.0f;
	hits_selected = 1;

	//Brick array
	brick = new RectangleShape*[n];
	for (int i = 0; i < n; i++){
		brick[i] = new RectangleShape[m];
	}
	//Collideable bricks
	collide = new bool*[n];
	for (int i = 0; i < n; i++) {
		collide[i] = new bool[m];
	}
	//Brick hits current
	hits_current = new int*[n];
	for (int i = 0; i < n; i++) {
		hits_current[i] = new int[m];
	}
	//Brick hits original
	hits_original = new int*[n];
	for (int i = 0; i < n; i++) {
		hits_original[i] = new int[m];
	}
	//Load textures
	if (!cracked1.loadFromFile("cracked1.png"))
	{
		cout << "Texture1 not loaded" << endl;
	}
	if (!cracked2.loadFromFile("cracked2.png"))
	{
		cout << "Texture2 not loaded" << endl;
	}

	for (int i = 0; i < n2; ++i)
	{
		for (int j = 0; j < m2; ++j)
		{
			brick[i][j].setSize(Vector2f(width, height));
			brick[i][j].setPosition(Vector2f(space + i*(width + gap), space2 + j*(height + gap)));
			brick[i][j].setFillColor(Color::Transparent);
			brick[i][j].setOutlineColor(Color::White);
			brick[i][j].setOutlineThickness(2.0f);
			collide[i][j] = true;
			hits_current[i][j] = 1;
		}
	}

	//Colour selection
	if (!image.loadFromFile("colors.png"))
	{
		cout << "Image not loaded" << endl;
	}

	current = Color::Transparent;
	Cwidth = 20;
	Cheight = 20;
	Cgap = 1;
	Cspace = window.getSize().x - Cwidth * o - 50;
	Cspace2 = window.getSize().y - Cheight * p - 50;
	brush_size = 1;

	Cbrick = new RectangleShape*[o];
	for (int i = 0; i < p; i++) {
		Cbrick[i] = new RectangleShape[p];
	}

	for (int i = 0; i < o; ++i)
	{
		for (int j = 0; j < p; ++j)
		{
			Cbrick[i][j].setSize(Vector2f(Cwidth, Cheight));
			Cbrick[i][j].setPosition(Vector2f(Cspace + i*(Cwidth + Cgap), Cspace2 + j*(Cheight + Cgap)));
			newC = image.getPixel(12 - i, 13 - j);
			Cbrick[i][j].setFillColor(newC);
		}
	}

	//Audio
	startB.loadFromFile("start.wav");
	startS.setBuffer(startB);

	hitB.loadFromFile("hit2.wav");
	hitS.setBuffer(hitB);

	breakB.loadFromFile("destroy2.wav");
	breakS.setBuffer(breakB);

	clickB.loadFromFile("bleep.wav");
	clickS.setBuffer(clickB);

	endB.loadFromFile("end.wav");
	endS.setBuffer(endB);

	music.openFromFile("music4.wav");
	music.play();

	//circle
	radius = 5.0f;
	circle.setRadius(radius);
	circle.setFillColor(Color::Red);
	circle.setPosition(window.getSize().x / 2 - radius, window.getSize().y - 300 - radius);
	srand(time(NULL));

	signx = 2 * (rand() % 2) - 1;
	signy = 2 * (rand() % 2) - 1;
	rxspeed = signx * (rand() % 10 + 200);
	ryspeed = signy * (rand() % 10 + 200);

	//Buttons
	gameplay = false;
	//RectangleShape grid_UI[n];
	grid_UI = new RectangleShape[n];
	//RectangleShape grid_UI2[m];
	grid_UI2 = new RectangleShape[m];

	//Play Button
	play.setSize(Vector2f(240, 100));
	play.setPosition(Vector2f(50, window.getSize().y - 350));
	play.setFillColor(Color::White);

	//Clear Button
	clear.setSize(Vector2f(240, 100));
	clear.setPosition(Vector2f(50, window.getSize().y - 150));
	clear.setFillColor(Color::White);

	//Brick Hits Buttons
	hits_txtr[0].loadFromFile("hits1.png");
	hits_txtr[1].loadFromFile("hits2.png");
	hits_txtr[2].loadFromFile("hits3.png");
	for (int i = 0; i < 3; i++)
	{
		hits_UI[i].setSize(Vector2f(100, 100));
		hits_UI[i].setPosition(Vector2f(window.getSize().x - (50 + hits_UI[i].getSize().x), space2 + (i * (hits_UI[i].getSize().y + 50))));
		hits_UI[i].setFillColor(Color::White);
		hits_UI[i].setTexture(&hits_txtr[i]);
	}

	//Grid Width Buttons
	for (int i = 0; i < n; i++)
	{
		grid_UI[i].setSize(Vector2f(50, 50));
		grid_UI[i].setPosition(Vector2f(i * (grid_UI[i].getSize().x + 10) + space, 10));
		grid_UI[i].setFillColor(Color::White);
	}

	//Grid Length Buttons
	for (int i = 0; i < m; i++)
	{
		grid_UI2[i].setSize(Vector2f(50, 50));
		grid_UI2[i].setPosition(Vector2f(i * (grid_UI2[i].getSize().x + 10) + space, 750));
		grid_UI2[i].setFillColor(Color::White);
	}

	//Brush Size Buttons
	brush_txtr[0].loadFromFile("brush1.png");
	brush_txtr[1].loadFromFile("brush2.png");
	brush_txtr[2].loadFromFile("brush3.png");
	for (int i = 0; i < 3; i++)
	{
		brush_UI[i].setSize(Vector2f(100, 100));
		brush_UI[i].setPosition(Vector2f(50, space2 + (i * (brush_UI[i].getSize().y + 50))));
		brush_UI[i].setFillColor(Color::White);
		brush_UI[i].setTexture(&brush_txtr[i]);
	}

	//Template Buttons
	if (!templates[0].loadFromFile("cupcake.png"))
	{
		cout << "Template Texture 1 not loaded" << endl;
	}
	if (!templates[1].loadFromFile("duck.png"))
	{
		cout << "Template Texture 2 not loaded" << endl;
	}
	if (!templates[2].loadFromFile("flower.png"))
	{
		cout << "Template Texture 3 not loaded" << endl;
	}
	for (int i = 0; i < 3; i++)
	{
		templates_image[i].loadFromImage(templates[i]);
		templates_UI[i].setSize(Vector2f(350, 200));
		templates_UI[i].setPosition(Vector2f(space + i * (templates_UI[i].getSize().x + 50), window.getSize().y - (templates_UI[i].getSize().y + 50)));
		templates_UI[i].setOutlineColor(Color::White);
		templates_UI[i].setOutlineThickness(2.0f);
		templates_UI[i].setFillColor(Color::White);
		templates_UI[i].setTexture(&templates_image[i]);
	}

	//Text
	if (!font.loadFromFile("secrcode.ttf"))
	{
		std::cout << "Font Load Error";
	}

	play_text.setString("Play!");
	play_text.setFont(font);
	play_text.setPosition(Vector2f(play.getPosition().x + 20, play.getPosition().y + 10));
	play_text.setColor(Color::Black);
	play_text.setCharacterSize(70);

	clear_text.setString("Clear");
	clear_text.setFont(font);
	clear_text.setPosition(Vector2f(clear.getPosition().x + 20, clear.getPosition().y + 10));
	clear_text.setColor(Color::Black);
	clear_text.setCharacterSize(70);

	back_text.setString("Press Escape Key to return to Editor.");
	back_text.setFont(font);
	back_text.setPosition(Vector2f(50, window.getSize().y - 120));
	back_text.setColor(Color::White);
	back_text.setCharacterSize(70);

	gridW_text.setString(" 1  2  3  4 5  6  7  8  9 10 1112 13 14 15");
	gridW_text.setFont(font);
	gridW_text.setPosition(grid_UI[0].getPosition().x, grid_UI[0].getPosition().y);
	gridW_text.setColor(Color::Black);
	gridW_text.setCharacterSize(40);

	gridL_text.setString(" 1  2  3  4 5  6  7  8  9 10 1112 13 14 15");
	gridL_text.setFont(font);
	gridL_text.setPosition(grid_UI2[0].getPosition().x, grid_UI2[0].getPosition().y);
	gridL_text.setColor(Color::Black);
	gridL_text.setCharacterSize(40);

	gridW_label.setString("Grid Width");
	gridW_label.setFont(font);
	gridW_label.setPosition(grid_UI[14].getPosition().x + grid_UI[0].getSize().x + 50, grid_UI[0].getPosition().y);
	gridW_label.setColor(Color::White);
	gridW_label.setCharacterSize(50);

	gridL_label.setString("Grid Height");
	gridL_label.setFont(font);
	gridL_label.setPosition(grid_UI2[14].getPosition().x + grid_UI2[0].getSize().x + 50, grid_UI2[0].getPosition().y);
	gridL_label.setColor(Color::White);
	gridL_label.setCharacterSize(50);

	brush_label.setString("Brush Size");
	brush_label.setFont(font);
	brush_label.setPosition(0, brush_UI[2].getPosition().y + 150);
	brush_label.setColor(Color::White);
	brush_label.setCharacterSize(50);

	hits_label.setString("Brick\nHits");
	hits_label.setFont(font);
	hits_label.setPosition(window.getSize().x - 200, hits_UI[2].getPosition().y + 100);
	hits_label.setColor(Color::White);
	hits_label.setCharacterSize(50);

	colour_label.setString("Colour Picker");
	colour_label.setFont(font);
	colour_label.setPosition(Cbrick[0][0].getPosition().x - 50, Cbrick[0][0].getPosition().y - 100);
	colour_label.setColor(Color::White);
	colour_label.setCharacterSize(50);

	return true;
}

//Destructor
app::~app()
{
	// Clean stuff up
	//brick shapes
	for (int i = 0; i < n; ++i) {
		delete[] brick[i];
	}
	delete[] brick;

	//collide
	for (int i = 0; i < n; ++i) {
		delete[] collide[i];
	}
	delete[] collide;

	//brick hits current
	for (int i = 0; i < n; ++i) {
		delete[] hits_current[i];
	}
	delete[] hits_current;

	//brick hits original
	for (int i = 0; i < n; ++i) {
		delete[] hits_original[i];
	}
	delete[] hits_original;

	//Colour selection
	for (int i = 0; i < o; ++i) {
		delete[] Cbrick[i];
	}
	delete[] Cbrick;

	//UI grids
	delete[] grid_UI;
	delete[] grid_UI2;
}

//do this every frame
void app::Update()
{
	//Update stuff
	deltaTime = clock.restart().asSeconds();

	//Brick Textures
	for (int i = 0; i < n2; ++i)
	{
		for (int j = 0; j < m2; ++j)
		{
			if (hits_current[i][j] == 1 && collide[i][j] == true)
			{
				brick[i][j].setTexture(&cracked1);
			}
			else if (hits_current[i][j] == 2)
			{
				brick[i][j].setTexture(&cracked2);
			}
			else if (hits_current[i][j] == 3)
			{
				brick[i][j].setTexture(NULL);
			}
		}
	}

	//Gameplay
	if (gameplay == true)
	{
		circle.move(rxspeed*deltaTime, ryspeed*deltaTime);

		//border collision
		if (circle.getPosition().x < 0)
		{
			rxspeed = -rxspeed;
			hitS.play();
		}
		else if (circle.getPosition().x > window.getSize().x - 2 * radius)
		{
			rxspeed = -rxspeed;
			hitS.play();
		}

		if (circle.getPosition().y < 0)
		{
			ryspeed = -ryspeed;
			hitS.play();
		}
		else if (circle.getPosition().y > window.getSize().y - 2 * radius)
		{
			ryspeed = -ryspeed;
			hitS.play();
		}

		//Brick Collision
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < m; ++j)
			{
				if (circle.getGlobalBounds().intersects(brick[i][j].getGlobalBounds()) && collide[i][j] == true)
				{
					--hits_current[i][j];
					if (hits_current[i][j] == 0)
					{
						collide[i][j] = false;
						breakS.play();
					}

					if (circle.getPosition().x < (brick[i][j].getPosition().x + width) && (circle.getPosition().y > brick[i][j].getPosition().y + height || circle.getPosition().y < brick[i][j].getPosition().y)) //top & bottom collision
					{
						ryspeed = -ryspeed;
						hitS.play();
					}
					else if (circle.getPosition().y < (brick[i][j].getPosition().y + height) && (circle.getPosition().x > brick[i][j].getPosition().x + width || circle.getPosition().x < brick[i][j].getPosition().x)) //side collisions
					{
						rxspeed = -rxspeed;
						hitS.play();
					}
				}
			}
		}
	}
}

//render
void app::Draw()
{
	window.clear();
	window.setView(View);

	//Draw stuff needed
	//draw bricks
	for (int i = 0; i < n2; ++i)
	{
		for (int j = 0; j < m2; ++j)
		{
			if (collide[i][j] == true)
			{
				window.draw(brick[i][j]);
			}
		}
	}

	//draw UI (when not in play mode)
	if (gameplay == false)
	{
		for (int i = 0; i < o; ++i)
		{
			for (int j = 0; j < p; ++j)
			{
				window.draw(Cbrick[i][j]);
			}
		}
		window.draw(play);
		window.draw(play_text);
		window.draw(clear);
		window.draw(clear_text);
		for (int i = 0; i < 3; i++)
		{
			window.draw(hits_UI[i]);
			window.draw(brush_UI[i]);
			window.draw(templates_UI[i]);
		}
		for (int i = 0; i < n; i++)
		{
			window.draw(grid_UI[i]);
			window.draw(grid_UI2[i]);
		}
		window.draw(gridW_text);
		window.draw(gridL_text);
		window.draw(gridW_label);
		window.draw(gridL_label);
		window.draw(brush_label);
		window.draw(hits_label);
		window.draw(colour_label);
	}
	else
	{
		window.draw(circle);
		window.draw(back_text);
	}

	window.display();
}

//user input
void app::HandleEvents()
{
	//Event Handling
	while (window.pollEvent(event))
	{		
		switch (event.type)
		{
		case Event::Closed:
			window.close();
			break;

		case Event::MouseButtonPressed:
			if (event.mouseButton.button == Mouse::Left)
			{
				clickS.play();
			}
			break;

		default:
			break;
		}
	}

	//User Click
	if (Mouse::isButtonPressed(Mouse::Left) && gameplay == false)
	{
		Vector2i localPosition = Mouse::getPosition(window);

		//Fill bricks with colour
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < m; ++j)
			{
				if (brick[i][j].getGlobalBounds().contains(localPosition.x, localPosition.y))
				{
					for (int k = 0; k < brush_size; k++)
					{
						for (int l = 0; l < brush_size; l++)
						{
							if (i + k < n && j + l < m)
							{
								brick[i + k][j + l].setFillColor(current);
								hits_current[i + k][j + l] = hits_selected;
								hits_original[i + k][j + l] = hits_selected;
							}
						}
					}
				}
			}
		}

		//Colour selection
		for (int i = 0; i < o; ++i)
		{
			for (int j = 0; j < p; ++j)
			{
				if (Cbrick[i][j].getGlobalBounds().contains(localPosition.x, localPosition.y))
				{
					current = Cbrick[i][j].getFillColor();
				}
			}
		}

		//Play Button
		if (play.getGlobalBounds().contains(localPosition.x, localPosition.y))
		{
			if (gameplay == false)
			{
				startS.play();
			}

			gameplay = true;
			circle.setPosition(window.getSize().x / 2 - radius, window.getSize().y - 300 - radius);
			for (int i = 0; i < n; ++i)
			{
				for (int j = 0; j < m; ++j)
				{
					if (brick[i][j].getFillColor() == Color::White)
					{
						brick[i][j].setFillColor(Color::Transparent);
					}
					brick[i][j].setOutlineColor(Color::Transparent);
					if (brick[i][j].getFillColor() == Color::Transparent)
					{
						collide[i][j] = false;
					}
				}
			}
		}

		//Clear Button
		if (clear.getGlobalBounds().contains(localPosition.x, localPosition.y))
		{
			for (int i = 0; i < n; ++i)
			{
				for (int j = 0; j < m; ++j)
				{
					brick[i][j].setFillColor(Color::Transparent);
					hits_current[i][j] = 1;
					hits_original[i][j] = 1;
				}
			}
		}

		//Brick Hit Selection
		for (int i = 0; i < 3; i++)
		{
			if (hits_UI[i].getGlobalBounds().contains(localPosition.x, localPosition.y))
			{
				hits_selected = i + 1;
			}
		}

		//Brush Size Selection
		for (int i = 0; i < 3; i++)
		{
			if (brush_UI[i].getGlobalBounds().contains(localPosition.x, localPosition.y))
			{
				brush_size = i + 1;
			}
		}

		//Grid Width Selection
		for (int i = 0; i < n; i++)
		{
			if (grid_UI[i].getGlobalBounds().contains(localPosition.x, localPosition.y))
			{
				n2 = i + 1;
				width = window.getSize().y / n2;
				height = window.getSize().x / (m2 * 6);
				for (int k = 0; k < n2; ++k)
				{
					for (int j = 0; j < m2; ++j)
					{
						brick[k][j].setSize(Vector2f(width, height));
						brick[k][j].setPosition(Vector2f(space + k*(width + gap), space2 + j*(height + gap)));
						collide[k][j] = true;
						hits_current[k][j] = 1;
						brick[k][j].setFillColor(Color::Transparent);
						brick[k][j].setOutlineColor(Color::White);
						brick[k][j].setOutlineThickness(2.0f);
					}
				}
			}
		}

		//Grid Length Selection
		for (int i = 0; i < m; i++)
		{
			if (grid_UI2[i].getGlobalBounds().contains(localPosition.x, localPosition.y))
			{
				m2 = i + 1;
				width = window.getSize().y / n2;
				height = window.getSize().x / (m2 * 6);
				for (int k = 0; k < n2; ++k)
				{
					for (int j = 0; j < m2; ++j)
					{
						brick[k][j].setSize(Vector2f(width, height));
						brick[k][j].setPosition(Vector2f(space + k*(width + gap), space2 + j*(height + gap)));
						collide[k][j] = true;
						hits_current[k][j] = 1;
						brick[k][j].setFillColor(Color::Transparent);
						brick[k][j].setOutlineColor(Color::White);
						brick[k][j].setOutlineThickness(2.0f);
					}
				}
			}
		}

		//Template Selection
		for (int i = 0; i < 3; i++)
		{
			if (templates_UI[i].getGlobalBounds().contains(localPosition.x, localPosition.y))
			{
				//set bricks to colours
				for (int k = 0; k < n2; k++)
				{
					for (int j = 0; j < m2; j++)
					{
						Color C = templates[i].getPixel(k * (templates[i].getSize().x / n2), j * (templates[i].getSize().y / m2));
						brick[k][j].setFillColor(C);
						hits_current[k][j] = hits_selected;
						hits_original[k][j] = hits_selected;
						collide[k][j] = true;
					}
				}
			}
		}
	}

	//Back to Editor
	if (Keyboard::isKeyPressed(Keyboard::Escape) && gameplay == true)
	{
		endS.play();

		gameplay = false;
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < m; ++j)
			{
				brick[i][j].setOutlineColor(Color::White);
				collide[i][j] = true;
				hits_current[i][j] = hits_original[i][j];
			}
		}
	}

	//Erase Bricks
	if (Mouse::isButtonPressed(Mouse::Right))
	{
		Vector2i localPosition = Mouse::getPosition(window);
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < m; ++j)
			{
				if (brick[i][j].getGlobalBounds().contains(localPosition.x, localPosition.y))
				{
					brick[i][j].setFillColor(Color::Transparent);
					hits_current[i][j] = 1;
					hits_original[i][j] = 1;
				}
			}
		}
	}
}

//Run the game
void app::Run()
{
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			HandleEvents();
		}
		Update();
		Draw();
	}
}