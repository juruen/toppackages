toppackages
===========

This a tool to list all the deb packages in your system ordered by most recently
used. Although it's still a proof of concept.

Whenever I'm trying a new library, tool or application, I end up with a whole
bunch of new packages installed on my Debian machine that I don't use any
longer. I know that aptitude already has some sort of support to avoid this
situation based on tags. But that requires me to explicitly use this feature and
doesn't help with the currently installed packages.

The idea behind toppackages is tracking which packages have been used recently
and report to the user those that haven't been used at all during a given
period. It's then up to you to remove them or not.

It uses systemtap to figure out which packages are being used under the hood.
The idea is simple, add a hook to the open system call and check whether the
file that is being opened belongs to an installed package or not. If it does
belong to an installed package, mark it and move it to the front of the list.

There is an ordered list that contains the packages. To make this work
reasonably fast, all the files that are contained in a deb package are stored
in a hash where the key is the file path and the value is a pointer to a node
of the ordered list. This relation of deb packages and files is created from
/var/lib/dpkg/info/\*.list.

When the open syscall is called, we check whether the file is in this hash or
not, and if it is, we update its time stamp and the node is moved to the front
of the list. Both operations are done in constant time.

There's still a bit of work to go from a proof-of-concept to a fully-fledged
service but the foundations are there already.

To show you how this works, I've done a sort of "top-like" tool where the
packages which are been currently used are displayed. If you want to check it
out, here's a youtube video.
