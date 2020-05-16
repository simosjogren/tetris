/* Tetris
 *
 * Class: MainWindow
 *
 * Program author
 * Name: Simo Sjögren
 * Student number: 283410
 * UserID: sjogren
 * E-Mail: simo.sjogren@tuni.fi
 *
 * Class is commented, optimized and ready to use.
 *
 * Documentation is in the header file.
 * */

#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QGraphicsItem>
#include <QTimer>
#include <QDebug>
#include <QKeyEvent>
#include <algorithm>
#include <string>
#include <fstream>
#include <map>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // We need a graphics scene in which to draw rectangles.
    scene_ = new QGraphicsScene(this);

    // We need a graphics scene to see what tetrominos are coming up next.

    // The graphicsView object is placed on the window
    // at the following coordinates, but if you want
    // different placement, you can change their values.
    int left_margin = 100; // x coordinate
    int top_margin = 150; // y coordinate

    // The width of the graphicsView is BORDER_RIGHT added by 2,
    // since the borders take one pixel on each side
    // (1 on the left, and 1 on the right).
    // Similarly, the height of the graphicsView is BORDER_DOWN added by 2.
    ui->graphicsView->setGeometry(left_margin, top_margin,
                                  BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui->graphicsView->setScene(scene_);
    ui->resetButton->setDisabled(true);

    // The width of the scene_ is BORDER_RIGHT decreased by 1 and
    // the height of it is BORDER_DOWN decreased by 1, because
    // each square of a tetromino is considered to be inside the sceneRect,
    // if its upper left corner is inside the sceneRect.
    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);
    scene_->setBackgroundBrush(Qt::black);

    // Setting random engine ready for the first real call.
    int seed = time(0); // You can change seed value for testing purposes
    randomEng.seed(seed);
    distr = std::uniform_int_distribution<int>(0, NUMBER_OF_TETROMINOS - 1);
    distr(randomEng); // Wiping out the first random number (which is always 0)
    // After the above settings, you can use randomEng to draw the next falling
    // tetromino by calling: distr(randomEng) in a suitable method.

    ui->currentlevelValueLabel->setText(QString::fromStdString(std::to_string
                                                               (currentlevel_)));

    // Stylesheets for making the ui beautiful.
    ui->logoLabel->setStyleSheet(QString( "font-size: 35px; font: bold; color:"
                                          " red; qproperty-alignment: "
                                          "AlignCenter;" ));
    ui->pointsValueLabel->setStyleSheet(QString( "font-size: 20px; "
                                                 "color: red" ));
    ui->pointsLabel->setStyleSheet(QString( "color: green" ));
    ui->highscoreLabel->setStyleSheet(QString( "font-size: 18px; font: bold;"
                                               " color: red; "
                                               "qproperty-alignment: "
                                               "AlignCenter;" ));
    ui->highscoreBrowser->setStyleSheet(QString( "font-size: 15px; font: bold;"
                                                 "color: yellow; "
                                                 "background-color: black"));
    ui->timeLabel->setStyleSheet(QString( "color: blue" ));
    ui->timeValueLabel->setStyleSheet(QString( "font-size: 20px; color: red"));
    ui->currentlevelLabel->setStyleSheet(QString( "color: purple" ));
    ui->currentlevelValueLabel->setStyleSheet(QString( "font-size: 20px; "
                                                       "color: green" ));
    ui->startButton->setStyleSheet(QString("background-color: green; "
                                           "font: bold 14px"));
    ui->resetButton->setStyleSheet(QString("background-color: purple; "
                                           "font: bold 14px"));
    ui->quitButton->setStyleSheet(QString("background-color: blue; "
                                          "font: bold 14px"));
    ui->textPrinter->setStyleSheet(QString("color: white; "
                                           "background-color: black"));

    currtetro_ = distr(randomEng);

    timer_ = new QTimer(this);
    clock_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(dropStep()));
    connect(clock_, SIGNAL(timeout()), this, SLOT(updateTime()));

    highscoreLoader();
}

