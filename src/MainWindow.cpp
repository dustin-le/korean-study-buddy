#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "study.h"
#include <iostream>
#include <fstream>
#include <QMessageBox>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    // Adds background image.
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    QPixmap bkgnd("background.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    // Read in data file. If file is not present, create it and return to main menu.
    std::string temp, token;
    std::fstream file("data.txt", std::fstream::in);

    if (file.is_open())
    {
        while (getline(file, temp))
        {
            token = temp.substr(0, temp.find(","));
            korean.push_back(token);

            token = temp.substr(temp.find(",")+1, temp.length()-1);
            english.push_back(token);
        }
    }

    else
    {
        std::fstream new_file("data.txt", std::fstream::out);
        QMessageBox::information(this, "File Created", "No file was found, so a new file was made. Add some phrases to study!");
    }

    // Main menu buttons.
    connect(ui->study, SIGNAL(clicked()), this, SLOT(study_click()));
    connect(ui->add, SIGNAL(clicked()), this, SLOT(add_click()));
    connect(ui->exit, SIGNAL(clicked()), this, SLOT(exit_click()));

    // Quiz buttons.
    connect(ui->A, SIGNAL(clicked()), this, SLOT(a_click()));
    connect(ui->B, SIGNAL(clicked()), this, SLOT(b_click()));
    connect(ui->C, SIGNAL(clicked()), this, SLOT(c_click()));
    connect(ui->D, SIGNAL(clicked()), this, SLOT(d_click()));
    connect(ui->quit, SIGNAL(clicked()), this, SLOT(quit_click()));

    // Add word buttons.
    connect(ui->add_word, SIGNAL(clicked()), this, SLOT(add_word_click()));
    connect(ui->add_back, SIGNAL(clicked()), this, SLOT(add_back_click()));
}

//                                                 MAIN MENU BUTTONS

void MainWindow::study_click()
{
    std::string temp, token;
    std::fstream file("data.txt", std::fstream::in);

    if (file.peek() == EOF)
    {
        QMessageBox::information(this, "Empty", "No phrases found!");
    }

    else if (korean.size() < 4)
    {
        QMessageBox::information(this, "Not enough phrases!", "You need at least four phrases to study!");
    }

    else
    {
        ui->stackedWidget->setCurrentIndex(1); // Only show quiz menu if file is present and not empty.
        MainWindow::play();
    }
}

void MainWindow::add_click() // Go to the add window.
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::exit_click()
{
    this->close();
}

//                                                 STUDY MENU BUTTONS

void MainWindow::play()
{
//                                                POPULATE THE BUTTONS

    // Get a random Korean phrase and set it as the question. The same number will be used for its corresponding English translation.
    random_korean = rand() % static_cast<int>(korean.size());
    QString qstr = QString::fromStdString(korean[random_korean]); // Must convert to qstr to display on QWidgets.
    ui->question->setText(qstr);

    // Status for if the button is occupied. 0 for unoccupied, 1 for occupied, 2 for correct answer.
    a_status = 0;
    b_status = 0;
    c_status = 0;
    d_status = 0;

    // Adding the correct answer to a random button. That button's status will be set to 1 (occupied).
    int random_button = rand() % 4;
    qstr = QString::fromStdString(english[random_korean]); // English translation of the Korean phrase in question.
    if (random_button == 0)
    {
        a_status = 2;
        ui->A->setText(qstr);
    }

    if (random_button == 1)
    {
        b_status = 2;
        ui->B->setText(qstr);
    }

    if (random_button == 2)
    {
        c_status = 2;
        ui->C->setText(qstr);
    }

    if (random_button == 3)
    {
        d_status = 2;
        ui->D->setText(qstr);
    }

    // Adding the incorrect answers to random buttons.
    int random_english;
    std::vector<int> used_english; // Stores the indexes of used (English) answer choices.
    while ((a_status == 0) || (b_status == 0) || (c_status == 0) || (d_status == 0)) // Putting in wrong answers to the rest of the buttons.
    {
        random_english = rand() % static_cast<int>(english.size()); // Used to add English phrases into answer choices.
        while (random_english == random_korean || (std::find(used_english.begin(), used_english.end(), random_english) != used_english.end())) // Accounts for failure: the possibility that we get the correct answer again or we get a value we've already used.
        {
            random_english = rand() % static_cast<int>(english.size());
        }
        qstr = QString::fromStdString(english[random_english]); // English translation of a random Korean word that isn't the right answer.
        random_button = rand() % 4;

        // If the button is unoccupied (only occupied slot on the first run will be the correct answer), then put the English translation in.
        if (random_button == 0 && a_status == 0)
        {
            used_english.push_back(random_english); // Only put the value into the vector if it's actually used. Made the mistake of putting it outside of these parameters and caused an infinite loop.
            a_status = 1;
            ui->A->setText(qstr);
        }

        if (random_button == 1 && b_status == 0)
        {
            used_english.push_back(random_english);
            b_status = 1;
            ui->B->setText(qstr);
        }

        if (random_button == 2 && c_status == 0)
        {
            used_english.push_back(random_english);
            c_status = 1;
            ui->C->setText(qstr);
        }

        if (random_button == 3 && d_status == 0)
        {
            used_english.push_back(random_english);
            d_status = 1;
            ui->D->setText(qstr);
        }
    }
}

