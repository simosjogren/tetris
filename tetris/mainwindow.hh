/* Tetris
 *
 * Header-file: MainWindow
 *
 * Program author
 * Name: Simo Sjögren
 * Student number: 283410
 * UserID: sjogren
 * E-Mail: simo.sjogren@tuni.fi
 *
 * Documentation:
 *
 * This Tetris game is under the name of Simo-Tetris. UI includes game logo, game
 * scene (place where the tetrominos drop), textbox for printing motivational
 * texts to the player after breaking rows, highscore-table for printing the
 * top three players of all time and a login spot, where you can choose your
 * nickname for the scoredata after losing, which prints out if you made it to
 * the top three players of all time.
 *
 * UI includes three main buttons:
 * login-button, start-button, reset-button and quit-button.
 *
 * login-button: You need to first login with your nickname before starting game
 * (for saving your top score).
 *
 * start-button: Game starts, only if you have logged in before pressing. Type your name in the
 * textbox and press login-button, and after that you are able to start the game.
 * Clock starts rolling and tetrominos dropping.
 *
 * reset-button: Resets the game whenever pressed. Score goes to zero and your score will NOT be
 * saved to highscoredata.
 *
 * quit-button: Terminates the game.
 *
 *
 * Game rules:
 * You need to collect as much points as possible by getting full horizontal rows.
 * If your "Tetris tower" builds up to the top, you have lost.
 * The dropping speed accelerates every 10 breaked rows, and your level rises by one.
 * When the droppingspeed accelerates, you get more points for a breaked row.
 *
 * You can move your tetromino with next key presses:
 * A - Moves your tetromino one step right.
 * D - Moves your tetromino one step left.
 * S - Moves your tetromino one step down (for making the block drop faster).
 * W - Rotates your tetromino IF there is space for rotation.
 *
 * Tetromino cant go outside the scene.
 *
 * There are 7 different tetrominotypes, which spawn randomly:
 * Square, horizontal, step to left, step to right, left corner, right corner,
 * and pyramid, with a color from red, green, blue and white (random).
 *
 * SPECIAL RULES:
 * You can rotate the leftstep and rightstep tetrominos the same way.
 * This makes the game a bit easier and is separates this Simo-Tetris from
 * the basic, boring tetris.
 *
 * */

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGraphicsScene>
#include <random>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    /**
     * @brief startGame
     * Game starts when this gets triggered.
     */
    void startGame();
    /**
     * @brief resetGame
     * Current game resets.
     */
    void resetGame();
    /**
     * @brief dropStep
     * Triggered by timeout, drops the tetrominovector by one step.
     */
    void dropStep();
    /**
     * @brief loginPress
     * Locks down the players name defined by user in the textbox.
     */
    void loginPress();
    /**
     * @brief updateTime
     * Updates the time by every second.
     */
    void updateTime();