MainWindow::~MainWindow()
{
    for (QGraphicsItem* to_be_removed_all : allvectors_) {
        scene_->removeItem(to_be_removed_all);
    }

    for (QGraphicsItem* to_be_removed_curr : currentvector_) {
        scene_->removeItem(to_be_removed_curr);
    }

    for (QGraphicsItem* temp_square : allvectors_) {
        delete temp_square;
    }

    for (QGraphicsItem* temp_active_square : currentvector_) {
        delete temp_active_square;
    }



    allvectors_.clear();
    currentvector_.clear();
    delete timer_;
    delete clock_;
    delete scene_;
    delete ui;
}

void MainWindow::startGame()
{
    // You need to login before starting the game.
    if (!loggedin_) {
        ui->textPrinter->setText(QString::fromStdString
                                 ("You need to log in."));
        return;
    }

    ui->startButton->setDisabled(true);
    ui->pointsValueLabel->setDisabled(false);
    ui->pointsValueLabel->setText(QString::fromStdString
                                  (std::to_string(points_)));
    ui->currentlevelValueLabel->setText(QString::fromStdString(std::to_string
                                                               (currentlevel_)));
    ui->resetButton->setDisabled(false);
    ui->textPrinter->setText(QString::fromStdString("Good Luck!"));
    ui->timeValueLabel->setText(QString::fromStdString("0:00"));

    // Tetrominos start falling.
    timer_->start(beginning_speed);

    // Clock starts to roll.
    clock_->start(1000);

    tetro_builder();
}

void MainWindow::resetGame()
{
    for (QGraphicsItem* to_be_removed_all : allvectors_) {
        scene_->removeItem(to_be_removed_all);
    }

    for (QGraphicsItem* to_be_removed_curr : currentvector_) {
        scene_->removeItem(to_be_removed_curr);
    }

    points_ = 0;
    breakedrows_ = 0;
    currentlevel_ = 1;
    secondspassed_ = 0;

    ui->pointsLabel->setDisabled(true);
    ui->pointsValueLabel->setDisabled(true);

    timer_->stop();
    clock_->stop();

    for (QGraphicsItem* temp_square : allvectors_) {
        delete temp_square;
    }

    for (QGraphicsItem* temp_active_square : currentvector_) {
        delete temp_active_square;
    }

    currentvector_.clear();
    allvectors_.clear();

    ui->resetButton->setDisabled(true);
    ui->startButton->setDisabled(false);
}

void MainWindow::dropStep() {

    // If moving is possible
    if (dropInspector()) {
        for (QGraphicsItem* selected_block : currentvector_) {
            selected_block->moveBy(0, static_cast<qreal>(SQUARE_SIDE));
        }

    // Current tetromino locks down, we do a row check and get new tetromino.
    } else {
        currtetro_ = distr(randomEng);
        for (QGraphicsItem* temp_item : currentvector_) {
            allvectors_.push_back(temp_item);
        }
        rowInspector();
        currentvector_.clear();
        tetro_builder();
    }
}

void MainWindow::loginPress()
{
    playername_ =  ui->nameBox->text().toStdString();
    loggedin_ = true;

    ui->loginButton->setText(QString::fromStdString("Logged in"));
    ui->loginButton->setStyleSheet("background-color: grey");
    ui->loginButton->setDisabled(true);
    ui->nameBox->setDisabled(true);
    ui->playingasLabel->setText(QString::fromStdString("Playing as " +
                                                       playername_));
}

void MainWindow::updateTime()
{
    secondspassed_ += 1;
    int temp_seconds = secondspassed_;
    int temp_minutes = 0;

    while (temp_seconds >= 60) {
        temp_minutes += 1;
        temp_seconds -= 60;
    }

    QString timeprint;

    if (temp_seconds < 10) {
        timeprint = QString::fromStdString(std::to_string(temp_minutes) + ":0"
                                           + std::to_string(temp_seconds));
    } else {
        timeprint = QString::fromStdString(std::to_string(temp_minutes) + ":"
                                           + std::to_string(temp_seconds));
    }
    ui->timeValueLabel->setText(timeprint);
}

