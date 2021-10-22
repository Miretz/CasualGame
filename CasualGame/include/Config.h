#pragma once

#include <string>
#include <string_view>
#include <array>

#include "FontLoader.h"

inline constexpr auto g_gameTitle = "Casual Game by Miretz";

inline constexpr auto g_defaultWidth = 800;
inline constexpr auto g_defaultHeight = 600;

inline constexpr int g_textureWidth = 128;
inline constexpr int g_textureHeight = 128;

inline constexpr auto g_frameRateLimit = 120;

//the constant value is in radians/second
inline constexpr double g_lookSpeed = 3.0;
inline constexpr double g_moveSpeed = 5.0;

// Resources

inline extern const FontLoader g_fontLoader{ "resources/font/OtherF.ttf" };

inline constexpr auto g_defaultLevelFile = "resources/levels/level1.txt";
inline constexpr auto g_defaultLevelSpriteFile = "resources/levels/level1_sprites.txt";

inline constexpr auto g_customLevelDirectory = "resources/levels/custom/";

inline constexpr int g_textureCount = 13;

inline constexpr std::array<std::string_view, 13> g_textureFiles{
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

inline constexpr auto g_gunSprite = "resources/sprites/gun.png";
inline constexpr auto g_gunSprite_fire = "resources/sprites/gun_fire.png";

inline constexpr auto g_mainVertexShader = "resources/shaders/main.vert";
inline constexpr auto g_mainFragmentShader = "resources/shaders/main.frag";

//Main menu

inline constexpr auto g_mainTxtTitle = "Casual Game";
inline constexpr auto g_mainTxtStartGame = "Start Game";
inline constexpr auto g_mainTxtRestartGame = "Restart Game";
inline constexpr auto g_mainTxtLevelEditor = "Level Editor";
inline constexpr auto g_mainTxtToggleFullscreen = "Toggle Fullscreen";
inline constexpr auto g_mainTxtQuit = "Quit";

inline constexpr auto g_mainMenuPadding = 20.0f;

// Level Editor

inline constexpr float g_editorPlayerArrowScale = 8.0f;
inline constexpr int g_editorMenuWidth = 230;
inline constexpr int g_editorPadding = 6;

inline constexpr auto g_editorTxtSwitchMode = "Switch mode";
inline constexpr auto g_editorTxtLoadDefault = "Load Default";
inline constexpr auto g_editorTxtLoad = "Load";
inline constexpr auto g_editorTxtSave = "Save";
inline constexpr auto g_editorTxtQuit = "Back";
inline constexpr auto g_editorTxtTexture = "Texture";
inline constexpr auto g_editorTxtSprite = "Sprite";

inline constexpr auto g_editorTxtModeWall = "Wall Mode (LMB - place, RMB - delete)";
inline constexpr auto g_editorTxtModeEntity = "Entities Mode (LMB - Select/Move, RMB - place, Del - delete)";

// Play state

inline constexpr float g_playMinimapScale = 8.0f;
inline constexpr int g_playMinimapTransparency = 140;

inline constexpr double g_Pi = 3.14159;

inline constexpr double g_gunShotTime = 0.15; //seconds
inline constexpr double g_gunShotDelayTime = 0.5; //seconds

