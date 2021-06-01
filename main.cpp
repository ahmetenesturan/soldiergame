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

class Object
{
protected:
	sf::Texture texture[14];
	sf::Sprite sprite;
	Coord pos;
public:
	virtual void init(string texturePath, Coord new_pos);
	virtual void paint(sf::RenderWindow &window);
};

void Object::init(string texturePath, Coord new_pos)
{
	pos = new_pos;
	if (!texture[0].loadFromFile(texturePath))
	{
		cout << "barrel cannot be loaded" << endl;
	}
	sprite.setTexture(texture[0]);
}

void Object::paint(sf::RenderWindow &window)
{
	sprite.setPosition(sf::Vector2f(pos.x, pos.y)); //setting the position of the barrel
	window.draw(sprite); //drawing
}

//class declarations to avoid errors
class Player;
class Bullet;
class Barrel;
class Sandbag;
class BulletList;



class Barrel : public Object
{
private:
	bool isVisible; //boolean variable that determines if the barrel is visible
public:
	bool inBound(Coord player_pos) //a function the know if player is colliding with the barrel
	{
		if ((player_pos.x > pos.x - 60) && (player_pos.x < pos.x + 20) && (player_pos.y > pos.y - 60) && (player_pos.y < pos.y + 20)) return true;
		else return false;
	}

	bool getVisible(); 

	void setVisible(bool);
};

bool Barrel::getVisible()
{
	return isVisible;
}

void Barrel::setVisible(bool visib)
{
	isVisible = visib;
}

class Sandbag : public Object
{
public:
	bool inBound(Coord player_pos) //a function the know if player is colliding with the sandbag
	{
		if ((player_pos.x > pos.x - 80) && (player_pos.x < pos.x + 40) && (player_pos.y > pos.y - 80) && (player_pos.y < pos.y + 40)) return true;
		else return false;
	}
};

class Bullet : public Object
{
private:
	float speed;
	Player* owner;
	WalkDirection direction;
	Bullet* prev;
	Bullet* next;
public:
	virtual void init(string texturePath, Coord new_pos, WalkDirection new_dir, Player* own)
	{
		owner = own;
		pos = new_pos;
		direction = new_dir;
		speed = 1;
		string path = texturePath + ".png";
		if (!texture[0].loadFromFile(path))
		{
			cout << "bullet cannot be loaded" << endl;
		}
		sprite.setTexture(texture[0]);
		prev = NULL;
		next = NULL;
	}
	virtual void paint(sf::RenderWindow &window);
	void move(sf::RenderWindow &window, WalkDirection dir, Player* otherPlayer, Barrel* barrels, Sandbag* sandbags, int nb, int ns);
	void setSpeed(float);
	bool checkCollision(WalkDirection dir, Player* otherPlayer, Barrel* barrels, Sandbag* sandbags, int nb, int ns);

	friend class BulletList;
	friend class Game;
};

void Bullet::paint(sf::RenderWindow &window)
{
	sprite.setPosition(sf::Vector2f(pos.x, pos.y)); //setting the position of the bullet
	if (direction.direc() == 0)
	{
		sprite.setRotation(0); //rotating the bullet
	}
	else if (direction.direc() == 1)
	{
		sprite.setRotation(90); //rotating the bullet
	}
	else if (direction.direc() == 2)
	{
		sprite.setRotation(180); //rotating the bullet
	}
	else if (direction.direc() == 3)
	{
		sprite.setRotation(270); //rotating the bullet
	}
	window.draw(sprite); //drawing
}

void Bullet::move(sf::RenderWindow &window, WalkDirection dir, Player* otherPlayer, Barrel* barrels, Sandbag* sandbags, int nb, int ns)
{
	if (!checkCollision(dir, otherPlayer, barrels, sandbags, nb, ns))
	{
		if (direction.direc() == 0)
		{
			sprite.setPosition(sf::Vector2f(pos.x, pos.y));//setting the position of the bullet
			sprite.setRotation(0);
			pos.y = pos.y - 10;
		}
		else if (direction.direc() == 1)
		{
			sprite.setPosition(sf::Vector2f(pos.x - 30, pos.y + 120));//setting the position of the bullet
			sprite.setRotation(180);
			pos.y = pos.y + 10;
		}
		else if (direction.direc() == 2)
		{
			sprite.setPosition(sf::Vector2f(pos.x - 70, pos.y + 40));//setting the position of the bullet
			sprite.setRotation(270);
			pos.x = pos.x - 10;
		}
		else
		{
			sprite.setPosition(sf::Vector2f(pos.x + 60, pos.y + 75));//setting the position of the bullet
			sprite.setRotation(90);
			pos.x = pos.x + 10;
		}
		window.draw(sprite);//drawing
	}
}

