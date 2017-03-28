/*
    Project: Tetris
    Author: Zane Christenson
    Date: 6/10/16

    Version: 1.0.3

    Description:
        An introduction to game design using c++ and the sfml graphics library. Concepts
        will be taught by programming the amazing classic TETRIS.


*/

#include "Tetris.h"
using namespace sf;
using namespace std;



sf::Sound click, bounce, bing;
sf::SoundBuffer buffer1, buffer2, buffer3;


// ##############################################
// MAIN
// Contains draw loop, and key-pressed checking.
// ##############################################

int main(){
    srand(time(0));



    // Window
    RenderWindow window(VideoMode(SCREEN_WIDTH,SCREEN_HEIGHT), "Tetris", Style::Close);
    window.setFramerateLimit(60);

    // Textures & Sprites
    Texture backgroundTexture, blockTexture;
    backgroundTexture.loadFromFile("images/board.png");
    blockTexture.loadFromFile("images/tiles.png");

    Sprite background(backgroundTexture), tile(blockTexture);


    // Music & Sound FX
    sf::Music music;
    if(!music.openFromFile("audio/jetpackBlues.ogg"))
        return -1;
    music.setVolume(50);
    music.setLoop(true);
    music.play();


    // Sounds
    if (!buffer1.loadFromFile("audio/rowRemoved.wav"))
        return -1;
    bounce.setBuffer(buffer1);
    bounce.setVolume(75);

    if (!buffer2.loadFromFile("audio/blockClick.wav"))
        return -1;
    click.setBuffer(buffer2);

    if (!buffer3.loadFromFile("audio/gameOver.wav"))
        return -1;
    bing.setBuffer(buffer3);




    // Text Class
    Font font;
    font.loadFromFile("fonts/arial.ttf");

    // Create a text
    scoreString = Text("Score: 0", font);
    scoreString.setCharacterSize(30);
    scoreString.setStyle(sf::Text::Bold);
    scoreString.setColor(sf::Color::White);
    scoreString.setPosition(486, 180);


    Text gameOverString("GAME OVER!", font);
    gameOverString.setCharacterSize(55);
    gameOverString.setStyle(sf::Text::Bold);
    gameOverString.setColor(sf::Color::Red);
    gameOverString.setPosition(163,308);


    Text pausedString("Paused", font);
    pausedString.setCharacterSize(45);
    pausedString.setStyle(sf::Text::Bold);
    pausedString.setColor(sf::Color::Blue);
    pausedString.setPosition(255,308);


    // Game variables
    int counter = 0;

    bool left  = false;
    bool right  = false;
    bool boosting = false;

    //Start new game
    newGame();


    // Game Loop
    while(window.isOpen()){




        // TICK
        if(counter == 29){
            counter = 0;
        } else {
            counter++;
        }

        // Event Handling
        Event event;
        while(window.pollEvent(event))
        {
            if(event.type == Event::Closed){
               window.close();
            }

            if(Keyboard::isKeyPressed(Keyboard::Down)){
                boosting = true;

            } else if(Keyboard::isKeyPressed(Keyboard::Right)){
                right = true;

            } else if(Keyboard::isKeyPressed(Keyboard::Left)){
                left = true;

            } else if(Keyboard::isKeyPressed(Keyboard::Up)){
                rotateBlock();

            } else if(Keyboard::isKeyPressed(Keyboard::R)) {
                newGame();

            } else if(!(event.type == sf::Event::KeyPressed) && Keyboard::isKeyPressed(Keyboard::Escape)){

                gameRunning = !gameRunning;
                paused = !paused;

            } else {
                //default variables
                left = false;
                right = false;
                boosting = false;
            }
        }


        /*      DRAW        */
        window.setActive();
        window.clear(Color::White);
        window.draw(background);
        window.draw(scoreString);


        // Draw Board
        for(int col = 0; col < 10; col++){
            for(int row = 0; row < 20; row++){

                if(board[row][col] != 0){
                    tile.setTextureRect(IntRect(abs(board[row][col])*25,0,25,25));
                    tile.setPosition(209 + 25*col,75 + 25*row);
                    window.draw(tile);
                }
            }
        }


        // Game Control
        if(paused){

            window.draw(pausedString);

        } else if(gameRunning){

            // down movement   [checks twice per second]
            if(counter % 15 == 0){
               blockDrop();
            }

            // down boosting
            if(boosting && (counter == 10 || counter == 25)){
                blockDrop();
            }

            // Left/Right Movement [checks six times per second]
            if(counter % 6 == 0){
                if(left){
                    moveLeft();
                } else if(right){
                    moveRight();
                }
            }

        } else {
            window.draw(gameOverString);
        }

        window.display();
    }


    return 0;
}