void MainWindow::a_click()
{
    if (a_status == 2)
    {
        QMessageBox::information(this, "Correct!", "That's right!");
    }

    else
    {
        QMessageBox::information(this, "Wrong!", "That's not quite it!");
    }

    play();
}

void MainWindow::b_click()
{
    if (b_status == 2)
    {
        QMessageBox::information(this, "Correct!", "That's right!");
    }

    else
    {
        QMessageBox::information(this, "Wrong!", "That's not quite it!");
    }

    play();
}

void MainWindow::c_click()
{
    if (c_status == 2)
    {
        QMessageBox::information(this, "Correct!", "That's right!");
    }

    else
    {
        QMessageBox::information(this, "Wrong!", "That's not quite it!");
    }

    play();
}

void MainWindow::d_click()
{
    if (d_status == 2)
    {
        QMessageBox::information(this, "Correct!", "That's right!");
    }

    else
    {
        QMessageBox::information(this, "Wrong!", "That's not quite it!");
    }

    play();
}

void MainWindow::quit_click()
{
    ui->stackedWidget->setCurrentIndex(0);
}

//                                                  ADD WORD BUTTONS

void MainWindow::add_word_click()
{
    std::string temp;
    std::fstream file("data.txt", std::fstream::in | std::fstream::app);

    if (std::find(korean.begin(), korean.end(), ui->korean_text->text().toStdString()) != korean.end()) // If a duplicate is found
    {
        QMessageBox::information(this, "Duplicate", "This entry already exists!");
        ui->korean_text->clear();
        ui->english_text->clear();
    }

    else if (ui->korean_text->text().isEmpty())
    {
        QMessageBox::critical(this, "Missing Korean Input", "Did you forget to add some Korean?");
    }

    else if (ui->english_text->text().isEmpty())
    {
        QMessageBox::critical(this, "Missing English Input", "Did you forget to add some English?");
    }

    else if (ui->korean_text->text().isEmpty() && ui->english_text->text().isEmpty())
    {
        QMessageBox::critical(this, "Missing Input", "Try putting in some phrases!");
    }

    else
    {
        std::string korean_input = ui->korean_text->text().toStdString();
        korean.push_back(korean_input);
        file << korean_input;
        ui->korean_text->clear();

        file << ","; // Delimiter

        std::string english_input = ui->english_text->text().toStdString();
        english.push_back(english_input);
        file << english_input << std::endl;
        ui->english_text->clear();

        // file << "0"; // Memorization scale
    }
}

void MainWindow::add_back_click() // Returning from the add window to the main menu.
{
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
