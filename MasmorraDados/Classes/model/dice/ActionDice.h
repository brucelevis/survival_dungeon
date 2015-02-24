//
//  ActionDice.h
//  MasmorraDados
//
//  Created by Marlon Andrade on 18/02/2015.
//
//

#ifndef __MasmorraDados__ActionDice__
#define __MasmorraDados__ActionDice__

#include "Dice.h"

class ActionDice : public Dice {
public:
  virtual bool init();
  CREATE_FUNC(ActionDice);
  
  virtual DiceFace* roll();
};

#endif /* defined(__MasmorraDados__ActionDice__) */
