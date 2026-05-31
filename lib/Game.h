#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

class Game {
   private:
    std::map<uint64_t, std::vector<uint64_t>> cords_;
    std::map<uint64_t, std::vector<uint64_t>> cords_hitted_;
    uint64_t width_ = 0;
    uint64_t height_ = 0;
    uint64_t count_[5] = {0};
    bool mode_ = false;
    bool strategy_ = false;
    bool state_ = false;

    uint64_t ships_killed_ = 0;

    bool isRunning_ = true;
    bool isFinished_ = false;
    bool isWin_ = false;
    bool isLose_ = false;

    std::string last_shot_ = "miss";
    bool has_success_shot_ = false;
    uint64_t last_shot_x_ = 0;
    uint64_t last_shot_y_ = 0;
    uint64_t last_success_shot_x_ = 0;
    uint64_t last_success_shot_y_ = 0;

    bool right_direction_ = true;
    bool left_direction_ = true;
    bool up_direction_ = true;
    bool down_direction_ = true;
    int deep_ = 1;

    uint64_t ordered_strategy_x_ = 0;
    uint64_t ordered_strategy_y_ = 0;

   public:
    ~Game() {
        for (auto& entry : cords_) {
            entry.second.clear();
        }
        cords_.clear();

        for (auto& entry : cords_hitted_) {
            entry.second.clear();
        }
        cords_hitted_.clear();
    }
    bool isRunning() { return isRunning_; }
    bool isWin() { return isWin_; }
    bool isLose() { return isLose_; }
    void setRunning(bool isRunning) { isRunning_ = isRunning; }
    void setState(bool State) { state_ = State; }
    void setWidth(uint64_t width) { width_ = width; }
    uint64_t getWidth() { return width_; }
    void setHeight(uint64_t height) { height_ = height; }
    uint64_t getHeight() { return height_; }
    void setCount(int type, uint64_t count) { count_[type] = count; }
    uint64_t getCount(int type) { return count_[type]; }
    void setStrategy(bool strategy) { strategy_ = strategy; }
    bool getStrategy() { return strategy_; }

    void setResult(std::string result) {
        last_shot_ = result;
        if (result == "kill") {
            ships_killed_++;
        }
        if (ships_killed_ == count_[1] + count_[2] + count_[3] + count_[4]) {
            isWin_ = true;
            isFinished_ = true;
        }
    }

    void placeBoat(int size, char dir, uint64_t x, uint64_t y) {
        for (int i = 0; i < size; ++i) {
            if (dir == 'h') {
                cords_[x + i].push_back(y);
            } else if (dir == 'v') {
                cords_[x].push_back(y + i);
            }
        }
    }

    void dump(std::string path) {
        if (mode_) {
            uint64_t count[5];
            std::copy(std::begin(count_), std::end(count_), std::begin(count));

            std::ofstream outFile(path);

            outFile << width_ << " " << height_ << "\n";

            uint64_t startX = 0;
            uint64_t startY = 0;

            for (uint64_t currentRow = 0; currentRow < height_; currentRow += 2) {
                startY = currentRow;
                startX = 0;
                for (int size = 4; size >= 0; --size) {
                    bool canPlace = true;
                    while (canPlace) {
                        if (count[size] > 0 && startX + (size) <= width_) {
                            outFile << (size) << " h " << startX << " " << startY << "\n";
                            startX += (size + 1) + 1;
                            --count[size];
                        } else {
                            canPlace = false;
                        }
                    }
                }
            }
            outFile.close();
        } else {
            std::ofstream outFile(path);

            outFile << width_ << " " << height_ << "\n";
            outFile << "4 v 0 0\n";
            outFile << "4 v 2 0\n";
            outFile << "4 v 4 0\n";
        }
    }

    void load(std::string path) {
        std::ifstream inFile(path);

        std::string line;
        if (std::getline(inFile, line)) {
            std::istringstream iss(line);
            iss >> width_ >> height_;
        }

        cords_.clear();
        uint64_t countShips[5] = {0};

        while (std::getline(inFile, line)) {
            std::istringstream iss(line);
            int size, x, y;
            char direction;
            if (iss >> size >> direction >> x >> y) {
                placeBoat(size + 1, direction, x, y);
                countShips[size]++;
            }
        }

        for (int i = 0; i < 5; ++i) {
            count_[i] = countShips[i];
        }
        inFile.close();
    }

