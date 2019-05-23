#include <SFML\Graphics.hpp>
#include <string>

using namespace sf;

float offsetX = 0;		//Sdvig po X(dlya privyazki kameri)
float offsetY = 0;		//Sdvig po Y

const int H = 16;
const int W = 56;


String TileMap[H] = {

"ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ",
"Z                               123                   Z",
"Z      123                                            Z",
"Z                                         123         Z",
"Z                                                     Z",
"Z         0000                BBBB                    Z",
"Z                                B                    Z",
"Z                                B                    Z",
"Z                                B                    Z",
"Z    B                           BB                   Z",
"Z                                                     Z",
"Z              BB                                     Z",
"Z                                                     Z",
"Z                          BB                         Z",
"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG",
"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG",

}; //Karta v vide massiva

class Player {
public:
	float dx, dy, currentFrame;
	FloatRect marioCoordinates;
	bool onGround;
	Sprite marioSprite;

	Player (Texture &image) {
		marioCoordinates = FloatRect(7 * 16, 12 * 16, 16, 32);

		marioSprite.setTexture(image);
		marioSprite.setTextureRect(IntRect(80, 1, 16, 32));

		dx = 0;
		dy = 0;
		currentFrame = 0;
	}

	void move(float time) {
		marioCoordinates.left += dx * time;
		Interaction(0);

		if (!onGround) 
			dy = dy + 0.000000001*time;

		marioCoordinates.top += 2*dy * time;
		onGround = false;
		Interaction(1);



		if (dx > 0)
		{
			marioSprite.setTextureRect(IntRect((80 + 17 * int(currentFrame)), 1, 16, 32));  //Animaciya
		}

		if (dx < 0)
		{
			marioSprite.setTextureRect(IntRect((96 + 17 * int(currentFrame)), 1, -16, 32));
		}

		currentFrame += 0.000007*time;   //Skorost' animacii

		if (currentFrame > 4)
			currentFrame = 0;


		marioSprite.setPosition(marioCoordinates.left - offsetX, marioCoordinates.top - offsetY);	
		dx = 0;
	}

	void Interaction(bool dir) {
		for (int i = marioCoordinates.top / 16; i < (marioCoordinates.top + marioCoordinates.height) / 16; i++)			//Proverka tol'ko po koordinatam, kotorie zadevaet sprite Mario
			for (int j = marioCoordinates.left / 16; j < (marioCoordinates.left + marioCoordinates.width) / 16; j++)
			{
				if ((TileMap[i][j] == 'B') || (TileMap[i][j] == 'G') || (TileMap[i][j] == 'Z'))
				{
					if ((dx > 0) && (dir == 0))
						marioCoordinates.left = j * 16 - marioCoordinates.width;

					if ((dx < 0) && (dir == 0)) 
						marioCoordinates.left = j * 16 + 16;

					if ((dy > 0) && (dir == 1)) { 
						marioCoordinates.top = i * 16 - marioCoordinates.height;  
						dy = 0;   
						onGround = true;
					}

					if ((dy < 0) && (dir == 1)) {
						marioCoordinates.top = i * 16 + 16;  
						dy = 0; 
					}
				}

				if (TileMap[i][j] == '0')
				{
					TileMap[i][j] = ' ';
				}

			}
	}
};

int main() {
	float currentFrame = 0;

	RenderWindow window(VideoMode(400, 250), "Mario");

	Texture marioTexture;
	marioTexture.loadFromFile("50365.png");

	Player mario(marioTexture);

	Texture TileTexture;
	TileTexture.loadFromFile("52571.png"); 

	Sprite TileSprite;
	TileSprite.setTexture(TileTexture);



	Clock lastTick;			//Peremennaya zasekaet vremya s poslednego takta processora
	
	while (window.isOpen()) {
		float time = lastTick.getElapsedTime().asMicroseconds();
		lastTick.restart();

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::A)) {
			mario.dx = -0.0002;
		}

		if (Keyboard::isKeyPressed(Keyboard::D)) {
			mario.dx = 0.0002;
		}

		if (Keyboard::isKeyPressed(Keyboard::W)) {
			if (mario.onGround) {
				mario.dy = -0.00030;
				mario.onGround = false;
			}
		}

		mario.move(time);

		if ((mario.marioCoordinates.left > 341 / 2) && (mario.marioCoordinates.left < 41*16))	//Proverka na krai karti po X
			offsetX = mario.marioCoordinates.left - 341 / 2;

		if ((mario.marioCoordinates.top > 192 / 2) && (mario.marioCoordinates.top < 5*16))		//po Y
			offsetY = mario.marioCoordinates.top - 192 / 2;


		window.clear(Color(99, 173, 255));

		for (int i = 0; i < H; i++)					//Vivod spraitov
			for (int j = 0; j < W; j++)
			{
				if (TileMap[i][j] == 'G') TileSprite.setTextureRect(IntRect(0, 0, 16, 16));

				if (TileMap[i][j] == 'B')
					if (TileMap[i - 1][j] == ' ')
					TileSprite.setTextureRect(IntRect(16, 0, 16, 16));
					else
					TileSprite.setTextureRect(IntRect(32, 0, 16, 16));

				if (TileMap[i][j] == '0')  TileSprite.setTextureRect(IntRect(384 + 16 * int(currentFrame), 0, 16, 16));

				if ((TileMap[i][j] == ' ') || (TileMap[i][j] == 'Z')) TileSprite.setTextureRect(IntRect(48, 336, 16, 16));

				if (TileMap[i][j] == '1') TileSprite.setTextureRect(IntRect(128, 320, 16, 16));
				if (TileMap[i][j] == '2') TileSprite.setTextureRect(IntRect(144, 320, 16, 16));
				if (TileMap[i][j] == '3') TileSprite.setTextureRect(IntRect(160, 320, 16, 16));

				TileSprite.setPosition(j * 16 - offsetX, i * 16 - offsetY);
				window.draw(TileSprite);
			}

		currentFrame += 0.000007*time;

		if (currentFrame > 3)
			currentFrame = 0;


		window.draw(mario.marioSprite);
		window.display();
	}
	return 0;
}