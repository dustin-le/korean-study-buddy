#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <vector>
#include <string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    std::vector<std::string> korean;
    std::vector<std::string> english;
    int random_korean, a_status, b_status, c_status, d_status;


    void play();


public slots:
    // Main menu buttons
    void study_click();
    void add_click();
    void exit_click();

    // Study menu buttons
    void a_click();
    void b_click();
    void c_click();
    void d_click();
    void quit_click();

    // Add menu buttons
    void add_word_click();
    void add_back_click();

private:
    Ui::MainWindow *ui;
};


#endif // WIDGET_H
