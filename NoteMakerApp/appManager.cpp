#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QMap>
#include <QLineEdit>

class AppManager
{
public:

    AppManager()
    {
        setupUI();
    }

    void show()
    {
        mainWindow->show();
    }

private:

    QMainWindow *mainWindow;
    QWidget *centralWidget;
    QHBoxLayout *mainLayout;

    QListWidget *noteList;
    QTextEdit *noteEditor;

    QPushButton *saveButton;
    QPushButton *deleteButton;
    QPushButton *newButton;

    QLineEdit *titleEditor;

    // Note storage
    QMap<QString, QString> notes;


    void setupUI()
    {
        mainWindow = new QMainWindow();

        mainWindow->setWindowTitle("Note Maker App");
        mainWindow->resize(600, 400);


        // Central Widget
        centralWidget = new QWidget();

        mainLayout = new QHBoxLayout(centralWidget);


        // --------------------
        // LEFT PANEL
        // --------------------

        QWidget *leftPanel = new QWidget();

        QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);

        noteList = new QListWidget();
        leftLayout->addWidget(noteList);

        newButton = new QPushButton("New Note");
        leftLayout->addWidget(newButton);

        deleteButton = new QPushButton("Delete");
        leftLayout->addWidget(deleteButton);

        // --------------------
        // RIGHT PANEL
        // --------------------

        QWidget *rightPanel = new QWidget();

        QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);

        titleEditor = new QLineEdit();
        titleEditor->setPlaceholderText("Note title...");

        rightLayout->addWidget(titleEditor);

        noteEditor = new QTextEdit();

        noteEditor->setPlaceholderText(
            "Write your notes here..."
            );

        rightLayout->addWidget(noteEditor);


        saveButton = new QPushButton("Save");

        rightLayout->addWidget(saveButton);


        // --------------------
        // MAIN LAYOUT
        // --------------------

        mainLayout->addWidget(leftPanel, 1);
        mainLayout->addWidget(rightPanel, 2);


        mainWindow->setCentralWidget(centralWidget);


        // --------------------
        // CONNECTIONS
        // --------------------

        QObject::connect(
            saveButton,
            &QPushButton::clicked,
            [this]()
            {
                saveCurrentNote();
            }
            );

        QObject::connect(
            deleteButton,
            &QPushButton::clicked,
            [this]()
            {
                onDeleteClicked();
            }
            );

        QObject::connect(
            noteList,
            &QListWidget::itemClicked,
            [this](QListWidgetItem *item)
            {
                onNoteSelected(item);
            }
            );

        QObject::connect(
            newButton,
            &QPushButton::clicked,
            [this]()
            {
                clearNotes();
            }
            );
    }


    void saveCurrentNote()
    {
        QString title = titleEditor->text().trimmed();
        QString content = noteEditor->toPlainText().trimmed();

        if (title.isEmpty())
        {
            QMessageBox::warning(
                mainWindow,
                "Error",
                "Please enter a title"
                );

            return;
        }

        if (content.isEmpty())
        {
            QMessageBox::warning(
                mainWindow,
                "Error",
                "No Text to Save"
                );

            return;
        }

        notes[title] = content;
        noteEditor->clear();
        titleEditor->clear();

        updateNotes();
    }


    void updateNotes()
    {
        titleEditor->clear();
        noteList->clear();

        for (const QString &title : notes.keys())
        {
            noteList->addItem(title);
        }
    }

    void onNoteSelected(QListWidgetItem *item){

        QString title = item->text();
        noteEditor->clear();
        titleEditor->clear();
        titleEditor->setText(title);
        noteEditor->setText(notes.value(title));
    }

    void onDeleteClicked(){

        QListWidgetItem *item = noteList->currentItem();

        if(!item){

            QMessageBox::warning(mainWindow,"error","No note Selected.");
            return;
        }

        QString title = item->text();
        notes.remove(title);

        delete item;
        noteEditor->clear();
        titleEditor->clear();
    }

    void clearNotes()
    {
        QMessageBox::StandardButton reply =
            QMessageBox::question(
                mainWindow,
                "Clear Notes",
                "Are you sure you want to clear the editor?",
                QMessageBox::Yes | QMessageBox::No
                );

        if (reply != QMessageBox::Yes)
            return;

        titleEditor->clear();
        noteEditor->clear();
    }
};