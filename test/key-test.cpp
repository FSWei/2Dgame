#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/select.h>
#include <string.h>
#include <thread>
#include <ncursesw/ncurses.h>

#include <linux/input.h>

#define KEYBOARD_DEVICE_1 "/dev/input/event0"
#define KEYBOARD_DEVICE_2 "/dev/input/event1"

#define MAX_KEYCODE 255

void keyboard_listener();
void _move();
void print_A();

int key_state[MAX_KEYCODE + 1] = {0};

int main() {
    std::thread listener_thread(keyboard_listener);
    std::thread move_thread(_move);
    std::thread print_thread(print_A);

    // 初始化ncurses
    initscr(); // 初始化ncurses屏幕
    cbreak();  // 禁用行缓冲，传递所有输入字符到程序
    noecho();  // 禁用字符回显
    refresh(); // 刷新屏幕

    listener_thread.join();
    move_thread.join();
    print_thread.join();

    // 结束ncurses
    endwin(); // 结束ncurses模式

    return 0;
}

void keyboard_listener() {
    int fd_keyboard1 = open(KEYBOARD_DEVICE_1, O_RDONLY);
    if (fd_keyboard1 == -1) {
        perror("open keyboard device 1 failed");
        return;
    }

    int fd_keyboard2 = open(KEYBOARD_DEVICE_2, O_RDONLY);
    if (fd_keyboard2 == -1) {
        perror("open keyboard device 2 failed");
        close(fd_keyboard1);
        return;
    }

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd_keyboard1, &readfds);
    FD_SET(fd_keyboard2, &readfds);
    int maxfd = (fd_keyboard1 > fd_keyboard2) ? fd_keyboard1 : fd_keyboard2;

    struct input_event ev;
    ssize_t ret;
    while (true) {
        ret = select(maxfd + 1, &readfds, NULL, NULL, NULL);
        if (ret == -1) {
            perror("select error");
            break;
        } else if (ret > 0) {
            if (FD_ISSET(fd_keyboard1, &readfds)) {
                ret = read(fd_keyboard1, &ev, sizeof(ev));
                if (ret == sizeof(ev) && ev.type == EV_KEY) {
                    if (ev.value == 1)
                        key_state[ev.code] = 1;
                    else if (ev.value == 0)
                        key_state[ev.code] = 0;
                }
            }

            if (FD_ISSET(fd_keyboard2, &readfds)) {
                ret = read(fd_keyboard2, &ev, sizeof(ev));
                if (ret == sizeof(ev) && ev.type == EV_KEY) {
                    if (ev.value == 1)
                        key_state[ev.code] = 1;
                    else if (ev.value == 0)
                        key_state[ev.code] = 0;
                }
            }
        }

        FD_ZERO(&readfds);
        FD_SET(fd_keyboard1, &readfds);
        FD_SET(fd_keyboard2, &readfds);
    }

    close(fd_keyboard1);
    close(fd_keyboard2);
}

void _move() {
    while (true) {
        clear(); // 清除当前窗口内容
        if (key_state[105]!=0) {
            printw("a"); // 打印字符'a'
        }
        if (key_state[103]!=0) {
            printw("w");
        }
        if (key_state[106]!=0) {
            printw("d");
        }
        if (key_state[108]!=0) {
            printw("j");
        }
        // refresh(); // 刷新屏幕
        usleep(10000); // 10毫秒
    }
}


void print_A() {
    while (true) {
        addch('A');
        // refresh();
        usleep(100000); // 100毫秒
    }
}
