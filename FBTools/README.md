# FBTools 3

This is a Hex-Rays IDA plugin for Frostbite games, and is really only useful for people reversing frostbite games.

<br>
The plugin used IChooseYou's FBTools plugin as a base, you can find it on unknowncheats.me frostbite engine section, and maybe also wiki.
Most stuff this plugin uses is custom, and barely has any original code from ICY left.

I dont like the way i did the TypeFactory, but its not that bad and works allright. Probably isnt even a factory by programming terms.<br>
Should probably have some interface like thing instead for this.

## Some stuff to do

- add a ea_t GetParent() to IFbType!
- add some way to get fields, and add fields to class types


## Some ideas Ideas
- [x] Automatically rename typeinfo with mangled names
- [x] rename default instance if it has one
- [ ] create local ida type from field data (seems annoying with the ida sdk docs)

<br>

- [ ] dump to FBT or cpp format

<br>

- [x] rename message vtables(might have an issue with base messages...)
- [x] rename message constructors, and seperate defaultmessage constructor (works but is a bit broken)
- [x] rename message handler functions (has an issue as of now...)


## Compiling

This should work:

1. Load this project into VisualStudio (i use 2019 for now)

2. Fix ida sdk paths in solution settings (Make sure you have right configuration and platform!)
	a. `VC++ Directories -> Include Directories`
	b. `VC++ Directories -> Library Directories` (Seems to be some extra stuff here, might be able to remove that (damn vs))

3. Compile
#### Authors:
- txt 
- ICY (used as a base)
