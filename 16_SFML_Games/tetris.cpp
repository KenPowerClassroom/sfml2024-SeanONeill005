#include <SFML/Graphics.hpp>
#include <time.h>
#include "tetrisPlayer.h"
using namespace sf;

const int height = 20;
const int width = 10;


int field[height][width] = {0};

struct Point
{int x,y;} blockHoriz[4], blockVertic[4];

int figures[7][4] =
{
    1,3,5,7, // I
    2,4,5,7, // Z
    3,5,4,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
};
void draw(RenderWindow& window, Sprite sprite, Sprite background, Sprite frame, int colorNum, TetrisPlayer player);


bool check(TetrisPlayer player)
{
    for (int i = 0;i < player.getBlockPerShapeNum();i++)
    {
        if (blockHoriz[i].x < 0 || blockHoriz[i].x >= width || blockHoriz[i].y >= height)
        {
            player.changeCheckBottom(false);
            return 0;
        }
        else if (field[blockHoriz[i].y][blockHoriz[i].x])
        {
            player.changeCheckBottom(false);
            return 0;
        }
        
    }
    player.changeCheckBottom(true);
   return 1;

};

int tetris()
{
    TetrisPlayer player;
    srand(time(0));     

    RenderWindow window(VideoMode(320, 480), "The Game!");

    Texture blockSprite,backgroundSprite,frameSprite;
    blockSprite.loadFromFile("images/tetris/tiles.png");
    backgroundSprite.loadFromFile("images/tetris/background.png");
    frameSprite.loadFromFile("images/tetris/frame.png");

    Sprite sprite(blockSprite), background(backgroundSprite), frame(frameSprite);

    int shapePosition=0; 
    bool rotate=0; 
    int colorNum=1;
    float timer=0, delay=0.3; 

    Clock clock;

    while (window.isOpen())
    {
        draw(window, sprite, background, frame, colorNum, player);
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
                

            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Up) 
                {
                    rotate = true;
                }
                else if (event.key.code == Keyboard::Left) 
                {
                    shapePosition = -1;
                }
                else if (event.key.code == Keyboard::Right) 
                {
                    shapePosition = 1;
                }
            }

        }

        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            delay = 0.05;
        }
    //// <- Move -> ///
    for (int i=0;i< player.getBlockPerShapeNum();i++)  
    { 
        blockVertic[i] = blockHoriz[i]; 
        blockHoriz[i].x += shapePosition; 
    }
    if (!check(player))  
    {
        for (int i = 0;i < player.getBlockPerShapeNum();i++)
        {
            blockHoriz[i] = blockVertic[i];
        }
    }

    //////Rotate//////
    if (rotate)
    {
        Point centerBlock = blockHoriz[1]; //center of rotation
        for (int i = 0;i < player.getBlockPerShapeNum(); i++)
          {
            int x = blockHoriz[i].y-centerBlock.y;
            int y = blockHoriz[i].x-centerBlock.x;
            blockHoriz[i].x = centerBlock.x - x;
            blockHoriz[i].y = centerBlock.y + y;
           }
           if (!check(player))  
           {
               for (int i = 0;i < 4;i++)
               {
                   blockHoriz[i] = blockVertic[i];
               }
           }
    }

    ///////Tick//////
    if (timer>delay)
    {
        for (int i = 0; i < player.getBlockPerShapeNum(); i++) 
        {
            blockVertic[i] = blockHoriz[i]; 
            blockHoriz[i].y += 1;
        }

        if (!check(player))
        {
            for (int i = 0; i < player.getBlockPerShapeNum(); i++) 
            {
                field[blockVertic[i].y][blockVertic[i].x] = colorNum;
            }

            colorNum = 1 + rand()% player.getTotalColourOptions();
            int n=rand()% player.getTotalColourOptions();

            for (int i=0; i < player.getBlockPerShapeNum(); i++)
            {
                blockHoriz[i].x = figures[n][i] % 2;
                blockHoriz[i].y = figures[n][i] / 2;
            }
        }

         timer = 0;
    }

    ///////check lines//////////
    int spawnPoint = height - 1;
    for (int i = spawnPoint; i > 0; i--)
    {
        int count = 0;
        for (int j = 0; j < width; j++)
        {
            if (field[i][j]) 
            {
                count++;
            }
            field[spawnPoint][j] = field[i][j];
        }
        if (count < width) 
        {
            spawnPoint--;
        }
    }

    shapePosition=0; rotate=0; delay=0.3;

    /////////draw//////////

    }//end of While(window is open)

    return 0;
}

void draw(RenderWindow &window, Sprite sprite, Sprite background, Sprite frame, int colorNum, TetrisPlayer player)
{
    window.clear(Color::White);
    window.draw(background);

    for (int i = 0; i < height; i++)//While the block is not at the bottom
    {
        for (int j = 0; j < width; j++)

        {
            if (field[i][j] == 0)
            {
                continue;
            }
            sprite.setTextureRect(IntRect(field[i][j] * player.getBlockSpriteSize(), 0, player.getBlockSpriteSize(), player.getBlockSpriteSize()));
            sprite.setPosition(j * player.getBlockSpriteSize(), i * player.getBlockSpriteSize());
            sprite.move(28, 31); //offset
            window.draw(sprite);
        }
    }


    for (int i = 0; i < 4; i++)
    {
        sprite.setTextureRect(IntRect(colorNum * player.getBlockSpriteSize(), 0, player.getBlockSpriteSize(), player.getBlockSpriteSize()));
        sprite.setPosition(blockHoriz[i].x * player.getBlockSpriteSize(), blockHoriz[i].y * player.getBlockSpriteSize());
        sprite.move(28, 31); //offset
        window.draw(sprite);
    }

    window.draw(frame);
    window.display();
}