//#########################################
// FUNCTION DEFINTIONS
// ########################################



/*
    Prints out the game table to console for debugging.
*/
void printTable(){

    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 10; j++){

            cout << board[i][j] << ",";
        }
        cout << endl;
    }
    cout << endl;

}



/*
    randomly generates a new color and block type.
*/
void newBlock(){

	blockRow = 0;
    blockCol = 3;

    //int type = 0;
    int type = rand() % 7;
    int color = 1 + rand() % 7;

	color = -1 * color;

    switch(type){

    case 0:   // I
        board[blockRow + 0][0 + blockCol] = color;
        board[blockRow + 1][0 + blockCol] = color;
        board[blockRow + 2][0 + blockCol] = color;
        board[blockRow + 3][0 + blockCol] = color;
        break;

    case 1:   // O
        board[blockRow + 1][0 + blockCol] = color;
        board[blockRow + 1][1 + blockCol] = color;
        board[blockRow + 2][0 + blockCol] = color;
        board[blockRow + 2][1 + blockCol] = color;
        break;
    case 2:   // T
        board[blockRow + 0][1 + blockCol] = color;
        board[blockRow + 1][1 + blockCol] = color;
        board[blockRow + 1][2 + blockCol] = color;
        board[blockRow + 2][1 + blockCol] = color;
        break;
    case 3:   // Z
        board[blockRow + 1][0 + blockCol] = color;
        board[blockRow + 1][1 + blockCol] = color;
        board[blockRow + 2][1 + blockCol] = color;
        board[blockRow + 2][2 + blockCol] = color;
        break;
    case 4:   // S
        board[blockRow + 0][1 + blockCol] = color;
        board[blockRow + 0][2 + blockCol] = color;
        board[blockRow + 1][0 + blockCol] = color;
        board[blockRow + 1][1 + blockCol] = color;
        break;
    case 5:   // L
        board[blockRow + 0][1 + blockCol] = color;
        board[blockRow + 1][1 + blockCol] = color;
        board[blockRow + 2][1 + blockCol] = color;
        board[blockRow + 2][2 + blockCol] = color;
        break;
    case 6:   // J
        board[blockRow + 0][1 + blockCol] = color;
        board[blockRow + 1][1 + blockCol] = color;
        board[blockRow + 2][1 + blockCol] = color;
        board[blockRow + 2][0 + blockCol] = color;
        break;

    }
}



/*
    changes last block from active to inactive by taking the abs value of the blocks.
*/
void spawnNewBlock(){

    //make active block inactive.
    for(int i = 19; i >= 0; i--){
		for(int j = 9; j >= 0; j--){

            board[i][j] = abs(board[i][j]);
		}
	}

	click.play();

    incrementScore();
	checkForCompleteRow();
    newBlock();

	//if room for new block, spawn... else game over
	for(int i = 0; i < 3; i++){
        for(int j = 3; j < 7; j++){

            if(board[i][j] > 0){
                gameOver();
            }
        }
	}


}



