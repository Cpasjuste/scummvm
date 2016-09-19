/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "xeen/worldofxeen/darkside_cutscenes.h"
#include "xeen/worldofxeen/worldofxeen.h"
#include "xeen/sound.h"
#include "xeen/xeen.h"

namespace Xeen {

bool DarkSideCutscenes::showDarkSideTitle() {
	EventsManager &events = *_vm->_events;
	Screen &screen = *_vm->_screen;
	Sound &sound = *_vm->_sound;

	screen.loadPalette("dark.pal");
	SpriteResource nwc[4] = {
		SpriteResource("nwc1.int"), SpriteResource("nwc2.int"),
		SpriteResource("nwc3.int"), SpriteResource("nwc4.int")
	};
	File voc[3];
	voc[0].open("dragon1.voc");
	voc[1].open("dragon2.voc");
	voc[2].open("dragon3.voc");

	// Load backgrounds
	screen.loadBackground("nwc1.raw");
	screen.loadPage(0);
	screen.loadBackground("nwc2.raw");
	screen.loadPage(1);

	// Draw the screen and fade it in
	screen.horizMerge(0);
	screen.draw();
	screen.fadeIn();

	sound.setMusicVolume(0x5f);
	sound.playFX(1);

	// Initial loop for dragon roaring
	int nwcIndex = 0, nwcFrame = 0;
	for (int idx = 0; idx < 55 && !_vm->shouldQuit(); ++idx) {
		// Render the next frame
		screen.vertMerge(0);
		nwc[nwcIndex].draw(screen, nwcFrame);
		screen.draw();

		switch (idx) {
		case 17:
			sound.playSound(voc[0]);
			break;
		case 34:
		case 44:
			++nwcIndex;
			nwcFrame = 0;
			break;
		case 35:
			sound.playSound(voc[1]);
			break;
		default:
			++nwcFrame;
		}

		WAIT(2);
	}

	// Loop for dragon using flyspray
	for (int idx = 0; idx < 42 && !_vm->shouldQuit(); ++idx) {
		screen.vertMerge(SCREEN_HEIGHT);
		nwc[3].draw(screen, idx);
		screen.draw();

		switch (idx) {
		case 3:
			sound.playFX(40);
			break;
		case 11:
			sound.playFX(0);
		case 27:
		case 30:
			sound.playFX(3);
			break;
		case 31:
			sound.playSound(voc[2]);
			break;
		case 33:
			sound.playFX(2);
			break;
		default:
			break;
		}

		WAIT(2);
	}
	if (_vm->shouldQuit())
		return false;

	// Pause for a bit
	WAIT(10);

	sound.setMusicVolume(95);

	screen.fadeOut(8);
	screen.loadBackground("jvc.raw");
	screen.draw();
	screen.fadeIn();
	
	WAIT(60);
	return true;
}

bool DarkSideCutscenes::showDarkSideIntro() {
	EventsManager &events = *_vm->_events;
	Screen &screen = *_vm->_screen;
	Sound &sound = *_vm->_sound;
	const int XLIST1[] = {
		0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 58, 60, 62
	};
	const int YLIST1[] = {
		0, 5, 10, 15, 20, 25, 30, 35, 40, 40, 39, 37, 35, 33, 31
	};
	const int XLIST2[] = {
		160, 155, 150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 98, 90, 82
	};

	screen.fadeOut(8);
	screen.loadPalette("dark.pal");
	screen.loadBackground("pyramid2.raw");
	screen.loadPage(0);
	screen.loadPage(1);
	screen.loadBackground("pyramid3.raw");
	screen.saveBackground(1);

	SpriteResource sprites[3] = {
		SpriteResource("title.int"), SpriteResource("pyratop.int"), SpriteResource("pyramid.int")
	};

	screen.vertMerge(SCREEN_HEIGHT);
	screen.loadPage(0);
	screen.loadPage(1);

	// Play the intro music
	sound.playSong("bigtheme.m");

	// Show Might and Magic Darkside of Xeen title, and gradualy scroll
	// the background vertically down to show the Pharoah's base
	int yp = 0;
	int frameNum = 0;
	int idx1 = 0;
	bool skipElapsed = false;
	uint32 timeExpired = 0;
	bool fadeFlag = true;

	for (int yCtr = SCREEN_HEIGHT; yCtr > 0; ) {
		screen.vertMerge(yp);

		sprites[0].draw(screen, 0);
		if (frameNum)
			sprites[0].draw(screen, frameNum);

		idx1 = (idx1 + 1) % 4;
		if (!idx1)
			frameNum = (frameNum + 1) % 10;

		screen.draw();
		if (!skipElapsed) {
			timeExpired = MAX((int)events.timeElapsed() - 1, 1);
			skipElapsed = true;
		}

		yCtr -= timeExpired;
		yp = MIN((uint)(yp + timeExpired), (uint)200);

		WAIT(1);

		if (fadeFlag) {
			screen.fadeIn();
			fadeFlag = false;
		}
	}

	screen.vertMerge(SCREEN_HEIGHT);
	screen.saveBackground(1);
	screen.draw();
	screen.freePages();

	WAIT(30);

	// Zoom into the Pharoah's base closeup view
	for (int idx = 14; idx >= 0; --idx) {
		sprites[1].draw(screen, 0, Common::Point(XLIST1[idx], YLIST1[idx]));
		sprites[1].draw(screen, 1, Common::Point(XLIST2[idx], YLIST1[idx]));
		screen.draw();

		if (idx == 2)
			sound.setMusicVolume(48);
		WAIT(2);
	}

	return true;
}

bool DarkSideCutscenes::showDarkSideEnding() {
	EventsManager &events = *_vm->_events;
	Screen &screen = *_vm->_screen;
	Sound &sound = *_vm->_sound;

	//sound.playSong("dngon3.m");
	screen.loadBackground("scene1.raw");
	screen.loadPalette("endgame.pal");
	screen.update();
/*
	screen.fadeIn();
	WAIT(30);

	screen.fadeOut();
	screen.loadBackground("scene2-b.raw");
	screen.update();
	screen.saveBackground();

	SpriteResource faceEnd("face.end");
	screen.restoreBackground();
	faceEnd.draw(screen, 0, Common::Point(29, 76), SPRFLAG_4000);
	screen.update();

	screen.fadeIn();
	WAIT(1);

	_subtitles.load("special.bin");
	recordTime();
	resetSubtitles();
	_subtitleLineNum = 21;

	// Alamar stands up
	for (int idx = 74; idx > 20; idx -= 2) {
		if (idx == 60)
			sound.songCommand(207);
		else if (idx == 22)
			sound.stopSong();

		screen.restoreBackground();
		faceEnd.draw(screen, 0, Common::Point(29, idx), SPRFLAG_4000);
		screen.update();

		WAIT(2);
	}
	faceEnd.clear();

	// Alamar says "Come to me"
	sound.playSound("come2.voc");
	WAIT(27);

	// Show the entire throne room
	screen.loadBackground("mainback.raw");
	SpriteResource sc03a("sc03a.end"), sc03b("sc03b.end"), sc03c("sc03c.end");
	sc03a.draw(screen, 0, Common::Point(250, 0));

	screen.saveBackground();
	screen.update();
	WAIT(30);

	// Silhouette of door opening
	sound.playSound("door.voc");
	for (int idx = 0; idx < 6; ++idx) {
		screen.restoreBackground();
		sc03b.draw(screen, idx, Common::Point(72, 125));
		screen.update();

		WAIT(4);
	}

	// Silhouette of playing entering
	for (int idx = 0; idx < 19; ++idx) {
		screen.restoreBackground();
		sc03c.draw(screen, idx, Common::Point(72, 125));
		screen.update();

		if (idx == 3 || idx == 11)
			sound.playFX(7);
		if (idx == 7 || idx == 16)
			sound.playFX(8);

		WAIT(4);
	}

	sc03a.clear();
	sc03b.clear();
	sc03c.clear();

	// Box throwing
	screen.loadBackground("scene4.raw");
	screen.loadPage(0);
	screen.loadBackground("scene4-1.raw");
	screen.loadPage(1);
	SpriteResource disk("disk.end");
	File whoosh("whoosh.voc");

	screen.horizMerge();
	int yp = 101, ctr = 0, frameNum = 0;
	for (int xp = 0; xp < 320; xp += 2) {
		screen.horizMerge(xp);

		disk.draw(screen, frameNum, Common::Point(xp / 2, yp));
		if (xp < 44)
			disk.draw(screen, 11, Common::Point(-xp, 22), SPRFLAG_800);
		disk.draw(screen, 10, Common::Point(0, 134));

		if (!(xp % 22))
			sound.playSound(whoosh);
		WAIT(1);

		if (++ctr % 2)
			frameNum = (frameNum + 1) % 10;

		if (xp < 100)
			--yp;
		else if (xp > 150)
			++yp;
	}

	whoosh.close();
	WAIT(10);

	// Play landing thud
	sound.playSound("thud.voc");
	while (!_vm->shouldQuit() && !events.isKeyMousePressed()
			&& sound.isPlaying()) {
		events.pollEventsAndWait();
	}

	// Fade over to box opening background
	events.updateGameCounter();
	screen.freePages();
	screen.fadeOut();
	screen.loadBackground("sc050001.raw");
	screen.update();
	screen.saveBackground();
	screen.fadeIn();
	events.updateGameCounter();

	// Box opening
	SpriteResource diskOpen[5] = {
		SpriteResource("diska.end"), SpriteResource("diskb.end"),
		SpriteResource("diskc.end"), SpriteResource("diskd.end"),
		SpriteResource("diske.end")
	};
	File click("click.voc");

	for (int idx = 0; idx < 34; ++idx) {
		screen.restoreBackground();
		diskOpen[idx / 8].draw(screen, idx % 8, Common::Point(28, 11));

		WAIT(3);
		if (idx == 0 || idx == 2 || idx == 4 || idx == 6)
			sound.playSound(click);
		else if (idx == 18)
			sound.playSound("explosio.voc");
		else if (idx == 9 || idx == 10 || idx == 11 || idx == 12)
			sound.playFX(30);
		else if (idx == 13)
			sound.playFX(14);
	}
	click.close();
	for (int idx = 0; idx < 5; ++idx)
		diskOpen[idx].clear();

	SpriteResource sc06[2] = {
		SpriteResource("sc06a.end"), SpriteResource("sc06b.end")
	};
	screen.loadBackground("mainback.raw");
	screen.saveBackground();
	sc06[0].draw(screen, 0);
	screen.update();
	sound.playSound("alien2.voc");

	// Zoomed out throneroom view of beam coming out of box
	for (int idx = 0; idx < 20; ++idx) {
		if (idx == 6 || idx == 8 || idx == 9 || idx == 10
				|| idx == 13 || idx == 15 || idx == 16)
			sound.playFX(3);

		screen.restoreBackground();
		sc06[idx / 10].draw(screen, idx % 10);
		screen.update();

		WAIT(3);
	}

	screen.fadeOut();
	sc06[0].clear();
	sc06[1].clear();

	// Corak appearing
	SpriteResource sc07[8] = {
		SpriteResource("sc07a.end"), SpriteResource("sc07b.end"),
		SpriteResource("sc07c.end"), SpriteResource("sc07d.end"),
		SpriteResource("sc07e.end"), SpriteResource("sc07f.end"),
		SpriteResource("sc07g.end"), SpriteResource("sc07h.end")
	};

	screen.loadBackground("sc070001.raw");
	screen.saveBackground();
	screen.update();
	screen.fadeIn();
	WAIT(2);

	resetSubtitles();
	_subtitleLineNum = 22;
	recordTime();

	for (int idx = 0; idx < 45; ++idx) {
		screen.restoreBackground();
		sc07[idx / 6].draw(screen, idx % 6, Common::Point(61, 12));

		if (_subtitleSize == 0)
			screen.update();
		else
			showSubtitles();

		WAIT(2);
		if (idx == 40)
			_subtitleSize = 1;
		else if (idx == 1 || idx == 19)
			// Wind storm
			sound.playSound("windstor.voc");
	}

	for (int idx = 0; idx < 8; ++idx)
		sc07[idx].clear();

	// Corak?!
	sound.playSound("corak2.voc");
	subtitlesWait(30);

	// Yep, that's my name, don't wear it out
	SpriteResource sc08("sc08.end");
	resetSubtitles();
	recordTime();
	_subtitleLineNum = 23;
	_subtitleSize = 1;
	sound.playFX(0);

	for (int idx = 0; idx < 15; ++idx) {
		if (idx == 2)
			sound.playSound("yes1.voc");

		// Animate Corak speaking
		sc08.draw(screen, sound.isPlaying() ? getSpeakingFrame(0, 2) : 0);
		showSubtitles();
		WAIT(3);
	}

	do {
		sc08.draw(screen, 0);
		showSubtitles();
	} while (_subtitleSize);
		
	sc08.clear();

	// Nowhere to run to
	SpriteResource sc09("sc09.end");

	screen.loadBackground("sc090001.raw");
	screen.saveBackground();
	screen.update();

	resetSubtitles();
	recordTime();
	_subtitleLineNum = 24;
	_subtitleSize = 1;

	for (int idx = 0; idx < 15; ++idx) {
		if (idx == 2)
			sound.playSound("nowre1.voc");

		screen.restoreBackground();
		sc09.draw(screen, getSpeakingFrame(0, 8));
		showSubtitles();
		WAIT(3);
	}

	do {
		screen.restoreBackground();
		showSubtitles();
	} while (_subtitleSize);
	sc09.clear();

	// Nor do you!
	SpriteResource sc10("sc10.end");
	resetSubtitles();
	recordTime();
	_subtitleLineNum = 25;
	_subtitleSize = 1;

	for (int idx = 0; idx < 15; ++idx) {
		if (idx == 3)
			sound.playSound("nordo2.voc");

		screen.restoreBackground();
		sc09.draw(screen, getSpeakingFrame(0, 1));
		showSubtitles();
		WAIT(3);
	}

	do {
		screen.restoreBackground();
		showSubtitles();
	} while (_subtitleSize);
	sc10.clear();
	*/_subtitles.load("special.bin");

	// Closeup of side of Alamar's helmet
	SpriteResource sc11("sc11.end");
	sound.setMusicVolume(95);
	sound.playSong("dngon2.m");
	screen.fadeOut();
	screen.loadBackground("blank.raw");
	screen.saveBackground();

	for (int idx = 0; idx < 15; ++idx) {
		screen.restoreBackground();
		sc11.draw(screen, idx, Common::Point(100, 44));

		if (idx == 0)
			screen.fadeIn();
		WAIT(3);

		if (idx == 3)
			sound.playSound("click.voc");
		else if (idx == 5)
			sound.playSound("airmon.voc");
	}

	screen.fadeOut();

	// Helmet raises, we see the face appear
	for (int idx = 80; idx > 20; idx -= 2) {
		screen.restoreBackground();
		sc11.draw(screen, 15, Common::Point(60, 80));
		sc11.draw(screen, 16, Common::Point(61, idx));
		sc11.draw(screen, 17);
		screen.update();

		if (idx == 80)
			screen.fadeIn();

		WAIT(3);
	}

	// Sheltem is revealed
	sc11.clear();
	screen.fadeOut();
	screen.loadBackground("scene12.raw");
	screen.update();
	screen.fadeIn();
	WAIT(30);
	screen.fadeOut();

	// I'm ready for you this time
	SpriteResource sc13("sc13.end");
	resetSubtitles();
	recordTime();
	_subtitleLineNum = 26;
	_subtitleSize = 1;
	sound.setMusicVolume(48);

	for (int idx = 0; idx < 16; ++idx) {
		if (idx == 1)
			sound.playSound("ready2.voc");

		sc13.draw(screen, getSpeakingFrame(0, 3));
		showSubtitles();

		if (idx == 0)
			screen.fadeIn();
		WAIT(3);
	}

	do {
		sc13.draw(screen, 0);
		showSubtitles();
		events.pollEventsAndWait();
	} while (_subtitleSize);

	sc13.clear();

	// This fight will be your last
	SpriteResource sc14("sc14.end");
	resetSubtitles();
	recordTime();
	_subtitleLineNum = 27;
	_subtitleSize = 1;
	screen.fadeOut();
	screen.loadBackground("blank.raw");
	screen.saveBackground();

	sc14.draw(screen, 0, Common::Point(37, 37));
	screen.update();
	screen.fadeIn();

	for (int waveNum = 0; waveNum < 3; ++waveNum) {
		for (int idx = 0; idx < 10; ++idx) {
			if (waveNum == 0 && idx == 3)
				sound.playSound("fight2.voc");

			screen.restoreBackground();
			sc14.draw(screen, idx, Common::Point(37, 37));
			showSubtitles();
			WAIT(3);
		}
	}

	while (_subtitleSize) {
		showSubtitles();
		events.pollEventsAndWait();
		showSubtitles();
	}
	sc14.clear();

	// Fighting start


	// TODO
	events.wait(5000);

	freeSubtitles();
	return true;
}

} // End of namespace Xeen