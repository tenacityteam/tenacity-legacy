tenacity(1)

# NAME

tenacity - Graphical cross-platform audio editor

# SYNOPSIS

*tenacity* -help++
*tenacity* -version

*tenacity* [-blocksize nnn] -test++
*tenacity* [-blocksize nnn] [ _AUDIO-FILE_ ] *...*

# OPTIONS

*-h --help*
	Displays a brief list of command line options.

*-v --version*
	Displays the Tenacity version number.

*-t --test*
	Runs self diagnostics tests (only present in development builds).

*-b --blocksize nnn*
	Sets the Tenacity block size for writing files to disk to nnn bytes.

# DESCRIPTION

*Tenacity* is a graphical audio editor. This man page does not describe all of
the features of Tenacity or how to use it; for this, see the html documentation
that came with the program, which should be accessible from the Help menu.
This man page describes the Unix-specific features, including special files and
environment variables.

Tenacity currently uses *libsndfile* to open many uncompressed audio formats
such as WAV, AIFF, and AU, and it can also be linked to *libmad*, *libvorbis*,
and *libflac*, to provide support for opening MP2/3, Ogg Vorbis, and FLAC
files, respectively. *LAME*, *libvorbis*, *libflac* and *libtwolame* provide
facilities to export files to all these formats as well.

Tenacity is primarily an interactive, graphical editor, not a batch-processing
tool. Whilst there is a basic batch processing tool it is experimental and
incomplete. If you need to batch-process audio or do simple edits from the
command line, using *sox* or *ecasound* driven by a bash script will be much
more powerful than tenacity.

# FILES

_~/.audacity-data/tenacity.cfg_
	Per-user configuration file.

_/var/tmp/tenacity-<user>/_
	Default location of Tenacity's temp directory, where <user> is your
	username. If this location is not suitable (not enough space in
	/var/tmp, for example), you should change the temp directory in the
	Preferences and restart Tenacity. Tenacity is a disk-based editor, so
	the temp directory is very important: it should always be on a fast
	(local) disk with lots of free space.

	Note that older versions of Tenacity put the temp directory inside of
	the user's home directory. This is undesirable on many systems, and
	using some directory in /tmp is recommended.

	On many modern Linux systems all files in /tmp/ will be deleted each
	time the system boots up, which makes recovering a recording that was
	going on when the system crashed much harder. This is why the default
	is to use a directory in /var/tmp/ which will not normally be deleted
	by the system. Open the Preferences to check.

# SEARCH PATH

When looking for plug-ins, help files, localization files, or other
configuration files, Tenacity searches the following locations, in this order:

_AUDACITY_PATH_
	Any directories in the _AUDACITY_PATH_ environment variable will be
	searched before anywhere else.

_._
	The current working directory when Tenacity is started.

_~/.audacity-data/Plug-Ins_

_<prefix>/share/tenacity_
	The system-wide Tenacity directory, where <prefix> is usually /usr or
	/usr/local, depending on where the program was installed.

_<prefix>/share/doc/tenacity_
	The system-wide Tenacity documentation directory, where <prefix> is
	usually /usr or /usr/local, depending on where the program was
	installed.

For localization files in particular (i.e. translations of Tenacity into other
languages), Tenacity also searches _<prefix>/share/locale_.

# PLUG-INS

Tenacity supports two types of plug-ins on Unix: LADSPA and Nyquist plug-ins.
These are generally placed in a directory called _plug-ins_ somewhere on the
search path (see above).

LADSPA plug-ins can either be in the plug-ins directory, or alternatively in a
_ladspa_ directory on the search path if you choose to create one. Tenacity
will also search the directories in the _LADSPA_PATH_ environment variable for
additional LADSPA plug-ins.

Nyquist plug-ins can either be in the plug-ins directory, or alternatively in a
_nyquist_ directory on the search path if you choose to create one.

# VERSION

This man page documents Tenacity version 1.3.5.

# LICENSE

Tenacity is distributed under the GPL, however some of the libraries it links
to are distributed under other free licenses, including the LGPL and BSD
licenses.

# AUTHORS

Tenacity would not be possible without the Audacity project,
wxWidgets, libsndfile, and many of the other libraries it is
built upon. You can find a full list of contributors on GitHub:

https://github.com/tenacityteam/tenacity/graphs/contributors

The Audacity project leaders include Dominic Mazzoni, Matt Brubeck,
James Crook, Vaughan Johnson, Leland Lucius, and Markus Meyer,
but dozens of others have contributed.

http://www.audacityteam.org/about/credits/
