# 🎮 Ludo Game - Red vs Green (C++ with Raylib)

Welcome to **Ludo - Red vs Green**, a 2-player board game coded in C++ using the **Raylib** graphics library! This game brings a classic Ludo-style experience with smooth animations and simple controls. 👾

## 🧩 Features

- 🟥 **Red vs Green**: Two-player token movement with individual home paths
- 🎲 **Dice Mechanics**: Roll the dice with `SPACEBAR`
- 🎯 **Token Logic**: Activate on a 6️⃣ and move based on dice value
- 📦 **Smooth Animation**: Token moves tile-by-tile with tweening
- 🧠 **Auto Selection**: Automatically selects the movable token after a roll

## 🕹️ Controls

- Press **`SPACE`** to roll the dice 🎲
- Tokens move automatically based on the roll
- The game alternates turns between Red 🔴 and Green 🟢

## 🧱 Project Structure

## 🛠️ Setup Instructions

1. **Install Raylib** (https://github.com/raysan5/raylib#installation)
2. Clone this repo or use the `main.cpp` file
3. Place your `ludo.jpg` and `dice.png` inside a `resource` folder
4. Compile using g++ or any C++ compiler with Raylib linked:

```bash
g++ main.cpp -o ludo -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./ludo
```
