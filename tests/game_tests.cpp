#include <sstream>
#include <fstream>
#include <gtest/gtest.h>
#include <lib/Game.h>

std::string captureCoutOutput(const std::function<void()>& func) {
    std::ostringstream test_output;
    std::streambuf* original_cout = std::cout.rdbuf();
    std::cout.rdbuf(test_output.rdbuf());

    func();

    std::cout.rdbuf(original_cout);
    return test_output.str();
}


TEST(GameTestSuite, PingPongTest) {
    Game game;

    std::string output = captureCoutOutput([&]() {
        game.ExecuteCommand("ping");
    });

    game.ExecuteCommand("exit");
    EXPECT_EQ(output, "pong\n");
}


TEST(GameTestSuite, WidthAndHeightTest) {
    Game game;

    game.ExecuteCommand("set height 20");
    game.ExecuteCommand("set width 5");

    std::string output = captureCoutOutput([&]() {
        game.ExecuteCommand("get height");
        game.ExecuteCommand("get width");
    });

    game.ExecuteCommand("exit");
    EXPECT_EQ(output, "20\n5\n");
}


TEST(GameTestSuite, CountTest) {
    Game game;

    game.ExecuteCommand("set count 1 1");
    game.ExecuteCommand("set count 2 2");
    game.ExecuteCommand("set count 3 3");
    game.ExecuteCommand("set count 4 4");

    std::string output = captureCoutOutput([&]() {
        game.ExecuteCommand("get count 1");
        game.ExecuteCommand("get count 2");
        game.ExecuteCommand("get count 3");
        game.ExecuteCommand("get count 4");
    });

    game.ExecuteCommand("exit");
    EXPECT_EQ(output, "1\n2\n3\n4\n");
}


TEST(GameTestSuite, ShotOutputTest) {
    Game game;

    game.ExecuteCommand("create slave");
    game.ExecuteCommand("set strategy custom");
    game.ExecuteCommand("set height 20");
    game.ExecuteCommand("set width 5");

    game.ExecuteCommand("set count 3 2");
    game.ExecuteCommand("set count 1 2");
    game.ExecuteCommand("start");

    std::string output = captureCoutOutput([&]() {
        game.ExecuteCommand("shot 0 0");
        game.ExecuteCommand("shot 1 0");
        game.ExecuteCommand("shot 2 0");
    });

    game.ExecuteCommand("exit");
    EXPECT_EQ(output, "hit\nhit\nkill\n");
}



TEST(GameTestSuite, LoseTest) {
    Game game;

    game.ExecuteCommand("create slave");
    game.ExecuteCommand("set height 20");
    game.ExecuteCommand("set width 5");

    game.ExecuteCommand("set count 1 1");
    game.ExecuteCommand("start");
    game.ExecuteCommand("shot 0 0");

    game.ExecuteCommand("exit");
    EXPECT_EQ(game.isLose(), true);
}


TEST(GameTestSuite, KillTest) {
    Game game;

    game.ExecuteCommand("create slave");
    game.ExecuteCommand("set strategy ordered");
    game.ExecuteCommand("set height 20");
    game.ExecuteCommand("set width 5");

    game.ExecuteCommand("set count 1 1");
    game.ExecuteCommand("start");

    game.ExecuteCommand("shot");
    game.ExecuteCommand("set result kill");

    game.ExecuteCommand("exit");
    EXPECT_EQ(game.isWin(), true);
}

TEST(GameTestSuite, MissTest) {
    Game game;

    game.ExecuteCommand("create master");
    game.ExecuteCommand("set strategy ordered");
    game.ExecuteCommand("set count 1 1");
    game.ExecuteCommand("start");

    game.ExecuteCommand("shot 0 1");

    game.ExecuteCommand("exit");
    EXPECT_EQ(game.isLose(), false);
}

TEST(GameTestSuite, DumpLoadTest) {
    Game game1;


    game1.ExecuteCommand("create slave");
    game1.ExecuteCommand("set strategy ordered");
    game1.ExecuteCommand("set height 20");
    game1.ExecuteCommand("set width 5");
    game1.ExecuteCommand("set count 1 4");
    game1.ExecuteCommand("start");

    game1.ExecuteCommand("dump DumpLoadTest.txt");
    game1.ExecuteCommand("exit");

    Game game2;

    game1.ExecuteCommand("create slave");
    game2.ExecuteCommand("load DumpLoadTest.txt");

    std::string output = captureCoutOutput([&]() {
        game2.ExecuteCommand("get count 1");
        game2.ExecuteCommand("get height");
        game2.ExecuteCommand("get width");
    });

    game1.ExecuteCommand("exit");

    EXPECT_EQ(output, "4\n20\n5\n");
}