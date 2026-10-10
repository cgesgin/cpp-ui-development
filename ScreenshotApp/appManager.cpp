#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "takeScreenshot.cpp";

class AppManager{

public :

    AppManager(){

        setupUI();
    }

    void show(){
        mainWindow->show();
    }

private :

    QMainWindow *mainWindow;
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;

    QLabel *titleLabel;
    QPushButton *screenshotButton;

    void setupUI(){

        mainWindow = new QMainWindow();
        mainWindow->setWindowTitle("Sreenshot App");
        mainWindow->resize(300,400);

        centralWidget = new QWidget();
        mainLayout = new QVBoxLayout(centralWidget);

        centralWidget->setLayout(mainLayout);
        mainWindow->setCentralWidget(centralWidget);

        titleLabel = new QLabel("Take Screenshot App");
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet("font-size:24px;color:blue");
        mainLayout->addWidget(titleLabel);

        screenshotButton = new QPushButton("Take Screenshot");
        screenshotButton->setStyleSheet(
            "font-size:18px;"
            "color:white;"
            "background-color:green"
            );
        mainLayout->addWidget(screenshotButton);

        QObject::connect(screenshotButton,&QPushButton::clicked,[=](){
            ScreenshotHandler *handler = new ScreenshotHandler(mainWindow,screenshotButton);
            handler->startScreenshotSession();
        });
    }
};