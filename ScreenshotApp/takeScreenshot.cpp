#include <QPushButton>
#include <QFileDialog>
#include <QScreen>
#include <QTimer>
#include <QDateTime>
#include <QShortcut>
#include <QDebug>

class ScreenshotHandler{

public:
    ScreenshotHandler(QWidget *mainWindow,QPushButton *button):
        mainWindow(mainWindow),screenshotButton(button){
    }

    void startScreenshotSession(){

        saveDirectory = QFileDialog::getExistingDirectory
            (mainWindow,"choose directory","");

        if(!saveDirectory.isEmpty()){
            QShortcut *shotcut = new QShortcut(QKeySequence("T"),mainWindow);
            QObject::connect(shotcut,&QShortcut::activated,[=](){
                takeScreenshotAfterMinimizing();
            });

            screenshotButton->setText("Press T  to take screenshot");
            screenshotButton->setEnabled(false);
        }
    }

private :
    QWidget *mainWindow;
    QPushButton *screenshotButton;
    QString saveDirectory;

    void takeScreenshotAfterMinimizing()
    {

        mainWindow->showMinimized();

        QTimer::singleShot(500, [this]() {

            QScreen *screen = QGuiApplication::primaryScreen();

            if (!screen) {
                qWarning() << "Screen not found!";
                mainWindow->showNormal();
                return;
            }

            QPixmap screenshot = screen->grabWindow(0);

            if (screenshot.isNull()) {
                qWarning() << "Screenshot could not be taken!";
                mainWindow->showNormal();
                return;
            }

            QString fileName =
                QString("screenshot_%1.png")
                                   .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss"));


            QString filePath = QDir(saveDirectory).filePath(fileName);

            if (screenshot.save(filePath, "PNG")) {
                qDebug() << "Screenshot saved" << filePath;
            } else {
                qWarning() << "Screenshot could not be saved!";
            }

            mainWindow->showNormal();
            mainWindow->raise();
            mainWindow->activateWindow();
        });
    }
};