void MainWindow::tetro_builder()
{
    QPen borderLine(Qt::black);
    borderLine.setWidth(1);

    // Lets randomize the color.
    std::uniform_int_distribution<int> color_randomizer =
            std::uniform_int_distribution<int>(0, NUMBER_OF_COLORS);
    int color_int = color_randomizer(randomEng);

    QBrush color;
    // Red is default.
    color = Qt::red;
    if (color_int == BLUE) {
        color = Qt::blue;
    }
    if (color_int == GREEN) {
        color = Qt::green;
    }
    if (color_int == WHITE) {
        color = Qt::white;
    }

    QGraphicsItem* square1 = scene_->addRect(0,0, SQUARE_SIDE, SQUARE_SIDE, borderLine,color);
    QGraphicsItem* square2 = scene_->addRect(0,0, SQUARE_SIDE, SQUARE_SIDE, borderLine,color);
    QGraphicsItem* square3 = scene_->addRect(0,0, SQUARE_SIDE, SQUARE_SIDE, borderLine,color);
    QGraphicsItem* square4 = scene_->addRect(0,0, SQUARE_SIDE, SQUARE_SIDE, borderLine,color);

    // Vectors are created in the left corner of the screen.
    // Joint piece for rotation is added to the vector first.

    if (currtetro_ == HORIZONTAL) {
        square1->setPos(static_cast<qreal>(SQUARE_SIDE),
                        static_cast<qreal>(0));
        square2->setPos(static_cast<qreal>(0), static_cast<qreal>(0));
        square3->setPos(static_cast<qreal>(SQUARE_SIDE*2),
                        static_cast<qreal>(0));
        square4->setPos(static_cast<qreal>(SQUARE_SIDE*3),
                        static_cast<qreal>(0));
    }

    if (currtetro_ == SQUARE) {
        square1->setPos(static_cast<qreal>(0), static_cast<qreal>(0));
        square2->setPos(static_cast<qreal>(SQUARE_SIDE),static_cast<qreal>(0));
        square3->setPos(static_cast<qreal>(0),static_cast<qreal>(SQUARE_SIDE));
        square4->setPos(static_cast<qreal>(SQUARE_SIDE),
                        static_cast<qreal>(SQUARE_SIDE));
    }

    if (currtetro_ == PYRAMID) {
        square1->setPos(static_cast<qreal>(SQUARE_SIDE),
                        static_cast<qreal>(SQUARE_SIDE));
        square2->setPos(static_cast<qreal>(0),static_cast<qreal>(SQUARE_SIDE));
        square3->setPos(static_cast<qreal>(SQUARE_SIDE*2),
                        static_cast<qreal>(SQUARE_SIDE));
        square4->setPos(static_cast<qreal>(SQUARE_SIDE),static_cast<qreal>(0));
    }

    if (currtetro_ == STEP_UP_RIGHT) {
        square1->setPos(static_cast<qreal>(SQUARE_SIDE),
                        static_cast<qreal>(SQUARE_SIDE));
        square2->setPos(static_cast<qreal>(0),static_cast<qreal>(SQUARE_SIDE));
        square3->setPos(static_cast<qreal>(SQUARE_SIDE),static_cast<qreal>(0));
        square4->setPos(static_cast<qreal>(SQUARE_SIDE*2),
                        static_cast<qreal>(0));
    }

    if (currtetro_ == STEP_UP_LEFT) {
        square1->setPos(static_cast<qreal>(SQUARE_SIDE),
                        static_cast<qreal>(SQUARE_SIDE));
        square2->setPos(static_cast<qreal>(0), static_cast<qreal>(0));
        square3->setPos(static_cast<qreal>(SQUARE_SIDE),static_cast<qreal>(0));
        square4->setPos(static_cast<qreal>(SQUARE_SIDE*2),
                        static_cast<qreal>(SQUARE_SIDE));
    }

    if (currtetro_ == LEFT_CORNER) {
        square1->setPos(static_cast<qreal>(0), static_cast<qreal>(0));
        square2->setPos(static_cast<qreal>(0),static_cast<qreal>(SQUARE_SIDE));
        square3->setPos(static_cast<qreal>(SQUARE_SIDE),static_cast<qreal>(0));
        square4->setPos(static_cast<qreal>(SQUARE_SIDE*2),
                        static_cast<qreal>(0));
    }

    if (currtetro_ == RIGHT_CORNER) {
        square1->setPos(static_cast<qreal>(SQUARE_SIDE*2),
                        static_cast<qreal>(0));
        square2->setPos(static_cast<qreal>(SQUARE_SIDE*2),
                        static_cast<qreal>(SQUARE_SIDE));
        square3->setPos(static_cast<qreal>(SQUARE_SIDE),static_cast<qreal>(0));
        square4->setPos(static_cast<qreal>(0), static_cast<qreal>(0));
    }

    currentvector_.push_back(square1);  // Joint square
    currentvector_.push_back(square2);
    currentvector_.push_back(square3);
    currentvector_.push_back(square4);

    // Move the vectors to the center.
    for (QGraphicsItem* to_move : currentvector_) {
        to_move->moveBy(120, 0);
    }
}

