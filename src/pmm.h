// src/pmm.h

#ifndef PMM_H
#define PMM_H

void pmm_init(unsigned int *info);

unsigned int pmm_alloc_page(void);

void pmm_free_page(unsigned int address);

unsigned int pmm_free_count(void);

#endif
