#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <iomanip>
#include <limits>
#include <cctype>
#include <sstream>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// ============================================================
//                 CONSOLE VISUALIZATION UI
// ============================================================
namespace UI {

    enum Color {
        DEFAULT = 7,
        CYAN = 11,
        GREEN = 10,
        YELLOW = 14,
        RED = 12,
        MAGENTA = 13,
        WHITE = 15,
        GRAY = 8,
        BLUE = 9
    };

    void color(int c) {
#ifdef _WIN32
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
#else
        const char* code = "37";
        if (c == GREEN) code = "32";
        else if (c == RED) code = "31";
        else if (c == YELLOW) code = "33";
        else if (c == CYAN) code = "36";
        else if (c == MAGENTA) code = "35";
        else if (c == BLUE) code = "34";
        else if (c == GRAY) code = "90";
        else if (c == WHITE) code = "97";
        cout << "\033[" << code << "m";
#endif
    }

    void reset() {
#ifdef _WIN32
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT);
#else
        cout << "\033[0m";
#endif
    }

    void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void line(char ch = '=', int width = 70) {
        cout << string(width, ch) << "\n";
    }

    void centered(const string& text, int width = 70) {
        int spaces = (width - static_cast<int>(text.length())) / 2;
        if (spaces < 0) spaces = 0;
        cout << string(spaces, ' ') << text << "\n";
    }

    void header(const string& title, const string& subtitle = "") {
        color(CYAN);
        cout << "\n";
        line('=', 70);
        centered(title);
        if (!subtitle.empty()) {
            color(WHITE);
            centered(subtitle);
        }
        color(CYAN);
        line('=', 70);
        reset();
    }

    void logo() {
        color(CYAN);
        cout << R"(
   ____  _   _ ___ _______    __  __    _    ____ _____ _____ ____
  / __ \| | | |_ _|__   __|  |  \/  |  / \  / ___|_   _| ____|  _ \
 | |  | | | | || |   | |     | |\/| | / _ \| |     | | |  _| | |_) |
 | |__| | |_| || |   | |     | |  | |/ ___ \ |___  | | | |___|  _ <
  \___\_\\___/|___|  |_|     |_|  |_/_/   \_\____| |_| |_____|_| \_\
)";
        reset();

        color(MAGENTA);
        centered("C++ OBJECT-ORIENTED PROGRAMMING CHALLENGE");
        reset();
        cout << "\n";
    }

    void progress(int current, int total) {
        const int width = 36;
        int filled = total > 0 ? (current * width / total) : 0;

        color(GRAY);
        cout << "  PROGRESS  [";
        color(CYAN);
        cout << string(filled, '#');
        color(GRAY);
        cout << string(width - filled, '.');
        cout << "] ";

        color(WHITE);
        cout << current << "/" << total;
        reset();
        cout << "\n";
    }

    void scoreBar(int correct, int attempted) {
        const int width = 30;
        int filled = attempted > 0 ? correct * width / attempted : 0;

        cout << "  SCORE     [";
        color(GREEN);
        cout << string(filled, '#');
        color(GRAY);
        cout << string(width - filled, '.');
        reset();
        cout << "] " << correct << "/" << attempted << "\n";
    }

    void option(int number, const string& value) {
        color(CYAN);
        cout << "       +----+  ";
        color(WHITE);
        cout << value;
        reset();
        cout << "\n";

        color(CYAN);
        cout << "       | ";
        color(YELLOW);
        cout << number;
        color(CYAN);
        cout << "  |";
        reset();
        cout << "\n";
    }

    void questionCard(const string& question) {
        color(WHITE);
        cout << "  +------------------------------------------------------------------+\n";
        cout << "  | ";
        color(YELLOW);
        cout << "QUESTION";
        color(WHITE);
        cout << "                                                           |\n";
        cout << "  +------------------------------------------------------------------+\n";
        cout << "  | " << left << setw(66) << question << "|\n";
        cout << "  +------------------------------------------------------------------+\n";
        reset();
    }

    void correct() {
        color(GREEN);
        cout << "\n  +------------------------------------------------------------------+\n";
        cout << "  |                         CORRECT! +1 POINT                       |\n";
        cout << "  +------------------------------------------------------------------+\n";
        reset();
    }

    void wrong() {
        color(RED);
        cout << "\n  +------------------------------------------------------------------+\n";
        cout << "  |                           WRONG ANSWER                          |\n";
        cout << "  +------------------------------------------------------------------+\n";
        reset();
    }

    void resultMetric(const string& label, const string& value, int c) {
        cout << "  | " << left << setw(20) << label << ": ";
        color(c);
        cout << setw(42) << left << value;
        reset();
        cout << "|\n";
    }

    void pause() {
        color(GRAY);
        cout << "\n  Press ENTER to continue...";
        reset();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
}

