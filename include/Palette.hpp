#pragma once

// ╔══════════════════════════════════════════════════════════════════╗
// ║                    DUNOPIA — Palette.hpp                         ║
// ║    Growtopia-inspired dark UI + vibrant world colors             ║
// ╚══════════════════════════════════════════════════════════════════╝

#include <SFML/Graphics.hpp>

namespace Palette
{
    // World
    inline const sf::Color DIRT         {139,  90,  43};
    inline const sf::Color GRASS        { 34, 139,  34};
    inline const sf::Color BAMBOO       {180, 135,  50};
    inline const sf::Color BAMBOO_DARK  {120,  85,  20};
    inline const sf::Color STONE        {105,  98,  90};
    inline const sf::Color STONE_DARK   { 65,  60,  55};
    inline const sf::Color QBLOCK_BASE  {210, 160,  25};
    inline const sf::Color QBLOCK_USED  { 70,  55,  25};

    // Sprites
    inline const sf::Color SKIN         {210, 155, 105};
    inline const sf::Color HAIR         { 40,  20,  10};
    inline const sf::Color BARONG       {245, 245, 228};
    inline const sf::Color BARONG_LINE  {210, 210, 195};
    inline const sf::Color PANTS        { 38,  38,  80};
    inline const sf::Color SHOES        { 25,  15,   8};
    inline const sf::Color ENEMY_ARMOR  {160,  28,  28};
    inline const sf::Color ENEMY_DARK   {100,  15,  15};
    inline const sf::Color ENEMY_HELM   { 72,  52,  32};
    inline const sf::Color ENEMY_PLUME  {200,  25,  25};
    inline const sf::Color WHITE        {255, 255, 255};

    // Growtopia-style UI
    inline const sf::Color UI_BG        { 22,  18,  14, 232};
    inline const sf::Color UI_BORDER    {180, 130,  50, 255};
    inline const sf::Color UI_BORDER2   { 90,  65,  25, 255};
    inline const sf::Color UI_HEADER    { 38,  28,  12, 255};
    inline const sf::Color UI_HEADER2   { 58,  44,  17, 255};
    inline const sf::Color UI_BTN_A     { 45,  80, 175};
    inline const sf::Color UI_BTN_B     { 35, 120,  45};
    inline const sf::Color UI_BTN_C     {150,  75,  15};
    inline const sf::Color UI_BTN_D     {120,  25, 120};
    inline const sf::Color UI_CORRECT   { 35, 185,  65};
    inline const sf::Color UI_WRONG     {195,  45,  45};
    inline const sf::Color UI_DISABLED  { 48,  42,  36};

    // Text
    inline const sf::Color TEXT_MAIN    {255, 240, 210};
    inline const sf::Color TEXT_GOLD    {255, 210,  50};
    inline const sf::Color TEXT_DIM     {155, 140, 115};
    inline const sf::Color TEXT_GREEN   { 80, 220, 100};
    inline const sf::Color TEXT_RED     {235,  80,  80};

    // HUD
    inline const sf::Color HUD_BAR_FG   { 50, 205,  80};
    inline const sf::Color HUD_BAR_BG   { 35,  30,  22};
    inline const sf::Color HEART_ON     {220,  48,  48};
    inline const sf::Color HEART_OFF    { 70,  38,  38};
}
