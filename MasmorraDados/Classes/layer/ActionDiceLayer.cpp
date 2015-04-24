//
//  ActionDiceLayer.cpp
//  MasmorraDados
//
//  Created by Marlon Andrade on 4/19/15.
//
//

#include "ActionDiceLayer.h"

#include "Definitions.h"

USING_NS_CC;

#pragma mark - Public Interface

ActionDiceLayer* ActionDiceLayer::createWithDices(Vector<ActionDice *> dices) {
  auto layer = new (std::nothrow) ActionDiceLayer();
  if (layer && layer->initWithDices(dices)) {
    layer->autorelease();
  } else {
    CC_SAFE_DELETE(layer);
  }
  return layer;
}

bool ActionDiceLayer::initWithDices(Vector<ActionDice *> dices) {
  if (!Layer::init()) {
    return false;
  }
  
  this->setDices(dices);
  this->_setupChilds(dices);
  this->resetRollCount();
  
  return true;
}

void ActionDiceLayer::resetRollCount() {
  auto rerollButton = this->_getRerollButton();
  rerollButton->setEnabled(true);
  rerollButton->setBright(true);
  this->_rollCount = 1;
}

#pragma mark - Private Interface

void ActionDiceLayer::_setupChilds(Vector<ActionDice *> dices) {
  auto backgroundLayer = LayerColor::create(Color4B(0, 0, 0, 100));
  
  auto diceSprite = dices.at(0)->getSprite();
  
  auto width = this->getContentSize().width;
  auto height = diceSprite->getContentSize().height + ACTION_DICE_MARGIN * 2;
  backgroundLayer->setContentSize(Size(width, height));
  
  this->addChild(backgroundLayer);
  
  auto actionDices = this->_createActionDices(dices);
  this->addChild(actionDices);
  auto dicesX = backgroundLayer->getContentSize().width / 2 -
                actionDices->getContentSize().width / 2;
  auto dicesY = ACTION_DICE_MARGIN;
  actionDices->setPosition(Vec2(dicesX, dicesY));
  
  auto rerollButton = this->_createRerollButton();
  this->addChild(rerollButton);
  auto rerollX = actionDices->getPosition().x / 2;
  auto rerollY = height / 2;
  rerollButton->setPosition(Vec2(rerollX, rerollY));
  
  auto okButton = this->_createOkButton();
  this->addChild(okButton);
  auto okX = width - (actionDices->getPosition().x / 2);
  auto okY = height / 2;
  okButton->setPosition(Vec2(okX, okY));
}

Node* ActionDiceLayer::_createActionDices(Vector<ActionDice*> dices) {
  auto actionDices = Node::create();
  
  auto marginPerDice = ACTION_DICE_MARGIN_PER_DICE;
  
  auto diceSprite = dices.at(0)->getSprite();
  
  int numberOfDices = dices.size();
  for (int i = 0; i < numberOfDices; i++) {
    diceSprite = dices.at(i)->getSprite();
    
    auto width = diceSprite->getContentSize().width;
    auto height = diceSprite->getContentSize().height;
    
    auto x = width * i + marginPerDice * i + width / 2;
    auto y = height / 2;
    
    diceSprite->setPosition(Vec2(x, y));
    actionDices->addChild(diceSprite, i);
  }
  
  auto totalWidth = diceSprite->getContentSize().width * numberOfDices +
                    marginPerDice * (numberOfDices - 1);
  auto totalHeight = diceSprite->getContentSize().height;
  
  actionDices->setContentSize(Size(totalWidth, totalHeight));
  
  return actionDices;
}

Node* ActionDiceLayer::_createRerollButton() {
  auto rerollButton = ui::Button::create("images/button/reroll-normal.png",
                                         "images/button/reroll-selected.png",
                                         "images/button/reroll-disabled.png");
  rerollButton->setName(DICE_REROLL_BUTTON_NAME);
  rerollButton->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
      this->_rollCount++;
      
      for (auto dice : this->getDices()) {
        if (dice->isSelected()) {
          dice->roll();
        } else if (!dice->isDisabled()) {
          dice->setDisabled();
        }
      }
      
      if (this->_rollCount > 3) {
        rerollButton->setEnabled(false);
        rerollButton->setBright(false);
      }
      log("reroll pressed");
    }
  });
  
  return rerollButton;
}

Node* ActionDiceLayer::_createOkButton() {
  auto okButton = ui::Button::create("images/button/ok-normal.png",
                                     "images/button/ok-selected.png",
                                     "images/button/ok-disabled.png");
  okButton->setName(DICE_OK_BUTTON_NAME);
  okButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
    if (type == ui::Widget::TouchEventType::ENDED) {
      log("ok pressed");
    }
  });
  
  return okButton;
}

ui::Button* ActionDiceLayer::_getRerollButton() {
  return (ui::Button*) this->getChildByName(DICE_REROLL_BUTTON_NAME);
}