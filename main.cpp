#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <algorithm>
#include <cctype>
#include <chrono>

using namespace std;


// ============================================================
// PLAYER CLASS
// ============================================================

class Player {
private:
    string name;
    int score;

public:
    Player(string playerName = "Player") {
        name = playerName;
        score = 0;
    }

    string getName() const {
        return name;
    }

    int getScore() const {
        return score;
    }

    void addScore(int points) {
        score += points;
    }

    void resetScore() {
        score = 0;
    }
};


// ============================================================
// WORD MANAGER CLASS
// ============================================================

class WordManager {
private:

    // Sports
    vector<string> sportsEasy = {
        "golf", "tennis", "soccer"
    };

    vector<string> sportsMedium = {
        "cricket", "baseball", "hockey"
    };

    vector<string> sportsHard = {
        "badminton", "volleyball", "wrestling"
    };


    // Cities
    vector<string> citiesEasy = {
        "paris", "delhi", "tokyo"
    };

    vector<string> citiesMedium = {
        "london", "sydney", "berlin"
    };

    vector<string> citiesHard = {
        "kathmandu", "singapore", "johannesburg"
    };


    // Countries
    vector<string> countriesEasy = {
        "india", "nepal", "japan"
    };

    vector<string> countriesMedium = {
        "canada", "brazil", "france"
    };

    vector<string> countriesHard = {
        "australia", "switzerland", "netherlands"
    };


public:

    string getRandomWord(int category, int difficulty) const {

        const vector<string>* words = nullptr;

        if (category == 1) {

            if (difficulty == 1)
                words = &sportsEasy;
            else if (difficulty == 2)
                words = &sportsMedium;
            else
                words = &sportsHard;

        }
        else if (category == 2) {

            if (difficulty == 1)
                words = &citiesEasy;
            else if (difficulty == 2)
                words = &citiesMedium;
            else
                words = &citiesHard;

        }
        else if (category == 3) {

            if (difficulty == 1)
                words = &countriesEasy;
            else if (difficulty == 2)
                words = &countriesMedium;
            else
                words = &countriesHard;
        }

        random_device rd;
        mt19937 gen(rd());

        uniform_int_distribution<int> dist(
            0,
            static_cast<int>(words->size()) - 1
        );

        return (*words)[dist(gen)];
    }


    string categoryName(int category) const {

        if (category == 1)
            return "Sports";

        if (category == 2)
            return "Cities";

        if (category == 3)
            return "Countries";

        return "Unknown";
    }


    string difficultyName(int difficulty) const {

        if (difficulty == 1)
            return "Easy";

        if (difficulty == 2)
            return "Medium";

        if (difficulty == 3)
            return "Hard";

        return "Unknown";
    }
};


// ============================================================
// SCORE MANAGER CLASS
// ============================================================

class ScoreManager {
private:
    const string fileName = "scores.txt";

public:

    void saveScore(const Player& player) const {

        ofstream file(fileName, ios::app);

        if (!file) {
            cout << "\nERROR: Could not open scores.txt for saving.\n";
            return;
        }

        // Store the complete name and score separated by |
        file << player.getName()
             << "|"
             << player.getScore()
             << "\n";

        file.close();

        cout << "\nScore saved successfully!\n";
    }


    void displayHighScores() const {

        ifstream file(fileName);

        cout << "\n====================================\n";
        cout << "           HIGH SCORES\n";
        cout << "====================================\n";

        if (!file) {
            cout << "No high scores found yet.\n";
            cout << "Play a game first to create a score.\n";
            cout << "====================================\n";
            return;
        }

        vector<pair<string, int>> scores;

        string line;

        while (getline(file, line)) {

            if (line.empty())
                continue;

            size_t separator = line.find('|');

            if (separator == string::npos)
                continue;

            string name =
                line.substr(0, separator);

            string scoreText =
                line.substr(separator + 1);

            try {

                int score = stoi(scoreText);

                scores.push_back({name, score});

            }
            catch (...) {
                // Ignore invalid score lines
            }
        }

        file.close();


        if (scores.empty()) {

            cout << "No high scores found yet.\n";
            cout << "====================================\n";

            return;
        }


        // Highest score first
        sort(scores.begin(), scores.end(),
             [](const pair<string, int>& a,
                const pair<string, int>& b) {

                 return a.second > b.second;
             });


        int position = 1;

        for (const auto& entry : scores) {

            cout << position
                 << ". "
                 << entry.first
                 << " - "
                 << entry.second
                 << " points\n";

            position++;

            if (position > 10)
                break;
        }

        cout << "====================================\n";
    }
};


// ============================================================
// GAME CLASS
// ============================================================

class Game {
private:

    Player player;
    WordManager wordManager;
    ScoreManager scoreManager;

    string secretWord;
    string guessedWord;