/*
    drops all active blocks down one unit every call assuming there are no collisions
    with walls or other non-active blocks.
*/
void blockDrop(){


    //check collisions
	for(int i = 19; i >= 0; i--){
		for(int j = 9; j >= 0; j--){

            if(board[i][j] < 0){

                if(i == 19){
                    cout << "[hit bottom bound]\n";
                    spawnNewBlock();
                    return;

                } else if(board[i+1][j] > 0){
                    cout << "[hit block]\n";
                    spawnNewBlock();
                    return;
                }
            }
		}
	}

    // if none, move block
	for(int i = 19; i >= 0; i--){
		for(int j = 9; j >= 0; j--){

            if(board[i][j] < 0 && i != 19){
                board[i+1][j] = board[i][j];
                board[i][j] = 0;
            }
		}
	}

    blockRow++;
   // printTable();
}



/*
    moves the active block left one unit per function call assuming there are no collisions
    with walls or other non-active blocks.
*/
void moveLeft(){

    cout << "[pressed left]\n" << endl;

    //check collisions
    for(int i = 19; i >= 0; i--){
		for(int j = 0; j < 10; j++){

            if(board[i][j] < 0){

                if(j == 0){
                    cout << "[hit left wall]\n";
                    return;

                } else if(board[i][j-1] > 0){
                    cout << "[hit block on left]\n";
                    return;
                }
            }
        }
    }

    // if none, move block
	for(int i = 19; i >= 0; i--){
		for(int j = 0; j < 10; j++){

               if(board[i][j] < 0 && j != 0){
                    board[i][j-1] = board[i][j];
                    board[i][j] = 0;
                }
		}
	}

    blockCol--;
	// printTable();
}


/*
    moves the active block right one unit per function call assuming there are no collisions
    with walls or other non-active blocks.
*/
void moveRight(){

    cout << "[pressed right]\n";

    //check collisions
    for(int i = 19; i >= 0; i--){
		for(int j = 9; j >= 0; j--){

            if(board[i][j] < 0){

                if(j == 9){
                    cout << "[hit right wall]\n";
                    return;

                } else if(board[i][j+1] > 0){
                    cout << "[hit block on right]\n";
                    return;
                }
            }
        }
    }

    // if none, move block
	for(int i = 19; i >= 0; i--){
		for(int j = 9; j >= 0; j--){


               if(board[i][j] < 0 && j != 9){
                    board[i][j+1] = board[i][j];
                    board[i][j] = 0;
                }
		}
	}
    blockCol++;
	// printTable();
}



/*
    Rotates block by 90 degrees clockwise.
*/
void rotateBlock(){

    int temp[4][4];

    //check collisions & copy to temp
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){

            if(board[i + blockRow][j + blockCol] > 0){
                return;
            } else {
                temp[j][3-i] = board[i + blockRow][j + blockCol];
            }
        }
    }

    // if none,
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){

            board[i + blockRow][j + blockCol] = temp[i][j];
        }
    }

}



/*
    removes row passed in argument.
*/
void removeRow(int row){

    cout << "[removed row " << row << "]\n";
    bounce.play();

     for(int i = row; i >= 0; i--){
        for(int j = 9; j >= 0; j--){

            board[i][j] = board[i-1][j];
            board[i-1][j] = 0;

        }
    }
}




/*
    Checks for complete row, if found, calls remove row.
*/
void checkForCompleteRow(){

    for(int i = 19; i >= 0; i--){
        for(int j = 9; j >= 0; j--){

            if( !(board[i][j] > 0) ){
                break;
            }

            if(j == 0){
                //complete row found
                removeRow(i);
            }
        }
    }
}




/*
    Sets game over screen and stops game play.
*/
void gameOver(){

    bing.play();
    gameRunning = false;

}



/*
    Initializes global game variables and creates a new blank field.
*/
void newGame(){

    gameRunning = true;
    paused = false;
    score = 0;
    scoreString.setString("Score: 0");


    for(int i = 19; i >= 0; i--){
        for(int j = 9; j >= 0; j--){

            board[i][j] = 0;
        }
    }

    //spawn new block.
    newBlock();


}


/*
    Increments the score text each time a new piece lands
*/
void incrementScore(){

    score += 15;

    ostringstream ss;
    ss << score;

    scoreString.setString("Score: " + ss.str());


}

