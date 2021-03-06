//
//  Definitions.h
//  SurvivalDungeon
//
//  Created by Marlon Andrade on 16/02/2015.
//
//

#ifndef __SurvivalDungeon__Definitions__
#define __SurvivalDungeon__Definitions__

#define FONT_SCALE_RATIO 950

#pragma mark - Dimensions

#define DUNGEON_SIZE            100
#define INITIAL_COORDINATE      cocos2d::Vec2(DUNGEON_SIZE/2, DUNGEON_SIZE/2)
#define TILE_DIMENSION          100
#define TILE_PLACEMENT_SCALE    0.3
#define NOT_CALCULATED_DISTANCE 1000

#define ACTION_DICE_BACKGROUND_BUTTON_MARGIN 5

#define ACTION_DICE_MARGIN          10
#define ACTION_DICE_MARGIN_PER_DICE 3
#define DOCK_MARGIN                 5
#define END_TURN_MARGIN             5

#define DOCKABLE_HIDDEN_MARGIN      13

#pragma mark - HUD

#define HUD_FONT_NAME "fonts/DOWNCOME.TTF"
#define HUD_FONT_SIZE 15

#pragma mark - Game Over

#define GAME_OVER_FONT_NAME "fonts/DOWNCOME.TTF"
#define GAME_OVER_FONT_SIZE 50

#pragma mark - Game Mechanics

#define MAX_ACTION_REROLLS 2
#define CHARACTER_MAX_HP   6

#pragma mark - Room Coeficients

#define RUNE_COEFICIENT          0.225
#define RUNE_CONSTANT            6
#define MINOR_MONSTER_COEFICIENT 0.18
#define MINOR_MONSTER_CONSTANT   2
#define TREASURE_COEFICIENT      0.05
#define TREASURE_CONSTANT        1

#pragma mark - Animation Durations

#define PLACE_ROOM_DURATION       0.5
#define MOVE_DICE_DURATION        0.2
#define OVERLAY_DURATION          0.2
#define MAGIC_DICE_DURATION       0.3
#define TURN_INFO_DURATION        1.5
#define ADJUST_CAMERA_DURATION    0.7

#pragma mark - Colors / Alpha

#define OK_COLOR                  cocos2d::Color3B(120, 255, 120)
#define OVERLAY_COLOR             cocos2d::Color4B(0, 0, 0, 0)
#define OVERLAY_OPACITY           180 // 255 -> fully opaque | 0 -> transparent

#pragma mark - Z Order

#define CHARACTER_DICE_Z_ORDER         0
#define DUNGEON_ROOM_Z_ORDER           0
#define GAME_OBJECTS_Z_ORDER           1
#define ACTION_DICE_Z_ORDER            2
#define DUNGEON_ROOM_WITH_CHAR_Z_ORDER 15
#define OVERLAY_Z_ORDER                10
#define DRAG_Z_ORDER_DELTA             50
#define MAX_Z_ORDER                    999

#pragma mark - Errors

#define NO_ERROR                       0
#define NO_REROLL_AVAILABLE            0x9000

#pragma mark - Helpers

#define IS(object, class) dynamic_cast<class*>(object) > 0

#pragma mark - Initial State

#define INITIAL_EXPERIENCE 0
#define INITIAL_COINS      0

#pragma mark - Strings

#define HUD_LEVEL_STRING "Andar"
#define HUD_XP_STRING    "Experiência"
#define HUD_COINS_STRING "Moedas"

#endif /* defined(__SurvivalDungeon__Definitions__) */
