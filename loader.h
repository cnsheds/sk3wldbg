/*
   Source for Sk3wlDbg IdaPro plugin
   Copyright (c) 2016 Chris Eagle

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
   more details.

   You should have received a copy of the GNU General Public License along with
   this program; if not, write to the Free Software Foundation, Inc., 59 Temple
   Place, Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef __IMAGE_LOADER_H
#define __IMAGE_LOADER_H

#include "sk3wldbg.h"

uint64_t alignsgm(uint64_t addr);
uint64_t get_maprange(std::vector <std::pair<uint64_t, uint64_t>>& sgms, uint64_t vaddr, uint64_t &vsize);

bool loadImage(sk3wldbg *uc, void *img, size_t sz, const char *args, uint64_t init_pc);
bool loadPE64(sk3wldbg *uc, void *img, size_t sz, const char *args, uint64_t init_pc);
bool loadPE32(sk3wldbg *uc, void *img, size_t sz, const char *args, uint64_t init_pc);
bool loadElf64(sk3wldbg *uc, void *img, size_t sz, const char *args, uint64_t init_pc);
bool loadElf32(sk3wldbg *uc, void *img, size_t sz, const char *args, uint64_t init_pc);

extern unsigned int ida_to_uc_perms_map[];
extern unsigned int ida_to_uc_perms_map_win[];
extern unsigned int uc_to_ida_perms_map[];

#endif
