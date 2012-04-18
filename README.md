# PVCS-merge #

PVCS-merge is a little program I wrote around early 2010 to let me use Winmerge instead of the diff tool that comes with PVCS. It was also something I did to learn a little more C, but don't expect the code to be any good as I didn't learn much.

PVCS is the VCS we were using at work before we switched to Perforce. It mostly sucked as there is no in-built merge command and the diff tool was plain awful. Thankfully this little program made merges semi-bareable.

## How it works ##

PVCS has hard coded into it a path to an executable for its internal file diff tool. There is no config option for this. It's rubbish.

My program sits in the place of the official diff tool (just called pvcsmerge.exe) and reads the input file it's given (because call arguments are too mainstream for PVCS to use...), and passes them as arguments to whatever diff tool you prefer.

## Instructions ##

Building should work with the build.bat that's included in the root folder of this project. If not, compile the stuff in /src into an executable called pvcsmerge.exe

I made an install.bat which should at the least provide a hint as to where files should end up.

## TODO / Known Issues ##

- The command in the config file cannot contain spaces in the path to the diff tool, should fix this at some point.
- Line length is hard coded. This was just me being lazy
- Perhaps have an option to fork instead of a system call and wait would be preferable
- I should read a book on C some time and learn the language properly, rather than just this afternoon hack at work to entertain myself