    vector<char> guessedLetters;

    int category;
    int difficulty;
    int attempts;
    int maxAttempts;
    int timeLimit;


public:

    Game(string playerName)
        : player(playerName) {

        category = 1;
        difficulty = 1;
        attempts = 0;
        maxAttempts = 8;
        timeLimit = 90;
    }


    // --------------------------------------------------------
    // CATEGORY SELECTION
    // --------------------------------------------------------

    void chooseCategory() {

        do {

            cout << "\nSelect a category:\n";
            cout << "1. Sports\n";
            cout << "2. Cities\n";
            cout << "3. Countries\n";
            cout << "Enter your choice: ";

            cin >> category;

            if (cin.fail() || category < 1 || category > 3) {

                cin.clear();
                cin.ignore(1000, '\n');

                cout << "Invalid choice. Please try again.\n";
            }

        } while (category < 1 || category > 3);

        cout << "Selected category: "
             << wordManager.categoryName(category)
             << endl;
    }


    // --------------------------------------------------------
    // DIFFICULTY SELECTION
    // --------------------------------------------------------

    void chooseDifficulty() {

        do {

            cout << "\nSelect difficulty:\n";
            cout << "1. Easy\n";
            cout << "2. Medium\n";
            cout << "3. Hard\n";
            cout << "Enter your choice: ";

            cin >> difficulty;

            if (cin.fail() ||
                difficulty < 1 ||
                difficulty > 3) {

                cin.clear();
                cin.ignore(1000, '\n');

                cout << "Invalid choice. Please try again.\n";
            }

        } while (difficulty < 1 || difficulty > 3);


        if (difficulty == 1) {
            maxAttempts = 8;
            timeLimit = 90;
        }
        else if (difficulty == 2) {
            maxAttempts = 6;
            timeLimit = 60;
        }
        else {
            maxAttempts = 4;
            timeLimit = 45;
        }

        cout << "Selected difficulty: "
             << wordManager.difficultyName(difficulty)
             << endl;

        cout << "Maximum attempts: "
             << maxAttempts << endl;

        cout << "Time limit: "
             << timeLimit
             << " seconds\n";
    }


    // --------------------------------------------------------
    // SETUP ROUND
    // --------------------------------------------------------

    void setupRound() {

        secretWord =
            wordManager.getRandomWord(
                category,
                difficulty
            );

        guessedWord =
            string(secretWord.length(), '_');

        guessedLetters.clear();
        attempts = 0;

        player.resetScore();
    }


    // --------------------------------------------------------
    // DISPLAY HANGMAN
    // --------------------------------------------------------

    void displayHangman() const {

        cout << "\n  +---+\n";

        if (attempts >= 1)
            cout << "  |   O\n";
        else
            cout << "  |\n";

        if (attempts >= 2)
            cout << "  |   |\n";
        else
            cout << "  |\n";

        if (attempts >= 4)
            cout << "  |  /|\\\n";
        else if (attempts >= 3)
            cout << "  |  /|\n";
        else
            cout << "  |\n";

        if (attempts >= 6)
            cout << "  |  / \\\n";
        else if (attempts >= 5)
            cout << "  |  /\n";
        else
            cout << "  |\n";

        cout << "  |\n";
        cout << "__|__\n";

        cout << "Attempts remaining: "
             << maxAttempts - attempts
             << endl;
    }


    // --------------------------------------------------------
    // DISPLAY WORD
    // --------------------------------------------------------

    void displayWord() const {

        cout << "\nWord: ";

        for (char c : guessedWord)
            cout << c << " ";

        cout << "\n";
    }


    // --------------------------------------------------------
    // CHECK DUPLICATE LETTER
    // --------------------------------------------------------

    bool alreadyGuessed(char letter) const {

        return find(
            guessedLetters.begin(),
            guessedLetters.end(),
            letter
        ) != guessedLetters.end();
    }


    // --------------------------------------------------------
    // GUESS LETTER
    // --------------------------------------------------------

    void guessLetter(char letter) {

        letter = static_cast<char>(
            tolower(
                static_cast<unsigned char>(letter)
            )
        );

        if (alreadyGuessed(letter)) {

            cout << "You already guessed '"
                 << letter
                 << "'. Try another letter.\n";

            return;
        }

        guessedLetters.push_back(letter);

        bool correct = false;

        for (size_t i = 0;
             i < secretWord.length();
             i++) {

            if (secretWord[i] == letter) {

                guessedWord[i] = letter;
                correct = true;
            }
        }

        if (correct) {
            cout << "Correct guess!\n";
        }
        else {
            cout << "Wrong guess!\n";
            attempts++;
        }
    }


    // --------------------------------------------------------
    // WIN / LOSS
    // --------------------------------------------------------

    bool won() const {
        return guessedWord == secretWord;
    }

