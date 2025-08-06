# Mastermind

A simple command-line implementation of the Mastermind game in C++.

## How to Play

- The computer generates a secret code consisting of 4 numbers (colors), each between 1 and 6.
- You have 10 guesses to crack the code.
- After each guess, you’ll be told:
  - How many numbers are correct and in the correct position.
  - How many numbers are correct but in the wrong position.
- Optionally, you can enable "cheat mode" to reveal the secret code at the start.

## Building

Make sure you have `g++` installed.  
To build the project, use the provided VS Code task or run:

```sh
g++ -g main.cpp -o main.exe
```

## Running

Run the executable:

```sh
./main.exe
```

## Files

- [`main.cpp`](main.cpp): Main source code.
- [`.vscode/tasks.json`](.vscode/tasks.json): VS Code build task configuration.
- [`.vscode/settings.json`](.vscode/settings.json): VS Code settings.

## License

MIT License (add your own
