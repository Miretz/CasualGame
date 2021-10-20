#pragma once

#include <string>
#include <string_view>
#include <array>

#include "FontLoader.h"

constexpr auto g_gameTitle = "Casual Game by Miretz";

constexpr auto g_defaultWidth = 800;
constexpr auto g_defaultHeight = 600;

constexpr int g_textureWidth = 128;
constexpr int g_textureHeight = 128;

constexpr auto g_frameRateLimit = 120;

//the constant value is in radians/second
constexpr double g_lookSpeed = 3.0;
constexpr double g_moveSpeed = 5.0;

// Resources

const FontLoader g_fontLoader{ "resources/font/OtherF.ttf" };

constexpr auto g_defaultLevelFile = "resources/levels/level1.txt";
constexpr auto g_defaultLevelSpriteFile = "resources/levels/level1_sprites.txt";

constexpr auto g_customLevelDirectory = "resources/levels/custom/";

constexpr int g_textureCount = 13;

constexpr std::array<std::string_view, 13> g_textureFiles{
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

constexpr auto g_gunSprite = "resources/sprites/gun.png";
constexpr auto g_gunSprite_fire = "resources/sprites/gun_fire.png";

constexpr auto g_mainVertexShader = "resources/shaders/main.vert";
constexpr auto g_mainFragmentShader = "resources/shaders/main.frag";

//Main menu

constexpr auto g_mainTxtTitle = "Casual Game";
constexpr auto g_mainTxtStartGame = "Start Game";
constexpr auto g_mainTxtRestartGame = "Restart Game";
constexpr auto g_mainTxtLevelEditor = "Level Editor";
constexpr auto g_mainTxtToggleFullscreen = "Toggle Fullscreen";
constexpr auto g_mainTxtQuit = "Quit";

constexpr auto g_mainMenuPadding = 20.0f;

// Level Editor

constexpr float g_editorPlayerArrowScale = 8.0f;
constexpr int g_editorMenuWidth = 230;
constexpr int g_editorPadding = 6;

constexpr auto g_editorTxtSwitchMode = "Switch mode";
constexpr auto g_editorTxtLoadDefault = "Load Default";
constexpr auto g_editorTxtLoad = "Load";
constexpr auto g_editorTxtSave = "Save";
constexpr auto g_editorTxtQuit = "Back";
constexpr auto g_editorTxtTexture = "Texture";
constexpr auto g_editorTxtSprite = "Sprite";

constexpr auto g_editorTxtModeWall = "Wall Mode (LMB - place, RMB - delete)";
constexpr auto g_editorTxtModeEntity = "Entities Mode (LMB - Select/Move, RMB - place, Del - delete)";

// Play state

constexpr float g_playMinimapScale = 8.0f;
constexpr int g_playMinimapTransparency = 140;

constexpr double g_Pi = 3.14159;

constexpr double g_gunShotTime = 0.15; //seconds
constexpr double g_gunShotDelayTime = 0.5; //seconds