    bool lost() const {
        return attempts >= maxAttempts;
    }


    // --------------------------------------------------------
    // SCORE CALCULATION
    // --------------------------------------------------------

    int calculateScore(int remainingTime) const {

        int baseScore;

        if (difficulty == 1)
            baseScore = 100;
        else if (difficulty == 2)
            baseScore = 200;
        else
            baseScore = 300;

        int attemptBonus =
            (maxAttempts - attempts) * 25;

        int timeBonus =
            remainingTime / 5;

        return baseScore +
               attemptBonus +
               timeBonus;
    }


    // --------------------------------------------------------
    // PLAY ROUND
    // --------------------------------------------------------

    void playRound() {

        setupRound();

        auto start =
            chrono::steady_clock::now();

        while (!won() && !lost()) {

            auto now =
                chrono::steady_clock::now();

            int elapsed =
                static_cast<int>(
                    chrono::duration_cast<
                        chrono::seconds
                    >(now - start).count()
                );

            int remainingTime =
                timeLimit - elapsed;

            if (remainingTime <= 0) {

                cout << "\nTime is up!\n";
                break;
            }

            displayHangman();

            cout << "\nCategory: "
                 << wordManager.categoryName(category)
                 << endl;

            cout << "Difficulty: "
                 << wordManager.difficultyName(difficulty)
                 << endl;

            displayWord();

            cout << "Time remaining: "
                 << remainingTime
                 << " seconds\n";

            cout << "Guessed letters: ";

            for (char c : guessedLetters)
                cout << c << " ";

            cout << "\n";

            cout << "Enter a letter: ";

            char letter;
            cin >> letter;

            if (!isalpha(
                    static_cast<unsigned char>(letter))) {

                cout << "Please enter a letter only.\n";
                continue;
            }

            guessLetter(letter);
        }


        // ----------------------------------------------------
        // RESULT
        // ----------------------------------------------------

        auto end =
            chrono::steady_clock::now();

        int elapsed =
            static_cast<int>(
                chrono::duration_cast<
                    chrono::seconds
                >(end - start).count()
            );

        int remainingTime =
            max(0, timeLimit - elapsed);


        cout << "\n====================================\n";

        if (won()) {

            int score =
                calculateScore(remainingTime);

            player.addScore(score);

            cout << "             YOU WON!\n";
            cout << "====================================\n";

            cout << "The word was: "
                 << secretWord << endl;

            cout << "Round score: "
                 << score << endl;

        }
        else {

            cout << "            GAME OVER!\n";
            cout << "====================================\n";

            cout << "The word was: "
                 << secretWord << endl;
        }

        cout << "Your score: "
             << player.getScore()
             << endl;

        cout << "====================================\n";
    }


    // --------------------------------------------------------
    // PLAY AGAIN
    // --------------------------------------------------------

    bool playAgain() const {

        char choice;

        cout << "\nWould you like to play again? (y/n): ";
        cin >> choice;

        return tolower(
            static_cast<unsigned char>(choice)
        ) == 'y';
    }


    // --------------------------------------------------------
    // START GAME
    // --------------------------------------------------------

    void startGame() {

        do {

            chooseCategory();
            chooseDifficulty();
            playRound();

        } while (playAgain());

        scoreManager.saveScore(player);

        cout << "\nYour final score is: "
             << player.getScore()
             << endl;

        cout << "Thank you for playing, "
             << player.getName()
             << "!\n";
    }


    // --------------------------------------------------------
    // MAIN MENU
    // --------------------------------------------------------

    void menu() {

        int choice;

        while (true) {

            cout << "\n====================================\n";
            cout << "          HANGMAN MAIN MENU\n";
            cout << "====================================\n";
            cout << "1. Start Game\n";
            cout << "2. View High Scores\n";
            cout << "3. Exit Game\n";
            cout << "====================================\n";
            cout << "Enter your choice: ";

            cin >> choice;

            if (cin.fail()) {

                cin.clear();
                cin.ignore(1000, '\n');

                cout << "Invalid input.\n";
                continue;
            }

            if (choice == 1) {
                startGame();
            }
            else if (choice == 2) {
                scoreManager.displayHighScores();
            }
            else if (choice == 3) {

                cout << "\nThank you for playing!\n";
                break;

            }
            else {

                cout << "Invalid choice. "
                     << "Please select 1, 2, or 3.\n";
            }
        }
    }
};


// ============================================================
// MAIN FUNCTION
// ============================================================

int main() {

    string playerName;

    cout << "====================================\n";
    cout << "       WELCOME TO HANGMAN GAME\n";
    cout << "====================================\n";

    cout << "Enter your name: ";
    getline(cin, playerName);

    if (playerName.empty())
        playerName = "Player";

    Game game(playerName);

    game.menu();

    return 0;
}
