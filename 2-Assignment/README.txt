COMP 354 2019 - 8 MINUTE EMPIRE

By Shelby Wams #29489177

0. DESCRIPTION

8 Minute Empire is a area control game via the console. The current implementation creates some key classes
and tests them, but it does not include a full game loop yet. However, if you run the Cards driver, 
you will be able to interact with the game in a way that closely mimics what the game loop might be like.

1. SRC DIRECTORY STRUCTURE

-- src
    |-- Bidder.cpp
    |-- Bidder.h
    |-- Cards.cpp
    |-- Cards.h
    |-- Map.cpp
    |-- Map.h
    |-- MapLoader.cpp
    |-- MapLoader.h
    |-- Player.cpp
    |-- Player.h
    |
    |-- drivers
    |   |-- BidderDriver.cpp
    |   |-- CardsDriver.cpp
    |   |-- MapDriver.cpp
    |   |-- MapLoaderDriver.cpp
    |   `-- PlayerDriver.cpp
    |
    `-- util
        |-- MapUtil.cpp
        |-- MapUtil.h
        |-- TestUtil.cpp
        `-- TestUtil.h

2. RUNNING THE DRIVERS

    2.a. If on Mac or Linux:
        Open a terminal window and change directory into the root directory of the project.
        Execute `./run_driver <driver_name>`, where 'driver_name' is either Player, Map, MapLoader, Bidder or Cards.

    2.b If on Windows:
        Open up Visual Studio.
        Create a New Project from Existing Source Code.
        Check project configuration in Properties->Linker->System->SubSystem. It should be CONSOLE.
        Run the project in debug. Make sure to only include the driver class you want to run since each driver has
        its own main().

3. ABOUT THE DRIVERS

    3.a Map
        - Creates several example maps and tests their validity according to the assignment criteria.

    3.b MapLoader
        - Reads from several map files located in the maps/ directory to test the map loader functions.

    3.c Player
        - Demonstrates the manipulation by a Player object on the construction, destruction and movement
        of armies and/or cities on a map.
        - One section of the driver tests the bidder object and requires input from the user.

    3.d Bidder
        - Tests the bidding function between several players.
        - In order to test the different scenarios that may occur during bidding, run the
        Bidder driver several times and play with different inputs.

    3.c Cards
        - Part 1 tests that the Deck object contains 42 cards and part 2 tests the Hand object, 
        the Cards visible from the Hand object, and the different actions the player can take.
        - This driver is very interactive!