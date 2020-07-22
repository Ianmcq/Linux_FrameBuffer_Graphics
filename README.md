# Linux_FrameBuffer_Graphics
A graphics engine for linux TTY console written in C++.

To try, first invoke this command in your console...

<code>usermod -a -G video uname</code>

Where username is your desktop account username.
This is simply to allow your account to write to /dev/fb0.
You may need to append <code>sudo </code> to the beginning of that command if you get a permission error.

Next you must press ctrl-alt-F1 to enter a TTY console (hold alt and press left arrow key to leave TTY).
Navigate to the directory you downloaded or cloned the repository to.
Then simply run the build.sh script to compile the demo program.
Once that completes, run the resulting FBG_Demo program and enjoy the graphics.

If you liked this, feel free to use the code for anything.
