#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>

#include "audioRecorder.cpp";

class AppManager{

public:
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
    QPushButton *recordButton;

    QString outputFilePath;

    AudioRecorder recorder;

    void setupUI() {
        mainWindow = new QMainWindow();
        mainWindow->setWindowTitle("Audio Recorder App");
        mainWindow->resize(300, 100);

        centralWidget = new QWidget();
        mainLayout = new QVBoxLayout(centralWidget);

        centralWidget->setLayout(mainLayout);
        mainWindow->setCentralWidget(centralWidget);

        titleLabel = new QLabel("Audio Recorder");
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet("font-size:20px; color:blue");
        mainLayout->addWidget(titleLabel);

        recordButton = new QPushButton("Record");
        recordButton->setStyleSheet("font-size:20px;color:white; background-color: green");
        mainLayout->addWidget(recordButton);


        QObject::connect(recordButton,&QPushButton::clicked,[this](){

            if(recordButton->text()=="Record"){

                outputFilePath=QFileDialog::getSaveFileName(mainWindow,"Save Recording","","Audio Files (*.mp3)");

                if(!outputFilePath.isEmpty()){
                    if(recorder.startRecording(outputFilePath)){
                        recordButton->setText("stop");
                        recordButton->setStyleSheet("background-color:red");

                        titleLabel->setText(recorder.message);

                        mainWindow->showMinimized();
                    }
                    else{
                        titleLabel->setText(recorder.message);
                    }
                }
            }
            else{
                recorder.stopRecording();
                recordButton->setText("Record");
                recordButton->setStyleSheet("background-color:green");
                titleLabel->setText(recorder.message);
                mainWindow->show();
            }
        });
    }
};