bool MainWindow::block_collision(QGraphicsItem* block, bool horizontal,
                                 bool &right)
{
    qreal current_x = block->x();
    qreal current_y = block->y();

    // If we want to inspect collisions in vertical way.
    if (!horizontal) {
        for ( QGraphicsItem* temp : allvectors_ ) {
            QRectF temp_rect(QPoint(temp->x(), temp->y()),
                             QSize(static_cast<qreal>(SQUARE_SIDE),
                                   static_cast<qreal>(SQUARE_SIDE)));

            if (temp_rect.contains(current_x+static_cast<qreal>(SQUARE_SIDE/2),
                                   current_y+static_cast<qreal>(SQUARE_SIDE)))
            {
                return true;
            }
        }

    // If we want to inspect collisions in horizontal way.
    } else {
        for ( QGraphicsItem* temp : allvectors_ ) {
            QRectF temp_rect(QPoint(temp->x(), temp->y()),
                             QSize(static_cast<qreal>(SQUARE_SIDE),
                                   static_cast<qreal>(SQUARE_SIDE)));

            // Right direction
            if (temp_rect.contains(current_x + static_cast<qreal>(SQUARE_SIDE),
                                   current_y + static_cast<qreal>
                                   (SQUARE_SIDE/2) ) ) {
                right = false;
                return true;
            }

            // Left direction
            if (temp_rect.contains(current_x, current_y +
                                   static_cast<qreal>(SQUARE_SIDE/2))) {
                right = true;
                return true;
            }
        }
    }
    return false;
}

bool MainWindow::dropInspector()
{
    // We need this variable for block collision function.
    bool right = false;
    QRectF scenerect = scene_->sceneRect();
    for (QGraphicsItem* inspected_block : currentvector_) {

        // If we will hit another block
        if (block_collision(inspected_block, false, right)) {
            if (inspected_block->y() <= static_cast<qreal>(SQUARE_SIDE)) {
                gameover();
                return true;
            }
            return false;
        }

        // If we will hit the bottom
        if (!scenerect.contains(inspected_block->x(), inspected_block->y() +
                                static_cast<qreal>(SQUARE_SIDE))) {
            return false;
        }
    }
    return true;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
        if (event->key() == Qt::Key_A) {
            moveDirection(LEFT);
        }
        else if (event->key() == Qt::Key_D) {
            moveDirection(RIGHT);
        }
        else if (event->key() == Qt::Key_S) {
            moveDirection(DOWN);
        }
        else if (event->key() == Qt::Key_W) {
            tetroRotator();
        }
}

void MainWindow::gameover()
{
    clock_->stop();
    phrasePrinter(GAME_ENDS);

    // We dont want empty scores to the data.
    if (points_ > 0) {
        std::ofstream highscoreCSV("highscores.csv", std::fstream::app);
        if (highscoreCSV.is_open()) {
            highscoreCSV << points_ << ";" << currentlevel_ << ";"
                         << playername_ << ";\n";
            highscoreCSV.close();
        }
    }

    highscoreLoader();
    resetGame();
}

