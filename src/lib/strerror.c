//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

	#include <std.h>
	#include <string.h>
	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>

	#include <mod/init.h>
	#include <errno.h>

#define ERR_TABLE_ENTRY(value, name, msg)		{value, name, COLOR_FG(C_RED, "[!] ") COLOR_FG(C_YELLOW, msg)}
#define ERR_TABLE_ENTRY_NULL(value, name, msg)	{value, NULL, NULL}

#ifndef MAX
	#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

//---------------------------------------------------

struct error_info {
	const int 		value;	/* The numeric value from <errno.h> */
	const char *const name;	/* The equivalent symbolic value */
	const char *const msg;	/* Short message about this value */
};

//---------------------------------------------------

static const char **error_names;
static int num_error_names = 0;
static int sys_nerr;
static const char **sys_errlist;

void init_error_tables (void);

static const struct error_info error_table[] = {
#if defined (EPERM)
	ERR_TABLE_ENTRY(EPERM, "EPERM", "Not owner"),
#endif
#if defined (ENOENT)
	ERR_TABLE_ENTRY(ENOENT, "ENOENT", "No such file or directory"),
#endif
#if defined (ESRCH)
	ERR_TABLE_ENTRY(ESRCH, "ESRCH", "No such process"),
#endif
#if defined (EINTR)
	ERR_TABLE_ENTRY(EINTR, "EINTR", "Interrupted system call"),
#endif
#if defined (EIO)
	ERR_TABLE_ENTRY(EIO, "EIO", "I/O error"),
#endif
#if defined (ENXIO)
	ERR_TABLE_ENTRY(ENXIO, "ENXIO", "No such device or address"),
#endif
#if defined (E2BIG)
	ERR_TABLE_ENTRY(E2BIG, "E2BIG", "Arg list too long"),
#endif
#if defined (ENOEXEC)
	ERR_TABLE_ENTRY(ENOEXEC, "ENOEXEC", "Exec format error"),
#endif
#if defined (EBADF)
	ERR_TABLE_ENTRY(EBADF, "EBADF", "Bad file number"),
#endif
#if defined (ECHILD)
	ERR_TABLE_ENTRY(ECHILD, "ECHILD", "No child processes"),
#endif
#if defined (EWOULDBLOCK)	/* Put before EAGAIN, sometimes aliased */
	ERR_TABLE_ENTRY(EWOULDBLOCK, "EWOULDBLOCK", "Operation would block"),
#endif
#if defined (EAGAIN)
	ERR_TABLE_ENTRY(EAGAIN, "EAGAIN", "No more processes"),
#endif
#if defined (ENOMEM)
	ERR_TABLE_ENTRY(ENOMEM, "ENOMEM", "Not enough space"),
#endif
#if defined (EACCES)
	ERR_TABLE_ENTRY(EACCES, "EACCES", "Permission denied"),
#endif
#if defined (EFAULT)
	ERR_TABLE_ENTRY(EFAULT, "EFAULT", "Bad address"),
#endif
#if defined (ENOTBLK)
	ERR_TABLE_ENTRY(ENOTBLK, "ENOTBLK", "Block device required"),
#endif
#if defined (EBUSY)
	ERR_TABLE_ENTRY(EBUSY, "EBUSY", "Device busy"),
#endif
#if defined (EEXIST)
	ERR_TABLE_ENTRY(EEXIST, "EEXIST", "File exists"),
#endif
#if defined (EXDEV)
	ERR_TABLE_ENTRY(EXDEV, "EXDEV", "Cross-device link"),
#endif
#if defined (ENODEV)
	ERR_TABLE_ENTRY(ENODEV, "ENODEV", "No such device"),
#endif
#if defined (ENOTDIR)
	ERR_TABLE_ENTRY(ENOTDIR, "ENOTDIR", "Not a directory"),
#endif
#if defined (EISDIR)
	ERR_TABLE_ENTRY(EISDIR, "EISDIR", "Is a directory"),
#endif
#if defined (EINVAL)
	ERR_TABLE_ENTRY(EINVAL, "EINVAL", "Invalid argument"),
#endif
#if defined (ENFILE)
	ERR_TABLE_ENTRY(ENFILE, "ENFILE", "File table overflow"),
#endif
#if defined (EMFILE)
	ERR_TABLE_ENTRY(EMFILE, "EMFILE", "Too many open files"),
#endif
#if defined (ENOTTY)
	ERR_TABLE_ENTRY(ENOTTY, "ENOTTY", "Not a typewriter"),
#endif
#if defined (ETXTBSY)
	ERR_TABLE_ENTRY(ETXTBSY, "ETXTBSY", "Text file busy"),
#endif
#if defined (EFBIG)
	ERR_TABLE_ENTRY(EFBIG, "EFBIG", "File too large"),
#endif
#if defined (ENOSPC)
	ERR_TABLE_ENTRY(ENOSPC, "ENOSPC", "No space left on device"),
#endif
#if defined (ESPIPE)
	ERR_TABLE_ENTRY(ESPIPE, "ESPIPE", "Illegal seek"),
#endif
#if defined (EROFS)
	ERR_TABLE_ENTRY(EROFS, "EROFS", "Read-only file system"),
#endif
#if defined (EMLINK)
	ERR_TABLE_ENTRY(EMLINK, "EMLINK", "Too many links"),
#endif
#if defined (EPIPE)
	ERR_TABLE_ENTRY(EPIPE, "EPIPE", "Broken pipe"),
#endif
#if defined (EDOM)
	ERR_TABLE_ENTRY(EDOM, "EDOM", "Math argument out of domain of func"),
#endif
#if defined (ERANGE)
	ERR_TABLE_ENTRY(ERANGE, "ERANGE", "Math result not representable"),
#endif
#if defined (ENOMSG)
	ERR_TABLE_ENTRY(ENOMSG, "ENOMSG", "No message of desired type"),
#endif
#if defined (EIDRM)
	ERR_TABLE_ENTRY(EIDRM, "EIDRM", "Identifier removed"),
#endif
#if defined (ECHRNG)
	ERR_TABLE_ENTRY(ECHRNG, "ECHRNG", "Channel number out of range"),
#endif
#if defined (EL2NSYNC)
	ERR_TABLE_ENTRY(EL2NSYNC, "EL2NSYNC", "Level 2 not synchronized"),
#endif
#if defined (EL3HLT)
	ERR_TABLE_ENTRY(EL3HLT, "EL3HLT", "Level 3 halted"),
#endif
#if defined (EL3RST)
	ERR_TABLE_ENTRY(EL3RST, "EL3RST", "Level 3 reset"),
#endif
#if defined (ELNRNG)
	ERR_TABLE_ENTRY(ELNRNG, "ELNRNG", "Link number out of range"),
#endif
#if defined (EUNATCH)
	ERR_TABLE_ENTRY(EUNATCH, "EUNATCH", "Protocol driver not attached"),
#endif
#if defined (ENOCSI)
	ERR_TABLE_ENTRY(ENOCSI, "ENOCSI", "No CSI structure available"),
#endif
#if defined (EL2HLT)
	ERR_TABLE_ENTRY(EL2HLT, "EL2HLT", "Level 2 halted"),
#endif
#if defined (EDEADLK)
	ERR_TABLE_ENTRY(EDEADLK, "EDEADLK", "Deadlock condition"),
#endif
#if defined (ENOLCK)
	ERR_TABLE_ENTRY(ENOLCK, "ENOLCK", "No record locks available"),
#endif
#if defined (EBADE)
	ERR_TABLE_ENTRY(EBADE, "EBADE", "Invalid exchange"),
#endif
#if defined (EBADR)
	ERR_TABLE_ENTRY(EBADR, "EBADR", "Invalid request descriptor"),
#endif
#if defined (EXFULL)
	ERR_TABLE_ENTRY(EXFULL, "EXFULL", "Exchange full"),
#endif
#if defined (ENOANO)
	ERR_TABLE_ENTRY(ENOANO, "ENOANO", "No anode"),
#endif
#if defined (EBADRQC)
	ERR_TABLE_ENTRY(EBADRQC, "EBADRQC", "Invalid request code"),
#endif
#if defined (EBADSLT)
	ERR_TABLE_ENTRY(EBADSLT, "EBADSLT", "Invalid slot"),
#endif
#if defined (EDEADLOCK)
	ERR_TABLE_ENTRY(EDEADLOCK, "EDEADLOCK", "File locking deadlock error"),
#endif
#if defined (EBFONT)
	ERR_TABLE_ENTRY(EBFONT, "EBFONT", "Bad font file format"),
#endif
#if defined (ENOSTR)
	ERR_TABLE_ENTRY(ENOSTR, "ENOSTR", "Device not a stream"),
#endif
#if defined (ENODATA)
	ERR_TABLE_ENTRY(ENODATA, "ENODATA", "No data available"),
#endif
#if defined (ETIME)
	ERR_TABLE_ENTRY(ETIME, "ETIME", "Timer expired"),
#endif
#if defined (ENOSR)
	ERR_TABLE_ENTRY(ENOSR, "ENOSR", "Out of streams resources"),
#endif
#if defined (ENONET)
	ERR_TABLE_ENTRY(ENONET, "ENONET", "Machine is not on the network"),
#endif
#if defined (ENOPKG)
	ERR_TABLE_ENTRY(ENOPKG, "ENOPKG", "Package not installed"),
#endif
#if defined (EREMOTE)
	ERR_TABLE_ENTRY(EREMOTE, "EREMOTE", "Object is remote"),
#endif
#if defined (ENOLINK)
	ERR_TABLE_ENTRY(ENOLINK, "ENOLINK", "Link has been severed"),
#endif
#if defined (EADV)
	ERR_TABLE_ENTRY(EADV, "EADV", "Advertise error"),
#endif
#if defined (ESRMNT)
	ERR_TABLE_ENTRY(ESRMNT, "ESRMNT", "Srmount error"),
#endif
#if defined (ECOMM)
	ERR_TABLE_ENTRY(ECOMM, "ECOMM", "Communication error on send"),
#endif
#if defined (EPROTO)
	ERR_TABLE_ENTRY(EPROTO, "EPROTO", "Protocol error"),
#endif
#if defined (EMULTIHOP)
	ERR_TABLE_ENTRY(EMULTIHOP, "EMULTIHOP", "Multihop attempted"),
#endif
#if defined (EDOTDOT)
	ERR_TABLE_ENTRY(EDOTDOT, "EDOTDOT", "RFS specific error"),
#endif
#if defined (EBADMSG)
	ERR_TABLE_ENTRY(EBADMSG, "EBADMSG", "Not a data message"),
#endif
#if defined (ENAMETOOLONG)
	ERR_TABLE_ENTRY(ENAMETOOLONG, "ENAMETOOLONG", "File name too long"),
#endif
#if defined (EOVERFLOW)
	ERR_TABLE_ENTRY(EOVERFLOW, "EOVERFLOW", "Value too large for defined data type"),
#endif
#if defined (ENOTUNIQ)
	ERR_TABLE_ENTRY(ENOTUNIQ, "ENOTUNIQ", "Name not unique on network"),
#endif
#if defined (EBADFD)
	ERR_TABLE_ENTRY(EBADFD, "EBADFD", "File descriptor in bad state"),
#endif
#if defined (EREMCHG)
	ERR_TABLE_ENTRY(EREMCHG, "EREMCHG", "Remote address changed"),
#endif
#if defined (ELIBACC)
	ERR_TABLE_ENTRY(ELIBACC, "ELIBACC", "Cannot access a needed shared library"),
#endif
#if defined (ELIBBAD)
	ERR_TABLE_ENTRY(ELIBBAD, "ELIBBAD", "Accessing a corrupted shared library"),
#endif
#if defined (ELIBSCN)
	ERR_TABLE_ENTRY(ELIBSCN, "ELIBSCN", ".lib section in a.out corrupted"),
#endif
#if defined (ELIBMAX)
	ERR_TABLE_ENTRY(ELIBMAX, "ELIBMAX", "Attempting to link in too many shared libraries"),
#endif
#if defined (ELIBEXEC)
	ERR_TABLE_ENTRY(ELIBEXEC, "ELIBEXEC", "Cannot exec a shared library directly"),
#endif
#if defined (EILSEQ)
	ERR_TABLE_ENTRY(EILSEQ, "EILSEQ", "Illegal byte sequence"),
#endif
#if defined (ENOSYS)
	ERR_TABLE_ENTRY(ENOSYS, "ENOSYS", "Operation not applicable"),
#endif
#if defined (ELOOP)
	ERR_TABLE_ENTRY(ELOOP, "ELOOP", "Too many symbolic links encountered"),
#endif
#if defined (ERESTART)
	ERR_TABLE_ENTRY(ERESTART, "ERESTART", "Interrupted system call should be restarted"),
#endif
#if defined (ESTRPIPE)
	ERR_TABLE_ENTRY(ESTRPIPE, "ESTRPIPE", "Streams pipe error"),
#endif
#if defined (ENOTEMPTY)
	ERR_TABLE_ENTRY(ENOTEMPTY, "ENOTEMPTY", "Directory not empty"),
#endif
#if defined (EUSERS)
	ERR_TABLE_ENTRY(EUSERS, "EUSERS", "Too many users"),
#endif
#if defined (ENOTSOCK)
	ERR_TABLE_ENTRY(ENOTSOCK, "ENOTSOCK", "Socket operation on non-socket"),
#endif
#if defined (EDESTADDRREQ)
	ERR_TABLE_ENTRY(EDESTADDRREQ, "EDESTADDRREQ", "Destination address required"),
#endif
#if defined (EMSGSIZE)
	ERR_TABLE_ENTRY(EMSGSIZE, "EMSGSIZE", "Message too long"),
#endif
#if defined (EPROTOTYPE)
	ERR_TABLE_ENTRY(EPROTOTYPE, "EPROTOTYPE", "Protocol wrong type for socket"),
#endif
#if defined (ENOPROTOOPT)
	ERR_TABLE_ENTRY(ENOPROTOOPT, "ENOPROTOOPT", "Protocol not available"),
#endif
#if defined (EPROTONOSUPPORT)
	ERR_TABLE_ENTRY(EPROTONOSUPPORT, "EPROTONOSUPPORT", "Protocol not supported"),
#endif
#if defined (ESOCKTNOSUPPORT)
	ERR_TABLE_ENTRY(ESOCKTNOSUPPORT, "ESOCKTNOSUPPORT", "Socket type not supported"),
#endif
#if defined (EOPNOTSUPP)
	ERR_TABLE_ENTRY(EOPNOTSUPP, "EOPNOTSUPP", "Operation not supported on transport endpoint"),
#endif
#if defined (EPFNOSUPPORT)
	ERR_TABLE_ENTRY(EPFNOSUPPORT, "EPFNOSUPPORT", "Protocol family not supported"),
#endif
#if defined (EAFNOSUPPORT)
	ERR_TABLE_ENTRY(EAFNOSUPPORT, "EAFNOSUPPORT", "Address family not supported by protocol"),
#endif
#if defined (EADDRINUSE)
	ERR_TABLE_ENTRY(EADDRINUSE, "EADDRINUSE", "Address already in use"),
#endif
#if defined (EADDRNOTAVAIL)
	ERR_TABLE_ENTRY(EADDRNOTAVAIL, "EADDRNOTAVAIL","Cannot assign requested address"),
#endif
#if defined (ENETDOWN)
	ERR_TABLE_ENTRY(ENETDOWN, "ENETDOWN", "Network is down"),
#endif
#if defined (ENETUNREACH)
	ERR_TABLE_ENTRY(ENETUNREACH, "ENETUNREACH", "Network is unreachable"),
#endif
#if defined (ENETRESET)
	ERR_TABLE_ENTRY(ENETRESET, "ENETRESET", "Network dropped connection because of reset"),
#endif
#if defined (ECONNABORTED)
	ERR_TABLE_ENTRY(ECONNABORTED, "ECONNABORTED", "Software caused connection abort"),
#endif
#if defined (ECONNRESET)
	ERR_TABLE_ENTRY(ECONNRESET, "ECONNRESET", "Connection reset by peer"),
#endif
#if defined (ENOBUFS)
	ERR_TABLE_ENTRY(ENOBUFS, "ENOBUFS", "No buffer space available"),
#endif
#if defined (EISCONN)
	ERR_TABLE_ENTRY(EISCONN, "EISCONN", "Transport endpoint is already connected"),
#endif
#if defined (ENOTCONN)
	ERR_TABLE_ENTRY(ENOTCONN, "ENOTCONN", "Transport endpoint is not connected"),
#endif
#if defined (ESHUTDOWN)
	ERR_TABLE_ENTRY(ESHUTDOWN, "ESHUTDOWN", "Cannot send after transport endpoint shutdown"),
#endif
#if defined (ETOOMANYREFS)
	ERR_TABLE_ENTRY(ETOOMANYREFS, "ETOOMANYREFS", "Too many references: cannot splice"),
#endif
#if defined (ETIMEDOUT)
	ERR_TABLE_ENTRY(ETIMEDOUT, "ETIMEDOUT", "Connection timed out"),
#endif
#if defined (ECONNREFUSED)
	ERR_TABLE_ENTRY(ECONNREFUSED, "ECONNREFUSED", "Connection refused"),
#endif
#if defined (EHOSTDOWN)
	ERR_TABLE_ENTRY(EHOSTDOWN, "EHOSTDOWN", "Host is down"),
#endif
#if defined (EHOSTUNREACH)
	ERR_TABLE_ENTRY(EHOSTUNREACH, "EHOSTUNREACH", "No route to host"),
#endif
#if defined (EALREADY)
	ERR_TABLE_ENTRY(EALREADY, "EALREADY", "Operation already in progress"),
#endif
#if defined (EINPROGRESS)
	ERR_TABLE_ENTRY(EINPROGRESS, "EINPROGRESS", "Operation now in progress"),
#endif
#if defined (ESTALE)
	ERR_TABLE_ENTRY(ESTALE, "ESTALE", "Stale NFS file handle"),
#endif
#if defined (EUCLEAN)
	ERR_TABLE_ENTRY(EUCLEAN, "EUCLEAN", "Structure needs cleaning"),
#endif
#if defined (ENOTNAM)
	ERR_TABLE_ENTRY(ENOTNAM, "ENOTNAM", "Not a XENIX named type file"),
#endif
#if defined (ENAVAIL)
	ERR_TABLE_ENTRY(ENAVAIL, "ENAVAIL", "No XENIX semaphores available"),
#endif
#if defined (EISNAM)
	ERR_TABLE_ENTRY(EISNAM, "EISNAM", "Is a named type file"),
#endif
#if defined (EREMOTEIO)
	ERR_TABLE_ENTRY(EREMOTEIO, "EREMOTEIO", "Remote I/O error"),
#endif
#if defined (ENOFS)
	ERR_TABLE_ENTRY(ENOFS, "ENOFS", "No filesystem detected on device"),
#endif
#if defined (ENOMALLOC)
	ERR_TABLE_ENTRY(ENOMALLOC, "ENOMALLOC", "Unable to allocate memory"),
#endif
	ERR_TABLE_ENTRY_NULL(0, NULL, NULL)
};