    void orderedShot() {
        std::cout << ordered_strategy_x_ << " " << ordered_strategy_y_
                  << std::endl;
        if (ordered_strategy_x_ < width_ && ordered_strategy_y_ < height_) {
            if (ordered_strategy_x_ < width_ - 1) {
                ordered_strategy_x_++;
            } else {
                ordered_strategy_x_ = 0;
                ordered_strategy_y_++;
            }
        }
    }

    void customShot() {
        if (last_shot_ == "kill") {
            has_success_shot_ = false;
            resetDirections();
            shootRandom();
            return;
        }

        if (last_shot_ == "miss" && has_success_shot_) {
            updateDirectionAfterMiss();
            return;
        }

        if (last_shot_ == "hit" && has_success_shot_) {
            if (!tryShootInCurrentDirection(deep_)) {
                updateDirectionAfterMiss();
            }
            return;
        }

        if (last_shot_ == "hit" && !has_success_shot_) {
            has_success_shot_ = true;
            last_success_shot_x_ = last_shot_x_;
            last_success_shot_y_ = last_shot_y_;
            resetDirections();
            if (!tryShootInCurrentDirection(1)) {
                updateDirectionAfterMiss();
            }
            deep_++;
            return;
        }

        shootRandom();
    }

    void shootRandom() {
        uint64_t x = rand() % width_;
        uint64_t y = rand() % height_;
        last_shot_x_ = x;
        last_shot_y_ = y;
        std::cout << x << " " << y << std::endl;
    }

    void resetDirections() {
        right_direction_ = true;
        left_direction_ = true;
        up_direction_ = true;
        down_direction_ = true;
        deep_ = 1;
    }

    void updateDirectionAfterMiss() {
        if (right_direction_) {
            right_direction_ = false;
        } else if (left_direction_) {
            left_direction_ = false;
        } else if (down_direction_) {
            down_direction_ = false;
        } else if (up_direction_) {
            up_direction_ = false;
        } else {
            has_success_shot_ = false;
            shootRandom();
        }
    }

    bool tryShootInCurrentDirection(int deep) {
        if (right_direction_ && last_success_shot_x_ + deep < width_) {
            last_shot_x_ = last_success_shot_x_ + deep;
            last_shot_y_ = last_success_shot_y_;
            std::cout << last_shot_x_ << " " << last_shot_y_ << std::endl;
            return true;
        } else if (left_direction_ && last_success_shot_x_ - deep >= 0) {
            last_shot_x_ = last_success_shot_x_ - deep;
            last_shot_y_ = last_success_shot_y_;
            std::cout << last_shot_x_ << " " << last_shot_y_ << std::endl;
            return true;
        } else if (down_direction_ && last_success_shot_y_ + deep < height_) {
            last_shot_x_ = last_success_shot_x_;
            last_shot_y_ = last_success_shot_y_ + deep;
            std::cout << last_shot_x_ << " " << last_shot_y_ << std::endl;
            return true;
        } else if (up_direction_ && last_success_shot_y_ - deep >= 0) {
            last_shot_x_ = last_success_shot_x_;
            last_shot_y_ = last_success_shot_y_ - deep;
            std::cout << last_shot_x_ << " " << last_shot_y_ << std::endl;
            return true;
        }
        return false;
    }

    void createAsMaster() {
        setHeight(4);
        setWidth(5);
        setCount(4, 3);
        placeBoat(4, 'v', 0, 0);
        placeBoat(4, 'v', 2, 0);
        placeBoat(4, 'v', 4, 0);
    }

    void createAsSlave() {
        uint64_t count[5];
        std::copy(std::begin(count_), std::end(count_), std::begin(count));

        uint64_t startX = 0;
        uint64_t startY = 0;

        for (uint64_t currentRow = 0; currentRow < height_; currentRow += 2) {
            startY = currentRow;
            startX = 0;
            for (int size = 3; size >= 0; --size) {
                bool canPlace = true;
                while (canPlace) {
                    if (count[size] > 0 && startX + (size) <= width_) {
                        placeBoat(size, 'h', startX, startY);
                        startX += (size) + 1;
                        --count[size];
                    } else {
                        canPlace = false;
                    }
                }
            }
        }
    }

