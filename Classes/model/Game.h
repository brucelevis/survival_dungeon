//
//  Game.h
//  SurvivalDungeon
//
//  Created by Marlon Andrade on 15/02/2015.
//
//

#ifndef __SurvivalDungeon__Game__
#define __SurvivalDungeon__Game__

#include "cocos2d.h"

#include "ActionDice.h"
#include "Dungeon.h"
#include "DungeonRoom.h"
#include "GameObject.h"
#include "Player.h"

class Turn;

class Game : public GameObject {
  CC_SYNTHESIZE_RETAIN(cocos2d::EventListenerCustom*,
                       _freeBootSpentListener,
                       FreeBootSpentListener);
  CC_SYNTHESIZE_RETAIN(cocos2d::EventListenerCustom*,
                       _diceSpentListener,
                       DiceSpentListener);
  CC_SYNTHESIZE_RETAIN(cocos2d::EventListenerCustom*,
                       _monsterKilledListener,
                       MonsterKilledListener);
  CC_SYNTHESIZE_RETAIN(cocos2d::EventListenerCustom*,
                       _endPlayerTurnListener,
                       EndPlayerTurnListener);
  
  CC_SYNTHESIZE_RETAIN(Dungeon*, _dungeon, Dungeon);
  CC_SYNTHESIZE_RETAIN(Player*, _player, Player);
  
  CC_SYNTHESIZE(cocos2d::Vector<ActionDice*>, _actionDices, ActionDices);
  CC_SYNTHESIZE(bool, _freeBootUsed, FreeBootUsed);
  
  CC_PROPERTY(int, _damageTaken, DamageTaken);
  
  CC_PROPERTY(int, _level, Level);
  CC_PROPERTY(int, _experience, Experience);
  CC_PROPERTY(int, _coins, Coins);
protected:
  cocos2d::Vector<DungeonRoom*> _availableRooms;
  Turn* _turn;
  cocos2d::Vec2 _characterCoordinate;
  
public:
  static Game* getInstance();
  
  virtual bool init();
  void resetForPlayerTurn();
  
  void advanceLevel();
  
  bool canCharacterMove();
  
  Turn* getTurn(void) const { return _turn; }
  void setTurn(Turn* turn);
  
  cocos2d::Vec2 getCharacterCoordinate(void) const { return _characterCoordinate; }
  void setCharacterCoordinate(cocos2d::Vec2 coordinate);
  
  void start();
  
  void characterMovedTo(cocos2d::Vec2 coordinate);
  
  cocos2d::Vector<ActionDice*> getDockedDices();
  cocos2d::ValueMap getAvailableSkills();
  
  bool isInitialTurn();
  bool isPlayerTurn();
  void executeCurrentTurn();
  void finishCurrentTurn();
  void restoreFreeBoot();
  
  void resolveCombat();
  void removeDamageDealtToMonsters();
  
  void calculateDamageTaken();
  DungeonRoom* getRoomForCharacterCoordinate();
  
private:
  void _setupInitialState();
  void _setupDungeon();
  void _setupAvaiableRooms();
  void _setupActionDices();
  void _setupEventHandlers();
  
  DungeonRoom* _pickRandomRoom();
  
  void _dispatchDiceSpent();
  
#pragma mark - Event Handlers
  void _handleActionFreeBootSpent(cocos2d::EventCustom* event);
  void _handleMonsterKilled(cocos2d::EventCustom* event);
  void _handleActionDiceSpent(cocos2d::EventCustom* event);
  void _handleEndPlayerTurn(cocos2d::EventCustom* event);
};

#endif /* defined(__SurvivalDungeon__Game__) */
