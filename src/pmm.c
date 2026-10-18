// src/pmm.c

#include "multiboot.h"

#define PAGE_SIZE            4096
#define MAX_MEMORY           0x08000000    // 128MB
#define MAX_PAGES            (MAX_MEMORY / PAGE_SIZE)
#define MULTIBOOT_INFO_SIZE  88

extern char kernel_end;

static unsigned char bitmap[MAX_PAGES / 8];
static unsigned int free_pages = 0;

static int page_is_used(unsigned int page)
{
    return bitmap[page / 8] & (1 << (page % 8));
}

static void page_set_used(unsigned int page)
{
    if (!page_is_used(page)) {
        bitmap[page / 8] |= (unsigned char)(1 << (page % 8));
        free_pages--;
    }
}

static void page_set_free(unsigned int page)
{
    if (page_is_used(page)) {
        bitmap[page / 8] &= (unsigned char)~(1 << (page % 8));
        free_pages++;
    }
}

static void free_range(unsigned int start, unsigned int end)
{
    unsigned int page = (start + PAGE_SIZE - 1) / PAGE_SIZE;
    unsigned int last = end / PAGE_SIZE;

    while (page < last && page < MAX_PAGES) {
        page_set_free(page);
        page++;
    }
}

static void use_range(unsigned int start, unsigned int end)
{
    unsigned int page = start / PAGE_SIZE;
    unsigned int last = (end + PAGE_SIZE - 1) / PAGE_SIZE;

    while (page < last && page < MAX_PAGES) {
        page_set_used(page);
        page++;
    }
}

void pmm_init(unsigned int *info)
{
    unsigned int i;
    unsigned int addr;
    unsigned int list_end;
    unsigned int start;
    unsigned int region_end;
    struct mmap_entry *entry;

    for (i = 0; i < sizeof(bitmap); i++) {
        bitmap[i] = 0xFF;
    }
    free_pages = 0;

    if (info [0] & MULTIBOOT_FLAG_MMAP) {
        addr = info[12];
        list_end = info[12] + info[11];
        while (addr < list_end) {
            entry = (struct mmap_entry *)addr;
            if (entry->type == MMAP_TYPE_FREE
                && entry->base_high == 0
                && entry->base_low < MAX_MEMORY) {
                start = entry->base_low;
                if (entry->length_high != 0
                    || entry->length_low > MAX_MEMORY - start) {
                    region_end = MAX_MEMORY;
                } else {
                    region_end = start + entry->length_low;
                }
                free_range(start, region_end);
            }
            addr = addr + entry->size + 4;
        }
        use_range(info[12], info[12] + info[11]);
    }
    use_range(0, (unsigned int)&kernel_end);
    use_range((unsigned int)info, (unsigned int)info + MULTIBOOT_INFO_SIZE);
}

unsigned int pmm_alloc_page(void)
{
    unsigned int page;

    for (page = 0; page < MAX_PAGES; page++) {
        if (!page_is_used(page)) {
            page_set_used(page);
            return page * PAGE_SIZE;
        }
    }
    return 0;
}

void pmm_free_page(unsigned int address)
{
    if (address % PAGE_SIZE != 0) {
        return;
    }
    if (address < (unsigned int)&kernel_end) {
        return;
    }
    if (address / PAGE_SIZE < MAX_PAGES) {
        page_set_free(address / PAGE_SIZE);
    }
}

unsigned int pmm_free_count(void)
{
    return free_pages;
}
