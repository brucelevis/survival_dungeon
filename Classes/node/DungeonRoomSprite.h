//
//  DungeonRoomSprite.h
//  SurvivalDungeon
//
//  Created by Marlon Andrade on 5/31/15.
//
//

#ifndef __SurvivalDungeon__DungeonRoomSprite__
#define __SurvivalDungeon__DungeonRoomSprite__

#include "cocos2d.h"

class CharacterDiceSprite;
class Dice;
class DungeonRoom;

class DungeonRoomSprite : public cocos2d::Sprite {
public:
  static DungeonRoomSprite* createWithRoom(DungeonRoom* room);
  virtual bool initWithRoom(DungeonRoom* room);
  
  CharacterDiceSprite* getCharacterDiceSprite();
  
  void setCoordinate(cocos2d::Vec2 coordinate);
  void addDice(Dice* dice);
  void adjustChildren();
  
  bool canCharacterMoveToLocation(cocos2d::Vec2 location);
};

#endif /* defined(__SurvivalDungeon__DungeonRoomSprite__) */