    void shot(uint64_t x, uint64_t y) {
        if (cords_.find(x) != cords_.end()) {
            auto& vec = cords_[x];
            auto it = std::find(vec.begin(), vec.end(), y);

            if (it != vec.end()) {
                vec.erase(it);

                if (vec.empty()) {
                    cords_.erase(x);
                }

                cords_hitted_[x].push_back(y);

                if (isShipKilled(x, y)) {
                    std::cout << "kill" << std::endl;

                    if (cords_.empty()) {
                        isLose_ = true;
                        isFinished_ = true;
                    }
                } else {
                    std::cout << "hit" << std::endl;
                }
            } else {
                std::cout << "miss" << std::endl;
            }
        } else {
            std::cout << "miss" << std::endl;
        }
    }

    bool isShipKilled(uint64_t x, uint64_t y) {
        int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        for (auto& dir : directions) {
            uint64_t nx = x + dir[0];
            uint64_t ny = y + dir[1];
            if (cords_.find(nx) != cords_.end() &&
                std::find(cords_[nx].begin(), cords_[nx].end(), ny) !=
                    cords_[nx].end()) {
                return false;
            }
        }
        return true;
    }

    void ExecuteCommand(std::string cmd) {
        if (cmd == "ping") {
            std::cout << "pong" << std::endl;
        } else if (cmd == "exit") {
            setRunning(false);
            std::cout << "ok" << std::endl;
        } else if (cmd.substr(0, 6) == "create") {
            if (cmd.substr(7, 6) == "master") {
                mode_ = false;
                createAsMaster();
                std::cout << "ok" << std::endl;
            } else if (cmd.substr(7, 5) == "slave") {
                mode_ = true;
                std::cout << "ok" << std::endl;
            } else {
                std::cout << "failed" << std::endl;
            }
        } else if (cmd == "start") {
            setState(true);
            if (mode_) {
                createAsSlave();
            }
            std::cout << "ok" << std::endl;
        } else if (cmd == "stop") {
            setState(false);
            std::cout << "ok" << std::endl;
        } else if (cmd.substr(0, 9) == "set width") {
            width_ = std::stoi(cmd.substr(10));
            std::cout << "ok" << std::endl;
        } else if (cmd == "get width") {
            std::cout << width_ << std::endl;
        } else if (cmd.substr(0, 10) == "set height") {
            height_ = std::stoi(cmd.substr(10));
            std::cout << "ok" << std::endl;
        } else if (cmd == "get height") {
            std::cout << height_ << std::endl;
        } else if (cmd.substr(0, 9) == "set count") {
            count_[std::stoi(cmd.substr(10, 1))] =
                std::stoi(cmd.substr(11));
            std::cout << "ok" << std::endl;
        } else if (cmd.substr(0, 9) == "get count") {
            std::cout << count_[std::stoi(cmd.substr(10, 1))] << std::endl;
        } else if (cmd.substr(0, 12) == "set strategy") {
            if (cmd.substr(13, 7) == "ordered") {
                setStrategy(false);
                std::cout << "ok" << std::endl;
            } else if (cmd.substr(13, 6) == "custom") {
                setStrategy(true);
                std::cout << "ok" << std::endl;
            } else {
                std::cout << "failed" << std::endl;
            }
        } else if (cmd == "shot") {
            if (!getStrategy()) {
                orderedShot();
            } else {
                customShot();
            }
        } else if (cmd.substr(0, 4) == "shot") {
            uint64_t x, y;
            std::istringstream iss(cmd.substr(5));
            iss >> x >> y;
            shot(x, y);
        } else if (cmd.substr(0, 10) == "set result") {
            setResult(cmd.substr(11));
        } else if (cmd == "finished") {
            std::cout << (isFinished_ ? "yes" : "no") << std::endl;
        } else if (cmd == "win") {
            std::cout << (isWin_ ? "yes" : "no") << std::endl;
        } else if (cmd == "lose") {
            std::cout << (isLose_ ? "yes" : "no") << std::endl;
        } else if (cmd.substr(0, 4) == "dump") {
            dump(cmd.substr(5));
            std::cout << "ok" << std::endl;
        } else if (cmd.substr(0, 4) == "load") {
            load(cmd.substr(5));
            std::cout << "ok" << std::endl;
        } else {
            std::cerr << "Command doesn't exist." << "\n";
        }
    }
};