#include <iostream>
#include <string>
#include <ctime>
#include <algorithm>

using namespace std;

string generateSecretCode(int length, int range = 10) {
    string code;
    for (int i = 0; i < length; ++i) {
        char digit = '0' + rand() % range + 1; 
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

void getdifficulty( string category, int& defaultValue) {
    cout << "Enter the value of " << category << " (default is " << defaultValue << ") : " << endl;
    cin >> defaultValue;
    if (category == "range"){
        while (defaultValue < 1 || defaultValue > 10) { 
            cout << "Invalid range. Must be between 1 to 10." << endl;
            cin >> defaultValue;
        }
    }
}

void cheatmode(string secretCode) {
    cout << "cheat mode open?(Y/N): ";
    char cheatMode = 'N';
    cin >> cheatMode;
    while (cheatMode != 'Y' && cheatMode != 'y' && cheatMode != 'N' && cheatMode != 'n') {
        cout << "Invalid input. Please enter Y or N: ";
        cin >> cheatMode;
    }
    if (cheatMode == 'Y' || cheatMode == 'y') {
        cout << "The secret code is: " << secretCode << endl;
    } else if (cheatMode == 'N' || cheatMode == 'n') {
        cout << "Cheat mode is off." << endl;
    }
    cout << endl ;
}

int main(){
    cout << "Welcome to the Number Guessing Game!" << endl;
    srand(static_cast<unsigned int>(time(0))); // Seed the random number generator

    int codeLength = 4;
    int range = 10; 
    int maxAttempts = 10; 
    getdifficulty("code length", codeLength);
    getdifficulty("range", range);      
    getdifficulty("max attempts", maxAttempts);
    cout << endl ;

    string secretCode = generateSecretCode(codeLength,range);

    cheatmode(secretCode);

    int attempts = 0;
    cout << "Secret code has been generated. Try to guess it!" << endl;
    cout << "Code length: " << codeLength << ", Range: 1-" << range  << ", Max attempts: " << maxAttempts << endl;
    cout << endl ;
    
    

    while (true) {
        string playerGuess = getPlayerGuess(codeLength);
        cout << checkposition(secretCode, playerGuess) << endl;
        cout << checknumber(secretCode, playerGuess) << endl;
        attempts++;
        cout << "Attempts used: " << attempts << "/" << maxAttempts << endl;
        cout << endl ;

        if (attempts >= 10) {
            cout << "You've used all attempts! The secret code was: " << secretCode << endl;
            break;
        }

        if (playerGuess == secretCode) {
            cout << "Congratulations! You've guessed the secret code: " << secretCode << endl;
            break;
        }
    }

    cout << "Thank you for playing!" << endl;

    cout << "try again? (Y/N): ";
    char tryAgain;  
    cin >> tryAgain;
    while (tryAgain != 'Y' && tryAgain != 'y' && tryAgain != 'N' && tryAgain != 'n') {
        cout << "Invalid input. Please enter Y or N: ";
        cin >> tryAgain;
    }
    if (tryAgain == 'Y' || tryAgain == 'y') {
        main(); // Restart the game
    } else {
        cout << "Goodbye!" << endl;
        return 0;
    }

    return 0;
}