/* Vector table skeleton for use with m68k */

	.data
	.extern SYM (_start)
	.extern SYM (_BError)
	.extern SYM (_AError)

_vectors:

	.long	__stack
	.long	_start
	.long	_BError
	.long	_AError