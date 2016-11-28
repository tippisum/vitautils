#include <psp2kern/types.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2kern/io/fcntl.h>
#include <psp2kern/sblacmgr.h>
#include <taihen/taihen.h>

typedef struct SceMemBlock {
	struct SceMemBlock* next;
	uint32_t u0004[3];
	int flags;
	uint32_t u0014;
	uintptr_t base;
	size_t size;
} SceMemBlock;

typedef struct SceMemRegion {
	uint32_t u0000[3];
	int lock;
	SceMemBlock* head;
	uintptr_t base;
	size_t size;
} SceMemRegion;

typedef void (*fInternalAddRef)(int* refCount, int value, int max);
typedef void (*fInternalDecRef)(int* refCount);
typedef int (*fInternalGetMemBlockType)(int flags, SceKernelMemBlockType* type);
typedef int (*fInternalCheckMemBlockFlags)(int flags);
typedef size_t(*fInternalGetMemBlockBaseOffset)(SceMemBlock* block);
typedef size_t(*fInternalGetMemBlockSize)(SceMemBlock* block);
fInternalGetMemBlockType InternalGetMemBlockType;
fInternalCheckMemBlockFlags InternalCheckMemBlockFlags;
fInternalGetMemBlockBaseOffset InternalGetMemBlockBaseOffset;
fInternalGetMemBlockSize InternalGetMemBlockSize;


uint32_t DecImm32(const void* ptr) {
	uint32_t a, b;
	a = ((uint32_t*)ptr)[0];
	b = ((uint32_t*)ptr)[1];
	return (((b & 0x000F0000) << 12) | ((b & 0x00000FFF) << 16) | ((a & 0x000F0000) >> 4) | ((a & 0x00000FFF)));
}

int _start(SceSize args, void* argp) {
	uintptr_t ptr[2];
	uintptr_t mm;
	SceMemRegion* reg;
	SceMemBlock* b;
	int i;
	SceUID f;

	f = sceIoOpenForDriver(argp, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0777);
	if (f < 0) { return 0; }
	ptr[0] = DecImm32(&sceKernelGetMemBlockBaseForKernel) - 0x1F15;
	InternalCheckMemBlockFlags = (fInternalCheckMemBlockFlags)(ptr[0] + 0x9BCD);
	InternalGetMemBlockType = (fInternalGetMemBlockType)(ptr[0] + 0x162E1);
	InternalGetMemBlockBaseOffset = (fInternalGetMemBlockBaseOffset)(ptr[0] + 0x1841);
	InternalGetMemBlockSize = (fInternalGetMemBlockSize)(ptr[0] + 0x18D9);
	mm = ((uintptr_t (*)())(ptr[0] + 0x1FBA9))();
	for (i = 0; i < 0x80; i = i + 4) {
		reg = *(SceMemRegion **)(mm + i + 0x1C);
		if (!reg) { continue; }
		for (b = reg->head->next; b && b != reg->head; b = b->next) {
			if (InternalCheckMemBlockFlags(b->flags) < 0) { continue; }
			InternalGetMemBlockType(b->flags, (int*)ptr);
			if (ptr[0] != SCE_KERNEL_MEMBLOCK_TYPE_USER_RW) { continue; }
			ptr[0] = b->base + InternalGetMemBlockBaseOffset(b);
			ptr[1] = InternalGetMemBlockSize(b);
			sceIoWriteForDriver(f, ptr, sizeof(ptr));
		}
	}
	sceIoCloseForDriver(f);
	return 0;
}

int _stop(SceSize args, void* argp) {
	return 0;
}
