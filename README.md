# remc2
# Reverse engineering of game Magic Carpet 2 from assembler to c/c++ language<br />
## STATUS: unplayable, in development<br />
<br />
To start the game, it needs data from the original distribution, which you can download at the minimum fee here:<br />
https://www.gog.com/game/magic_carpet_2_the_netherworlds<br />
(or from any abbadone archive, if it is legal in your country)<br />
<br />
Now you can only compile only 32-bit binary version (64bit will be available after rewriting all arrays to structures).<br />
<br />
simple dev blog:https://github.com/turican0/remc2/wiki<br />
<h2>ROADMAP:</h2><br />
<h3>MILLSTONE 6</h3><br />
-can build x64 binary<br />
-can build in Linux<br />
<h3>MILLSTONE 5</h3><br />
-add next resolutions<br />
<h3>MILLSTONE 4</h3><br />
-fix music (can switch between Original MIDI, Music in wave format and Alternative music)<br />
-fix multiplayer
<h3>MILLSTONE 3</h3><br />
-Fix resolution 640x480<br />
-can play All game same as in original in 320x200<br />
<h3>MILLSTONE 2</h3><br />
-Change basic arrays to structures<br />
-Fix Load and Save game<br />
<h3>MILLSTONE 1</h3><br />
-Can Play whole First level same as in original in 320x200<br />
<br />
## 0.39:<br />
-HI-res fixed<br />
-fixed carpet position in level menu<br />
## 0.38:<br />
-First test HI-res<br />
-finded problems with texts, menus and terrain<br />
## 0.37:<br />
-Can play level 1 as original<br />
-in game music<br />
-can load and save game<br />
<br />
## 0.36:<br />
-fixed second level of castle<br />
-!!!can finalize level I!!!(without fireflys now)<br />
<br />
## 0.35:<br />
-fixed bad rendering(blurr effect)<br />
-fix many errors<br />
-fix spell menu<br />
-add fullscreen switch by ALT+ENTER<br />
-fix some crashs<br />
<br />
## 0.34:<br />
-fix billboard draw bug<br />
<br />
## 0.33:<br />
-fixed many problems with castle creating<br />
<br />
## 0.32:<br />
-fixed and rewriting many some structures<br />
-begin of fixing castle creating<br />
<br />
## 0.31:<br />
-fixed all key(maybe :)<br />
-fixed spells fire and posses<br />
<br />
## 0.30:<br />
-fix in game menu(mostly)<br />
-can fly over one level<br />
(can not use spells, change reflections, resolutions, brigtness not and etc. not work)<br />
<br />
## 0.29:<br />
-fixed begin of game<br />
-fixed help system<br />
<br />
## 0.28:<br />
-fixed first 110 frames!<br />
<br />
## 0.27:<br />
-many fixes hidden in code<br />
-fixed first 7 frames!<br />
<br />
## 0.26:<br />
-many fixes hidden in code<br />
-fixed first 3 frames!<br />
<br />
## 0.25:<br />
-many fixes hidden in code<br />
-first frame is same as original!<br />
<br />
## 0.24:<br />
-fixed menu texture<br />
-fixed minimap<br />
<br />
## 0.23:<br />
-fixed textures<br />
<br />
## 0.22:<br />
-fixed some rendering errors<br />
<br />
## 0.21:<br />
-fixed cleaning game screen<br />
-first successfull texture drawing<br />
<br />
## 0.20:<br />
-fixed pallette<br />
-fixed spell<br />
-some minimap problems fixed<br />
<br />
## 0.19:<br />
-fixing many structures<br />
-first in game graphics<br />
<br />
## 0.18:<br />
-loading level product same data as original<br />
-retyping and fixing hundreds functions<br />
-fixed first bug from original<br />
<br />
## 0.17:<br />
-fixed Load, Save, Restart and Exit button in Map Menu<br />
-add czech translation<br />
-fix load level initialize<br />
-begin of fixing game draw pipeline<br />
<br />
## 0.16:<br />
-map menu graphics full fixed!<br />
-added config<br />
-added alternative music<br />
<br />
## 0.15:<br />
-BIG work on SOUND and MUSIC<br />
--rewrite mss and connect it to SDL<br />
(full function WAV or MIDI music - at now native MIDI, I test timiditi or fluidsynth, I maybe add OPL syntesis later)<br />
-add SDL2 support<br />
<br />
## 0.14:<br />
-can open load menu<br />
-can open save menu<br />
-fixed setting keys<br />
-fixed setting keys menu<br />
-can open game progress menu<br />
<br />
## 0.13:<br />
-fixed language menu<br />
-fixed change name menu<br />
-added basic get key routine<br />
-can exit from game!<br />
<br />
## 0.12:<br />
-main menu is fixed<br />
<br />
## 0.11:<br />
-fixed text rendering<br />
-can play all intro<br />
<br />
## 0.10:<br />
-fixed load and save language config<br />
-can play part of intro<br />
<br />
## 0.09:<br />
-fix cursor draw subroutine<br />
-can select language<br />
<br />
## 0.08:<br />
-added mouse cursor!<br />
<br />
## 0.07:<br />
-render of language select screen is same as original!<br />
<br />
## 0.06:<br />
-some sprites works<br />
<br />
## 0.05:<br />
-add primitive console mode<br />
-repairing fade pallete<br />
<br />
## 0.04:<br />
-fixed pallette<br />
<br />
## 0.03:<br />
-fixed resolution<br />
<br />
## 0.02 - first graphics:<br />
-show first graphics<br />
<br />
## 0.01 - first release:<br />
-can compile under Windows/Visual Studio<br />
-repair some structures<br />
-replace rnc utilites<br />

For develop I use modified version of Dosbox-x:
https://github.com/turican0/dosbox-x-remc2
