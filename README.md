# Time & Tide

Time & Tide is a small C++ game I made as a first game development project.

The idea is simple: **time works like an ocean**, and you are a courier who can travel across it and deliver messages to people in different time periods.

The choices you make in the past can change what happens in the future.

## What the game has

* Simple C++ code
* A short story
* Dialogue choices
* Different time periods
* Obstacle-course style movement
* Points and scoring
* Different endings
* Choices that change the future
* A simple coloured terminal style inspired by old handheld RPG games

## How it works

You travel through time and deliver letters.

Sometimes you can:

* Deliver the letter normally
* Read it
* Change it
* Destroy it
* Give it to someone else

Your choices affect the future.

The game keeps track of:

* Prosperity
* Freedom
* Stability
* Timeline Integrity
* Route Points

There is not always one correct choice. A decision can improve one thing while making something else worse.

## Controls

* `W` - Up
* `A` - Left
* `S` - Down
* `D` - Right

Other choices are made using the number keys when the game asks you.

## Build

You need a C++17 compiler.

Using g++:

```bash
g++ -std=c++17 -O2 time_and_tide.cpp -o time_and_tide.exe
```

## Run

On Windows:

```bash
.\time_and_tide.exe
```

## Why I made this game simple

I wanted to make a small game where the main challenge is not fighting enemies.

Instead, the player has to think about:

* What information they should deliver
* Who they should trust
* Whether changing history is worth it
* What kind of future their choices create

I also wanted to use this project to practise:

* C++
* Game logic
* Functions
* Player input
* Branching choices
* Keeping track of game state
* Git and GitHub

  note that this is a primary version

## Current version

This is an early version of the project.

I plan to keep improving it while learning more about C++ and game development.