void Bullet::setSpeed(float spe)
{
	speed = spe;
}

class BulletList
{
private:
	Bullet* head; //first node
	int count;
public:
	BulletList(void)
	{
		head = NULL;
		count = 0;
	}
	void add(Coord pos, WalkDirection direc, Player* owner);
	void delete_list(Bullet*);
	void checkCollision(WalkDirection dir, Player* otherPlayer, Barrel* barrels, Sandbag* sandbags, int nb, int ns);
	void move(sf::RenderWindow &window, WalkDirection dir, Player* otherPlayer, Barrel* barrels, Sandbag* sandbags, int nb, int ns);
	friend class Bullet;
	friend class Game;
};

void BulletList::move(sf::RenderWindow &window, WalkDirection dir, Player* otherPlayer, Barrel* barrels, Sandbag* sandbags, int nb, int ns)
{
	checkCollision(dir, otherPlayer, barrels, sandbags, nb, ns);
	Bullet* bullet = head;
	while (bullet != NULL)
	{
		bullet->move(window, dir, otherPlayer, barrels, sandbags, nb, ns);
		bullet = bullet->next;
	}
}

/*
void BulletList::add(Coord pos, WalkDirection direc, Player* owner)
{
	if (head == NULL)
	{
		head = new Bullet();
		head->init("bullet", pos, direc, owner);
	}
	else
	{
		Bullet* newBullet = new Bullet();
		newBullet->init("bullet", pos, direc, owner);
		Bullet* dummy = head;
		while (dummy->next != NULL)
		{
			dummy = dummy->next;
		}
		dummy->next = newBullet;
	}
}
*/

void BulletList::add(Coord pos, WalkDirection direc, Player* owner)
{
	Bullet* newBullet = new Bullet();
	newBullet->init("bullet", pos, direc, owner);
	if (count == 0)
	{
		head = newBullet;
	}
	else
	{
		Bullet* dummy = head;
		int i = 1;
		while (i <= count)
		{
			if (i == count)
			{
				newBullet->prev = dummy;
				dummy->next = newBullet;
				break;
			}
			else
			{
				dummy = dummy->next;
				i++;
			}
		}
	}
}

void BulletList::delete_list(Bullet* bullet)
{
	Bullet* dummy1;
	Bullet* dummy2;
	if (bullet->prev == NULL) //first node
	{
		head = bullet->next;
	}
	else if (bullet->next == NULL) //last node
	{
		dummy1 = bullet->prev;
		dummy1->next = NULL;
	}
	else
	{
		dummy1 = bullet->prev;
		dummy2 = bullet->next;
		dummy1->next = dummy2;
		dummy2->prev = dummy1;
	}
}

class Player : public Object
{
private:
	int score; //score of the player
public:
	virtual void init(string textBasePath, Coord n_pos)
	{
		score = 0;
		pos = n_pos;
		for (int i = 0; i < 14; i++) //for loop the set all soldier stances
		{
			string textpath = textBasePath + to_string(i) + ".png";
			if (!texture[i].loadFromFile(textpath))
			{
				cout << "soldier cannot be loaded" << endl;
			}
		}
		sprite.setTexture(texture[0]);
		sprite.setPosition(sf::Vector2f(pos.x, pos.y));
	}
	bool inBound(Coord); //a function the know if player is colliding with the other player

	int checkCollision(WalkDirection, Player*, Barrel*, Sandbag*, int, int); //returns which way soldier is colliding with object

	void walk(float, WalkDirection, Player*, Barrel*, Sandbag*, int, int); //method the ensure walk movement

	virtual void paint(sf::RenderWindow &window) //drawing the soldier
	{
		window.draw(sprite);
	}
	void fire(sf::RenderWindow &window, WalkDirection dir, float, BulletList*, Player*, Barrel*, Sandbag*, int, int);
	void incrScore(void);
	int getScore(void);

	friend class Game;
};

void Player::incrScore()//score increament
{
	score++;
}

int Player::getScore()//returns the score
{
	return score;
}

void Player::fire(sf::RenderWindow &window, WalkDirection dir, float bullet_speed, BulletList* bullet_list, Player* otherPlayer, Barrel* barrels, Sandbag* sandbags, int nb, int ns)
{
	bullet_list->add(pos.change(60, 0), dir, this);
}

