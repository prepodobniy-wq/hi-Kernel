// src/vga.c
// displaying text on the screen in VGA text mode

#define VGA_MEMORY  0xB8000
#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_COLOR   0x0F

static volatile unsigned short *video = (volatile unsigned short *)VGA_MEMORY;
static int cursor_row = 0;
static int cursor_col = 0;

// assebles a screen cell: color (high byte) and character (low byte)
static unsigned short make_cell(char c)
{
    return (VGA_COLOR << 8) | (unsigned char)c;
}

// shifts all rows up by one and clears the bottom row
static void scroll(void)
{
    int i;

    for (i=0; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++) {
        video[i] = video[i + VGA_WIDTH];
    }
    for (i = VGA_WIDTH * (VGA_HEIGHT - 1); i < VGA_WIDTH * VGA_HEIGHT; i++) {
        video[i] = make_cell(' ');
    }
    cursor_row = VGA_HEIGHT - 1;
}

// moves the cursor to the beginning of the next line
static void new_line(void)
{
    cursor_col = 0;
    cursor_row++;
    if (cursor_row >= VGA_HEIGHT) {
        scroll();
    }
}

void vga_clear(void)
{
    int i;

    for (i=0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        video[i] = make_cell(' ');
    }
    cursor_row = 0;
    cursor_col = 0;
}

void vga_put_char(char c)
{
    if (c == '\n') {
        new_line();
        return;
    }

    video[cursor_row * VGA_WIDTH + cursor_col] = make_cell(c);
    cursor_col++;
    if (cursor_col >= VGA_WIDTH) {
        new_line();
    }
}

void vga_print(const char *text)
{
    int i = 0;

    while (text[i] != '\0') {
        vga_put_char(text[i]);
        i++;
    }
}