/*
    PROJECT TITLE: QUIZ GAME SYSTEM
    LANGUAGE     : C++
    CONCEPT      : Object-Oriented Programming (OOP)

    OOP concepts demonstrated:
    1. Classes & Objects
    2. Encapsulation
    3. Inheritance
    4. Polymorphism
    5. Abstraction

    Main Modules:
    - User Module
    - Question Bank Module
    - Quiz Engine Module
    - Validation Module
    - Scoring Module
    - Result Module
*/

// ============================================================
//                         USER MODULE
// ============================================================
class Player {
private:
    string name;
    int score;

public:
    Player(const string& playerName = "Player")
        : name(playerName), score(0) {}

    string getName() const {
        return name;
    }

    int getScore() const {
        return score;
    }

    void increaseScore() {
        score++;
    }
};

// ============================================================
//              ABSTRACT BASE CLASS: QUESTION
// ============================================================
class Question {
protected:
    string questionText;
    vector<string> options;
    int correctOption;
    string explanation;

public:
    Question(const string& text,
             const vector<string>& opts,
             int correct,
             const string& exp)
        : questionText(text),
          options(opts),
          correctOption(correct),
          explanation(exp) {}

    virtual ~Question() = default;

    virtual void display(int number, int total) const = 0;

    bool checkAnswer(int answer) const {
        return answer == correctOption;
    }

    int getCorrectOption() const {
        return correctOption;
    }

    string getCorrectAnswer() const {
        if (correctOption >= 1 &&
            correctOption <= static_cast<int>(options.size())) {
            return options[correctOption - 1];
        }
        return "Unknown";
    }

    string getExplanation() const {
        return explanation;
    }
};

// ============================================================
//                  DERIVED CLASS: MCQ QUESTION
// ============================================================
class MCQQuestion : public Question {
public:
    MCQQuestion(const string& text,
                const vector<string>& opts,
                int correct,
                const string& exp)
        : Question(text, opts, correct, exp) {}

    void display(int number, int total) const override {
        cout << "\n";
        UI::header("QUIZ ARENA", "C++ / OOP Knowledge Challenge");

        UI::color(UI::CYAN);
        cout << "  QUESTION " << number << " OF " << total << "\n";
        UI::reset();

        UI::progress(number, total);
        cout << "\n";

        UI::questionCard(questionText);
        cout << "\n";

        for (size_t i = 0; i < options.size(); ++i) {
            UI::color(UI::CYAN);
            cout << "  +----";
            UI::color(UI::WHITE);
            cout << "[" << i + 1 << "]";
            UI::color(UI::CYAN);
            cout << "---------------------------------------------------------+\n";
            UI::reset();

            cout << "       ";
            UI::color(UI::WHITE);
            cout << options[i] << "\n";
            UI::reset();
        }

        UI::color(UI::CYAN);
        cout << "  +------------------------------------------------------------------+\n";
        UI::reset();
    }
};

// ============================================================
//                     QUESTION BANK MODULE
// ============================================================
class QuestionBank {
public:
    static vector<shared_ptr<Question>> getCppQuestions() {
        vector<shared_ptr<Question>> questions;

        questions.push_back(make_shared<MCQQuestion>(
            "Which keyword is used to create a class in C++?",
            vector<string>{"class", "object", "create", "define"},
            1,
            "The 'class' keyword is used to declare a class in C++."));

        questions.push_back(make_shared<MCQQuestion>(
            "Which OOP concept hides data using private members?",
            vector<string>{"Inheritance", "Encapsulation", "Polymorphism", "Recursion"},
            2,
            "Encapsulation protects data by controlling access through public methods."));

        questions.push_back(make_shared<MCQQuestion>(
            "Which feature allows one class to acquire properties of another class?",
            vector<string>{"Inheritance", "Looping", "Compilation", "Pointer Arithmetic"},
            1,
            "Inheritance allows a derived class to reuse members of a base class."));

        questions.push_back(make_shared<MCQQuestion>(
            "What is an object in C++?",
            vector<string>{"A loop", "An instance of a class", "A header file", "A compiler"},
            2,
            "An object is an instance created from a class."));

        questions.push_back(make_shared<MCQQuestion>(
            "Which keyword is commonly used for runtime polymorphism in C++?",
            vector<string>{"static", "friend", "virtual", "namespace"},
            3,
            "Virtual functions enable runtime polymorphism through method overriding."));

        questions.push_back(make_shared<MCQQuestion>(
            "Which access specifier provides maximum data hiding?",
            vector<string>{"public", "private", "global", "extern"},
            2,
            "Private members can be accessed directly only inside the class."));

        questions.push_back(make_shared<MCQQuestion>(
            "A constructor is mainly used to:",
            vector<string>{"Delete objects", "Initialize objects", "Print output", "Create loops"},
            2,
            "A constructor initializes an object's data when the object is created."));

        questions.push_back(make_shared<MCQQuestion>(
            "Which symbol is used for inheritance in a C++ class declaration?",
            vector<string>{":", "::", "->", "#"},
            1,
            "A colon is used before the base-class access specifier and base-class name."));

        questions.push_back(make_shared<MCQQuestion>(
            "Which standard container is used in this project to store multiple questions?",
            vector<string>{"vector", "fstream", "iomanip", "cmath"},
            1,
            "std::vector dynamically stores the collection of questions."));

        questions.push_back(make_shared<MCQQuestion>(
            "What does method overriding mean?",
            vector<string>{
                "Creating two variables with the same name",
                "Redefining a base-class virtual function in a derived class",
                "Deleting a constructor",
                "Using a loop inside a function"
            },
            2,
            "Overriding gives a derived class its own implementation of a virtual function."));

        return questions;
    }
};

