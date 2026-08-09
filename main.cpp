#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <random>
#include <chrono>
#include <cctype>
#include <iomanip>

using namespace std;

// ============================================================
// PLAYER CLASS
// ============================================================

class Player {
private:
    string name;
    int score;

public:
    Player() {
        name = "Player";
        score = 0;
    }

    Player(string name) {
        this->name = name;
        score = 0;
    }

    // Getters
    string getName() const {
        return name;
    }

    int getScore() const {
        return score;
    }

    // Add score
    void addScore(int points) {
        score += points;
    }

    // Reset score
    void resetScore() {
        score = 0;
    }
};


// ============================================================
// WORD MANAGER CLASS
// ============================================================

class WordManager {
private:
    vector<string> sports;
    vector<string> cities;
    vector<string> countries;
    vector<string> personalities;

public:

    WordManager() {

        // Sports
        sports = {
            "football",
            "basketball",
            "cricket",
            "tennis",
            "volleyball",
            "badminton",
            "baseball",
            "swimming",
            "boxing",
            "wrestling"
        };

        // Cities
        cities = {
            "kathmandu",
            "london",
            "paris",
            "tokyo",
            "sydney",
            "delhi",
            "seoul",
            "dubai",
            "berlin",
            "singapore"
        };

        // Countries
        countries = {
            "nepal",
            "india",
            "japan",
            "canada",
            "brazil",
            "australia",
            "germany",
            "france",
            "italy",
            "mexico"
        };

        // Personalities
        personalities = {
            "einstein",
            "shakespeare",
            "napoleon",
            "gandhi",
            "lincoln",
            "newton",
            "mozart",
            "plato",
            "aristotle",
            "socrates"
        };
    }


    // --------------------------------------------------------
    // GET RANDOM WORD
    // --------------------------------------------------------

    string getRandomWord(int category) const {

        const vector<string>* selectedCategory;

        switch (category) {

            case 1:
                selectedCategory = &sports;
                break;

            case 2:
                selectedCategory = &cities;
                break;

            case 3:
                selectedCategory = &countries;
                break;

            case 4:
                selectedCategory = &personalities;
                break;

            default:
                selectedCategory = &sports;
        }

        // Random number generator
        random_device rd;
        mt19937 generator(rd());

        uniform_int_distribution<int> distribution(
            0,
            static_cast<int>(selectedCategory->size()) - 1
        );

        return (*selectedCategory)[distribution(generator)];
    }


    // --------------------------------------------------------
    // GET CATEGORY NAME
    // --------------------------------------------------------

    string getCategoryName(int category) const {

        switch (category) {

            case 1:
                return "Sports";

            case 2:
                return "Cities";

            case 3:
                return "Countries";

            case 4:
                return "Personalities";

            default:
                return "Unknown";
        }
    }
};


// ============================================================
// SCORE MANAGER CLASS
// ============================================================

class ScoreManager {
private:

    struct HighScore {
        string playerName;
        int score;
    };

    vector<HighScore> highScores;

    const string FILE_NAME = "highscores.txt";

public:

    // --------------------------------------------------------
    // LOAD HIGH SCORES
    // --------------------------------------------------------

    void loadScores() {

        highScores.clear();

        ifstream file(FILE_NAME);

        if (!file) {
            return;
        }

        string name;
        int score;

        while (file >> name >> score) {
            highScores.push_back({name, score});
        }

        file.close();

        sort(
            highScores.begin(),
            highScores.end(),
            [](const HighScore& a, const HighScore& b) {
                return a.score > b.score;
            }
        );
    }


    // --------------------------------------------------------
    // SAVE SCORE
    // --------------------------------------------------------

    void saveScore(const Player& player) {

        highScores.push_back({
            player.getName(),
            player.getScore()
        });

        sort(
            highScores.begin(),
            highScores.end(),
            [](const HighScore& a, const HighScore& b) {
                return a.score > b.score;
            }
        );

        // Keep only top 10
        if (highScores.size() > 10) {
            highScores.resize(10);
        }

        ofstream file(FILE_NAME);

        if (!file) {
            cout << "Error: Could not save high scores.\n";
            return;
        }

        for (const HighScore& entry : highScores) {
            file << entry.playerName << " "
                 << entry.score << "\n";
        }

        file.close();
    }


