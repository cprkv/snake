#include <ncurses.h>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <chrono>
#include <cstring>
#include <iostream>
#include <list>
#include <thread>

struct point {
    int x;
    int y;
};

enum snake_move {
    MOVE_TOP,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_BOTTOM,
    MOVE_BREAK,
};

bool s_exit = false;
char** s_field;
const int s_width = 30;
const int s_height = 30;
std::list<point>* s_snake;
snake_move s_moving;
point s_eat;
char* s_difficult;

void init_game(const char* difficult) {
    s_field = (char**) malloc(s_height * sizeof(char*));

    for (int i = 0; i < s_height; i++) {
        s_field[i] = (char*) malloc(s_width * sizeof(char));

        if (i == 0 || i == s_height - 1)
            for (int j = 0; j < s_width; j++)
                s_field[i][j] = 1;
        else {
            s_field[i][0] = 1;
            s_field[i][s_width - 1] = 1;
        }
    }

    s_snake = new std::list<point>();
    s_snake->push_front({10, 10});
    s_snake->push_front({11, 10});
    s_snake->push_front({12, 10});

    s_moving = MOVE_RIGHT;

    s_eat = {20, 20};

    s_difficult = strdup(difficult);
}

void free_game() {
    for (int i = 0; i < s_height; i++) {
        free(s_field[i]);
    }
    free(s_field);
    free(s_difficult);
    delete s_snake;
}

point* get_snake(int x, int y) {
    for (auto& it : *s_snake) {
        if (it.x == x && it.y == y) {
            return &it;
        }
    }

    return nullptr;
}

bool operator==(point a, point b) {
    return a.x == b.x && a.y == b.y;
}

void create_eat() {
    while (true) {
        int x = rand() % (s_width - 3) + 1;
        int y = rand() % (s_height - 3) + 1;

        if (get_snake(x, y) == nullptr) {
            s_eat = {x, y};
            break;
        }
    }
}

bool head_bound_ok() {
    point& head = s_snake->front();

    if (head.x < 1)
        return false;
    if (head.y < 1)
        return false;
    if (head.x > s_width - 2)
        return false;
    if (head.y > s_height - 2)
        return false;

    return true;
}

void move_snake(snake_move moving) {
    if (s_moving == MOVE_BREAK)
        return;

    //--- check oposite or forward moving
    bool no_change_move = ((s_moving == MOVE_LEFT || s_moving == MOVE_RIGHT) &&
                           (moving == MOVE_LEFT || moving == MOVE_RIGHT)) ||
                          ((s_moving == MOVE_TOP || s_moving == MOVE_BOTTOM) &&
                           (moving == MOVE_TOP || moving == MOVE_BOTTOM));

    if (!no_change_move)
        s_moving = moving;

    //--- make new head
    point head = s_snake->front();

    if (s_moving == MOVE_LEFT) {
        head.x--;
    } else if (s_moving == MOVE_RIGHT) {
        head.x++;
    } else if (s_moving == MOVE_TOP) {
        head.y--;
    } else if (s_moving == MOVE_BOTTOM) {
        head.y++;
    }

    //--- check for bounds and intersections
    if (!head_bound_ok() || get_snake(head.x, head.y) != nullptr) {
        s_moving = MOVE_BREAK;
        return;
    }

    //--- check head for food
    if (head.x == s_eat.x && head.y == s_eat.y) {
        create_eat();
    } else {
        s_snake->pop_back();
    }

    s_snake->push_front(head);
}

void handle_key() {
    int input = getch();

    if (input == KEY_F(10)) {
        s_exit = true;
    } else if (input == KEY_LEFT) {
        move_snake(MOVE_LEFT);
    } else if (input == KEY_RIGHT) {
        move_snake(MOVE_RIGHT);
    } else if (input == KEY_UP) {
        move_snake(MOVE_TOP);
    } else if (input == KEY_DOWN) {
        move_snake(MOVE_BOTTOM);
    } else {
        move_snake(s_moving);
    }
}

void draw() {
    printw("\n  ");
    point* snake_point = nullptr;

    for (int i = 0; i < s_height; i++) {
        for (int j = 0; j < s_width; j++) {
            if (s_field[i][j] == 1) {
                printw("%c", '+');
            } else if ((snake_point = get_snake(j, i)) && (*snake_point) == s_snake->front()) {
                printw("%c", 'S');
            } else if (snake_point) {
                printw("%c", 'o');
            } else if (s_eat.x == j && s_eat.y == i) {
                printw("%c", 'X');
            } else {
                printw(" ");
            }
        }
        printw("\n  ");
    }

    printw("\n");

    printw("    difficult : %s\n", s_difficult);
    printw("    score     : %d", (int) s_snake->size() - 3);
}

int main(int argc, char** argv) {
    int set_timeout;
    srand((unsigned int) time(NULL));

    if (argc != 2 || strlen(argv[1]) != 4) {
    difficult_error:
        printf("\n  Error:  you need to pass 1 argument: difficult");
        printf("\n          it can be EASY / NORM / HARD / HELL\n\n");
        return 0;
    } else {
        if (strcmp(argv[1], "EASY") == 0) {
            set_timeout = 150;
        } else if (strcmp(argv[1], "NORM") == 0) {
            set_timeout = 100;
        } else if (strcmp(argv[1], "HARD") == 0) {
            set_timeout = 70;
        } else if (strcmp(argv[1], "HELL") == 0) {
            set_timeout = 35;
        } else {
            goto difficult_error;
        }
    }

    init_game(argv[1]);

    /**/ initscr();
    /**/ cbreak();
    /**/ noecho();
    /**/ keypad(stdscr, TRUE);  // make keys work
    /**/ curs_set(0);           // hide cursor
    /**/ timeout(set_timeout);

    int xmax;
    int ymax;
    /**/ getmaxyx(stdscr, ymax, xmax);

    if (ymax <= s_height + 4 || xmax <= s_width + 4) {
        /**/ refresh();
        /**/ endwin();
        printf("\n  Error:  you need to set your terminal window bigger!\n\n");
        free_game();
        return -1;
    }

    /**/ printw("started");
    /**/ refresh();

    while (!s_exit) {
        /**/ clear();
        draw();
        /**/ refresh();
        handle_key();

        if (s_moving == MOVE_BREAK) {
            s_exit = true;
        }
    }

    /**/ printw("exit");
    /**/ refresh();
    /**/ endwin();

    printf("\n");
    printf("       game over        \n\n");
    printf("    difficult: %s       \n\n", s_difficult);
    printf("       scores: %d       \n\n", (int) s_snake->size() - 3);

    free_game();

    return 0;
}