void MainWindow::moveDirection(int direction)
{
    // True, if we want to inspect from the right side.
    bool right = false;

    // One dropstep for keypress.
    if (direction == DOWN) {
        dropStep();
    }

    if (direction  == UP) {
        tetroRotator();
    }

    QRectF scene_rect = scene_->sceneRect();

    if (direction == LEFT) {
        right = false;
        for (QGraphicsItem* temp : currentvector_) {
            if (! scene_rect.contains(temp->x() + static_cast<qreal>
                                      ((-SQUARE_SIDE)), temp->y())) {
                // Hits left wall.
                return;
            }
            if (block_collision(temp, true, right)) {
                if (right == true) {
                    // Hits block from left.
                    return;
                }
            }
        }
        // Inspection success, time to move.
        for (QGraphicsItem* to_move : currentvector_) {
            to_move->setX(to_move->x() + static_cast<qreal>(-SQUARE_SIDE));
        }
    }

    if (direction == RIGHT) {
        right = true;
        for (QGraphicsItem* temp : currentvector_) {
            if (! scene_rect.contains(temp->x() + static_cast<qreal>
                                      ((SQUARE_SIDE)), temp->y())) {
                // Hits right wall.
                return;
            }
            if (block_collision(temp, true, right)) {
                if (right == false) {
                    // Hits right block.
                    return;
                }
            }
        }
        // Inspection success, time to move.
        for (QGraphicsItem* to_move : currentvector_) {
            to_move->setX(to_move->x() + static_cast<qreal>(SQUARE_SIDE));
        }
    }
}

void MainWindow::rowInspector()
{
    int horizontal_lenght = static_cast<int>(scene_->width());
    long unsigned int vertical_lenght = static_cast<int>(scene_->height());

    // true, if atleast one row gets destroyed.
    bool boolbreak = false;

    // this vector includes all items in certain row.
    std::vector<QGraphicsItem*> align_vector;

    // Loop for vertical lines
    for (long unsigned int curr_line = 1;
        (curr_line * static_cast<long unsigned int>(SQUARE_SIDE)) <
         vertical_lenght; ++curr_line) {

        // This creates a rectancle sized (scene width) x (square size).
        QRectF horizontal_rect(QPoint(static_cast<qreal>(0), static_cast<qreal>
                                      ( (curr_line * static_cast
                                        <long unsigned int>(SQUARE_SIDE) ) ) ),
                               QSize(static_cast<qreal>(horizontal_lenght),
                                     static_cast<qreal>(SQUARE_SIDE) ) );

        for (QGraphicsItem* temp_item : allvectors_) {
            // We divide the lenght by two so we get the middle spot.
            if (horizontal_rect.contains(temp_item->x() + static_cast<qreal>
                                         (SQUARE_SIDE/2), temp_item->y() +
                                         static_cast<qreal>(SQUARE_SIDE/2))) {

                align_vector.push_back(temp_item);
            }
        }
        // Next means that the row is full and we need to break it.
        if (align_vector.size() > ( horizontal_lenght /
                                    static_cast<long unsigned int>
                                    (SQUARE_SIDE) ) ) {
            breakedrows_ += 1;
            rowBreaker(align_vector, curr_line);
            boolbreak = true;
        }
        align_vector.clear();
    }
    if (boolbreak) {
        phrasePrinter(ROWBREAK);
    }

}

void MainWindow::rowBreaker(std::vector<QGraphicsItem*> breakrow,
                            long unsigned int row_num)
{
    std::vector<QGraphicsItem*>::iterator vector_itr;

    for (QGraphicsItem* temp_item : breakrow) {
        vector_itr = std::find(allvectors_.begin(), allvectors_.end(),
                               temp_item);

        // If the searched item is found
        if ( !(vector_itr == allvectors_.end()) ) {
            allvectors_.erase(vector_itr);
            scene_->removeItem(temp_item);
            delete temp_item;
        }
    }

    // Now we need to drop all items which are above this row by one step.
    for (QGraphicsItem* to_be_dropped : allvectors_) {

        // If current block is over the breaked row.
        if (static_cast<long unsigned int>(to_be_dropped->y()) <
                SQUARE_SIDE*row_num) {
            to_be_dropped->moveBy(0, static_cast<qreal>(SQUARE_SIDE));
        }
    }

    // Points you get, when you break a row (my formula).
    points_ += 100 + ((currentlevel_-1) * 25);
    ui->pointsValueLabel->setText(QString::fromStdString
                                  (std::to_string(points_)));
}

