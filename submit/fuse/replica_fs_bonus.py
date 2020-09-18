#!/usr/bin/env python

from __future__ import with_statement

import os
import sys
import errno
from datetime import datetime

from fuse import FUSE, FuseOSError, Operations


class Passthrough(Operations):
    def __init__(self, root, reps):
        self.root = root
        self.reps = int(reps)
        self.rr = 0
        self.fp = open("replica_fs.log", "w")
        self.fp.write("Time \t\t\t\tRequest Type \tHandler \n")
        if not root.endswith("/"):
            root = root + "/"
        mpath = os.path.join(root, "master")
        self.mpath = mpath
        if not os.path.exists(mpath):
            os.mkdir(mpath)
        for i in range(int(reps)):
            spath = os.path.join(root, "slave_" + str(i))
            if not os.path.exists(spath):
                os.mkdir(spath)

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
            ret = os.rmdir(full_path)
            self.fp.write(str(datetime.now()) + "\trmdir() \t" + full_path + "\n")
            for i in range(self.reps):
                spath = full_path.replace("/master/", "/slave_"+str(i)+"/")
                os.rmdir(spath)
                self.fp.write(str(datetime.now()) + "\trmdir() \t" + spath + "\n")
            return ret
        return 

    def mkdir(self, path, mode):
        full_path = self._full_path(path)
        if "/master/" in full_path:
            ret = os.mkdir(full_path, mode)
            self.fp.write(str(datetime.now()) + "\tmkdir() \t" + full_path + "\n")
            for i in range(self.reps):
                spath = full_path.replace("/master/", "/slave_"+str(i)+"/")
                os.mkdir(spath, mode)
                self.fp.write(str(datetime.now()) + "\tmkdir() \t" + spath + "\n")
            return ret
        return

    def unlink(self, path):
        full_path = self._full_path(path)
        if "/master/" in full_path:
            ret = os.unlink(full_path)
            self.fp.write(str(datetime.now()) + "\tunlink() \t" + full_path + "\n")
            for i in range(self.reps):
                spath = full_path.replace("/master/", "/slave_"+str(i)+"/")
                os.unlink(spath)
                self.fp.write(str(datetime.now()) + "\tunlink() \t" + spath + "\n")
            return ret
        return 

    def rename(self, old, new):
        full_path = self._full_path(old)
        if "/master/" in full_path:
            ret = os.rename(full_path, self._full_path(new))
            self.fp.write(str(datetime.now()) + "\trename() \t" + full_path + "\n")
            for i in range(self.reps):
                spath = full_path.replace("/master/", "/slave_"+str(i)+"/")
                os.rename(spath, self._full_path(new).replace("/master/", "/slave_"+str(i)+"/"))
                self.fp.write(str(datetime.now()) + "\trename() \t" + spath + "\n")
            return ret

    # File methods
    # ============
    def open(self, path, flags):
        full_path = self._full_path(path)
        return os.open(full_path, flags)

    def create(self, path, mode, fi=None):
        full_path = self._full_path(path)
        if "/master/" in full_path:
            ret = os.open(full_path, os.O_WRONLY | os.O_CREAT, mode)
            self.fp.write(str(datetime.now()) + "\tcreate() \t" + full_path + "\n")
            for i in range(self.reps):
                spath = full_path.replace("/master/", "/slave_"+str(i)+"/")
                os.open(spath, os.O_WRONLY | os.O_CREAT, mode)
                self.fp.write(str(datetime.now()) + "\tcreate() \t" + spath + "\n")
            return ret
        return

    def read(self, path, length, offset, fh):
        full_path = self._full_path(path)
        spath = ""
        if "/master/" in full_path:
            spath = full_path.replace("/master/", "/slave_"+str(self.rr)+"/")
        else:
            ind = full_path.index("/slave_") + 7
            spath = full_path[:ind] + str(self.rr) + full_path[ind+1:]
        fd = os.open(spath, os.O_RDONLY)
        os.lseek(fd, offset, os.SEEK_SET)
        ret = os.read(fd, length)
        self.fp.write(str(datetime.now()) + "\tread() \t\t" + spath + "\n")
        self.rr = self.rr+1 if self.rr!=self.reps-1 else 0
        return ret

    def write(self, path, buf, offset, fh):
        full_path = self._full_path(path)
        if "/master/" in full_path:
            os.lseek(fh, offset, os.SEEK_SET)
            ret = os.write(fh, buf)
            self.fp.write(str(datetime.now()) + "\twrite() \t" + full_path + "\n")
            for i in range(self.reps):
                spath = full_path.replace("/master/", "/slave_"+str(i)+"/")
                fd = os.open(spath, os.O_WRONLY | os.O_CREAT)
                os.lseek(fd, offset, os.SEEK_SET)
                os.write(fd, buf)
                self.fp.write(str(datetime.now()) + "\twrite() \t" + spath + "\n")
            return ret
        return

    def release(self, path, fh):
        return os.close(fh)

    def __del__(self):
        self.fp.close()
        for i in range(self.reps):
            spath = self.mpath.replace("/master", "/slave_" + str(i))
            if os.path.exists(spath):
                os.rmdir(spath)
        if os.path.exists(self.mpath):
            os.rmdir(self.mpath)

def main(mountpoint, root, reps):
    FUSE(Passthrough(root, reps), mountpoint, nothreads=True, foreground=True)

if __name__ == '__main__':
    main(sys.argv[2], sys.argv[1], sys.argv[3])
