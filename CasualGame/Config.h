#pragma once

#include <string>

#include "FontLoader.h"

static const auto g_gameTitle = "Casual Game by Miretz";

static const auto g_defaultWidth = 1024;
static const auto g_defaultHeight = 768;

static const int g_textureWidth = 128;
static const int g_textureHeight = 128;

// Resources

static const FontLoader* g_fontLoader = new FontLoader("resources/font/OtherF.ttf");

static const auto g_defaultLevelFile = "resources/levels/level1.txt";
static const auto g_defaultLevelSpriteFile = "resources/levels/level1_sprites.txt";

static const auto g_customLevelDirectory = "resources/levels/custom/";

static const int g_textureCount = 13;

static const std::string g_textureFiles[13] = {
	"resources/textures/stonebricks.png",//1
	"resources/textures/concbase.png",//2
	"resources/textures/metalblocks.png",//3
	"resources/textures/concbricks.png",//4
	"resources/textures/concblocks.png",//5
	"resources/textures/redbricks.png",//6
	"resources/textures/metalwall.png",//7
	"resources/textures/redbricks2.png",//8
	"resources/textures/diagonal.png", //floor 9
	"resources/textures/stoneblocks.png", //ceiling 10
	"resources/sprites/barrel.png",//11 
	"resources/sprites/pillar.png",//12
	"resources/sprites/greenlight.png"//13
};

static const std::string g_gunSprite = "resources/sprites/gun.png";
static const std::string g_gunSprite_fire = "resources/sprites/gun_fire.png";

//Main menu

static const auto g_mainTxtTitle = "Casual Game";
static const auto g_mainTxtStartGame = "Start Game";
static const auto g_mainTxtRestartGame = "Restart Game";
static const auto g_mainTxtLevelEditor = "Level Editor";
static const auto g_mainTxtToggleFullscreen = "Toggle Fullscreen";
static const auto g_mainTxtQuit = "Quit";

// Level Editor

static const float g_editorPlayerArrowScale = 8.0f;
static const int g_editorMenuWidth = 230;

static const auto g_editorTxtSwitchMode = "Switch mode";
static const auto g_editorTxtLoadDefault = "Load Default";
static const auto g_editorTxtLoad = "Load";
static const auto g_editorTxtSave = "Save";
static const auto g_editorTxtQuit = "Back";
static const auto g_editorTxtTexture = "Texture";
static const auto g_editorTxtSprite = "Sprite";

static const auto g_editorTxtModeWall = "Wall Mode (LMB - place, RMB - delete)";
static const auto g_editorTxtModeEntity = "Entities Mode (LMB - Select/Move, RMB - place, Del - delete)";

// Play state

static const float g_playMinimapScale = 8.0f;
static const int g_playMinimapTransparency = 140;

static const double g_Pi = 3.141592653589793238463;
static const int g_playDrawDarkened = 1;
static const int g_playhDrawHighlighted = 2;

static const double g_gunShotTime = 0.15; //seconds
static const double g_gunShotDelayTime = 0.5; //seconds

