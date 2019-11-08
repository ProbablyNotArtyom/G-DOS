// Copyright (C) 2016 Mars Saxman. All rights reserved.
// Permission is granted to use at your own risk and distribute this software
// in source and binary forms provided all source code distributions retain
// this paragraph and the above copyright notice. THIS SOFTWARE IS PROVIDED "AS
// IS" WITH NO EXPRESS OR IMPLIED WARRANTY.

#include <errno.h>
#include <string.h>

// These stub implementations are defined as weak symbols so that libc can be
// compiled on its own as a static library which can then be linked against a
// runtime library which actually implements the syscall interface in some
// fashion appropriate for the target environment.

void *sbrk(intptr_t increment) {
	return (void*)-1;
}

int close(int fd) {
	return -ENOSYS;
}