    // --------------------------------------------------------
    // DISPLAY HIGH SCORES
    // --------------------------------------------------------

    void displayHighScores() {

        loadScores();

        cout << "\n";
        cout << "============================================\n";
        cout << "              HIGH SCORES\n";
        cout << "============================================\n";

        if (highScores.empty()) {
            cout << "No high scores available yet.\n";
            return;
        }

        cout << left
             << setw(8) << "Rank"
             << setw(20) << "Player"
             << "Score\n";

        cout << "--------------------------------------------\n";

        for (size_t i = 0; i < highScores.size(); i++) {

            cout << left
                 << setw(8) << i + 1
                 << setw(20) << highScores[i].playerName
                 << highScores[i].score
                 << "\n";
        }

        cout << "============================================\n";
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

    int attempts;
    int maxAttempts;
    int difficulty;

    int timeLimit;

public:

    Game(string playerName)
        : player(playerName) {

        attempts = 0;
        maxAttempts = 6;
        difficulty = 1;
        timeLimit = 60;
    }


    // --------------------------------------------------------
    // DISPLAY TITLE
    // --------------------------------------------------------

    void displayTitle() const {

        cout << "\n";
        cout << "============================================\n";
        cout << "               HANGMAN GAME\n";
        cout << "============================================\n";
    }


    // --------------------------------------------------------
    // SELECT CATEGORY
    // --------------------------------------------------------

    int selectCategory() const {

        int choice;

        cout << "\n";
        cout << "========== SELECT CATEGORY ==========\n";
        cout << "1. Sports\n";
        cout << "2. Cities\n";
        cout << "3. Countries\n";
        cout << "4. Personalities\n";
        cout << "======================================\n";

        while (true) {

            cout << "Enter your choice: ";
            cin >> choice;

            if (choice >= 1 && choice <= 4) {
                return choice;
            }

            cout << "Invalid choice. Please select 1-4.\n";
        }
    }


    // --------------------------------------------------------
    // SELECT DIFFICULTY
    // --------------------------------------------------------

    void selectDifficulty() {

        int choice;

        cout << "\n";
        cout << "========== SELECT DIFFICULTY ==========\n";
        cout << "1. Easy\n";
        cout << "2. Medium\n";
        cout << "3. Hard\n";
        cout << "=======================================\n";

        while (true) {

            cout << "Enter your choice: ";
            cin >> choice;

            if (choice >= 1 && choice <= 3) {
                difficulty = choice;
                break;
            }

            cout << "Invalid choice. Please select 1-3.\n";
        }

        // Configure difficulty
        switch (difficulty) {

            case 1:
                maxAttempts = 8;
                timeLimit = 90;
                break;

            case 2:
                maxAttempts = 6;
                timeLimit = 60;
                break;

            case 3:
                maxAttempts = 4;
                timeLimit = 45;
                break;
        }
    }


    // --------------------------------------------------------
    // INITIALIZE GAME
    // --------------------------------------------------------

    void initializeGame(int category) {

        secretWord = wordManager.getRandomWord(category);

        guessedWord = string(
            secretWord.length(),
            '_'
        );

        guessedLetters.clear();

        attempts = 0;
    }


    // --------------------------------------------------------
    // DISPLAY HANGMAN
    // --------------------------------------------------------

    void displayHangman() const {

        cout << "\n";

        switch (attempts) {

            case 0:

                cout << "  +---+\n";
                cout << "  |   |\n";
                cout << "      |\n";
                cout << "      |\n";
                cout << "      |\n";
                cout << "=========\n";

                break;

            case 1:

                cout << "  +---+\n";
                cout << "  |   |\n";
                cout << "  O   |\n";
                cout << "      |\n";
                cout << "      |\n";
                cout << "=========\n";

                break;

            case 2:

                cout << "  +---+\n";
                cout << "  |   |\n";
                cout << "  O   |\n";
                cout << "  |   |\n";
                cout << "      |\n";
                cout << "=========\n";

                break;

            case 3:

                cout << "  +---+\n";
                cout << "  |   |\n";
                cout << "  O   |\n";
                cout << " /|   |\n";
                cout << "      |\n";
                cout << "=========\n";

                break;

            case 4:

                cout << "  +---+\n";
                cout << "  |   |\n";
                cout << "  O   |\n";
                cout << " /|\\  |\n";
                cout << "      |\n";
                cout << "=========\n";

                break;

            case 5:

                cout << "  +---+\n";
                cout << "  |   |\n";
                cout << "  O   |\n";
                cout << " /|\\  |\n";
                cout << " /    |\n";
                cout << "=========\n";

                break;

            default:

                cout << "  +---+\n";
                cout << "  |   |\n";
                cout << "  O   |\n";
                cout << " /|\\  |\n";
                cout << " / \\  |\n";
                cout << "=========\n";

                break;
        }
    }


    // --------------------------------------------------------
    // DISPLAY GAME INFORMATION
    // --------------------------------------------------------

    void displayGameInfo() const {

        cout << "\n";
        cout << "Word: ";

        for (char letter : guessedWord) {
            cout << letter << " ";
        }

        cout << "\n";

        cout << "Attempts remaining: "
             << maxAttempts - attempts
             << "\n";

        cout << "Score: "
             << player.getScore()
             << "\n";

        cout << "Guessed letters: ";

        if (guessedLetters.empty()) {
            cout << "None";
        }
        else {
            for (char letter : guessedLetters) {
                cout << letter << " ";
            }
        }

        cout << "\n";
    }


    // --------------------------------------------------------
    // CHECK IF LETTER WAS ALREADY GUESSED
    // --------------------------------------------------------

    bool alreadyGuessed(char letter) const {

        return find(
            guessedLetters.begin(),
            guessedLetters.end(),
            letter
        ) != guessedLetters.end();
    }


    // --------------------------------------------------------
    // PROCESS LETTER GUESS
    // --------------------------------------------------------

    bool processGuess(char letter) {

        // Check repeated guess
        if (alreadyGuessed(letter)) {

            cout << "You already guessed '"
                 << letter
                 << "'. Try another letter.\n";

            return false;
        }

        guessedLetters.push_back(letter);

        bool correct = false;

        for (size_t i = 0; i < secretWord.length(); i++) {

            if (secretWord[i] == letter) {

                guessedWord[i] = letter;
                correct = true;
            }
        }

        if (correct) {

            cout << "Correct guess!\n";

            return true;
        }

        attempts++;

        cout << "Incorrect guess!\n";

        return false;
    }


    // --------------------------------------------------------
    // CHECK WIN
    // --------------------------------------------------------

    bool hasWon() const {

        return guessedWord == secretWord;
    }


    // --------------------------------------------------------
    // CHECK LOSS
    // --------------------------------------------------------

    bool hasLost() const {

        return attempts >= maxAttempts;
    }


    // --------------------------------------------------------
    // CALCULATE SCORE
    // --------------------------------------------------------

    int calculateRoundScore() const {

        int baseScore;

        switch (difficulty) {

            case 1:
                baseScore = 100;
                break;

            case 2:
                baseScore = 200;
                break;

            case 3:
                baseScore = 300;
                break;

            default:
                baseScore = 100;
        }

        int remainingAttempts =
            maxAttempts - attempts;

        int bonus =
            remainingAttempts * 20;

        return baseScore + bonus;
    }


    // --------------------------------------------------------
    // PLAY ONE ROUND
    // --------------------------------------------------------

    bool playRound() {

        int category = selectCategory();

        selectDifficulty();

        initializeGame(category);

        cout << "\n";
        cout << "Category: "
             << wordManager.getCategoryName(category)
             << "\n";

        cout << "Difficulty: ";

        if (difficulty == 1)
            cout << "Easy\n";
        else if (difficulty == 2)
            cout << "Medium\n";
        else
            cout << "Hard\n";

        cout << "Time Limit: "
             << timeLimit
             << " seconds\n";

        auto startTime =
            chrono::steady_clock::now();

        while (!hasWon() && !hasLost()) {

            auto currentTime =
                chrono::steady_clock::now();

            auto elapsed =
                chrono::duration_cast<
                    chrono::seconds
                >(currentTime - startTime).count();

            if (elapsed >= timeLimit) {

                cout << "\n";
                cout << "TIME'S UP!\n";
                cout << "The word was: "
                     << secretWord
                     << "\n";

                return false;
            }

            displayHangman();
            displayGameInfo();

            char letter;

            cout << "\nEnter a letter: ";
            cin >> letter;

            letter =
                static_cast<char>(
                    tolower(
                        static_cast<unsigned char>(letter)
                    )
                );

            if (!isalpha(
                    static_cast<unsigned char>(letter)
                )) {

                cout << "Please enter a valid alphabet letter.\n";
                continue;
            }

            processGuess(letter);
        }


        // ----------------------------------------------------
        // WIN
        // ----------------------------------------------------

        if (hasWon()) {

            int roundScore =
                calculateRoundScore();

            player.addScore(roundScore);

            displayHangman();

            cout << "\n";
            cout << "============================================\n";
            cout << "              YOU WON!\n";
            cout << "============================================\n";

            cout << "The word was: "
                 << secretWord
                 << "\n";

            cout << "Round Score: "
                 << roundScore
                 << "\n";

            cout << "Total Score: "
                 << player.getScore()
                 << "\n";

            cout << "============================================\n";

            return true;
        }


        // ----------------------------------------------------
        // LOSS
        // ----------------------------------------------------

        if (hasLost()) {

            displayHangman();

            cout << "\n";
            cout << "============================================\n";
            cout << "              GAME OVER!\n";
            cout << "============================================\n";

            cout << "The correct word was: "
                 << secretWord
                 << "\n";

            cout << "Your score: "
                 << player.getScore()
                 << "\n";

            cout << "============================================\n";

            return false;
        }

        return false;
    }


    // --------------------------------------------------------
    // START GAME
    // --------------------------------------------------------

    void startGame() {

        player.resetScore();

        bool playAgain = true;

        while (playAgain) {

            playRound();

            char choice;

            cout << "\n";
            cout << "Play another round? (y/n): ";
            cin >> choice;

            choice =
                static_cast<char>(
                    tolower(
                        static_cast<unsigned char>(choice)
                    )
                );

            if (choice != 'y') {
                playAgain = false;
            }
        }

        // Save final score
        scoreManager.saveScore(player);

        cout << "\n";
        cout << "Final Score for "
             << player.getName()
             << ": "
             << player.getScore()
             << "\n";
    }


    // --------------------------------------------------------
    // MAIN MENU
    // --------------------------------------------------------

    void menu() {

        int choice;

        do {

            displayTitle();

            cout << "Welcome, "
                 << player.getName()
                 << "!\n\n";

            cout << "1. Start Game\n";
            cout << "2. View High Scores\n";
            cout << "0. Exit\n";

            cout << "\nEnter your choice: ";
            cin >> choice;

            switch (choice) {

                case 1:
                    startGame();
                    break;

                case 2:
                    scoreManager.displayHighScores();
                    break;

                case 0:

                    cout << "\n";
                    cout << "Thank you for playing Hangman!\n";

                    break;

                default:

                    cout << "Invalid choice. Please try again.\n";
            }

        } while (choice != 0);
    }
};


// ============================================================
// MAIN FUNCTION
// ============================================================

int main() {

    string playerName;

    cout << "============================================\n";
    cout << "           WELCOME TO HANGMAN\n";
    cout << "============================================\n";

    cout << "Enter your name: ";
    getline(cin, playerName);

    if (playerName.empty()) {
        playerName = "Player";
    }

    // Create Game object
    Game game(playerName);

    // Start game menu
    game.menu();

    return 0;
}
