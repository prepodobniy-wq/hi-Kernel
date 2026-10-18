// src/shell.c
// basic command prompt

#include "vga.h"
#include "string.h"
#include "timer.h"
#include "shell.h"

#define LINE_SIZE           64
#define TICKS_PER_SECOND    100

static char line[LINE_SIZE];
static int length = 0;

static void print_prompt(void)
{
    vga_print("$ ");
}

static void cmd_help(void)
{
    vga_print("commands:\n");
    vga_print("  help    - shows this list\n");
    vga_print("  uptime  - time since start\n");
    vga_print("  clear   - clear the screen\n");
}

static void cmd_uptime(void)
{
    char buffer[11];

    uint_to_string(timer_get_ticks() / TICKS_PER_SECOND, buffer);
    vga_print("Uptime: ");
    vga_print(buffer);
    vga_print(" s\n");
}

static void run_cmd(void)
{
    if (length == 0) {
        return;
    }

    if (strcmp(line, "help") == 0) {
        cmd_help();
    } else if (strcmp(line, "uptime") == 0) {
        cmd_uptime();
    } else if (strcmp(line, "clear") == 0) {
        vga_clear();
    } else {
        vga_print("hi!Sh> Unknown command: ");
        vga_print(line);
        vga_print("\n");
    }
}

void sh_init(void)
{
    length = 0;
    vga_print("Type 'help' for commands.\n");
    print_prompt();
}

void sh_handle_char(char c)
{
    if (c == '\n') {
        vga_put_char('\n');
        line[length] = '\0';
        run_cmd();
        length = 0;
        print_prompt();
    } else if (c == '\b') {
        if (length > 0) {
            length--;
            vga_put_char('\b');
        }
    } else if (length < LINE_SIZE - 1) {
        line[length] = c;
        length++;
        vga_put_char(c);
    }
}

