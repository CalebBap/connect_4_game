# Connect Four Type Game in Command Prompt
This program implements a game similar to Connect Four. It can be played inside Command Prompt.

## How to Use
### Prerequisites
- You must have a C compiler, [such as GCC](https://gcc.gnu.org/), installed on your computer to compile the program.

### Installation
These installation steps will assume that you are using GCC to compile the program.

1. Clone or download the repository
2. In Command Prompt, navigate to the directory in which the repository has been cloned/downloaded
3. Enter the command `gcc -o connect4 connect4.c` to create an executable file called "connect4.exe"
4. Run the program either by entering the command `connect4.exe` in Command Prompt or by double-clicking on the connect4.exe file

### Usage
The rules of this game are similar to those of traditional Connect 4. However, in this version a token may be entered from any side of 
the board. Once entered into the board, it will continue to "fall" until it reaches the other side of the board, the center of the board 
or another token.

The size of the board may also be set by the user. Additionally, the game can be played in three modes: 
- Human vs. Human
- Human vs. Bot
- Bot vs. Bot 

When the game mode is Bot vs. Bot, it is possible to set how many games the bots play against each other.
