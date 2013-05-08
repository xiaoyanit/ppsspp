// Copyright (c) 2012- PPSSPP Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0 or later versions.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official git repository and contact information can be found at
// https://github.com/hrydgard/ppsspp and http://www.ppsspp.org/.

#pragma once

#include <string>

#include "../System.h"
#include "HLE.h"
#include "sceKernel.h"

extern "C" {
#include "ext/libkirk/amctrl.h"
};


#ifdef USE_FFMPEG

// TODO: Move this!

struct LASTESTFILECACHE {
	char packagefile[256];
	u32  start_pos;
	u8   idbuf[0x20];
	bool npdrm;
	PGD_DESC pgd_info;
};

struct LASTESTACCESSFILE {
	// media file
	char filename[256];
	u32 data_addr;
	// package file
	LASTESTFILECACHE cache[256];
	u8  cachepos;
	LASTESTFILECACHE* findmatchcache(u8* buffer)
	{
		if (!buffer)
			return 0;
		u8 idbuf[0x20];
		generateidbuf(buffer, idbuf);
		for (int i = 0; i < 256; i++) {
			int pos = (256 + cachepos - i) & 0xFF;
			if (memcmp(idbuf, cache[pos].idbuf, 0x20) == 0)
			{
				return &cache[pos];
			}
		}
		return 0;
	}
	void generateidbuf(u8* inbuf, u8* idbuf) {
		if (inbuf[0] == 'R') {
			u16 codeType = *(u16*)(inbuf + 0x14);
			if (codeType == 0xfffe)
				idbuf[0] = 0;
			else if (codeType == 0x270)
				idbuf[0] = 1;
			else 
				idbuf[0] = 2;
			memcpy(idbuf + 1, inbuf + 4, 4);
			memcpy(idbuf + 5, inbuf + 0xa0, 0x20 - 5);
		}
		else {
			memcpy(idbuf, inbuf, 0x20);
		}
	}
};

extern LASTESTACCESSFILE lastestAccessFile;

#endif // _USE_FFMPEG_



void __IoInit();
void __IoDoState(PointerWrap &p);
void __IoShutdown();
u32 __IoGetFileHandleFromId(u32 id, u32 &outError);
KernelObject *__KernelFileNodeObject();
KernelObject *__KernelDirListingObject();

void Register_IoFileMgrForUser();
void Register_StdioForUser();
