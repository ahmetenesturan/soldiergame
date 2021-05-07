#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

class Coord //a class to use x and y coordinates
{
public:
	int x;
	int y;
	Coord()
	{
		x = 0;
		y = 0;
	}
	Coord(int new_x, int new_y)
	{
		x = new_x;
		y = new_y;
	}

	Coord change(int, int);
};

Coord Coord::change(int a, int b) //returns a new coordinate that's been moved
{
	int u = a + x;
	int v = b + y;
	Coord out = Coord(u, v);
	return out;
}

class WalkDirection //class to symbolize walk direction
{
private:
	int direction;
public:
	WalkDirection()
	{
		direction = 4;
	}
	void goup()
	{
		direction = 0;
	}
	void godown()
	{
		direction = 1;
	}
	void goleft()
	{
		direction = 2;
	}
	void goright()
	{
		direction = 3;
	}

	void still()
	{
		direction = 4;
	}

	int direc()
	{
		return direction;
	}
};

class Barrel
{
private:
	sf::Texture texture;
	sf::Sprite sprite;
	Coord pos;
public:
	void init(string texturePath, Coord new_pos)
	{
		pos = new_pos;
		if (!texture.loadFromFile(texturePath))
		{
			cout << "barrel cannot be loaded" << endl;
		}
		sprite.setTexture(texture);
	}
	void paint(sf::RenderWindow &window)
	{
		sprite.setPosition(sf::Vector2f(pos.x, pos.y)); //setting the position of the barrel
		window.draw(sprite); //drawing
	}

	bool inBound(Coord player_pos) //a function the know if player is colliding with the barrel
	{
		if ((player_pos.x > pos.x - 60) && (player_pos.x < pos.x + 20) && (player_pos.y > pos.y - 60) && (player_pos.y < pos.y + 20)) return true;
		else return false;
	}
};

class Sandbag
{
private:
	sf::Texture texture;
	sf::Sprite sprite;
	Coord pos;
public:
	void init(string texturePath, Coord new_pos)
	{
		pos = new_pos;
		if (!texture.loadFromFile(texturePath))
		{
			cout << "sandbag cannot be loaded" << std::endl;
		}
		sprite.setTexture(texture);
	}

	bool inBound(Coord player_pos) //a function the know if player is colliding with the sandbag
	{
		if ((player_pos.x > pos.x - 80) && (player_pos.x < pos.x + 40) && (player_pos.y > pos.y - 80) && (player_pos.y < pos.y + 40)) return true;
		else return false;
	}

	void paint(sf::RenderWindow &window)
	{
		sprite.setPosition(sf::Vector2f(pos.x, pos.y)); //setting the position
		window.draw(sprite); //drawing
	}
};

class Player
{
private:
	sf::Texture textures[14]; //Player texture array
	sf::Sprite sprite; //Player sprite
	Coord pos; //Position of the player on screen

public:
	void init(string textBasePath, int numTextures, Coord n_pos)
	{
		pos = n_pos;
		for (int i = 0; i < numTextures; i++) //for loop the set all soldier stances
		{
			string textpath = textBasePath + to_string(i) + ".png";
			if (!textures[i].loadFromFile(textpath))
			{
				cout << "soldier cannot be loaded" << endl;
			}
		}
		sprite.setTexture(textures[0]);

	}

	int checkCollision(WalkDirection, Barrel*, Sandbag*, int, int); //returns which way soldier is colliding with object

	void walk(float, WalkDirection, Barrel*, Sandbag*, int, int); //method the ensure walk movement

	void paint(sf::RenderWindow &window) //drawing the soldier
	{
		window.draw(sprite);
	}
};

void Player::walk(float speed, WalkDirection dir, Barrel* barrels, Sandbag* sandbags, int nb, int ns)
{
	pos = Coord(sprite.getPosition().x, sprite.getPosition().y); //position of player
	if (dir.direc() == 0)//going up
	{
		sprite.setTexture(textures[0]);
		if (checkCollision(dir ,barrels, sandbags, nb, ns) != dir.direc()) sprite.move(sf::Vector2f(0, -1 * speed));
	}

	else if (dir.direc() == 1)//going down
	{
		sprite.setTexture(textures[4]);
		if (checkCollision(dir, barrels, sandbags, nb, ns) != dir.direc()) sprite.move(sf::Vector2f(0, speed));
	}

	else if (dir.direc() == 2)//going left
	{
		sprite.setTexture(textures[12]);
		if (checkCollision(dir, barrels, sandbags, nb, ns) != dir.direc()) sprite.move(sf::Vector2f(-1 * speed, 0));
	}

	else if (dir.direc() == 3)//going right
	{
		sprite.setTexture(textures[9]);
		if (checkCollision(dir, barrels, sandbags, nb, ns) != dir.direc()) sprite.move(sf::Vector2f(speed, 0));
	}
}

