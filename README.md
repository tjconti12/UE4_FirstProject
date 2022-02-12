# FirstProject

Developed with Unreal Engine 4.27

Third Person melee combat game. The game is set in a medieval temple that has been infested with zombies. The lone character, Erika Archer [(courtesy of Mixamo)](https://www.mixamo.com/#/), must fend off these creatures. The game requires both skill and strategy in order to survive as many rounds as possible.

![Gameplay1](./ScreenShots/Gameplay4.png)

## Motivation

I have always been interested in working with UE4. This game is a way for me to practice working with the engine, as well as develop my C++ language skills. Being a Software Engineer, there is a heavy focus on creating C++ classes and functionality. Most of the game logic is held in the `Source/FirstProject` directory. Before tackling this project, my knowledge of the C++ language was very minimal. While challenging at times, I am very proud of the work and determination that went into this project. 

## Build Status

Currently at an Alpha release. All game play mechanics and characters have been created. The game is currently fully functional without the presence of any known bugs.

## Tech/Framwork Used

This project is developed using Unreal Engine 4.27. All classes have been created using C++, with Blueprints derived from the C++ classes. Assets were obtained from both the [Unreal Engine Marketplace](https://www.unrealengine.com/marketplace/en-US/store), and Adobe's [Mixamo](https://www.mixamo.com/#/).

## Features

The game features a round count, which will increment after all the zombies for that round have been destroyed. There are 4 random spawn points located throughout the level. The first two are in the player start area, while the other two are behind a closed door. Once the player reaches 500 coins, they have to option of opening the door. Once that door is opened, all 4 spawn points are used. The 4 spawn points are chosen at random throughout the round.

There are also multiple weapons for pickup. There is a starting sword, which can be equipped for free. As the player earns coins, they have the option of purchasing more powerful weapons. There is also a randomly spawned pickup between rounds, with a 1/4 chance of getting a health potion vs a coin.

## Gameplay

![Gameplay1](./ScreenShots/Gameplay3.png)
![Gameplay1](./ScreenShots/Gameplay2.png)
![Gameplay1](./ScreenShots/Gameplay1.png)
