#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

struct Player {
    string name;
    int health;
    int coins;
    int roomsCleared;
    bool hasKey;
    bool hasPotion;
    bool hasSword;
    Player() {
        health = 100;
        coins = 0;
        roomsCleared = 0;
        hasKey = false;
        hasPotion = false;
        hasSword = false;
    }
};
//cute ascii art hehe, used ai here a bit for padding
void printCastleArt() {
    cout << R"(
                          |>>>                    |>>>
                          |                        |
                      _  _|_  _                _  _|_  _
                     |;|_|;|_|;|              |;|_|;|_|;|
                     \\.    .  /              \\.    .  /
                      \\:  .  /                \\:  .  /
                       ||:   |                  ||:   |
                       ||:.  |                  ||:.  |
                       ||:  .|                  ||:  .|
                       ||:   |                  ||:   |
                       ||: , |                  ||: , |
                      _||_   |      CASTLE     _||_   |
                 __  /____\  |   __        __  /____\  |  __
                /  \/      \ |  /  \  /\  /  \/      \ | /  \
               /  /\        \| / /\ \/  \/ /\        \|/ /\ \
              /__/  \________/__/  \______ /  \_________/  \__\
             |  _     __     _ |  _    _   _     __     _    _ |
             | | |   /  \   | || | |  | | | |   /  \   | |  | ||
             | |_|  | () |  | || |_|  | |_| |  | () |  | |_| ||
             |      \__/   _|_||      \____/   \__/   _|      ||
             |_______________________________________________|
    )" << '\n';
}

void status(const Player &p) {
    cout << "\n--- STATUS ---\n";
    cout << "Name: " << p.name << endl;
    cout << "Health: " << p.health << endl;
    cout << "Coins: " << p.coins << endl;
    cout << "Rooms Cleared: " << p.roomsCleared << endl;
    cout << "Key: " << (p.hasKey ? "Yes" : "No") << ", Potion: " << (p.hasPotion ? "Yes" : "No") << ", Sword: " << (p.hasSword ? "Yes" : "No") << endl;
    cout << "--------------\n\n";
}

void heal(Player &p, int h) {
    p.health += h;
    if (p.health > 100) p.health = 100;
    cout << "You healed " << h << " health!\n";
}

void damage(Player &p, int d) {
    p.health -= d;
    if (p.health < 0) p.health = 0;
    cout << "You took " << d << " damage!\n";
}

void addCoins(Player &p, int c) {
    p.coins += c;
    cout << "You gained " << c << " coins!\n";
}

void usePotion(Player &p) {
    if (p.hasPotion) {
        heal(p, 50);
        p.hasPotion = false;
        cout << "You used a potion!\n";
    } else {
        cout << "You have no potion to use!\n";
    }
}

void pickSword(Player &p) {
    p.hasSword = true;
    cout << "You found a sword!\n";
}

void pickKey(Player &p) {
    p.hasKey = true;
    cout << "You found a key!\n";
}

void pickPotion(Player &p) {
    p.hasPotion = true;
    cout << "You found a potion!\n";
}

void npcRoom(Player &p) {
    cout << "You meet a generous wizard!\n";
    int r = rand() % 3;
    if (r == 0) {
        cout << "\"Take this healing potion!\"\n";
        pickPotion(p);
    } else if (r == 1) {
        cout << "\"Here is a golden sword!\"\n";
        pickSword(p);
    } else {
        cout << "\"This key will open secret doors.\"\n";
        pickKey(p);
    }
}

void fightRoom(Player &p) {
    int enemyHP = 30 + rand() % 30;
    int enemyAtk = 10 + rand() % 11;
    int reward = 15 + rand() % 16;
    cout << "An enemy appears! HP: " << enemyHP << ", Attack: " << enemyAtk << endl;
    while (enemyHP > 0 && p.health > 0) {
        cout << "1. Attack   2. Run   3. Use Potion\n";
        int c; cin >> c;
        if (c == 3) { usePotion(p); continue; }
        if (c == 2) {
            if (rand()%2 == 0) { cout << "You escaped!\n"; return; }
            else { cout << "Failed to escape!\n";}
        }
        int playerDmg = 10 + (p.hasSword ? 10 : 0);
        enemyHP -= playerDmg;
        cout << "You dealt " << playerDmg << "!\n";
        if (enemyHP > 0) {
            damage(p, enemyAtk);
        }
    }
    if (enemyHP <= 0) {
        cout << "You won the fight!\n";
        addCoins(p, reward);
        p.roomsCleared++;
    } else {
        cout << "You lost the fight!\n";
    }
}

