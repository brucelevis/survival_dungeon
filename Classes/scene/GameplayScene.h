//
//  GameplayScene.h
//  SurvivalDungeon
//
//  Created by Marlon Andrade on 16/02/2015.
//
//

#ifndef __SurvivalDungeon__GameplayScene__
#define __SurvivalDungeon__GameplayScene__

#include "cocos2d.h"

#include "Game.h"
#include "Turn.h"

class DungeonLayer;
class HudLayer;
class PlayerTurnLayer;
class TurnChangeLayer;

class GameplayScene : public cocos2d::Scene {
  CC_SYNTHESIZE_RETAIN(cocos2d::EventListenerCustom*,
                       _actionDicesRolledListener,
                       ActionDicesRolledListener);
  CC_SYNTHESIZE_RETAIN(cocos2d::EventListenerCustom*,
                       _levelAdvancedListener,
                       LevelAdvancedListener);
  CC_SYNTHESIZE_RETAIN(cocos2d::EventListenerCustom*,
                       _experienceChangedListener,
                       ExperienceChangedListener);
  CC_SYNTHESIZE_RETAIN(cocos2d::EventListenerCustom*,
                       _coinsChangedListener,
                       CoinsChangedListener);
  CC_SYNTHESIZE_RETAIN(cocos2d::EventListenerCustom*,
                       _characterDiedListener,
                       CharacterDiedListener);
  CC_SYNTHESIZE_RETAIN(cocos2d::EventListenerCustom*,
                       _turnHasStartedListener,
                       TurnHasStartedListener);
  CC_SYNTHESIZE_RETAIN(cocos2d::EventListenerCustom*,
                       _turnHasEndedListener,
                       TurnHasEndedListener);
  
public:
  virtual bool init();
  CREATE_FUNC(GameplayScene);
  
  DungeonLayer* getDungeonLayer();
  
private:
  void _setupChildren();
  
  void _setupEventHandlers();
  
  HudLayer* _getHudLayer();
  PlayerTurnLayer* _getPlayerTurnLayer();
  TurnChangeLayer* _getTurnChangeLayer();
  
  void _roomsHasBeenPlaced(cocos2d::Vector<RoomPlacementData*> placements);
  
#pragma mark - Events
  void _handleActionDicesRolled(cocos2d::EventCustom* event);
  void _handleLevelAdvanced(cocos2d::EventCustom* event);
  void _handleExperienceChanged(cocos2d::EventCustom* event);
  void _handleCoinsChanged(cocos2d::EventCustom* event);
  void _handleCharacterDied(cocos2d::EventCustom* event);
  void _handleTurnHasStarted(cocos2d::EventCustom* event);
  void _handleTurnHasEnded(cocos2d::EventCustom* event);
  
};

#endif /* defined(__SurvivalDungeon__GameplayScene__) */
