//
//  ActionDiceStateRolled.h
//  MasmorraDados
//
//  Created by Marlon Andrade on 4/26/15.
//
//

#ifndef __MasmorraDados__ActionDiceStateRolled__
#define __MasmorraDados__ActionDiceStateRolled__

#include "DiceState.h"
#include "cocos2d.h"

class ActionDiceStateRolled : public DiceState {
public:
  CREATE_FUNC(ActionDiceStateRolled);
  virtual bool init();
  
  virtual bool canChangeState();
  virtual void changeState(Dice *dice);
};

#endif /* defined(__MasmorraDados__ActionDiceStateRolled__) */