void treasureRoom(Player &p) {
    int reward = 20 + rand()%20;
    cout << "You found a treasure chest!\n1. Open\n2. Ignore\n";
    int c; cin >> c;
    if (c == 1) {
        cout << "Coins inside: " << reward << endl;
        addCoins(p, reward);
        if (rand()%2 == 0) {
            cout << "Bonus: Potion found!\n";
            pickPotion(p);
        }
    } else {
        cout << "You ignore the chest.\n";
    }
    p.roomsCleared++;
}

void healRoom(Player &p) {
    cout << "You found a healing fountain.\n";
    heal(p, 25);
    p.roomsCleared++;
}

void keyRoom(Player &p) {
    pickKey(p);
    p.roomsCleared++;
}

void swordRoom(Player &p) {
    pickSword(p);
    p.roomsCleared++;
}

void potionRoom(Player &p) {
    pickPotion(p);
    p.roomsCleared++;
}

void bossRoom(Player &p) {
    cout << "You encounter the final boss!\n";
    int bossHP = 80;
    int bossAtk = 25;
    if (!p.hasSword) {
        cout << "Without a sword, you can't damage the boss!\n";
        cout << "You are defeated...\n";
        p.health = 0; return;
    }
    if (!p.hasKey) {
        cout << "You need a key to reach the boss!\n";
        return;
    }
    while (bossHP > 0 && p.health > 0) {
        cout << "1. Attack   2. Use Potion\n";
        int c; cin >> c;
        if (c == 2) { usePotion(p); continue; }
        int playerDmg = 20;
        bossHP -= playerDmg;
        cout << "You deal " << playerDmg << "!\n";
        if (bossHP > 0) damage(p, bossAtk);
    }
    if (bossHP <= 0) {
        cout << "** You defeated the boss! **\n";
        addCoins(p, 100);
        p.roomsCleared++;
    } else {
        cout << "You lost to the boss!\n";
    }
}

void printMenu() {
    cout << "\n1. Move to Next Room\n2. Use Potion\n3. Show Status\n4. Quit\n5.Shop\n";
}

void roomRandom(Player &p) {
    int type = rand()%6;
    if (type == 0) fightRoom(p);
    else if (type == 1) treasureRoom(p);
    else if (type == 2) healRoom(p);
    else if (type == 3) npcRoom(p);
    else if (type == 4) potionRoom(p);
    else if (type == 5) swordRoom(p);
}
void shop(Player &p) {
    cout << "\n=== Castle Shop ===\n";
    cout << "You have " << p.coins << " coins.\n";
    cout << "1. Buy Potion (30 coins)\n";
    cout << "2. Heal (+50 HP for 40 coins)\n";
    cout << "3. Buy Sword Upgrade (50 coins)\n";
    cout << "4. Exit Shop\n";
    cout << "5. Visit Shop\n";

    int choice; cin >> choice;
    switch (choice) {
        case 1:
            if (p.coins >= 30) {
                p.coins -= 30;
                pickPotion(p);
            } else cout << "Not enough coins!\n";
            break;
        case 2:
            if (p.coins >= 40) {
                p.coins -= 40;
                heal(p, 50);
            } else cout << "Not enough coins!\n";
            break;
        case 3:
            if (p.coins >= 50) {
                p.coins -= 50;
                p.hasSword = true;
                cout << "You upgraded/bought a sword!\n";
            } else cout << "Not enough coins!\n";
            break;
        default: cout << "Leaving the shop.\n"; break;
    }
}
int main() {
    srand(time(0));
    Player p;
    cout << "===== Castle-of-the-Hack-Lords =====\n\n";
    printCastleArt();
    cout << "Enter your name: ";
    getline(cin, p.name);
    cout << "\nWelcome, " << p.name << "!\n";
    int moves = 0;
    while (p.health > 0) {
        printMenu();
        int menu; cin >> menu;
        if (menu == 1) {
            moves++;
            if (moves == 10) { keyRoom(p); }
            else if (moves == 15) { swordRoom(p); }
            else if (moves == 20) { bossRoom(p); break; }
            else roomRandom(p);
        } else if (menu == 2) {
            usePotion(p);
        } else if (menu == 3) {
            status(p);
        } else if (menu == 4) {
            cout << "You left the castle.\n"; break;
        }
        else if (menu == 5) {
    shop(p);
}

        if (p.health <= 0) {
            cout<<"Game Over!\n";

            break;
        }
    }
    if (p.health > 0 && moves >= 20) {
        cout <<"Congratulations, You beat the game!"<<endl;
        printCastleArt();
        status(p);
    }
    cout << "Thanks for playing!"<<endl;
    return 0;
}
