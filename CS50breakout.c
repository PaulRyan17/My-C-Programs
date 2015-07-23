//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    //velocity for the ball
    double x_velocity = drand48() * 10;
    double y_velocity = 3.0;
   
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {

        //Scoreboard
        updateScoreboard(window, label, points);
        // TODO
        GEvent event = getNextEvent(MOUSE_EVENT);
        
        if(event != NULL)
        {
            if(getEventType(event)==MOUSE_MOVED)
            {
                double x = getX(event) - getWidth(paddle) / 2;
                double y = 550;
                setLocation(paddle,x, y);
            }
        }

         // move circle along x-axis
        move(ball, x_velocity, y_velocity);

        // bounce off right edge of window
        if (getX(ball) + getWidth(ball) >= getWidth(window))
        {
            x_velocity = -x_velocity;
        }

        // bounce off left edge of window
        if (getX(ball) <= 0)
        {
            x_velocity = -x_velocity;
        }

        // bounce off the top of the window
        if(getY(ball) <= 0)
        {
            y_velocity = -y_velocity;
            
        }
        
        if(getY(ball) + getHeight(ball) >= getHeight(window))
        {
            
            lives--;
            setLocation(ball, 200,300);
            setLocation(paddle, 160, 500);
            waitForClick();
        }
        
        //Collision
        GObject object = detectCollision(window, ball);
        
        if(object != NULL)
        {
            if(object == paddle)
            {
                y_velocity = -y_velocity;
            }
        
            else if(strcmp(getType(object), "GRect") == 0)
            {
                 removeGWindow(window, object);
                 y_velocity = -y_velocity;
                 points++;
                 bricks--;
            }
        }
        
        if(lives == 0)
        {
            GLabel lose = newGLabel("Try again");
            setFont(lose, "SansSerif-40");
            setColor(lose, "RED");
            add(window, lose);
            setLocation(lose, 100, 300);
        }
        
        if(bricks == 0)
        {
            GLabel win = newGLabel("You win");
            setFont(win, "SansSerif-40");
            setColor(win, "RED");
            add(window, win);
            setLocation(win, 200, 300);
        }
        
        
        // linger before moving again
        pause(10);
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    // TODO
    int brickwidth = 35;
    int brickheight = 10; 
    int x = 5;
    int y = 60;
      
    GRect brick;
    
    for(int ROW = 1;ROW <= 5;ROW++, x = 5)
    {
        if(ROW >= 1)
        {
            y = y + 15;
        }
        
        for(int COL =1;COL<=10;COL++, x+=39)
        {
            brick = newGRect(x,y,brickwidth, brickheight);
            setFilled(brick, true);
            
            switch(ROW)
            {
                case 1:
                    setColor(brick, "RED");
                    break;
                case 2:
                    setColor(brick, "BLUE");
                    break;
                case 3:
                    setColor(brick, "YELLOW");
                    break;
                case 4:
                    setColor(brick, "BLACK");
                    break;
                case 5:
                    setColor(brick, "GREEN");
                    break;        
            }
            
            
            
            add(window, brick);
        }
        
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // TODO
    int x = 400/2;
    int y = 600/2; 
    GOval ball = newGOval(x, y, 17, 17);
    setFilled(ball, true);
    setColor(ball, "PINK");
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    //x, y, width, height
    GRect rect = newGRect(165, 550, 75, 5);
    setFilled(rect, true);
    setColor(rect, "RED");
    add(window, rect);
    return rect;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel label = newGLabel(" ");
    setFont(label, "SANSSERIF-40");
    add(window,label);
    setLocation(label, 185, 300);
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
