#!/usr/bin/env python

from __future__ import with_statement

import os
import sys
import errno

from fuse import FUSE, FuseOSError, Operations


class Passthrough(Operations):
    def __init__(self, root):
        self.root = root
        if not root.endswith("/"):
            root = root + "/"
        self.mpath = os.path.join(root, "master")
        self.spath = os.path.join(root, "slave")
        if not os.path.exists(self.mpath):
            os.mkdir(self.mpath)
        if not os.path.exists(self.spath):
            os.mkdir(self.spath)

    # Helpers
    # =======

    def _full_path(self, partial):
        if partial.startswith("/"):
            partial = partial[1:]
        path = os.path.join(self.root, partial)
        return path

    # Filesystem methods
    # ==================

    def chmod(self, path, mode):
        full_path = self._full_path(path)
        if "/master/" in full_path:
            os.chmod(full_path.replace("/master/","/slave/"), mode)
            return os.chmod(full_path, mode)
        return

    def chown(self, path, uid, gid):
        full_path = self._full_path(path)
        if "/master/" in full_path:
            os.chown(full_path.replace("/master/","/slave/"), uid, gid)
            return os.chown(full_path, uid, gid)
        return

    def getattr(self, path, fh=None):
        full_path = self._full_path(path)
        st = os.lstat(full_path)
        return dict((key, getattr(st, key)) for key in ('st_atime', 'st_ctime',
         'st_gid', 'st_mode', 'st_mtime', 'st_nlink', 'st_size', 'st_uid'))

    def readdir(self, path, fh):
        full_path = self._full_path(path)

        dirents = ['.', '..']
        if os.path.isdir(full_path):
            dirents.extend(os.listdir(full_path))
        return dirents

    def rmdir(self, path):
        full_path = self._full_path(path)
        if "/master/" in full_path:
            os.rmdir(full_path.replace("/master/","/slave/"))
            return os.rmdir(full_path)
        return 

    def mkdir(self, path, mode):
        full_path = self._full_path(path)
        if "/master/" in full_path:
            os.mkdir(full_path.replace("/master/","/slave/"), mode)
            return os.mkdir(full_path, mode)
        return 

    def unlink(self, path):
        full_path = self._full_path(path)
        if "/master/" in full_path:
            os.unlink(full_path.replace("/master/","/slave/"))
            return os.unlink(full_path)
        return

    def rename(self, old, new):
        full_path = self._full_path(old)
        if "/master/" in full_path:
            os.rename(full_path.replace("/master/","/slave/"), self._full_path(new).replace("/master/","/slave/"))
            return os.rename(full_path, self._full_path(new))

    # File methods
    # ============

    def open(self, path, flags):
        full_path = self._full_path(path)
        return os.open(full_path, flags)

    def create(self, path, mode, fi=None):
        full_path = self._full_path(path)
        if "/master/" in full_path:
            os.open(full_path.replace("/master/","/slave/"), os.O_WRONLY | os.O_CREAT, mode)
            return os.open(full_path, os.O_WRONLY | os.O_CREAT, mode)
        return

    def read(self, path, length, offset, fh):
        os.lseek(fh, offset, os.SEEK_SET)
        return os.read(fh, length)

    def write(self, path, buf, offset, fh):
        full_path = self._full_path(path)
        if "/master/" in full_path:
            fd = os.open(full_path.replace("/master/","/slave/"), os.O_WRONLY | os.O_CREAT)
            os.lseek(fd, offset, os.SEEK_SET)
            os.lseek(fh, offset, os.SEEK_SET)
            os.write(fd, buf)
            return os.write(fh, buf)
        return

    def release(self, path, fh):
        return os.close(fh)

    def __del__(self):
        if os.path.exists(self.mpath):
            os.rmdir(self.mpath)
        if os.path.exists(self.spath):
            os.rmdir(self.spath)

def main(mountpoint, root):
    FUSE(Passthrough(root), mountpoint, nothreads=True, foreground=True)

if __name__ == '__main__':
    main(sys.argv[2], sys.argv[1])