bool Player::inBound(Coord other_player_pos)
{
	if ((other_player_pos.x > pos.x - 20) && (other_player_pos.x < pos.x + 20) && (other_player_pos.y > pos.y - 20) && (other_player_pos.y < pos.y + 20)) return true;
	else return false;
}

void Player::walk(float speed, WalkDirection dir, Player* otherPlayer, Barrel* barrels, Sandbag* sandbags, int nb, int ns)
{
	pos = Coord(sprite.getPosition().x, sprite.getPosition().y); //position of player
	if (dir.direc() == 0)//going up
	{
		sprite.setTexture(texture[0]);
		if (checkCollision(dir, otherPlayer, barrels, sandbags, nb, ns) != dir.direc()) sprite.move(sf::Vector2f(0, -1 * speed));
	}

	else if (dir.direc() == 1)//going down
	{
		sprite.setTexture(texture[4]);
		if (checkCollision(dir, otherPlayer, barrels, sandbags, nb, ns) != dir.direc()) sprite.move(sf::Vector2f(0, speed));
	}

	else if (dir.direc() == 2)//going left
	{
		sprite.setTexture(texture[12]);
		if (checkCollision(dir, otherPlayer, barrels, sandbags, nb, ns) != dir.direc()) sprite.move(sf::Vector2f(-1 * speed, 0));
	}

	else if (dir.direc() == 3)//going right
	{
		sprite.setTexture(texture[9]);
		if (checkCollision(dir, otherPlayer, barrels, sandbags, nb, ns) != dir.direc()) sprite.move(sf::Vector2f(speed, 0));
	}
	pos.x = sprite.getPosition().x;
	pos.y = sprite.getPosition().y;
}

int Player::checkCollision(WalkDirection dir, Player* otherPlayer, Barrel* barrels, Sandbag* sandbags, int nb, int ns)
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

	}

	for (int i = 0; i < ns; i++)
	{
		if (sandbags[i].inBound(pos)) //checking if soldier collides with sandbags
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

	if (otherPlayer->inBound(pos)) //checking if soldier collides with barrels
	{
		if (dir.direc() == 0 && otherPlayer->inBound(pos.change(0, -5)))
		{
			out.goup();
		}
		else if (dir.direc() == 1 && otherPlayer->inBound(pos.change(0, 5)))
		{
			out.godown();
		}
		else if (dir.direc() == 2 && otherPlayer->inBound(pos.change(-5, 0)))
		{
			out.goleft();
		}
		else if (dir.direc() == 3 && otherPlayer->inBound(pos.change(5, 0)))
		{
			out.goright();
		}
	}

	return out.direc(); //which way soldier collides with object relative to soldier
}

bool Bullet::checkCollision(WalkDirection dir, Player* otherPlayer, Barrel* barrels, Sandbag* sandbags, int nb, int ns)
{
	if (otherPlayer->inBound(pos))
	{
		owner->incrScore();
		return true;
	}

	for (int i = 0; i < nb; i++)
	{
		if (barrels[i].inBound(pos))
		{
			barrels[i].setVisible(false);
			return true;
		}
	}

	for (int i = 0; i < ns; i++)
	{
		if (sandbags[i].inBound(pos))
		{
			return true;
		}
	}
}

void BulletList::checkCollision(WalkDirection dir, Player* otherPlayer, Barrel* barrels, Sandbag* sandbags, int nb, int ns) //checking collision inside the list
{
	Bullet* bullet = head;
	while (bullet != NULL)
	{
		if (bullet->checkCollision(dir, otherPlayer, barrels, sandbags, nb, ns))
		{
			Bullet* dummy = bullet;
			bullet = bullet->next;
			delete_list(dummy);
		}
		else bullet = bullet->next;
	}
}

class Game
{
private:
	float speed;
	float bullet_speed;
	int numBarrels;
	int numSandbags;
	int numPlayers;
	int width;
	int height;
	sf::Texture bgTexture;
	sf::Sprite bgSprite;
	Barrel* barrels;
	Sandbag* sandbags;
	Player* player1;
	Player* player2;
	BulletList* bullets1;
	BulletList* bullets2;
	sf::RenderWindow window;
	sf::Text text;
	sf::Font font;

	void drawBackground();//drawing the grass tiles

	void createSandbags();//setting attributes of sandbags

