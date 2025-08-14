# Crossword Generator

A C++ console application that generates random crossword puzzles, displays hints, and allows users to fill in the crossword interactively. The game tracks user performance, including time taken and number of attempts, and maintains a scoreboard.

---

## Features

- **Random Crossword Generation:** Each game session generates a new crossword from a large word and hint bank.
- **Interactive Gameplay:** Users fill in words based on hints, with colored terminal output for a better experience.
- **Scoreboard:** Tracks user name, time taken, and number of attempts for each game.
- **Hints System:** Displays hints for each word in the crossword.
- **Huffman Compression Utility:** Includes a tool to compress and decompress crossword data files.

---

## File Structure

- `crossword.cpp` — Main crossword game source code.
- `huffman.cpp` — Huffman coding utility for compressing/decompressing crossword data.
- `crossword.txt` — Example input file for crossword words and hints.
- `input_compressed.txt` — Compressed crossword data (binary).
- `input_decompressed.txt` — Decompressed crossword data (text).
- `README.md` — Project documentation.

---

## How to Build

Make sure you have a C++ compiler (e.g., g++).

```sh
g++ crossword.cpp -o crossword
```

For the Huffman utility:

```sh
g++ huffman.cpp -o huffman
```

---

## How to Run

To play the crossword game:

```sh
./crossword
```

To compress and decompress crossword data using Huffman coding:

```sh
./huffman
```

---

## Requirements

- C++11 or later
- Linux terminal (for color output)

---

## License

This project is for educational purposes.