void MainWindow::tetroRotator_Turn(int x_direction, int y_direction,
                                   int x_distance, int y_distance,
                                   QGraphicsItem* block)
{
    if (x_direction == SAME_X) {
        if (y_direction == UP_Y) {
            block->moveBy(abs(y_distance), abs(y_distance));
        } else if (y_direction == DOWN_Y) {
            block->moveBy(-abs(y_distance), -abs(y_distance));
        }

    } else if (x_direction == RIGHT_X) {
        if (y_direction == SAME_Y) {
            block->moveBy(-abs(x_distance), abs(x_distance));
        } else if (y_direction == DOWN_Y) {
            block->moveBy(-2*abs(x_distance), 0);
        } else if (y_direction == UP_Y) {
            block->moveBy(0, 2*abs(x_distance));
        }

    } else if (x_direction == LEFT_X) {
        if (y_direction == SAME_Y) {
            block->moveBy(abs(x_distance), -abs(x_distance));
        } else if (y_direction == DOWN_Y) {
            block->moveBy(0, -2*abs(x_distance));
        } else if (y_direction == UP_Y) {
            block->moveBy(0, 2*abs(x_distance));
        }
    }
}

bool MainWindow::tetroRotator_Inspector()
{
    int x_direction;
    int y_direction;
    bool joint_gone = false;

    // Temporal coordinates for inspecting the rotation.
    std::pair<int, int> temp_coords;
    std::pair<int, int> joint_coords = {
        static_cast<int>(currentvector_.at(0)->x()),
        static_cast<int>(currentvector_.at(0)->y()) };

    for (QGraphicsItem* temp_square : currentvector_) {
        // Let's get the first item (joint) outta way.
        if (!joint_gone) {
            joint_gone = true;
            continue;
        }

        temp_coords = { static_cast<int>(temp_square->x()),
                        static_cast<int>(temp_square->y()) };
        int x_distance = (temp_coords.first - joint_coords.first);
        int y_distance = (temp_coords.second - joint_coords.second);

        // x direction.
        if (x_distance < 0) {
            x_direction = LEFT_X;
        } else if (x_distance > 0) {
            x_direction = RIGHT_X;
        } else {
            x_direction = SAME_X;
        }

        // y direction.
        if (y_distance < 0) {
            y_direction = UP_Y;
        } else if (y_distance > 0) {
            y_direction = DOWN_Y;
        } else {
            y_direction = SAME_Y;
        }

        // Now we do the testing moves.
        if (x_direction == SAME_X) {
            if (y_direction == UP_Y) {
                QRectF temp_rect(QPoint(temp_square->x() + static_cast<qreal>
                                        (abs(y_distance)), temp_square->y() +
                                        static_cast<qreal>(abs(y_distance))),
                                 QSize(static_cast<qreal>(SQUARE_SIDE),
                                       static_cast<qreal>(SQUARE_SIDE)));

                if (tetroRotator_SquareCollision_Inspector(temp_rect)) {
                    return false;
                }
            } else if (y_direction == DOWN_Y) {
                QRectF temp_rect(QPoint(temp_square->x() +
                                        static_cast<qreal>(-abs(y_distance)),
                                        temp_square->y() + static_cast<qreal>
                                        (-abs(y_distance))),
                                 QSize(static_cast<qreal>(SQUARE_SIDE),
                                       static_cast<qreal>(SQUARE_SIDE)));

                if (tetroRotator_SquareCollision_Inspector(temp_rect)) {
                    return false;
                }
            }

        } else if (x_direction == RIGHT_X) {
            if (y_direction == SAME_Y) {
                QRectF temp_rect(QPoint(temp_square->x() +
                                        static_cast<qreal>(-abs(x_distance)),
                                        temp_square->y() + static_cast<qreal>
                                        (abs(x_distance))),
                                 QSize(static_cast<qreal>(SQUARE_SIDE),
                                       static_cast<qreal>(SQUARE_SIDE)));

                if (tetroRotator_SquareCollision_Inspector(temp_rect)) {
                    return false;
                }
            } else if (y_direction == DOWN_Y) {
                QRectF temp_rect(QPoint(temp_square->x() +
                                        static_cast<qreal>(-2*abs(x_distance)),
                                        temp_square->y()),
                                 QSize(static_cast<qreal>(SQUARE_SIDE),
                                       static_cast<qreal>(SQUARE_SIDE)));

                if (tetroRotator_SquareCollision_Inspector(temp_rect)) {
                    return false;
                }
            } else if (y_direction == UP_Y) {
                QRectF temp_rect(QPoint(temp_square->x(), temp_square->y() +
                                        static_cast<qreal>(2*abs(x_distance))),
                                 QSize(static_cast<qreal>(SQUARE_SIDE),
                                       static_cast<qreal>(SQUARE_SIDE)));

                if (tetroRotator_SquareCollision_Inspector(temp_rect)) {
                    return false;
                }
            }

        } else if (x_direction == LEFT_X) {
            if (y_direction == SAME_Y) {
                QRectF temp_rect(QPoint(temp_square->x() +
                                        static_cast<qreal>(abs(x_distance)),
                                        temp_square->y() + static_cast<qreal>
                                        (-abs(x_distance))),
                                 QSize(static_cast<qreal>(SQUARE_SIDE),
                                       static_cast<qreal>(SQUARE_SIDE)));

                if (tetroRotator_SquareCollision_Inspector(temp_rect)) {
                    return false;
                }
            } else if (y_direction == DOWN_Y) {
                QRectF temp_rect(QPoint(temp_square->x(), temp_square->y() +
                                       static_cast<qreal>(-2*abs(x_distance))),
                                 QSize(static_cast<qreal>(SQUARE_SIDE),
                                       static_cast<qreal>(SQUARE_SIDE)));

                if (tetroRotator_SquareCollision_Inspector(temp_rect)) {
                    return false;
                }
            } else if (y_direction == UP_Y) {
                QRectF temp_rect(QPoint(temp_square->x(), temp_square->y() +
                                        static_cast<qreal>(2*abs(x_distance))),
                                 QSize(static_cast<qreal>(SQUARE_SIDE),
                                       static_cast<qreal>(SQUARE_SIDE)));

                if (tetroRotator_SquareCollision_Inspector(temp_rect)) {
                    return false;
                }
            }
        }
    }
    // If all went fine, we will return true.
    return true;
}

