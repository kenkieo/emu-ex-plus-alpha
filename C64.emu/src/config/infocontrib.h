/*
 * infocontrib.h - Text of contributors to VICE, as used in info.c
 *
 * Autogenerated by geninfocontrib_h.sh, DO NOT EDIT !!!
 *
 * Written by
 *  Marco van den Heuvel <blackystardust68@yahoo.com>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#ifndef VICE_INFOCONTRIB_H
#define VICE_INFOCONTRIB_H

const char info_contrib_text[] =
"\n"
"  VICE derives from X64, the first Commodore 64 emulator for the\n"
"  X Window System. Here is an informal list of the people who were\n"
"  mostly involved in the development of X64 and VICE:\n"
"\n"
"  The VICE core team:\n"
"\n"
"  Andreas Matthies <andreas.matthies@gmx.net>\n"
"  Improved the datasette support, the VIC20 video emulation and\n"
"  some UI stuff in the Win32 and DOS ports. He also wrote the\n"
"  BeOS port and implemented video/audio capture support. Improved\n"
"  history recording/playback and implemented support for video\n"
"  recording and the netlink feature. Made the Win32 user\n"
"  changable keyboard shortcut system. Improved CIA and VIA\n"
"  emulation. Worked on x64sc, especially interrupt timing. Wrote\n"
"  test programs. Various bug(fixe)s. ;-)\n"
"\n"
"  Martin Pottendorder <pottendo@gmx.net>\n"
"  Implemented the Gnome Port based on Oliver Schaertels GTK+\n"
"  port. Added support code for internationalization based on\n"
"  gettext. Improved the *nix fullscreen support. Translated the\n"
"  UI to German. Implemented the fliplists + UI (*nix).\n"
"\n"
"  Marco van den Heuvel <blackystardust68@yahoo.com>\n"
"  Translated the UI to Dutch. Made the internationalization\n"
"  support for the Win32 and Amiga ports. Wrote the GEO-RAM and\n"
"  RamCart cartridge code. Wrote the c64 +60K, +256K and 256K\n"
"  memory expansions code. Wrote the pet REU code. Wrote the plus4\n"
"  memory expansions code. Made the ethernet support for the DOS\n"
"  port. Maintains the QNX 4.x, QNX 6.x, Solaris, Openserver,\n"
"  Unixware, Minix 3.x, Amiga, Syllable and OS/2 binary ports.\n"
"  Maintains the Win64 and Open Watcom project files. Maintains\n"
"  the SDL port(s). Added new .crt support. Added new screenshot\n"
"  formats. Added new sound recording support. Added SIDcart\n"
"  support for xpet, xplus4 and xvic. Improved the MMC64\n"
"  emulation. Added 2 MHz mode and banks 2/3 support for x128.\n"
"  Added the various userport joystick emulations. Added text copy\n"
"  and paste support to the Amiga and BeOS ports. Added DQBB and\n"
"  ISEPIC cartridge support. Added SFX Sound Sampler and SFX Sound\n"
"  Expander support. Added PCI support to the Amiga and DOS ports.\n"
"  Rewrote the sound system into a modular one, added always mono\n"
"  and always stereo support for the sound output. Added the RTC\n"
"  system. Added digiblaster support. Added 3rd SID support. Added\n"
"  the 6309 CPU emulation. Added the 65(S)C02 emulation. And\n"
"  lots of other fixes and improvements.\n"
"\n"
"  Fabrizio Gennari <fabrizio.ge@tiscalinet.it>\n"
"  Added some improvements to the DOS and GTK+ ports. Changed the\n"
"  Windows video to use GDI as fallback, making it compile without\n"
"  DX if needed. Fixed the t64 support. Added monitor window support\n"
"  using VTE to the GTK+ GUI.\n"
"\n"
"  Daniel Kahlin <daniel@kahlin.net>\n"
"  Worked on DTV VIC emulation, palette, DTV SID support in resid,\n"
"  better DMA/Blitter support and did lots of refactoring. Added\n"
"  new monitor commands and features. Improved the VIC emulation\n"
"  for xvic. Made MIDI driver code for Win32. Rewrote the xvic\n"
"  cartridge system. Added Mega-Cart and Final Expansion V3.2\n"
"  support to xvic. Wrote large parts of the new VIC-II emulation\n"
"  used in x64sc, especially the dot clock domain emulation. Wrote\n"
"  many test programs for hardware analysis.\n"
"\n"
"  Groepaz <groepaz@gmx.net>\n"
"  Added new more precise CRT emulation. Added support for the new\n"
"  cartridge system and many new cartridges. Fixed up parts of\n"
"  cartconv, c1541 and petcat. Added video to audio leak sound support.\n"
"  Implemented many bug fixes. \n"
"  Wrote test programs. Updated this document after a long period\n"
"  of outdated mess.\n"
"\n"
"  Errol Smith <strobey@users.sourceforge.net>\n"
"  Improved VDC emulation.\n"
"\n"
"  Olaf Seibert <rhialto@falu.nl>\n"
"  Contributed some PET, including PET DWW hires, Xaw, lightpen,\n"
"  hardware scaling, and disk drive patches. Added proper SuperPET\n"
"  support, including 6809/6309 CPU emulation. Maintains the Xaw UI.\n"
"\n"
"  Marcus Sutton <loggedoubt@gmail.com>\n"
"  Made some console, dialog and joystick fixes for the BeOS port.\n"
"  Maintains the BeOS port.\n"
"\n"
"  Kajtar Zsolt <soci@c64.rulez.org>\n"
"  Wrote the IDE64 interface emulation, FD2000/4000 drive emulation, SCPU64\n"
"  emulation and alot of fixes. Improved the mouse support. Added drive\n"
"  burst modification support. Added 1541 drive sounds emulation. Improved\n"
"  c64 cart emulation.\n"
"\n"
"\n"
"  Former/inactive team members:\n"
"\n"
"\n"
"  Ulrich Schulz <peiselulli@t-online.de>\n"
"  Maintains the Dingoo port(s).\n"
"\n"
"  Stefan Haubenthal <polluks@users.sourceforge.net>\n"
"  Added some Amiga fixes.\n"
"\n"
"  Thomas Giesel <skoe@directbox.com>\n"
"  Added new monitor commands, features and improvements.\n"
"\n"
"  Ingo Korb <ingo@akana.de>\n"
"  Corrected block allocation and interleave for c1541/vdrive,\n"
"  added rudimentary xplus4 tape recording support, corrected a\n"
"  case of missing Pi symbols in petcat, changed the trap opcode\n"
"  byte, stopped the high-level serial drive code from responding\n"
"  to addresses 16-30 and was forced to update this entry himself.\n"
"\n"
"  Antti S. Lankila <alankila@bel.fi>\n"
"  Made the ReSID-fp engine, rewrote the PAL emulation code and\n"
"  fixed the sound core for lower latency. Rewrote DTV SID support\n"
"  (ReSID-dtv). Improved 1541 drive rotation emulation. Worked on\n"
"  x64sc.\n"
"\n"
"  Christian Vogelgsang <chris@vogelgsang.org>\n"
"  Maintained the Mac OS X port. Added Intel Mac support and\n"
"  universal binary creation. Wrote the build scripts for all\n"
"  external Mac libraries and the bindist bundle tool. Improved\n"
"  the TFE chip emulation. Added some GTK+ fixes.\n"
"\n"
"  Dag Lem <resid@nimrod.no>\n"
"  Implemented the reSID SID emulation engine and video hardware\n"
"  scaling.\n"
"\n"
"  Spiro Trikaliotis <Spiro.Trikaliotis@gmx.de>\n"
"  Copyright (C) 2000-2011\n"
"  Wrote the Win32 console implementation for the built-in\n"
"  monitor, corrected some REU related bugs, improved the CIA\n"
"  emulation, added com-port CIA support to the Win32 port, added\n"
"  text copy and paste support to the Win32 port, added support\n"
"  for the TFE and RR-Net (cs8900a), and wrote some further\n"
"  patches.\n"
"\n"
"  Hannu Nuotio <hannu.nuotio@tut.fi>\n"
"  Copyright (C) 2007-2011\n"
"  Implemented DTV flash emulation, DTV support in the monitor,\n"
"  large parts of the DTV VIC, burst mode and skip cycle emulation\n"
"  as well as many other things. Added NEOS and Amiga mouse,\n"
"  paddle and light pen support. Added new monitor commands and\n"
"  features, including memmap. Made MIDI support and OSS MIDI\n"
"  driver. Implemented most of the SDL UI. Rewrote xvic CPU/VIC-I\n"
"  core for cycle based emulation. Implemented C64 cartridge\n"
"  snapshot support. Initiated and worked on all parts of\n"
"  implementing x64sc. Wrote test programs.\n"
"\n"
"  Andreas Boose <viceteam@t-online.de>\n"
"  Copyright (C) 1998-2010\n"
"  Gave lots of information and bug reports about the VIC-II, the\n"
"  6510 and the CIAs; moreover, he wrote several test-routines\n"
"  that were used to improve the emulation. He also added\n"
"  cartridge support and has been the main head behind the drive\n"
"  and datasette emulation since version 0.15. Also added several\n"
"  UI elements to the DOS, Win32 and *nix ports. He rewrote the\n"
"  C128 emulation adding Z80 mode, C64 mode and function ROM\n"
"  support, wrote the screenshot and the event system and started\n"
"  the plus4 emulator. Restructured the serial bus emulation and\n"
"  added realdrive and rawdrive support.\n"
"\n"
"  Tibor Biczo <crown@mail.matav.hu>\n"
"  Copyright (C) 1998-2010\n"
"  Improved the Win32 port and plus4 emulation.\n"
"\n"
"  M. Kiesel <mayne@users.sourceforge.net>\n"
"  Copyright (C) 2007-2010\n"
"  Started implementing x64dtv. The C64DTV memory model and early\n"
"  versions of the DMA and Blitter engine have been implemented by\n"
"  him. Added new monitor commands and features.\n"
"\n"
"  Andreas Dehmel <zarquon@t-online.de>\n"
"  Copyright (C) 1999-2007\n"
"  Wrote the Acorn RISC OS port.\n"
"\n"
"  David Hansel <david@hansels.net>\n"
"  Copyright (C) 2003-2005\n"
"  Wrote the Star NL10 printer driver, implemented IEC devices and\n"
"  improved the tape emulation.\n"
"\n"
"  Markus Brenner <markus@brenner.de>\n"
"  Copyright (C) 2000-2004\n"
"  Added VDC emulation to x128 and added support for some more\n"
"  cartridges.\n"
"\n"
"  Thomas Bretz <tbretz@ph.tum.de>\n"
"  Copyright (C) 1999-2004\n"
"  Started the OS/2 port.\n"
"\n"
"  Daniel Sladic <sladic@eecg.toronto.edu>\n"
"  Copyright (C) 1997-2001\n"
"  Started the work on hardware-level 1541 emulation and wrote the\n"
"  new monitor introduced with VICE 0.15.\n"
"\n"
"  Andr� Fachat <fachat@physik.tu-chemnitz.de>\n"
"  Copyright (C) 1996-2001\n"
"  Wrote the PET and CBM-II emulators, the CIA and VIA emulation,\n"
"  the IEEE488 interface, implemented the IEC serial bus in `xvic'\n"
"  and made tons of bug fixes.\n"
"\n"
"  Ettore Perazzoli <ettore@comm2000.it>\n"
"  Copyright (C) 1996-1999\n"
"  Made the 6510, VIC-II, VIC-I and CRTC emulations, part of the\n"
"  hardware-level 1541 emulation, speed optimizations, bug fixes,\n"
"  the event-driven cycle-exact engine, the Xt/Xaw/Xfwf-based GUI\n"
"  for X11, a general code reorganization, the new resource\n"
"  handling, most of the documentation. He also wrote the DOS port\n"
"  and the initial Win32 port (well, somebody had to do it).\n"
"\n"
"  Teemu Rantanen <tvr@cs.hut.fi>\n"
"  Copyright (C) 1993-1994, 1997-1999\n"
"  Implemented the SID emulation and the trap-based disk drive and\n"
"  serial bus implementation; added support for multiple display\n"
"  depths under X11. Also wrote `c1541'\n"
"\n"
"  Jouko Valta <jopi@stekt.oulu.fi>\n"
"  Copyright (C) 1993-1996\n"
"  Wrote `petcat' and `c1541', `T64' handling, user\n"
"  service and maintenance (most of the work in x64 0.3.x was made\n"
"  by him); retired from the project in July 96, after VICE\n"
"  0.10.0.\n"
"\n"
"  Jarkko Sonninen <sonninen@lut.fi>\n"
"  Copyright (C) 1993-1994\n"
"  He was the founder of the project, wrote the old version of the\n"
"  6502 emulation and the XDebugger, and retired from the project\n"
"  after x64 0.2.1.\n"
"\n"
"\n"
"  Internationalization Team:\n"
"\n"
"\n"
"  Mikkel Holm Olsen <vice-dk@symlink.dk>\n"
"  Copyright (C) 2009-2015\n"
"  Provided the Danish user interface translations and fixed a few monitor bugs.\n"
"\n"
"  Martin Pottendorder <pottendo@gmx.net>\n"
"  Copyright (C) 2000-2015\n"
"  Provided the German user interface translations.\n"
"\n"
"  Manuel Antonio Rodriguez Bas <formater@winuaespanol.com>\n"
"  Copyright (C) 2011-2015\n"
"  Provided the Spanish user interface translations.\n"
"\n"
"  Paul Dub� \n"
"  Copyright (C) 2004-2015\n"
"  Provided the French user interface translations.\n"
"\n"
"  Czirkos Zoltan <cirix@fw.hu>\n"
"  Copyright (C) 2006-2015\n"
"  Provided the Hungarian user interface translations.\n"
"\n"
"  Karai Csaba <cskarai@freemail.hu>\n"
"  Copyright (C) 2006-2015\n"
"  Provided the Hungarian user interface translations.\n"
"\n"
"  Andrea Musuruane <musuruan@bmm.it>\n"
"  Copyright (C) 2001-2015\n"
"  Provided the Italian user interface translations.\n"
"\n"
"  Jesse Lee <interpreters.jesse@gmail.com>\n"
"  Copyright (C) 2011-2015\n"
"  Provided the Korean user interface translations.\n"
"\n"
"  Marco van den Heuvel <blackystardust68@yahoo.com>\n"
"  Copyright (C) 2005-2015\n"
"  Provided the Dutch user interface translations.\n"
"\n"
"  Jarek Sobolewski <r@sledinmay.com>\n"
"  Copyright (C) 2011-2015\n"
"  Provided the Polish user interface translations.\n"
"\n"
"  Michael Litvinov <litvinov.m@i.ua>\n"
"  Copyright (C) 2010-2015\n"
"  Provided the Russian user interface translations.\n"
"\n"
"  Peter Krefting <peter@softwolves.pp.se>\n"
"  Copyright (C) 2000-2015\n"
"  Provided the Swedish user interface translations.\n"
"\n"
"  Emir Akaydin (aka: Skate) <emir@akaydin.com>\n"
"  Copyright (C) 2008-2015\n"
"  Provided the Turkish user interface translations.\n"
"\n"
"\n"
"  Documentation Team:\n"
"\n"
"\n"
"  Daniel Kahlin <daniel@kahlin.net>\n"
"\n"
"  Groepaz <groepaz@gmx.net>\n"
"\n"
"  Kajtar Zsolt <soci@c64.rulez.org>\n"
"\n"
"  Marco van den Heuvel <blackystardust68@yahoo.com>\n"
"\n"
"  Olaf Seibert <rhialto@falu.nl>\n"
"\n"
"\n"
"\n"
"  External contributors:\n"
"\n"
"\n"
"  Christian Bauer <bauec002@goofy.zdv.uni-mainz.de>\n"
"  Wrote the very interesting ``VIC article'' from which we got\n"
"  invaluable information about the VIC-II chip: without this, the\n"
"  VIC-II implementation would not have been possible.\n"
"\n"
"  Eliseo Bianchi <e.bianchi@libero.it>\n"
"  Provided the italian Amiga translations.\n"
"\n"
"  ck! <pixelite@lycos.de>\n"
"  Provided a win32 cbm character font.\n"
"\n"
"  iAN CooG <iancoog@email.it>\n"
"  Added win32 vsid GUI and contributed various patches.\n"
"\n"
"  Mike Dawson <mike@gp2x.org>Mike\n"
"  Provided the GP2X port.\n"
"\n"
"  Paul David Doherty <h0142kdd@rz.hu-berlin.de>\n"
"  Wrote `zip2disk', on which the Zipcode support in `c1541' is based.\n"
"\n"
"  Sven A. Droll\n"
"  Added Supergrafik support to petcat.\n"
"\n"
"  Peter Edwards <sweetlilmre@users.sourceforge.net>\n"
"  Implemented the SDL UI slider control and fixed some\n"
"  GP2X/Dingoo SDL UI issues.\n"
"\n"
"  Daniel Fandrich <dan@fch.wimsey.bc.ca>\n"
"  Contributed some disk drive patches.\n"
"\n"
"  Dirk Farin <farin@ti.uni-mannheim.de>\n"
"  Rewrote the MITSHM code.\n"
"\n"
"  Georg Feil <georg@sgl.sci.yorku.ca>\n"
"  Added support for toggling CB2 sound output line in the PET\n"
"  emulator.\n"
"\n"
"  Peter Andrew Felvegi aka Petschy <petschy@derceto.sch.bme.hu>\n"
"  Fixed a couple of bugs in the fast serial emulation.\n"
"\n"
"  Ricardo Ferreira <storm@atdot.org>\n"
"  Contributed the `unlynx' and `system' commands in `c1541' and\n"
"  added aRts sound support.\n"
"\n"
"  Flooder <flooder@gower.pl>\n"
"  Provided parts of the Polish user interface translations.\n"
"\n"
"  Robert H. Forsman Jr.\n"
"  Provided parts of the widget set for implementing the Xaw GUI.\n"
"\n"
"  Ian Gledhill <ian.gledhill@btinternet.com>\n"
"  Added support for the catweasel.device driver.\n"
"\n"
"  Peter Gordon <pete@petergordon.org.uk>\n"
"  Provided support for native AmigaOS4 compiling.\n"
"\n"
"  Richard Hable <hab@fim.uni-linz.ac.at>\n"
"  Contributed the initial version of the REU emulation.\n"
"\n"
"  Shawn Hargreaves <shawn@talula.demon.co.uk>\n"
"  Wrote Allegro, the graphics and audio library used in the DOS\n"
"  version.\n"
"\n"
"  Ville-Matias Heikkila <viznut@iki.fi>\n"
"  Rewrote the vic20 sound code.\n"
"\n"
"  David Holz <whiteflame52@yahoo.com>\n"
"  Provided a label file which gives the built-in monitor the\n"
"  labels for the C64.\n"
"\n"
"  Nathan Huizinga <nathan.huizinga@chess.nl>\n"
"  Added support for Expert and Super Snapshot carts.\n"
"\n"
"  Derrick Inksley <dink@lemon64>\n"
"  Fixed loading of zip files with brackets ([]) in\n"
"  the filename for the windows port. Added drive selection\n"
"  functionality to the window sdl port.\n"
"\n"
"  Craig Jackson <Craig.Jackson@lambada.oit.unc.edu>\n"
"  Contributed miscellaneous patches in the old X64 times.\n"
"\n"
"  Dirk Jagdmann <doj@cubic.org>\n"
"  Wrote the Catweasel sound driver.\n"
"\n"
"  Uffe Jakobsen <microtop@starion.dk>\n"
"  Wrote the Silverrock cartridge emulation and fixed the\n"
"  ocean cartridge bank wrap.\n"
"\n"
"  Lasse Jyrkinen <Lasse.Jyrkinen@uz.kuleuven.ac.be>\n"
"  Contributed miscellaneous patches in the old X64 times.\n"
"\n"
"  Peter Karlsson\n"
"  Provided the swedish UI translations in the past.\n"
"\n"
"  Greg King <greg.king4@verizon.net>\n"
"  Added a working RTC to the emulation of the IDE64\n"
"  cartridge. Provided some vdrive fixes.\n"
"\n"
"  Michael Klein <nip@c64.org>\n"
"  Contributed the ESD sound driver, basic support for the OPENCBM\n"
"  library and some other patches.\n"
"\n"
"  Frank K�nig <frank_koenig@t-online.de>\n"
"  Contributed the Win32 joystick autofire feature.\n"
"\n"
"  Bernd Kortz <bernd.korz@yellowtab.com>\n"
"  Provided some fixes for ZETA and the ZETA binary package.\n"
"\n"
"  Bernhard Kuhn <kuhn@eikon.e-technik.tu-muenchen.de>\n"
"  Made some joystick improvements for Linux.\n"
"\n"
"  Alexander Lehmann <alex@mathematik.th-darmstadt.de>\n"
"  Added complete support for all the VIC20 memory configurations\n"
"  for the old VICE 0.12.\n"
"\n"
"  Ilkka \"itix\" Lehtoranta <ilkleht@isoveli.org>\n"
"  Provided the routines for the cybergraphics support for the\n"
"  Amiga ports.\n"
"\n"
"  Magnus Lind\n"
"  Atari ST mouse and Atari CX-22 trackball emulation and pixel\n"
"  aspect fixes. Improved the Amiga mouse emulation. Improved\n"
"  the vic20 sound output. Added windows POV hat support.\n"
"  Improved sound fragment size handling.\n"
"\n"
"  Locnet <android.locnet@gmail.com>\n"
"  Made the initial android port of x64.\n"
"\n"
"  Wolfgang Lorenz\n"
"  Wrote an excellent 6510 test suite that helped us to debug the\n"
"  CPU emulation.\n"
"\n"
"  Marko M�kel� <msmakela@gmail.com>\n"
"  Wrote lots of CPU documentation. Wrote the VIC Flash Plugin\n"
"  cartridge emulation in xvic.\n"
"\n"
"  mar77i\n"
"  Fixed some resource handling issues.\n"
"\n"
"  Robert W. McMullen <rwmcm@orion.ae.utexas.edu>\n"
"  Provided parts of the widget set for implementing the Xaw GUI.\n"
"\n"
"  Jennifer Medkief <jennifermedkief@gmail.com>\n"
"  Is in charge of checking up on the GUIs for elements that are\n"
"  wrong, unaccessable, and missing.\n"
"\n"
"  Dan Miner <dminer@nyx10.cs.du.edu>\n"
"  Contributed some patches to the fast disk drive emulation.\n"
"\n"
"  Luca Montecchiani <m.luca@usa.net>\n"
"  Contributed a new Unix joystick driver.\n"
"\n"
"  Wolfgang Moser <womo@d81.de>\n"
"  Provided small optimization fixes to the GCR code, provided an\n"
"  excellent REU test suite and added REU fixes, and is always the\n"
"  good guy reviewing and commenting changes in the background.\n"
"\n"
"  Roberto Muscedere <cococommie@cogeco.ca>\n"
"  Improved support for REL files.\n"
"\n"
"  Tomi Ollila <Tomi.Ollila@tfi.net>\n"
"  Donated `findpath.c'.\n"
"\n"
"  Per Olofsson <MagerValp@Goth.Org>\n"
"  Digitalized the C64 colors used in the (old) default palette.\n"
"\n"
"  Lasse ��rni <loorni@mail.student.oulu.fi>\n"
"  Contributed the Windows Multimedia sound driver\n"
"\n"
"  Helfried Peyrl\n"
"  Supplied a patch that fixes REL file records larger 256 bytes\n"
"  when using vdrive.\n"
"\n"
"  Frank Prindle <Frank.Prindle@lambada.oit.unc.edu>\n"
"  Contributed some patches.\n"
"\n"
"  Giuliano Procida <gpp10@cam.ac.uk>\n"
"  Used to maintain the VICE `deb' package for the Debian\n"
"  distribution, and also helped proofreading the documentation.\n"
"\n"
"  Vesa-Matti Puro <vmp@lut.fi>\n"
"  Wrote the very first 6502 CPU emulator in x64 0.1.0. That was\n"
"  the beginning of the story....\n"
"\n"
"  Rami Rasanen <raipe@neutech.fi>\n"
"  Rewrote the VIC20 sound code.\n"
"\n"
"  David Roden\n"
"  Fixed various issues related to ffmpeg settings.\n"
"\n"
"  Pablo Rold�n <pdroldan@yahoo.com>\n"
"  Contributed initial patch for VIC-II PAL-N model selection.\n"
"\n"
"  Mathias Roslund <vice.emu@amidog.se>\n"
"  Provided the AmigaOS4 port.\n"
"\n"
"  Gunnar Ruthenberg <Krill.Plush@gmail.com>\n"
"  Provided some VIC-II enhancements and improved the Win32 port.\n"
"\n"
"  Johan Samuelsson <spot@triad.se>\n"
"  Provided the Swedish Amiga translations.\n"
"\n"
"  Oliver Schaertel <schaertel@atrada.net>\n"
"  Wrote the X11 full screen, parts of custom ROM set support and\n"
"  1351 mouse emulation for unix.\n"
"\n"
"  Peter Schepers <schepers@ist.uwaterloo.ca>\n"
"  Contributed a document describing the G64 image format.\n"
"\n"
"  Michael Schwendt <sidplay@geocities.com>\n"
"  Helped with the SID (audio) chip emulation, bringing important\n"
"  suggestions and bug reports, as well as the wave tables and\n"
"  filter emulation from his SIDplay emulator.\n"
"\n"
"  Heiko Selber <selber@fhi-berlin.mpg.de>\n"
"  Contributed some VIC20 I/O patches.\n"
"\n"
"  John Selck <graham@cruise.de>\n"
"  Improved the video rendering and added the fast PAL emulation.\n"
"  Implemented new color generation based on P. Timmermanns\n"
"  knowledge.\n"
"\n"
"  Chris Sharp <sharpc@hurlsey.ibm.com>\n"
"  Wrote the AIX sound driver.\n"
"\n"
"  Andr351 \"JoBBo\" Siegel\n"
"  Provided the native MorphOS icons.\n"
"\n"
"  Harry \"Piru\" Sintonen <sintonen@iki.fi>\n"
"  Provided lots of fixes and improvements for the Amiga ports.\n"
"\n"
"  Manfred Spraul <manfreds@colorfullife.com>\n"
"  Wrote the Win32 text lister.\n"
"\n"
"  Markus Stehr <bastetfurry@ircnet.de>\n"
"  Provided the MMC64 emulation.\n"
"\n"
"  Dominique Strigl <strigl@sxb.bsf.alcatel.fr>\n"
"  Contributed miscellaneous patches in the old X64 times.\n"
"\n"
"  Samuli Suominen\n"
"  Fixed XShm includes for newer xextproto versions and updated\n"
"  libpng check for newer versions.\n"
"\n"
"  Steven Tieu <stieu@physics.ubc.ca>\n"
"  Added initial support for 16/24 bpp X11 displays.\n"
"\n"
"  Philip Timmermann <pepto@pepto.de>\n"
"  Did a lot of research about the VIC-II colors.\n"
"\n"
"  Brian Totty <totty@cs.uiuc.edu>\n"
"  Provided parts of the widget set for implementing the Xaw GUI.\n"
"\n"
"  Mustafa \"GnoStiC\" Tufan <mtufan@gmail.com>\n"
"  Made improvements to the GP2x port.\n"
"\n"
"  Lionel Ulmer\n"
"  Implemented joystick support for Linux and a first try of a SID\n"
"  emulation for SGI machines.\n"
"\n"
"  Krister Walfridsson <cato@df.lth.se>\n"
"  Implemented joystick and sound support for NetBSD.\n"
"\n"
"  webulator <webulator@mailandnews.com>\n"
"  Provided Win32 drag & drop support\n"
"\n"
"  Robert Willie <hydrofilic@hotmail.com>\n"
"  Added some additional commands to the fsdevice emulation.\n"
"\n"
"  Peter Weighill <stuce@csv.warwick.ac.uk>\n"
"  Gave many ideas and contributed the ROM patcher.\n"
"\n"
"  Gerhard Wesp <gwesp@cosy.sbg.ac.at>\n"
"  Contributed the `extract' command in `c1541'.\n"
"\n"
"  Maciej Witkowiak <ytm@elysium.pl>\n"
"  Did some IDE64 and C1541 fixes.\n"
"\n"
"  Peter Rittwage\n"
"  Made 1541 GCR hardware tests.\n"
"\n"
"  Robert McIntyre\n"
"  Bugged people enough to get the improved g64 support rolling, \n"
"  updated g64 support to allow variable-length tracks, and \n"
"  performed initial development of half-track support.\n"
"\n"
"  Istv�n F�bi�n\n"
"  Contributed a initial patch with the more correct 1541 bus\n"
"  timing code and which gave us hints for to improving the 1541\n"
"  emulation.\n"
"\n"
"\n"
"  (We hope we have not forgotten anybody; if you think we have,\n"
"   please tell us.)\n"
"\n"
"  The people around the world providing results from running our\n"
"  test programs on various machines deserve a special mention:\n"
"\n"
"\n"
"  hedning (Drean C64 PAL-N, various C64 PAL boxes)\n"
"  Jason Compton (Various C64 and C128 NTSC boxes)\n"
"  The Woz (Drean C64 PAL-N)\n"
"  Thierry (Drean C64 PAL-N)\n"
"  MOS6569 (C64C PAL)\n"
"  Mike (VIC-20 PAL)\n"
"  Wilson (VIC-20 NTSC)\n"
"  Vicassembly (VIC-20 NTSC)\n"
"  David \"jbevren\" Wood (C64 NTSC-OLD)\n"
"\n"
"  Thanks also to everyone else for sending suggestions, ideas,\n"
"  bug reports, questions and requests. In particular, a warm\n"
"  thanks goes to the following people:\n"
"\n"
"  Lutz Sammer\n"
"  Ralph Mason <RALPHM@msmail02.liffe.com>\n"
"  George Caswell <timbuktu@the-eye.res.wpi.edu>\n"
"  Jasper Phillips <philljas@newt.cs.orst.edu>\n"
"  Luca Forcucci <frk@geocities.com>\n"
"  Asger Alstrup <alstrup@diku.dk>\n"
"  Bernhard Schwall <uzs484@uni-bonn.de>\n"
"  Salvatore Valente <svalente@mit.edu>\n"
"  Arthur Hagen <art@broomstick.com>\n"
"  Douglas Carmichael <dcarmich@mcs.com>\n"
"  Ferenc Veres <lion@c64.rulez.org>\n"
"  Frank Reichel <Frank.Reichel@forchheim.baynet.de>\n"
"  Ullrich von Bassewitz\n"
"  Holger Busse\n"
"  David \"jbevren\" Wood\n"
"  Gary Glenn\n"
"\n"
"  Last but not least, a very special thank to Andreas Arens, Lutz\n"
"  Sammer, Edgar Tornig, Christian Bauer, Wolfgang Lorenz, Miha\n"
"  Peternel, Per H�kan Sundell and David Horrocks for writing\n"
"  cool emulators to compete with.  :-)\n"
"\n"
"\n";

vice_team_t core_team[] = {
    { "1999-2015", "Andreas Matthies", "Andreas Matthies <andreas.matthies@gmx.net>" },
    { "1999-2015", "Martin Pottendorfer", "Martin Pottendorder <pottendo@gmx.net>" },
    { "2005-2015", "Marco van den Heuvel", "Marco van den Heuvel <blackystardust68@yahoo.com>" },
    { "2007-2015", "Fabrizio Gennari", "Fabrizio Gennari <fabrizio.ge@tiscalinet.it>" },
    { "2007-2015", "Daniel Kahlin", "Daniel Kahlin <daniel@kahlin.net>" },
    { "2009-2015", "Groepaz", "Groepaz <groepaz@gmx.net>" },
    { "2009-2015", "Errol Smith", "Errol Smith <strobey@users.sourceforge.net>" },
    { "2010-2015", "Olaf Seibert", "Olaf Seibert <rhialto@falu.nl>" },
    { "2011-2015", "Marcus Sutton", "Marcus Sutton <loggedoubt@gmail.com>" },
    { "2011-2015", "Kajtar Zsolt", "Kajtar Zsolt <soci@c64.rulez.org>" },
    { NULL, NULL, NULL }
};

vice_team_t ex_team[] = {
    { "2012-2014", "Benjamin 'BeRo' Rosseaux", "Benjamin 'BeRo' Rosseaux <benjamin@rosseaux.com>" },
    { "2011-2014", "Ulrich Schulz", "Ulrich Schulz <peiselulli@t-online.de>" },
    { "2011-2014", "Stefan Haubenthal", "Stefan Haubenthal <polluks@users.sourceforge.net>" },
    { "2011-2014", "Thomas Giesel", "Thomas Giesel <skoe@directbox.com>" },
    { "2009-2014", "Ingo Korb", "Ingo Korb <ingo@akana.de>" },
    { "2008-2014", "Antti S. Lankila", "Antti S. Lankila <alankila@bel.fi>" },
    { "2006-2014", "Christian Vogelgsang", "Christian Vogelgsang <chris@vogelgsang.org>" },
    { "1998-2014", "Dag Lem", "Dag Lem <resid@nimrod.no>" },
    { "2000-2011", "Spiro Trikaliotis", "Spiro Trikaliotis <Spiro.Trikaliotis@gmx.de>" },
    { "2007-2011", "Hannu Nuotio", "Hannu Nuotio <hannu.nuotio@tut.fi>" },
    { "1998-2010", "Andreas Boose", "Andreas Boose <viceteam@t-online.de>" },
    { "1998-2010", "Tibor Biczo", "Tibor Biczo <crown@mail.matav.hu>" },
    { "2007-2010", "M. Kiesel", "M. Kiesel <mayne@users.sourceforge.net>" },
    { "1999-2007", "Andreas Dehmel", "Andreas Dehmel <zarquon@t-online.de>" },
    { "2003-2005", "David Hansel", "David Hansel <david@hansels.net>" },
    { "2000-2004", "Markus Brenner", "Markus Brenner <markus@brenner.de>" },
    { "1999-2004", "Thomas Bretz", "Thomas Bretz <tbretz@ph.tum.de>" },
    { "1997-2001", "Daniel Sladic", "Daniel Sladic <sladic@eecg.toronto.edu>" },
    { "1996-2001", "Andr� Fachat", "Andr� Fachat <fachat@physik.tu-chemnitz.de>" },
    { "1996-1999", "Ettore Perazzoli", "Ettore Perazzoli <ettore@comm2000.it>" },
    { "1993-1994, 1997-1999", "Teemu Rantanen", "Teemu Rantanen <tvr@cs.hut.fi>" },
    { "1993-1996", "Jouko Valta", "Jouko Valta <jopi@stekt.oulu.fi>" },
    { "1993-1994", "Jarkko Sonninen", "Jarkko Sonninen <sonninen@lut.fi>" },
    { NULL, NULL, NULL }
};

vice_trans_t trans_team[] = {
    { "2009-2015", "Mikkel Holm Olsen", "Danish", "Mikkel Holm Olsen <vice-dk@symlink.dk>" },
    { "2000-2015", "Martin Pottendorfer", "German", "Martin Pottendorder <pottendo@gmx.net>" },
    { "2011-2015", "Manuel Antonio Rodriguez Bas", "Spanish", "Manuel Antonio Rodriguez Bas <formater@winuaespanol.com>" },
    { "2004-2015", "Paul Dub�", "French", "Paul Dub� " },
    { "2006-2015", "Czirkos Zoltan", "Hungarian", "Czirkos Zoltan <cirix@fw.hu>" },
    { "2006-2015", "Karai Csaba", "Hungarian", "Karai Csaba <cskarai@freemail.hu>" },
    { "2001-2015", "Andrea Musuruane", "Italian", "Andrea Musuruane <musuruan@bmm.it>" },
    { "2011-2015", "Jesse Lee", "Korean", "Jesse Lee <interpreters.jesse@gmail.com>" },
    { "2005-2015", "Marco van den Heuvel", "Dutch", "Marco van den Heuvel <blackystardust68@yahoo.com>" },
    { "2011-2015", "Jarek Sobolewski", "Polish", "Jarek Sobolewski <r@sledinmay.com>" },
    { "2010-2015", "Michael Litvinov", "Russian", "Michael Litvinov <litvinov.m@i.ua>" },
    { "2000-2015", "Peter Krefting", "Swedish", "Peter Krefting <peter@softwolves.pp.se>" },
    { "2008-2015", "Emir Akaydin", "Turkish", "Emir Akaydin (aka: Skate) <emir@akaydin.com>" },
    { NULL, NULL, NULL, NULL }
};

char *doc_team[] = {
    "Daniel Kahlin",
    "Groepaz",
    "Kajtar Zsolt",
    "Marco van den Heuvel",
    "Olaf Seibert",
    NULL
};

#endif
