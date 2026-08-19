#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

const string RESET = "\033[0m";
const string RED = "\033[38;5;196m";
const string ORANGE = "\033[38;5;208m";
const string CREAM = "\033[38;5;230m";
const string GREEN = "\033[38;5;82m";
const string BLUE = "\033[38;5;39m";
const string PURPLE = "\033[38;5;135m";
const string GREY = "\033[38;5;245m";
const string WHITE = "\033[38;5;15m";

struct World {
    int prosperity = 50;
    int freedom = 50;
    int stability = 50;
    int integrity = 100;
    int route_points = 0;
};

void enableAnsi() {
#ifdef _WIN32
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(out, &mode);
    SetConsoleMode(out, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

void clearScreen() {
    cout << "\033[2J\033[H";
}

void line() {
    cout << RED << "============================================================" << RESET << "\n";
}

void waitForEnter() {
    cout << GREY << "\nPress ENTER to continue." << RESET;
    string s;
    getline(cin, s);
}

int clampStat(int n) {
    return max(0, min(100, n));
}

void normalize(World& w) {
    w.prosperity = clampStat(w.prosperity);
    w.freedom = clampStat(w.freedom);
    w.stability = clampStat(w.stability);
    w.integrity = clampStat(w.integrity);
}

string bar(int value) {
    int filled = value / 10;
    return string(filled, '#') + string(10 - filled, '-');
}

int worldScore(const World& w) {
    return w.prosperity + w.freedom + w.stability + w.integrity + w.route_points;
}

string rankName(int score) {
    if (score >= 650) return "BLACK SEAL COURIER";
    if (score >= 560) return "DEAD LETTER AGENT";
    if (score >= 470) return "TEMPORAL COURIER";
    if (score >= 390) return "DECK OFFICER";
    return "DECKHAND";
}

void showStats(const World& w) {
    line();
    cout << CREAM << "CURRENT-TIME REPORT" << RESET << "\n";
    cout << GREEN  << "PROSPERITY " << RESET << "[" << bar(w.prosperity) << "] " << w.prosperity << "\n";
    cout << BLUE   << "FREEDOM    " << RESET << "[" << bar(w.freedom) << "] " << w.freedom << "\n";
    cout << ORANGE << "STABILITY  " << RESET << "[" << bar(w.stability) << "] " << w.stability << "\n";
    cout << PURPLE << "INTEGRITY  " << RESET << "[" << bar(w.integrity) << "] " << w.integrity << "\n";
    cout << WHITE << "ROUTE PTS   " << RESET << w.route_points << "\n";
    cout << CREAM << "TOTAL SCORE " << RESET << worldScore(w) << "\n";
    cout << RED << "RANK        " << RESET << rankName(worldScore(w)) << "\n";
    line();
}

void say(const string& speaker, const string& text, const string& colour = CREAM) {
    cout << colour << speaker << RESET << ": " << text << "\n\n";
}

int ask(const vector<string>& options) {
    for (int i = 0; i < (int)options.size(); ++i) {
        cout << RED << i + 1 << RESET << ". " << options[i] << "\n";
    }
    while (true) {
        cout << CREAM << "\n> " << RESET;
        string s;
        getline(cin, s);
        try {
            int n = stoi(s);
            if (n >= 1 && n <= (int)options.size()) return n;
        } catch (...) {}
        cout << ORANGE << "Choose a number from 1 to " << options.size() << ".\n" << RESET;
    }
}

void drawMap(const vector<string>& map, int px, int py, int points, int hazards) {
    clearScreen();
    cout << RED << "TIME & TIDE" << RESET << "   " << CREAM << "Temporal Route" << RESET
         << "   " << GREEN << "Route Points: " << points << RESET
         << "   " << ORANGE << "Hazards: " << hazards << RESET << "\n";
    line();
    for (int y = 0; y < (int)map.size(); ++y) {
        for (int x = 0; x < (int)map[y].size(); ++x) {
            if (x == px && y == py) {
                cout << WHITE << '@' << RESET;
                continue;
            }
            char c = map[y][x];
            if (c == '#') cout << RED << '#' << RESET;
            else if (c == '~') cout << PURPLE << '~' << RESET;
            else if (c == 'G') cout << GREEN << 'G' << RESET;
            else if (c == '.') cout << CREAM << '.' << RESET;
            else cout << ' ';
        }
        cout << "\n";
    }
    line();
    cout << GREY << "W A S D = move   Q = show route key" << RESET << "\n";
}

int runCourse(const string& title, const vector<string>& map) {
    int px = 1;
    int py = 1;
    int steps = 0;
    int hazards = 0;

    while (true) {
        int points = max(20, 130 - steps * 2 - hazards * 12);
        drawMap(map, px, py, points, hazards);
        cout << CREAM << title << RESET << "\n";
        cout << "Reach " << GREEN << "G" << RESET << ". Avoid " << PURPLE << "~" << RESET << " when possible.\n\n> ";

        string input;
        getline(cin, input);
        if (input.empty()) continue;
        char c = (char)tolower((unsigned char)input[0]);
        if (c == 'q') {
            cout << "\n" << WHITE << "@" << RESET << " courier   " << RED << "#" << RESET << " blocked   "
                 << PURPLE << "~" << RESET << " unstable time   " << GREEN << "G" << RESET << " destination\n";
            waitForEnter();
            continue;
        }

        int nx = px;
        int ny = py;
        if (c == 'w') ny--;
        else if (c == 's') ny++;
        else if (c == 'a') nx--;
        else if (c == 'd') nx++;
        else continue;

        if (ny < 0 || ny >= (int)map.size() || nx < 0 || nx >= (int)map[ny].size()) continue;
        if (map[ny][nx] == '#') continue;

        px = nx;
        py = ny;
        steps++;
        if (map[py][px] == '~') hazards++;

        if (map[py][px] == 'G') {
            int earned = max(20, 130 - steps * 2 - hazards * 12);
            drawMap(map, px, py, earned, hazards);
            cout << "\n" << GREEN << "ROUTE CLEARED! +" << earned << " ROUTE POINTS" << RESET << "\n";
            waitForEnter();
            return earned;
        }
    }
}

vector<string> courseOne() {
    return {
        "#########################",
        "#......#.................#",
        "#.####.#.~~~~~.#########.#",
        "#....#.#.....#.........#.#",
        "####.#.#####.#.#######.#.#",
        "#....#.....#.#...~~~~#...#",
        "#.########.#.#######.###.#",
        "#..........#.......#.....#",
        "#.~~~~~~~~.#######.#####.#",
        "#.......................G#",
        "#########################"
    };
}

vector<string> courseTwo() {
    return {
        "#########################",
        "#....~~~~....#...........#",
        "#.##########.#.#########.#",
        "#............#.....#.....#",
        "#.###############.#.###.#",
        "#.....~~~~.........#...#.#",
        "#####.###########.###.#..#",
        "#.....#.........#.....##.#",
        "#.###.#.~~~~~~~.########.#",
        "#...#...................G#",
        "#########################"
    };
}

vector<string> courseThree() {
    return {
        "#########################",
        "#.....#~~~~~#............#",
        "#.###.#.###.#.##########.#",
        "#...#...#...#......~~~~#.#",
        "###.#####.###########.#..#",
        "#...#.....#...........##.#",
        "#.###.#####.~~~~~~.####..#",
        "#.....#.....######.......#",
        "#.#####.~~~........#####.#",
        "#.......................G#",
        "#########################"
    };
}

void prologue() {
    clearScreen();
    cout << RED << R"(
 _______  ___  __   __  _______    __   _______    _______  ___   ______   _______
|       ||   ||  |_|  ||       |  |  | |       |  |       ||   | |      | |       |
|_     _||   ||       ||    ___|  |  | |_     _|  |_     _||   | |  _    ||    ___|
  |   |  |   ||       ||   |___   |  |   |   |      |   |  |   | | | |   ||   |___
  |   |  |   ||       ||    ___|  |  |   |   |      |   |  |   | | |_|   ||    ___|
  |   |  |   || ||_|| ||   |___   |  |   |   |      |   |  |   | |       ||   |___
  |___|  |___||_|   |_||_______|  |__|   |___|      |___|  |___| |______| |_______|
)" << RESET << "\n";
    cout << CREAM << "All of history is an ocean. Every era is a shore.\n" << RESET;
    cout << "The Meridian sails between them carrying messages that should never meet.\n\n";
    say("CAPTAIN VOSS", "Courier 17. One rule: the mail arrives exactly as written.", ORANGE);
    say("PIP", "Current timeline stable. Integrity: 100.", BLUE);
    cout << "Your first letter waits below deck.\n";
    waitForEnter();
}

void letterOne(World& w, int& choice) {
    clearScreen();
    cout << RED << "LETTER 001" << RESET << "\n";
    line();
    cout << "YEAR: 1719\nRECIPIENT: CAPTAIN GIDEON VALE\nSENDER: THOMAS VALE\nLOCATION: PORT MERCY\n\n";
    cout << CREAM << "Gideon,\nThe navy knows where the Providence will sail.\nDo not leave Port Mercy tomorrow.\nThey are waiting for you.\n- Thomas\n" << RESET;
    line();
    say("ELIZA QUINN", "Vale is a violent pirate. If the navy catches him, the sea will be better for it.");
    say("SAMUEL", "Vale steals from governors and merchants. Never ordinary sailors.");
    say("LT. HARROW", "Tell me where Vale is and I will see you rewarded.");
    cout << RED << "THE DELIVERY" << RESET << "\n";
    choice = ask({
        "Deliver the warning unchanged.",
        "Destroy the letter and let Vale sail into the ambush.",
        "Give the letter to Lieutenant Harrow.",
        "Alter the warning: tell Vale to leave immediately."
    });

    if (choice == 1) {
        w.prosperity += 10; w.freedom -= 6; w.stability += 4; w.integrity -= 5;
        say("PIP", "Gideon Vale survived. Vale Industries now exists in the current era.", BLUE);
    }
    if (choice == 2) {
        w.freedom += 2; w.stability += 3;
        say("PIP", "Gideon Vale died in 1719. The timeline remains close to baseline.", BLUE);
    }
    if (choice == 3) {
        w.prosperity += 2; w.freedom -= 2; w.stability += 8; w.integrity -= 6;
        say("PIP", "Vale was captured. Port Mercy became a naval stronghold.", BLUE);
    }
    if (choice == 4) {
        w.prosperity += 12; w.freedom -= 8; w.stability += 2; w.integrity -= 12;
        say("PIP", "Vale escaped early. His later voyages changed the Atlantic economy.", BLUE);
    }
    normalize(w);
    cout << GREEN << "History has settled. Your reward is calculated in the present." << RESET << "\n";
    waitForEnter();
}

void returnOne(World& w) {
    clearScreen();
    cout << RED << "RETURN: CURRENT TIME" << RESET << "\n\n";
    showStats(w);
    say("PIP", "A new letter has appeared. It is dated 2280, but addressed to 1719.", BLUE);
    cout << CREAM << "FROM: DR. LENA VALE\nTO: GIDEON VALE\n\n"
         << "Beneath Blackglass Island you will find a mineral unlike any known in your century.\n"
         << "Do not sell it. Hide it. Humanity is not ready.\n" << RESET;
    waitForEnter();
}

void letterTwo(World& w, int& choice) {
    clearScreen();
    cout << RED << "LETTER 002: BLACKGLASS" << RESET << "\n\n";
    say("PIP", "The letter is temporally impossible. That does not make it harmless.", BLUE);
    choice = ask({
        "Deliver it unchanged. Vale hides the mineral.",
        "Alter it: tell Vale to release the discovery publicly.",
        "Destroy the letter.",
        "Add an instruction: destroy the Blackglass mine."
    });

    if (choice == 1) {
        w.prosperity += 6; w.stability += 4; w.integrity -= 4;
        say("PIP", "The mineral vanished into a private archive. Progress continued carefully.", BLUE);
    }
    if (choice == 2) {
        w.prosperity += 15; w.freedom += 8; w.stability -= 15; w.integrity -= 12;
        say("PIP", "Blackglass entered public science. Technology accelerated. So did warfare.", BLUE);
    }
    if (choice == 3) {
        w.prosperity -= 8; w.freedom += 5; w.stability += 10; w.integrity -= 5;
        say("PIP", "Future knowledge never reached Vale. Civilization advanced more slowly.", BLUE);
    }
    if (choice == 4) {
        w.prosperity -= 12; w.freedom += 10; w.stability += 15; w.integrity -= 15;
        say("PIP", "Blackglass was destroyed before history could claim it.", BLUE);
    }
    normalize(w);
    waitForEnter();
}

void luna(World& w) {
    clearScreen();
    cout << RED << "LUNA CITY, 2280" << RESET << "\n\n";
    showStats(w);
    if (w.prosperity >= 75 && w.stability < 45) {
        say("PIP", "Luna is brilliant, rich and heavily armed. Three orbital wars are remembered here.", BLUE);
    } else if (w.prosperity >= 65 && w.freedom < 45) {
        say("PIP", "Luna is wealthy and immaculate. Most infrastructure belongs to a handful of dynasties.", BLUE);
    } else if (w.stability >= 70) {
        say("PIP", "Luna is modest, peaceful and years behind the old expansion forecasts.", BLUE);
    } else {
        say("PIP", "Luna survived. History is mixed, contradictory and recognisably human.", BLUE);
    }

    say("DR. LENA VALE", "Someone has been changing the mail before it reaches you.", GREEN);
    say("CAPTAIN VOSS", "Destroy Lena's message to Thomas Vale.", ORANGE);
    say("COURIER 17", "The Meridian delivers mail. Why destroy it?", WHITE);
    say("CAPTAIN VOSS", "Because the first letter was already a lie.", ORANGE);
    waitForEnter();

    clearScreen();
    cout << RED << "THE REVELATION" << RESET << "\n\n";
    say("CAPTAIN VOSS", "Gideon Vale was supposed to die in 1719.", ORANGE);
    say("CAPTAIN VOSS", "Thomas never wrote the warning you carried.", ORANGE);
    say("CAPTAIN VOSS", "I wrote it in 2314 and sent it backward.", ORANGE);
    say("CAPTAIN VOSS", "In the untouched history, humanity collapsed. Billions died.", ORANGE);
    say("CAPTAIN VOSS", "Keeping Vale alive gave us the technology to survive.", ORANGE);
    say("CAPTAIN VOSS", "You call it interference because you never saw the world where we did nothing.", ORANGE);
    waitForEnter();
}

void finalChoice(World& w, int& ending) {
    clearScreen();
    cout << RED << "FINAL DELIVERY: PORT MERCY, 1719" << RESET << "\n\n";
    cout << "One letter. One shore. One history.\n\n";
    ending = ask({
        "Restore the original timeline. Vale dies.",
        "Preserve Voss's timeline. Vale survives and the future is protected.",
        "Release Blackglass to everyone. No dynasty owns the future.",
        "Destroy Blackglass completely. Slow humanity down.",
        "Refuse the choice. Cast the final letter into the temporal sea."
    });

    if (ending == 1) {
        w.prosperity = 38; w.freedom = 61; w.stability = 41; w.integrity = 100;
        w.route_points += 80;
    }
    if (ending == 2) {
        w.prosperity = 86; w.freedom = 31; w.stability = 78; w.integrity = 71;
        w.route_points += 120;
    }
    if (ending == 3) {
        w.prosperity = 91; w.freedom = 78; w.stability = 19; w.integrity = 54;
        w.route_points += 150;
    }
    if (ending == 4) {
        w.prosperity = 44; w.freedom = 73; w.stability = 84; w.integrity = 46;
        w.route_points += 130;
    }
    if (ending == 5) {
        w.prosperity = 50; w.freedom = 50; w.stability = 50; w.integrity = 0;
        w.route_points += 200;
    }
}

void endingScreen(const World& w, int ending) {
    clearScreen();
    cout << RED << "TIME & TIDE" << RESET << "\n\n";

    if (ending == 1) {
        cout << CREAM << "THE ORIGINAL TIMELINE\n\n" << RESET;
        cout << "History remained untouched.\nHumanity was free to make its own mistakes.\n\n";
    }
    if (ending == 2) {
        cout << CREAM << "VOSS'S TIMELINE\n\n" << RESET;
        cout << "Humanity survived.\nThe price was a future shaped five centuries before anyone living could object.\n\n";
    }
    if (ending == 3) {
        cout << CREAM << "THE OPEN FUTURE\n\n" << RESET;
        cout << "No person owned the future.\nUnfortunately, everyone had access to it.\n\n";
    }
    if (ending == 4) {
        cout << CREAM << "THE SLOW STAR\n\n" << RESET;
        cout << "Humanity reached the stars much later.\nPerhaps that was not a tragedy.\n\n";
    }
    if (ending == 5) {
        cout << CREAM << "RETURN TO SENDER\n\n" << RESET;
        say("CAPTAIN VOSS", "What are you doing?", ORANGE);
        say("COURIER 17", "Returning the future to sender.", WHITE);
        say("PIP", "I can no longer determine which history is ours.", BLUE);
    }

    if (ending != 5) showStats(w);
    else {
        line();
        cout << GREEN << "PROSPERITY  ?\n" << RESET;
        cout << BLUE << "FREEDOM     ?\n" << RESET;
        cout << ORANGE << "STABILITY   ?\n" << RESET;
        cout << PURPLE << "INTEGRITY   ?\n" << RESET;
        cout << CREAM << "COURIER SCORE " << worldScore(w) << "\n" << RESET;
        line();
    }

    cout << RED << "\nTHE MAIL HAS ARRIVED.\n" << RESET;
}

int main() {
    enableAnsi();

    World world;
    int firstChoice = 0;
    int secondChoice = 0;
    int ending = 0;

    prologue();

    world.route_points += runCourse("TEMPORAL SHORE I: Navigate to Port Mercy, 1719", courseOne());
    letterOne(world, firstChoice);
    returnOne(world);

    world.route_points += runCourse("TEMPORAL SHORE II: Cross the altered current", courseTwo());
    letterTwo(world, secondChoice);

    world.route_points += runCourse("TEMPORAL SHORE III: Reach Luna City, 2280", courseThree());
    luna(world);

    world.route_points += runCourse("FINAL CURRENT: Return to Port Mercy", courseOne());
    finalChoice(world, ending);
    endingScreen(world, ending);

    return 0;
}