bool MainWindow::tetroRotator_SquareCollision_Inspector(QRectF temp_rect)
{
    // If square gets outside the screen while rotating.
    QRectF scene_rect = scene_->sceneRect();
    if (!scene_rect.contains(temp_rect.x()+static_cast<qreal>(SQUARE_SIDE/2),
                             temp_rect.y()+static_cast<qreal>(SQUARE_SIDE/2)))
    {
        // Block would hit the wall.
        return true;
    }

    // If it hits collision while rotating.
    for (QGraphicsItem* temp_thing : allvectors_) {
        if (temp_rect.contains(temp_thing->x() + static_cast<qreal>
                               (SQUARE_SIDE/2), temp_thing->y() +
                               static_cast<qreal>(SQUARE_SIDE/2)))
        {
            // Block would hit another block.
            return true;
        }
    }
    return false;
}

void MainWindow::tetroRotator()
{
    // Square is always the same.
    if (currtetro_ == SQUARE) {
        return;
    }

    int x_direction;
    int y_direction;

    bool joint_gone = false;
    std::pair<int, int> temp_coords;
    std::pair<int, int> joint_coords =
        {   static_cast<int>(currentvector_.at(0)->x()),
            static_cast<int>(currentvector_.at(0)->y())   };

    for (int run_number = 1; run_number < 3; ++run_number) {
        // If this is first loop, we do the investigation first.
        if (run_number == 1) {
            if (tetroRotator_Inspector()) {
                continue;
            } else {
                return;
            }
        }

        // If tetroRotator_Inspector returns true
        for (QGraphicsItem* temp_square : currentvector_) {
            // Let's get the first item outta way.
            if (!joint_gone) {
                joint_gone = true;
                continue;
            }
            temp_coords = { static_cast<int>(temp_square->x()),
                            static_cast<int>(temp_square->y()) };
            int x_distance = (temp_coords.first - joint_coords.first);
            int y_distance = (temp_coords.second - joint_coords.second);

            // x direction
            if (x_distance < 0) {
                x_direction = LEFT_X;
            } else if (x_distance > 0) {
                x_direction = RIGHT_X;
            } else {
                x_direction = SAME_X;
            }

            // y direction
            if (y_distance < 0) {
                y_direction = UP_Y;
            } else if (y_distance > 0) {
                y_direction = DOWN_Y;
            } else {
                y_direction = SAME_Y;
            }

            // Turns the tetro 90 degrees to clockwise.
            tetroRotator_Turn(x_direction, y_direction, x_distance,
                              y_distance, temp_square);
        }
    }
}

