CELL
====

Cell is a multi-Kinect interactive installation by James Alliban and Keiichi Matsuda. You can see it in action and learn more about it here:

http://installcell.com

SOFTWARE
--------

Cell Server is built using openFrameworks 0.8.0 and Visual Studio 2012 (and Xcode)

Cell Client is built using openFrameworks 0.7.0 and Visual Studio 2010

The piece uses the official KinectSDK 1.5.


ADDONS
------

The following addons are used:

ofxKinectNui by sadmb

ofxUI by Reza Ali

ofxUnicode by Christopher Baker

ofxFontStash by Oriol Ferrer Mesi�

ofxXmlSettings

ofxOSC


NOTES
-----

A small alteration to ofURLFileLoader::start() is required to download xml tag data from a server.

Replace:

startThread(true, false);

With:

startThread(false, false);