#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

const int height = 20;
const int width = 10;
const int blockSpriteSize = 18;
const int blockPerShapeNum = 4;
const int totalColourOptions = 7;

int field[height][width] = {0};

struct Point
{int x,y;} blockHoriz[blockPerShapeNum], blockVertic[blockPerShapeNum];

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

bool check()
{
   for (int i=0;i< blockPerShapeNum;i++)
      if (blockHoriz[i].x<0 || blockHoriz[i].x>= width || blockHoriz[i].y>= height) return 0;
      else if (field[blockHoriz[i].y][blockHoriz[i].x]) return 0;

   return 1;
};


int tetris()
{
    srand(time(0));     

    RenderWindow window(VideoMode(320, 480), "The Game!");

    Texture blockSprite,backgroundSprite,frameSprite;
    blockSprite.loadFromFile("images/tetris/tiles.png");
    backgroundSprite.loadFromFile("images/tetris/background.png");
    frameSprite.loadFromFile("images/tetris/frame.png");

    Sprite sprite(blockSprite), background(backgroundSprite), frame(frameSprite);

    int shapePosition=0; bool rotate=0; int colorNum=1;
    float timer=0,delay=0.3; 

    Clock clock;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer+=time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed)
              if (e.key.code==Keyboard::Up) rotate=true;
              else if (e.key.code==Keyboard::Left) shapePosition=-1;
              else if (e.key.code==Keyboard::Right) shapePosition=1;
        }

    if (Keyboard::isKeyPressed(Keyboard::Down)) delay=0.05;

    //// <- Move -> ///
    for (int i=0;i< blockPerShapeNum;i++)  { blockVertic[i]=blockHoriz[i]; blockHoriz[i].x+=shapePosition; }
    if (!check()) for (int i=0;i< blockPerShapeNum;i++) blockHoriz[i]=blockVertic[i];

    //////Rotate//////
    if (rotate)
      {
        Point centerBlock = blockHoriz[1]; //center of rotation
        for (int i=0;i< blockPerShapeNum;i++)
          {
            int x = blockHoriz[i].y-centerBlock.y;
            int y = blockHoriz[i].x-centerBlock.x;
            blockHoriz[i].x = centerBlock.x - x;
            blockHoriz[i].y = centerBlock.y + y;
           }
           if (!check()) for (int i=0;i<4;i++) blockHoriz[i]=blockVertic[i];
      }

    ///////Tick//////
    if (timer>delay)
      {
        for (int i=0;i< blockPerShapeNum;i++) { blockVertic[i]=blockHoriz[i]; blockHoriz[i].y+=1; }

        if (!check())
        {
         for (int i=0;i< blockPerShapeNum;i++) field[blockVertic[i].y][blockVertic[i].x]=colorNum;

         colorNum=1+rand()% totalColourOptions;
         int n=rand()% totalColourOptions;
         for (int i=0;i< blockPerShapeNum;i++)
           {
            blockHoriz[i].x = figures[n][i] % 2;
            blockHoriz[i].y = figures[n][i] / 2;
           }
        }

         timer=0;
      }

    ///////check lines//////////
    int spawnPoint= height -1 ;
    for (int i= spawnPoint;i>0;i--)
    {
        int count=0;
        for (int j=0;j< width;j++)
        {
            if (field[i][j]) count++;
            field[spawnPoint][j]=field[i][j];
        }
        if (count< width) spawnPoint--;
    }

    shapePosition=0; rotate=0; delay=0.3;

    /////////draw//////////
    window.clear(Color::White);    
    window.draw(background);
          
    for (int i = 0;i < height;i++)//While the block is not at the bottom
     for (int j=0;j< width;j++)

       {
         if (field[i][j]==0) continue;
         sprite.setTextureRect(IntRect(field[i][j]* blockSpriteSize,0, blockSpriteSize, blockSpriteSize));
         sprite.setPosition(j* blockSpriteSize,i* blockSpriteSize);
         sprite.move(28,31); //offset
         window.draw(sprite);
       }

    for (int i=0;i<4;i++)
      {
        sprite.setTextureRect(IntRect(colorNum* blockSpriteSize,0, blockSpriteSize, blockSpriteSize));
        sprite.setPosition(blockHoriz[i].x* blockSpriteSize,blockHoriz[i].y* blockSpriteSize);
        sprite.move(28,31); //offset
        window.draw(sprite);
      }

    window.draw(frame);
    window.display();
    }

    return 0;
}