void MainWindow::phrasePrinter(int phrasetype)
{
    if (phrasetype == GAME_ENDS) {
        ui->textPrinter->setText(QString::fromStdString
                                 ("Oh no! Game over! Your score: " +
                                  std::to_string(points_)));
    }

    if (phrasetype == ROWBREAK) {

        // If we have achieved the max level, there is no more acceleration.
        if (currentlevel_ >= max_level) {
            ui->textPrinter->setText(QString::fromStdString
                                     ("YOU'RE LEGEND! " +
                                      std::to_string(breakedrows_) +
                                      " rows breaked!"));
            return;
        }

        // Speed gets increased every 10 broken rows.
        if (breakedrows_ / 10 >= currentlevel_) {
            currentlevel_ += 1;
            ui->textPrinter->setText(QString::fromStdString
                                     (std::to_string(breakedrows_) +
                                      " rows breaked. Current level: " +
                                      std::to_string(currentlevel_) + "/" +
                                      std::to_string(max_level)));
            // Formula for increasing the speed
            currentspeed_ = (currentspeed_ - ((currentlevel_-1) *
                                             (currentspeed_/(max_level*2.5))));
            timer_->stop();
            timer_->start(static_cast<int>(currentspeed_));

            ui->currentlevelValueLabel->setText(QString::fromStdString
                                              (std::to_string(currentlevel_)));

        } else if (breakedrows_ >= 3) {
            if (breakedrows_ % 2 == 0) {
                ui->textPrinter->setText(QString::fromStdString
                                         ("LET'S GOO! " +
                                          std::to_string(breakedrows_) +
                                          " rows breaked!"));
            } else {
                ui->textPrinter->setText(QString::fromStdString
                                         ("YEAA! " +
                                          std::to_string(breakedrows_) +
                                          " rows breaked!"));
            }
        }
        if (breakedrows_ == 1) {
            ui->textPrinter->setText(QString::fromStdString
                                     ("First break! Nice!"));
        }
        if (breakedrows_ == 2) {
            ui->textPrinter->setText(QString::fromStdString
                                     ("Two breaks! Damn!"));
        }
    }
}

void MainWindow::highscoreLoader()
{
    std::ifstream hs_file("highscores.csv");
    if (!hs_file) {
        ui->highscoreBrowser->setText(QString::fromStdString
                                      ("Error loading highscores."));
        return;
    }

    std::string new_line;

    // This includes all data from csv.
    std::vector<std::vector<std::string>> highscore_data;
    std::vector<std::string> new_line_vector;

    while (getline(hs_file, new_line)) {
        new_line_vector = split(new_line);
        highscore_data.push_back(new_line_vector);
    }
    hs_file.close();

    // This sorts the data by key value (score) using our algorithm.
    std::sort(highscore_data.begin(), highscore_data.end(),
              [](std::vector<std::string> vector1,
              std::vector<std::string> vector2) {

        return stoi(vector1.at(0)) > stoi(vector2.at(0));
    });

    // Lets print out the top 3:
    QString highscoreprint1;
    QString highscoreprint2;
    QString highscoreprint3;

    if (highscore_data.size() > 0) {
        highscoreprint1 = QString::fromStdString
                ("1. " + highscore_data.at(0).at(2) + ":  " +
                 highscore_data.at(0).at(0) + "\n");

        if (highscore_data.size() > 1) {
            highscoreprint2 = QString::fromStdString
                    ("2. " + highscore_data.at(1).at(2) + ":  " +
                     highscore_data.at(1).at(0) + "\n");

            if (highscore_data.size() > 2) {
                highscoreprint3 = QString::fromStdString
                        ("3. " + highscore_data.at(2).at(2) + ":  " +
                         highscore_data.at(2).at(0));
            }
        }
    }

    ui->highscoreBrowser->setText(highscoreprint1 + highscoreprint2 +
                                  highscoreprint3);
}

std::vector<std::string> MainWindow::split(std::string &str)
{
    std::vector<std::string> result = {""};
    bool cont = false;

    // 59 means ";"
    const char delim = { 59 };
    for ( auto cha : str ){
        if ( cha == '"' ){
            cont = !cont;
        } else if ( cha == delim && !cont){
            result.push_back("");
        } else {
            result.back().push_back(cha);
        }
    }
    if ( result.back() == "" ){
        result.erase(--result.end());
    }
    return result;
}