	void createBarrels();//setting attributes of barrels

public:
	Game(float new_speed, float new_bullet_speed, int w, int h, int nb, int ns, int np)//dynamic creation of objects
	{
		speed = new_speed;
		bullet_speed = new_bullet_speed;
		numBarrels = nb;
		numSandbags = ns;
		numPlayers = np;
		width = w;
		height = h;
		window.create(sf::VideoMode(w, h), "game");
		player1 = new Player();
		player2 = new Player();
		player1->init("soldier", Coord(0, 0));
		player2->init("soldier", Coord(300, 300));
		sandbags = new Sandbag[ns];
		barrels = new Barrel[nb];
		bullets1 = new BulletList();
		bullets2 = new BulletList();
		font.loadFromFile("font.ttf"); // loading font file
		text.setFont(font);
		text.setCharacterSize(24);
	}

	~Game()//destructor, dynamicly deleting objects
	{
		delete player1;
		delete player2;
		delete[] sandbags;
		delete[] barrels;
	}

	void display();

	void printScore(sf::RenderWindow &window ,int, int);
};

void Game::printScore(sf::RenderWindow &window, int s1, int s2) //method to print scores
{
	string s = "Player1: " + to_string(s1) + "\nPlayer2: " + to_string(s2);
	text.setFillColor(sf::Color::Black);
	text.setString(s);
	text.setPosition(sf::Vector2f(width/2 - 50, height - 150));
	window.draw(text);
}

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
			h0 = (float)j * 350;
			w0 = (float)i * 350;
			grass[b].setTexture(grasstex);
			grass[b].setPosition(sf::Vector2f(w0, h0));
			window.draw(grass[b]);
		}
	}
}

void Game::display()
{
	Bullet* bullet1 = new Bullet();
	Bullet* bullet2 = new Bullet();
	createSandbags();
	createBarrels();
	WalkDirection direc1, direc2; //soldiers' walk directions
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
			direc1.goup();
			player1->walk(speed, direc1, player2, barrels, sandbags, numBarrels, numSandbags);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))//keyboard input for going down
		{
			direc1.godown();
			player1->walk(speed, direc1, player2, barrels, sandbags, numBarrels, numSandbags);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))//keyboard input for going left
		{
			direc1.goleft();
			player1->walk(speed, direc1, player2, barrels, sandbags, numBarrels, numSandbags);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))//keyboard input for going right
		{
			direc1.goright();
			player1->walk(speed, direc1, player2, barrels, sandbags, numBarrels, numSandbags);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			bullet1->init("bullet", player1->pos, direc1, player1);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			direc2.goup();
			player2->walk(speed, direc2, player1, barrels, sandbags, numBarrels, numSandbags);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))//keyboard input for going down
		{
			direc2.godown();
			player2->walk(speed, direc2, player1, barrels, sandbags, numBarrels, numSandbags);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))//keyboard input for going left
		{
			direc2.goleft();
			player2->walk(speed, direc2, player1, barrels, sandbags, numBarrels, numSandbags);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))//keyboard input for going right
		{
			direc2.goright();
			player2->walk(speed, direc2, player1, barrels, sandbags, numBarrels, numSandbags);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			/*
			if (bullets2->head == NULL)
			{
				bullets2->head = new Bullet();
				bullets2->head->init("bullet", player2->pos, direc2, player2);
			}
			else
			{
				Bullet* temp = bullets2->head;
				while (temp->next != NULL)
				{
					temp = temp->next;
				}
				temp->next = new Bullet();
				temp->next->init("bullet", player2->pos, direc2, player2);
			}
			*/
			//bullets2->add(player2->pos, direc2, player2);
			
			bullet2->init("bullet", player2->pos, direc2, player2);
		}
		player1->paint(window);//drawing players
		player2->paint(window);
		//bullets1->move(window, direc1, player2, barrels, sandbags, numBarrels, numSandbags);
		//bullets2->move(window, direc2, player1, barrels, sandbags, numBarrels, numSandbags);
		bullet1->move(window, direc1, player2, barrels, sandbags, numBarrels, numSandbags);
		bullet2->move(window, direc2, player1, barrels, sandbags, numBarrels, numSandbags);
		sf::Event event;
		while (window.pollEvent(event))//closing the window if user presses close button
		{
			if (event.type == sf::Event::Closed) window.close();
		}

		printScore(window, player1->getScore(), player2->getScore()); //displaying the scores
		window.display(); //displaying the window
	}
}

void Game::createSandbags()//placing sandbags randomly
{
	srand((unsigned int)time(NULL));
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
	Game* game = new Game(2.5, 8, 1920, 1080, 1, 1, 1);
	game->display();
	delete game;
}