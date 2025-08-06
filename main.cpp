#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

// Function to generate a random secret code
string generateSecretCode(int codeLength, int numColors) {
    string code(codeLength, '0');
    for (int i = 0; i < codeLength; ++i) {
        code[i] = '0' + (rand() % numColors + 1); // Colors numbered 1 to numColors
    }
    return code;
}

// Function to get player's guess
string getPlayerGuess(int codeLength, int numColors) {
    string guess(codeLength, '0');
    cout << "Enter your guess (" << codeLength << " numbers, each from 1 to " << numColors << "): ";
    for (int i = 0; i < codeLength; ++i) {
        cin >> guess[i];
        while (guess[i] < '1' || guess[i] > '0' + numColors) {
            cout << "Invalid color. Enter a number from 1 to " << numColors << ": ";
            cin >> guess[i];
        }
    }
    return guess;
}

// Function to evaluate guess against secret code
pair<int, int> evaluateGuess(const string& secret, const string& guess) {
    int correctPosition = 0; // Correct color in correct position
    int correctColor = 0;    // Correct color in wrong position

    string secretUsed(secret.size(), false);
    string guessUsed(guess.size(), false);

    // Count correct colors in correct positions
    for (size_t i = 0; i < secret.size(); ++i) {
        if (guess[i] == secret[i]) {
            correctPosition++;
            secretUsed[i] = true;
            guessUsed[i] = true;
        }
    }

    // Count correct colors in wrong positions
    for (size_t i = 0; i < guess.size(); ++i) {
        if (!guessUsed[i]) {
            for (size_t j = 0; j < secret.size(); ++j) {
                if (!secretUsed[j] && guess[i] == secret[j]) {
                    correctColor++;
                    secretUsed[j] = true;
                    break;
                }
            }
        }
    }

    return {correctPosition, correctColor};
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    const int codeLength = 4;  // Length of the code
    const int numColors = 6;   // Number of possible colors
    const int maxGuesses = 10; // Maximum number of guesses

    cout << "Welcome to Mastermind!\n";
    cout << "Try to guess the secret code of " << codeLength << " colors.\n";
    cout << "Colors are numbers from 1 to " << numColors << ".\n";
    cout << "You have " << maxGuesses << " guesses.\n\n";
    cout << "cheat mode open? " << "\n\n" ;
    char cheat = 'N' ;
    cin >> cheat ;

    string secretCode = generateSecretCode(codeLength, numColors); 
    if (cheat == 'Y'){
       cout << secretCode <<"\n\n";
    }

    for (int guessNum = 1; guessNum <= maxGuesses; ++guessNum) {
        cout << "Guess #" << guessNum << ": ";
        string guess = getPlayerGuess(codeLength, numColors);
        auto [correctPos, correctCol] = evaluateGuess(secretCode, guess);

        cout << "Correct positions: " << correctPos << "\n";
        cout << "Correct colors (wrong position): " << correctCol << "\n\n";

        if (correctPos == codeLength) {
            cout << "Congratulations! You cracked the code in " << guessNum << " guesses!\n";
            char end = 'N';
            cout << "play again? (Y/N)" ;
            cin >> end ;
            if(end == 'N'){
                return 0 ;
            }
        }
    }

    cout << "Game Over! You ran out of guesses.\n";
    cout << "The secret code was: ";
    for (char color : secretCode) {
        cout << color << " ";
    }
    cout << "\n";

    return 0;
}