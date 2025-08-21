#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

string generateSecretCode(int length) {
    string code;
    for (int i = 0; i < length; ++i) {
        char digit = '0' + rand() % 10; // Generate a random digit from '0' to '9'
        code += digit;
    }
    return code;
}

string getPlayerGuess(int length) {
    string guess;
    cout << "Enter your guess (" << length << " digits): ";
    cin >> guess;
    while (guess.length() != length || !all_of(guess.begin(), guess.end(), ::isdigit)) {
        cout << "Invalid input. Please enter a " << length << "-digit number: ";
        cin >> guess;
    }
    return guess;
}

string checkposition(string secretCode, string playerGuess) {
    int correctPosition = 0;
    for (int i = 0; i < secretCode.length(); ++i) {
        if (secretCode[i] == playerGuess[i]) {
            correctPosition++;
        }
    }
    string output = "Number of Correct positions: " + to_string(correctPosition);
    return output;
}

string checknumber(string secretCode, string playerGuess) {
    int correctNumber = 0;
    for ( int i = 0 ; i < playerGuess.length(); ++i) {
        for ( int j = 0 ; j < secretCode.length() ; ++j ) {
            if (playerGuess[i] == secretCode[j]) {
                correctNumber++;
                break;
            }
        }
    }
    string output = "Number of Correct numbers: " + to_string(correctNumber);
    return output;  
}

int main(){
    cout << "Welcome to the Number Guessing Game!" << endl;
    srand(static_cast<unsigned int>(time(0))); // Seed the random number generator
    int codeLength = 4;
    cout << "Enter the length of the secret code (default is 4): "; 
    cin >> codeLength;
    string secretCode = generateSecretCode(codeLength);
    cout << "Try to guess the " << codeLength << "-digit secret code." << endl;
    cout << "cheat mode open?(Y/N): ";
    char cheatMode = 'N';
    cin >> cheatMode;
    if (cheatMode == 'Y' || cheatMode == 'y') {
        cout << "The secret code is: " << secretCode << endl;
    } 

    while (true) {
        string playerGuess = getPlayerGuess(codeLength);
        cout << checkposition(secretCode, playerGuess) << endl;
        cout << checknumber(secretCode, playerGuess) << endl;

        if (playerGuess == secretCode) {
            cout << "Congratulations! You've guessed the secret code: " << secretCode << endl;
            break;
        }
    }

    cout << "Thank you for playing!" << endl;
    cout << "try again? (Y/N): ";
    char tryAgain;  
    cin >> tryAgain;
    if (tryAgain == 'Y' || tryAgain == 'y') {
        main(); // Restart the game
    } else {
        cout << "Goodbye!" << endl;
        return 0;
    }

    return 0;
}