int Player::checkCollision(WalkDirection dir,Barrel* barrels, Sandbag* sandbags, int nb, int ns)
{
	WalkDirection out;
	out.still();
	for (int i = 0; i < nb; i++)
	{
		if (barrels[i].inBound(pos)) //checking if soldier collides with barrels
		{
			if (dir.direc() == 0 && barrels[i].inBound(pos.change(0, -5)))
			{
				out.goup();
			}
			else if (dir.direc() == 1 && barrels[i].inBound(pos.change(0, 5)))
			{
				out.godown();
			}
			else if (dir.direc() == 2 && barrels[i].inBound(pos.change(-5, 0)))
			{
				out.goleft();
			}
			else if (dir.direc() == 3 && barrels[i].inBound(pos.change(5, 0)))
			{
				out.goright();
			}
		}
		else if (sandbags[i].inBound(pos)) //checking if soldier collides with sandbags
		{
			if (dir.direc() == 0 && sandbags[i].inBound(pos.change(0, -5)))
			{
				out.goup();
			}
			else if (dir.direc() == 1 && sandbags[i].inBound(pos.change(0, 5)))
			{
				out.godown();
			}
			else if (dir.direc() == 2 && sandbags[i].inBound(pos.change(-5, 0)))
			{
				out.goleft();
			}
			else if (dir.direc() == 3 && sandbags[i].inBound(pos.change(5, 0)))
			{
				out.goright();
			}
		}
	}
	return out.direc(); //which way soldier collides with object relative to soldier
}

class Game
{
private:
	float speed;
	int numBarrels;
	int numSandbags;
	int numPlayers;
	int width;
	int height;
	sf::Texture bgTexture;
	sf::Sprite bgSprite;
	Barrel* barrels;
	Sandbag* sandbags;
	Player* players;
	sf::RenderWindow window;

	void drawBackground();//drawing the grass tiles

	void createSandbags();//setting attributes of sandbags

	void createBarrels();//setting attributes of barrels

public:
	Game(float new_speed, int w, int h, int nb, int ns, int np)//dynamic creation of objects
	{
		speed = new_speed;
		numBarrels = nb;
		numSandbags = ns;
		numPlayers = np;
		width = w;
		height = h;
		window.create(sf::VideoMode(w, h), "game");
		players = new Player;
		players->init("soldier", 14, Coord(0, 0));
		sandbags = new Sandbag[ns];
		barrels = new Barrel[nb];
	}

	~Game()//destructor, dynamicly deleting objects
	{
		delete players;
		delete[] sandbags;
		delete[] barrels;
	}

	void display();
};

void Game::drawBackground()
{
	sf::Texture grasstex;
	if (!grasstex.loadFromFile("grass.png"))
	{
		cout << "grass cannot be loaded" << endl;
	}
	int wi, he;
	wi = width / 350 + 1; //how many tiles needed horizontaly
	he = height / 350 + 1; //how many tiles needed vertically
	sf::Sprite grass[30];
	int b; //dummy index variable for grass[]
	float h0, w0; //drawing location for each tile
	for (int i = 0; i < wi; i++)
	{
		for (int j = 0; j < he; j++)
		{
			b = i + j;
			h0 = (float) j * 350;
			w0 = (float) i * 350;
			grass[b].setTexture(grasstex);
			grass[b].setPosition(sf::Vector2f(w0, h0));
			window.draw(grass[b]);
		}
	}
}

void Game::display()
{
	createSandbags();
	createBarrels();
	WalkDirection direc; //soldier walk direction
	while (window.isOpen()) //while loop to keep window open
	{

		drawBackground(); //drawing grass
		for (int i = 0; i < numSandbags; i++)//drawing sandbags
		{
			sandbags[i].paint(window);
		}
		for (int i = 0; i < numBarrels; i++)//drawing barrels
		{
			barrels[i].paint(window);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))//keyboard input for going up
		{
			direc.goup();
			players->walk(speed, direc, barrels, sandbags, numBarrels, numSandbags);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))//keyboard input for going down
		{
			direc.godown();
			players->walk(speed, direc, barrels, sandbags, numBarrels, numSandbags);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))//keyboard input for going left
		{
			direc.goleft();
			players->walk(speed, direc, barrels, sandbags, numBarrels, numSandbags);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))//keyboard input for going right
		{
			direc.goright();
			players->walk(speed, direc, barrels, sandbags, numBarrels, numSandbags);
		}
		players->paint(window);//drawing player
		sf::Event event;
		while (window.pollEvent(event))//closing the window if user presses close button
		{
			if (event.type == sf::Event::Closed) window.close();
		}

		window.display(); //displaying the window
	}
}

void Game::createSandbags()//placing sandbags randomly
{
	srand((unsigned int) time(NULL));
	int ns = numSandbags;
	Coord n_pos;
	int new_w, new_h;
	for (int i = 0; i < ns; i++)
	{
		new_w = rand() % width;
		new_h = rand() % height;
		sandbags[i].init("bags.png", Coord(new_w, new_h));
	}
}

void Game::createBarrels()//placing barrels randomly
{
	srand((unsigned int)(time(NULL) + 1e5));
	int ns = numBarrels;
	Coord n_pos;
	int new_w, new_h;
	for (int i = 0; i < ns; i++)
	{
		new_w = rand() % width;
		new_h = rand() % height;
		barrels[i].init("barrel.png", Coord(new_w, new_h));
	}
}

int main()
{
	Game* game = new Game(2.5, 1920, 1080, 3, 3, 1);
	game->display();
	delete game;
}