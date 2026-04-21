# Shopping List Manager (C++)

A simple console-based shopping list manager built in C++ for learning purposes.

## Features

- Add items with name and category
- Display the full list grouped by category
- Display only needed (not purchased) items
- Mark items as purchased
- Remove items by name
- Save the current list to `shopping_list.txt`
- Load the list from `shopping_list.txt` at startup
- Validate menu input and confirmation prompts

## Data Format

Data is stored in `shopping_list.txt` using one item per line:

`name,category,purchased`

Where:

- `purchased = 0` means needed
- `purchased = 1` means purchased

## Project Files

- `main.cpp` - application logic (menu, linked list operations, file I/O)
- `shopping_list.txt` - persisted shopping list data

## Build

Compile with `g++`:

```bash
g++ -std=c++17 -Wall -Wextra -o main main.cpp
```

## Run

On Windows:

```bash
./main.exe
```

On Linux/macOS:

```bash
./main
```

## Menu

1. Add Item
2. Display List
3. Display Needed Items Only
4. Mark Item as Purchased
5. Remove Item
6. Save List
0. Exit

## Learning Topics Practiced

- structs and pointers
- dynamic memory (`new` / `delete`)
- linked list insertion and traversal
- file input/output with streams
- string parsing and validation
- defensive console input handling
