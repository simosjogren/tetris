/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGraphicsView *graphicsView;
    QPushButton *quitButton;
    QPushButton *startButton;
    QLabel *pointsLabel;
    QLabel *pointsValueLabel;
    QPushButton *resetButton;
    QTextBrowser *textPrinter;
    QLabel *currentlevelLabel;
    QLabel *timeLabel;
    QLabel *currentlevelValueLabel;
    QLabel *timeValueLabel;
    QLabel *logoLabel;
    QTextBrowser *highscoreBrowser;
    QLabel *highscoreLabel;
    QLineEdit *nameBox;
    QPushButton *loginButton;
    QLabel *playingasLabel;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(709, 701);
        MainWindow->setStyleSheet(QStringLiteral("background-color: rgb(10, 10, 20);"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(130, 411, 171, 71));
        quitButton = new QPushButton(centralWidget);
        quitButton->setObjectName(QStringLiteral("quitButton"));
        quitButton->setGeometry(QRect(610, 90, 84, 28));
        startButton = new QPushButton(centralWidget);
        startButton->setObjectName(QStringLiteral("startButton"));
        startButton->setGeometry(QRect(610, 10, 84, 28));
        pointsLabel = new QLabel(centralWidget);
        pointsLabel->setObjectName(QStringLiteral("pointsLabel"));
        pointsLabel->setGeometry(QRect(400, 10, 63, 20));
        pointsValueLabel = new QLabel(centralWidget);
        pointsValueLabel->setObjectName(QStringLiteral("pointsValueLabel"));
        pointsValueLabel->setGeometry(QRect(460, 10, 63, 20));
        resetButton = new QPushButton(centralWidget);
        resetButton->setObjectName(QStringLiteral("resetButton"));
        resetButton->setGeometry(QRect(610, 40, 84, 28));
        textPrinter = new QTextBrowser(centralWidget);
        textPrinter->setObjectName(QStringLiteral("textPrinter"));
        textPrinter->setGeometry(QRect(400, 180, 261, 41));
        currentlevelLabel = new QLabel(centralWidget);
        currentlevelLabel->setObjectName(QStringLiteral("currentlevelLabel"));
        currentlevelLabel->setGeometry(QRect(400, 40, 91, 20));
        timeLabel = new QLabel(centralWidget);
        timeLabel->setObjectName(QStringLiteral("timeLabel"));
        timeLabel->setGeometry(QRect(400, 70, 101, 20));
        currentlevelValueLabel = new QLabel(centralWidget);
        currentlevelValueLabel->setObjectName(QStringLiteral("currentlevelValueLabel"));
        currentlevelValueLabel->setGeometry(QRect(500, 40, 63, 20));
        timeValueLabel = new QLabel(centralWidget);
        timeValueLabel->setObjectName(QStringLiteral("timeValueLabel"));
        timeValueLabel->setGeometry(QRect(500, 70, 63, 20));
        logoLabel = new QLabel(centralWidget);
        logoLabel->setObjectName(QStringLiteral("logoLabel"));
        logoLabel->setGeometry(QRect(70, 20, 301, 61));
        highscoreBrowser = new QTextBrowser(centralWidget);
        highscoreBrowser->setObjectName(QStringLiteral("highscoreBrowser"));
        highscoreBrowser->setGeometry(QRect(400, 290, 261, 71));
        highscoreLabel = new QLabel(centralWidget);
        highscoreLabel->setObjectName(QStringLiteral("highscoreLabel"));
        highscoreLabel->setGeometry(QRect(410, 250, 251, 31));
        nameBox = new QLineEdit(centralWidget);
        nameBox->setObjectName(QStringLiteral("nameBox"));
        nameBox->setGeometry(QRect(400, 390, 181, 28));
        nameBox->setStyleSheet(QLatin1String("color : white;\n"
"background-color: black;"));
        loginButton = new QPushButton(centralWidget);
        loginButton->setObjectName(QStringLiteral("loginButton"));
        loginButton->setGeometry(QRect(600, 390, 84, 28));
        loginButton->setStyleSheet(QStringLiteral("color: black; background-color: rgb(19, 147, 60); font: bold 14px"));
        playingasLabel = new QLabel(centralWidget);
        playingasLabel->setObjectName(QStringLiteral("playingasLabel"));
        playingasLabel->setGeometry(QRect(400, 430, 281, 20));
        playingasLabel->setStyleSheet(QStringLiteral("color: blue;"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 709, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);
        QObject::connect(quitButton, SIGNAL(clicked()), MainWindow, SLOT(close()));
        QObject::connect(startButton, SIGNAL(clicked()), MainWindow, SLOT(startGame()));
        QObject::connect(resetButton, SIGNAL(clicked()), MainWindow, SLOT(resetGame()));
        QObject::connect(loginButton, SIGNAL(clicked()), MainWindow, SLOT(loginPress()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        quitButton->setText(QApplication::translate("MainWindow", "Quit", Q_NULLPTR));
        startButton->setText(QApplication::translate("MainWindow", "Start Game", Q_NULLPTR));
        pointsLabel->setText(QApplication::translate("MainWindow", "Points:", Q_NULLPTR));
        pointsValueLabel->setText(QString());
        resetButton->setText(QApplication::translate("MainWindow", "Reset", Q_NULLPTR));
        currentlevelLabel->setText(QApplication::translate("MainWindow", "Current level:", Q_NULLPTR));
        timeLabel->setText(QApplication::translate("MainWindow", "Time elapsed:", Q_NULLPTR));
        currentlevelValueLabel->setText(QString());
        timeValueLabel->setText(QString());
        logoLabel->setText(QApplication::translate("MainWindow", "SIMO-TETRIS", Q_NULLPTR));
        highscoreLabel->setText(QApplication::translate("MainWindow", "High Scores", Q_NULLPTR));
        loginButton->setText(QApplication::translate("MainWindow", "Log in", Q_NULLPTR));
        playingasLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
