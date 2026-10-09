# 🎮 C++ Quiz Game

A console-based **C++ Quiz Game System** built to practice and demonstrate **Object-Oriented Programming (OOP)** concepts. The project also includes a polished terminal interface with progress bars, score visualization, feedback, explanations, and a final performance dashboard.

## ✨ Features

- Interactive C++ / OOP multiple-choice quiz
- Player name input
- Input validation for answer choices
- Progress bar and score visualization
- Correct / wrong answer feedback
- Answer explanations
- Final score and accuracy dashboard
- Performance level indicator
- Play-again option
- Windows and ANSI-compatible console color handling

## 🛠️ Technologies Used

- **C++17**
- Object-Oriented Programming
- Standard Template Library (STL)
- `vector`, `shared_ptr`, `string`, `iomanip`, `sstream`
- Console UI and color handling

## 📚 OOP Concepts Demonstrated

| Concept | Where it appears |
|---|---|
| Classes & Objects | `Player`, `Question`, `MCQQuestion`, `Quiz`, `Result` |
| Encapsulation | Private data members with public methods |
| Inheritance | `MCQQuestion : public Question` |
| Polymorphism | Virtual `display()` and overriding |
| Abstraction | Abstract `Question` base class |

## 📂 Project Structure

```text
Cpp-Quiz-Game/
│
├── 📄 README.md
├── 📄 main.cpp
├── 📄 functions.cpp
├── 📄 functions.h
│
├── 📁 data
│   └── 📄 input.txt
│
├── 📁 output
│   └── 📄 sample_output.txt
│
└── 📁 screenshots
    ├── 🖼️ output1.png
    └── 🖼️ output2.png
```

> **Note:** The current implementation is intentionally kept in `main.cpp`. `functions.cpp` and `functions.h` are included as the prepared structure for future modularization.

## ▶️ How to Run

### Windows / MinGW

```bash
g++ -std=c++17 main.cpp -o quiz_game.exe
quiz_game.exe
```

### Linux / macOS

```bash
g++ -std=c++17 main.cpp -o quiz_game
./quiz_game
```

## 🖥️ Sample Output

A recorded sample run is available here:

[`output/sample_output.txt`](output/sample_output.txt)

## 📸 Screenshots

The `screenshots` folder contains visual captures of the quiz interface and final results dashboard.

## 🎯 Project Purpose

This project was created to strengthen C++ programming and OOP skills through an interactive application, while also improving console-based UI design and user experience.

## 👨‍💻 Author

**Akash Basina**
