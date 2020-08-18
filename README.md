# FBTools

This is a Hex-Rays IDA plugin (7.2) for Frostbite games, and is really only useful for people reversing frostbite games.
<br>
<br>
The plugin used IChooseYou's FBTools plugin as a base, you can find it on unknowncheats.me frostbite engine section, and maybe also wiki.
Most stuff this plugin uses is custom, and barely has any original code from ICY left.
<br>

I dont like the way i did the TypeFactory, but its not that bad and works allright. Probably isnt even a factory by programming terms.<br>
Should probably have some interface like thing instead for this.


## Supported/Not supported:
- [x] Frostbite 2
	- Tested:
		- bf3 release (32 bit)
- [x] fb2013
	- Tested: 
		- bf4 (64 bit)
	- Not tested:
		- bf4 (32 bit)
- [x] fb2014 
	- Tested: 
		- MEC Closed Beta
		- bf1
	- Not tested:
		- early bfv
		- NFS Rivals
- [ ] fb2017 
	- Not tested:
		- anthem closed alpha
- [x] fb2018 
	- Tested: 
		- late bfv
<br>

maybe some more games i didnt test
<br>


## Some stuff to do

- Add dumpers
- Finish ida struct stuff (`Actions/CreateTypes.h`)

## Some Features
- Print types to ida output
- Rename typeinfo with mangled names
- Find message handlers
- Rename message vtables, and some related functions

## Some Ideas maybe (old stuff)
<details>

- [x] Automatically rename typeinfo with mangled names
- [x] rename default instance if it has one
- [ ] create local ida type from field data (seems annoying with the ida sdk docs)
<br>

- [ ] dump to FBT or cpp format
<br>

- [x] rename message vtables(might have an issue with base messages...)
- [x] rename message constructors, and seperate defaultmessage constructor (works but is a bit broken)
- [x] rename message handler functions (has an issue as of now...)

</details>

## Installing

1. Copy dll<br>
	a. Copy `Bin/FBTools64.dll` to ida plugin dir (for 64 bit support)<br>
	b. Copy `Bin/FBTools.dll` to ida plugin dir (for 32 bit support)<br>

2. Run IDA!


## Compiling

This should work:

1. Load this project into VisualStudio (i use 2019 for now)

2. Fix ida sdk paths in solution settings (Make sure you have right configuration and platform!)<br>
	- `VC++ Directories -> Include Directories`<br>
	- `VC++ Directories -> Library Directories` (Seems to be some extra stuff here, might be able to remove that (damn vs))

3. Compile

## Usage

In the toolbar you will have a new entry "FBTools"

!["image showing it"](https://i.imgur.com/h8moMi8.png)


once typeinfo is found, more options are aded in this submenu!

<br>

### Credits:
- jd62 (some fb2018 stuff)
- ICY
- Reunion (bf4 sdk)
- Some guy on uc for ptr fix code (not used)

Also credits to anyone else involed i guess

#### Authors:
- txt 
- ICY (used as a base)