// ============================================================
//                     VALIDATION MODULE
// ============================================================
class InputValidator {
public:
    static int readOption(int minValue, int maxValue) {
        int choice;

        while (true) {
            UI::color(UI::YELLOW);
            cout << "\n  >> ENTER YOUR ANSWER (" << minValue
                 << "-" << maxValue << "): ";
            UI::reset();

            if (cin >> choice &&
                choice >= minValue &&
                choice <= maxValue) {
                return choice;
            }

            UI::color(UI::RED);
            cout << "  Invalid input! Please enter a valid option.\n";
            UI::reset();

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
};

// ============================================================
//                       RESULT MODULE
// ============================================================
class Result {
public:
    static void show(const Player& player, int totalQuestions) {
        int correct = player.getScore();
        int wrong = totalQuestions - correct;

        double percentage = totalQuestions > 0
            ? (static_cast<double>(correct) / totalQuestions) * 100.0
            : 0.0;

        UI::clearScreen();
        UI::header("FINAL RESULTS", "C++ OOP Quiz Performance Dashboard");

        cout << "\n";
        UI::color(UI::CYAN);
        cout << "  +------------------------------------------------------------------+\n";
        UI::reset();

        UI::resultMetric("PLAYER", player.getName(), UI::WHITE);
        UI::resultMetric("TOTAL QUESTIONS", to_string(totalQuestions), UI::WHITE);
        UI::resultMetric("CORRECT ANSWERS", to_string(correct), UI::GREEN);
        UI::resultMetric("WRONG ANSWERS", to_string(wrong), UI::RED);

        string scoreText =
            to_string(correct) + " / " + to_string(totalQuestions);
        UI::resultMetric("FINAL SCORE", scoreText, UI::YELLOW);

        stringstream dummy;
        dummy << fixed << setprecision(1) << percentage << "%";
        UI::resultMetric("ACCURACY", dummy.str(), UI::CYAN);

        UI::color(UI::CYAN);
        cout << "  +------------------------------------------------------------------+\n";
        UI::reset();

        cout << "\n  ACCURACY VISUALIZATION\n  [";
        int bars = static_cast<int>(percentage / 5.0);

        UI::color(UI::GREEN);
        cout << string(bars, '#');
        UI::color(UI::GRAY);
        cout << string(20 - bars, '.');
        UI::reset();

        cout << "] " << fixed << setprecision(1)
             << percentage << "%\n\n";

        cout << "  PERFORMANCE LEVEL: ";

        if (percentage >= 80) {
            UI::color(UI::GREEN);
            cout << "EXCELLENT!  ★★★★★";
        }
        else if (percentage >= 60) {
            UI::color(UI::CYAN);
            cout << "GOOD JOB!   ★★★★☆";
        }
        else if (percentage >= 40) {
            UI::color(UI::YELLOW);
            cout << "KEEP PRACTICING!   ★★★☆☆";
        }
        else {
            UI::color(UI::RED);
            cout << "NEEDS IMPROVEMENT   ★★☆☆☆";
        }

        UI::reset();
        cout << "\n\n";

        UI::color(UI::MAGENTA);
        cout << "             +--------------------------------+\n";
        cout << "             |      THANK YOU FOR PLAYING!    |\n";
        cout << "             +--------------------------------+\n";
        UI::reset();

        UI::line('=', 70);
    }
};

// ============================================================
//                    QUIZ ENGINE MODULE
// ============================================================
class Quiz {
private:
    Player player;
    vector<shared_ptr<Question>> questions;

public:
    Quiz(const string& playerName,
         const vector<shared_ptr<Question>>& questionSet)
        : player(playerName), questions(questionSet) {}

    void start() {
        UI::clearScreen();
        UI::logo();

        UI::color(UI::WHITE);
        cout << "  Welcome, " << player.getName() << "!\n";
        cout << "  You have " << questions.size()
             << " questions in this challenge.\n";
        cout << "  Select the correct option and build your score.\n";
        UI::reset();

        UI::color(UI::YELLOW);
        cout << "\n  GAME RULES\n";
        UI::reset();
        cout << "  * Choose one answer for each question.\n";
        cout << "  * Every correct answer gives +1 point.\n";
        cout << "  * Your final accuracy is shown at the end.\n";

        UI::line('-', 70);

        for (size_t i = 0; i < questions.size(); ++i) {
            UI::clearScreen();

            // Polymorphic call
            questions[i]->display(
                static_cast<int>(i + 1),
                static_cast<int>(questions.size())
            );

            int answer = InputValidator::readOption(1, 4);

            if (questions[i]->checkAnswer(answer)) {
                UI::correct();
                cout << "  Explanation: "
                     << questions[i]->getExplanation() << "\n";
                player.increaseScore();
            }
            else {
                UI::wrong();

                UI::color(UI::YELLOW);
                cout << "  Correct Option : "
                     << questions[i]->getCorrectOption() << "\n";
                cout << "  Correct Answer : "
                     << questions[i]->getCorrectAnswer() << "\n";
                UI::reset();

                cout << "  Explanation    : "
                     << questions[i]->getExplanation() << "\n";
            }

            cout << "\n";
            UI::scoreBar(
                player.getScore(),
                static_cast<int>(i + 1)
            );

            UI::color(UI::CYAN);
            cout << "\n  SCOREBOARD: "
                 << player.getScore()
                 << " / " << i + 1 << "\n";
            UI::reset();

            if (i + 1 < questions.size()) {
                UI::color(UI::GRAY);
                cout << "\n  Press ENTER for the next question...";
                UI::reset();

                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
            }
        }

        Result::show(player, static_cast<int>(questions.size()));
    }
};

// ============================================================
//                           MAIN
// ============================================================
int main() {
    UI::clearScreen();
    UI::logo();

    UI::color(UI::WHITE);
    cout << "  PROJECT: QUIZ GAME SYSTEM\n";
    cout << "  LANGUAGE: C++\n";
    cout << "  TOPIC  : OBJECT-ORIENTED PROGRAMMING\n";
    UI::reset();

    UI::line('-', 70);

    string playerName;
    UI::color(UI::YELLOW);
    cout << "\n  ENTER PLAYER NAME: ";
    UI::reset();
    getline(cin, playerName);

    if (playerName.empty()) {
        playerName = "Player";
    }

    UI::header("QUIZ SELECTION", "Choose your challenge");

    UI::color(UI::CYAN);
    cout << "  +--------------------------------------------------------------+\n";
    cout << "  |  [1]  C++ / OOP                                             |\n";
    cout << "  |       Classes • Inheritance • Polymorphism • Abstraction    |\n";
    cout << "  +--------------------------------------------------------------+\n";
    UI::reset();

    int category = InputValidator::readOption(1, 1);
    (void)category;

    vector<shared_ptr<Question>> questions =
        QuestionBank::getCppQuestions();

    char playAgain;

    do {
        Quiz quiz(playerName, questions);
        quiz.start();

        UI::header("PLAY AGAIN?", "Keep improving your score!");

        UI::color(UI::GREEN);
        cout << "  [Y] YES - Start another attempt\n";
        UI::reset();

        UI::color(UI::RED);
        cout << "  [N] NO  - Exit the quiz\n";
        UI::reset();

        cout << "\n  Enter choice: ";
        cin >> playAgain;

        playAgain = static_cast<char>(
            tolower(static_cast<unsigned char>(playAgain))
        );

        while (playAgain != 'y' && playAgain != 'n') {
            UI::color(UI::RED);
            cout << "  Invalid choice! Please enter Y or N: ";
            UI::reset();

            cin >> playAgain;

            playAgain = static_cast<char>(
                tolower(static_cast<unsigned char>(playAgain))
            );
        }

    } while (playAgain == 'y');

    UI::clearScreen();
    UI::header("SESSION ENDED", "Thank you for playing!");

    UI::color(UI::CYAN);
    cout << "  Keep practicing C++ and OOP. See you next time!\n";
    UI::reset();

    UI::line('=', 70);

    return 0;
}
