package api.communication;

import com.sun.jna.LastErrorException;
import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.NativeLong;
import com.sun.jna.Pointer;

/**
 * Permet l'accès à certaines fonctions de la libc (ioctl open, close, read,
 * write)
 * 
 * @author mickael
 */
public abstract class LibC implements Library {
	public static final int O_RDWR = 0x02;
	public static final int O_SYNC = 0x101000;

	public static native int ioctl(int fd, int cmd, int arg) throws LastErrorException;

	public static native int ioctl(int fd, int cmd, Pointer arg) throws LastErrorException;

	public static native int open(String path, int flags) throws LastErrorException;

	public static native int close(int fd) throws LastErrorException;

	public static native NativeLong write(int fd, byte[] buffer, NativeLong count) throws LastErrorException;

	public static native NativeLong read(int fd, byte[] buffer, NativeLong count) throws LastErrorException;

	public static native Pointer mmap(Pointer addr, NativeLong len, int prot, int flags, int filedes, NativeLong offset) throws LastErrorException;

	public static native void munmap(Pointer addr, NativeLong len);

	static {
		Native.register("c");
	}

	public static int write(int fd, byte[] buffer, int len) {
		return write(fd, buffer, new NativeLong(len)).intValue();
	}

	public static int read(int fd, byte[] buffer, int len) {
		return read(fd, buffer, new NativeLong(len)).intValue();
	}
}