private:

    Ui::MainWindow *ui;

    QGraphicsScene* scene_;

    // Calls timeout when tetromino is dropped by one step.
    QTimer* timer_;
    // Calls timeout when second has passed.
    QTimer* clock_;
    // Tells the current dropping tetromino type.
    int currtetro_;

    // Current dropping blocks.
    std::vector<QGraphicsItem*> currentvector_;
    // All blocks which have settled down.
    std::vector<QGraphicsItem*> allvectors_;

    /**
     * @brief keyPressEvent
     * @param event
     * Triggers if any key is pressed.
     */
    void keyPressEvent(QKeyEvent* event);

    /**
     * @brief gameover
     * Prints gameover-message, saves your score to highscores.csv and resets.
     */
    void gameover();
    /**
     * @brief tetro_builder
     * Builds new random tetromino when called.
     */
    void tetro_builder();
    /**
     * @brief block_collision
     * @param block
     * @param horizontal
     * @param right
     * @return
     * bool, if the block can be moved down, left or right.
     */
    bool block_collision(QGraphicsItem* block, bool horizontal, bool &right);
    /**
     * @brief dropInspector
     * @return
     * Checks, if you can drop the square one block down.
     */
    bool dropInspector();
    /**
     * @brief moveDirection
     * @param direction
     * Moves one step to the direction that user wants to.
     */
    void moveDirection(int direction);
    /**
     * @brief rowInspector
     * Checks, if there are any full rows, that need to be breaked.
     */
    void rowInspector();
    /**
     * @brief rowBreaker
     * @param breakrow
     * @param row_num
     * This breaks full row.
     */
    void rowBreaker(std::vector<QGraphicsItem*> breakrow, long unsigned int row_num);
    /**
     * @brief tetroRotator_Turn
     * @param x_direction
     * @param y_direction
     * @param x_distance
     * @param y_distance
     * @param block
     * "Main" function for the tetromino rotation process.
     */
    void tetroRotator();
    /**
     * @brief tetroRotator_Inspector
     * @return
     * Inspects, that the tetromino blocks we are about to rotate, wont go
     * inside already dropped tetromino or outside the scene.
     */
    bool tetroRotator_Inspector();
    /**
     * @brief tetroRotator_Turn
     * @param x_direction
     * @param y_direction
     * @param x_distance
     * @param y_distance
     * @param block
     * Rotates the current tetromino 90 degrees clockwise, if called.
     */
    void tetroRotator_Turn(int x_direction, int y_direction, int x_distance, int y_distance, QGraphicsItem* block);
    /**
     * @brief tetroRotator_Inspector
     * @return
     * Checks that the choosen tetrominoblock doesnt collide with
     * any else else and doesnt go outside the scene.
     */
    bool tetroRotator_SquareCollision_Inspector(QRectF temp_rect);
    /**
     * @brief tetroRotator
     * Prints out motivational phrases to player and
     * inspects, if the level has right to rise. Also accelerates the game.
     */

    void phrasePrinter(int phrasetype);
    /**
     * @brief highscoreLoader
     * Refreshes the highscore-table from csv-data when called.
     */
    void highscoreLoader();
    /**
     * @brief split
     * @param str
     * @return
     * Splits the string into vector when called.
     */
    std::vector<std::string> split(std::string& str);

    // Constants describing scene coordinates
    // Copied from moving circle example and modified a bit
    const int BORDER_UP = 0;
    const int BORDER_DOWN = 480; // 260; (in moving circle)
    const int BORDER_LEFT = 0;
    const int BORDER_RIGHT = 240; // 680; (in moving circle)

    // Size of a tetromino component
    const int SQUARE_SIDE = 20;
    // Number of horizontal cells (places for tetromino components)
    const int COLUMNS = BORDER_RIGHT / SQUARE_SIDE;
    // Number of vertical cells (places for tetromino components)
    const int ROWS = BORDER_DOWN / SQUARE_SIDE;

    // Name of the current player.
    std::string playername_;
    // This goes to true when you login. You need to login to start the game.
    bool loggedin_ = false;

    // Beginning speed.
    int beginning_speed = 500;

    // Maximum level (speed wont accelerate after this level).
    int max_level = 10;

    // We define the current speed to the beginning speed when launching.
    float currentspeed_ = static_cast<float>(beginning_speed);
    // We will start from level 1.
    int currentlevel_ = 1;
    // Player's personal points.
    int points_ = 0;
    // Increases by one every second when game starts, used for clock.
    int secondspassed_ = 0;
    // Counts breaked rows, used when leveling up.
    int breakedrows_ = 0;

    // Constants for different tetrominos and the number of them
    enum Tetromino_kind { HORIZONTAL,
                          LEFT_CORNER,
                          RIGHT_CORNER,
                          STEP_UP_RIGHT,
                          STEP_UP_LEFT,
                            SQUARE,
                            PYRAMID,
                             NUMBER_OF_TETROMINOS};

    // From the enum values above, only the last one is needed in this template.
    // Recall from enum type that the value of the first enumerated value is 0,
    // the second is 1, and so on.
    // Therefore the value of the last one is 7 at the moment.
    // Remove those tetromino kinds above that you do not implement,
    // whereupon the value of NUMBER_OF_TETROMINOS changes, too.
    // You can also remove all the other values, if you do not need them,
    // but most probably you need a constant value for NUMBER_OF_TETROMINOS.


    // Constants for different directions and the number of them.
    enum Direction_ints { LEFT,
                          RIGHT,
                          DOWN,
                          UP,
                          NUMBER_OF_DIRECTIONS
                            };
    // Constants for different colors and number of them.
    enum Tetromino_colors {
        RED, GREEN, BLUE, WHITE,
        NUMBER_OF_COLORS
    };

    // Constants for different print types in phrasePrinter-function.
    enum Phrase_types {
        ROWBREAK,
        GAME_ENDS,
    };

    // Constants for making the tetromino rotationfunctions look more clear.
    enum Distance_x {
        RIGHT_X,
        LEFT_X,
        SAME_X
    };
    enum Distance_y {
        UP_Y,
        DOWN_Y,
        SAME_Y
    };


    // For randomly selecting the next dropping tetromino
    std::default_random_engine randomEng;
    std::uniform_int_distribution<int> distr;

};

#endif // MAINWINDOW_HH