//---------------------------------------------------

char * strerror (int errnum) {
	const char *msg;
	static char buf[32];

	if (error_names == NULL)
		init_error_tables();
	if (errnum < 0 || errnum >= sys_nerr)
		msg = NULL;
	else if (sys_errlist == NULL || sys_errlist[errnum] == NULL) {
		/* In range, but no sys_errlist or no ERR_TABLE_ENTRY at this index. */
		sprintf (buf, "Error %d", errnum);
		msg = buf;
	} else
		msg = (char *)sys_errlist[errnum];
	return msg;
}

int strtoerrno (const char *name) {
	int errnoval = 0;

    if (name != NULL) {
        if (error_names == NULL) {
			init_error_tables();
  		}
        for (errnoval = 0; errnoval < num_error_names; errnoval++) {
			if ((error_names[errnoval] != NULL) && (strcmp(name, error_names[errnoval]) == 0)) {
				break;
			}
		}
        if (errnoval == num_error_names) {
			errnoval = 0;
		}
	}
    return errnoval;
}

void init_error_tables (void) {
	const struct error_info *eip;
 	int nbytes;

	puts("Initializing error tables");

    /* If we haven't already scanned the error_table once to find the maximum
       errno value, then go find it now. */

    if (num_error_names == 0) {
        for (eip = error_table; eip->name != NULL; eip++) {
			if (eip->value >= num_error_names) {
  	      		num_error_names = eip->value + 1;
  	    	}
  		}
	}
	printf("Error names found: %d\r\n", num_error_names);

    /* Now attempt to allocate the error_names table, zero it out, and then
       initialize it from the statically initialized error_table. */

    nbytes = num_error_names * sizeof (char *);
	if ((error_names = (const char **)malloc(nbytes)) != NULL) {
		memset(error_names, 0, nbytes);
		for (eip = error_table; eip->name != NULL; eip++) {
			error_names[eip->value] = eip->name;
		}
	} else
		puts("[!] Unable to allocate error name table");

	/* Now attempt to allocate the sys_errlist table, zero it out, and then
     initialize it from the statically initialized error_table. */

	nbytes = num_error_names * sizeof (char *);
	if ((sys_errlist = (const char **) malloc (nbytes)) != NULL) {
		memset(sys_errlist, 0, nbytes);
		sys_nerr = num_error_names;
		for (eip = error_table; eip -> name != NULL; eip++) {
			sys_errlist[eip -> value] = eip -> msg;
		}
	} else
		puts("[!] Unable to allocate system error table");
}

int errno_max (void) {
	int maxsize;

    if (error_names == NULL) {
        init_error_tables();
	}

    maxsize = MAX(sys_nerr, num_error_names);
    return (maxsize - 1);
}

core_initcall(init_error_tables);
