#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <string>

#include "Mtmchkin.h"
#include "Exception.h"
#include "utilities.h"
#include "Cards/Card.h"
#include "Cards/Vampire.h"
#include "Cards/Goblin.h"
#include "Cards/Dragon.h"
#include "Cards/Treasure.h"
#include "Cards/Fairy.h"
#include "Cards/Pitfall.h"
#include "Cards/Barfight.h"
#include "Cards/Merchant.h"
#include "Cards/Gang.h"
#include "Players/Player.h"
#include "Players/Wizard.h"
#include "Players/Rogue.h"
#include "Players/Fighter.h"


std::unique_ptr<Card> Mtmchkin::chooseCardByType(std::string &cardType, int deckSize) {
    switch(Card::CARDS_MAP[cardType]) {
        case Card::CardTypes::Vampire: {
            return std::unique_ptr<Card>(new Vampire());
        }
        
        case Card::CardTypes::Goblin: {
            return std::unique_ptr<Card>(new Goblin());
        }
        
        case Card::CardTypes::Dragon: {
            return std::unique_ptr<Card>(new Dragon());
        }
            
        case Card::CardTypes::Treasure: {
            return std::unique_ptr<Card>(new Treasure());
        }
            
        case Card::CardTypes::Merchant: {
            return std::unique_ptr<Card>(new Merchant());
        }
            
        case Card::CardTypes::Fairy: {
            return std::unique_ptr<Card>(new Fairy());
        }
            
        case Card::CardTypes::Pitfall: {
            return std::unique_ptr<Card>(new Pitfall());
        }
            
        case Card::CardTypes::Barfight: {
            return std::unique_ptr<Card>(new Barfight());
        }
            
        default: {
            throw DeckFileFormatError(deckSize);
        }
    }
}

std::unique_ptr<Battle> Mtmchkin::chooseBattleCardByType(std::string &cardType, int deckSize) {
    switch(Card::CARDS_MAP[cardType]) {
        case Card::CardTypes::Vampire: {
            return std::unique_ptr<Battle>(new Vampire());
        }
        
        case Card::CardTypes::Goblin: {
            return std::unique_ptr<Battle>(new Goblin());
        }
        
        case Card::CardTypes::Dragon: {
            return std::unique_ptr<Battle>(new Dragon());
        }
            
        default: {
            throw DeckFileFormatError(deckSize);
        }
    }
}

 std::unique_ptr<Player> Mtmchkin::choosePlayerByClass(std::string &name, std::string &playerClass) {
     switch(Player::PLAYERS_MAP[playerClass]) {
         case Player::PlayerClasses::Rogue: {
             return std::unique_ptr<Player>(new Rogue(name));
         }
            
         case Player::PlayerClasses::Wizard: {
             return std::unique_ptr<Player>(new Wizard(name));
         }
             
         case Player::PlayerClasses::Fighter: {
             return std::unique_ptr<Player>(new Fighter(name));
         }
         
         default: {
             throw InvalidPlayer();
         }
     }
}

bool Mtmchkin::isNumber(std::string str) {
    for(unsigned int i=0; i < str.size(); i++) {
        if(i==0 && str[i] == MINUS_SIGN) {
            continue;
        }
        
        if(str[i] < ZERO_DIGIT || str[i] > NINE_DIGIT) {
            return false;
        }
    }
    
    return true;
}

void Mtmchkin::createDeck(std::ifstream &deckFile, std::deque<std::unique_ptr<Card>> &m_deck) {
    std::string cardType;
    std::unique_ptr<Card> newCard;
    std::unique_ptr<Gang> gang;
    
    int deckSize = 0;
    int lineNumber = 0;
    bool isGang = false;
    
    while(std::getline(deckFile, cardType)) {
        lineNumber++;
        if(!isGang) {
            deckSize++;
        }
        
        if(Card::CARDS_MAP[cardType] == Card::CardTypes::Gang && !isGang) {
            isGang = true;
            gang = std::unique_ptr<Gang>(new Gang());
        }
        
        else if(cardType == Card::END_GANG && isGang) {
            isGang = false;
            m_deck.push_back(std::move(gang));
        }
        
        else {
            newCard = chooseCardByType(cardType, lineNumber);
            
            if(isGang) {
                gang->addMonster(chooseBattleCardByType(cardType, lineNumber));
            }
 
            else {
                m_deck.push_back(std::move(newCard));
            }
        }
    }
    
    if(deckSize < MIN_DECK_SIZE) {
        throw DeckFileInvalidSize();
    }
    
    if(isGang) {
        throw DeckFileFormatError(lineNumber+1);
    }
}

bool Mtmchkin::validatePlayerName(std::string &input, std::string &name) {
    char currentChar;
    
    for(unsigned int i = 0; i < input.size(); i++) {
        currentChar = input[i];
        if(currentChar == SPACE) {
            break;
        }
        
        if(i == Player::NAME_MAX_LENGTH) {
            return false;
        }
        
        if((currentChar < A_LETTER_UPPERCASE || currentChar > Z_LETTER_UPPERCASE) &&
           (currentChar < A_LETTER_LOWERCASE || currentChar > Z_LETTER_LOWERCASE)) {
            return false;
        }
        
        name += currentChar;
    }
    
    return true;
}

bool Mtmchkin::validateClassAndCreatePlayer(std::string input, std::string name, std::unique_ptr<Player> &player) {
    std::string playerClass;
    for(unsigned int i = (int)name.size() + 1; i < input.size(); i++) {
        playerClass += input[i];
    }

    try {
        player = choosePlayerByClass(name, playerClass);
    }
    catch (const InvalidPlayer& err) {
        return false;
    }
    
    return true;
}

void Mtmchkin::createPlayersQueue(int teamSize, std::deque<std::unique_ptr<Player>> &playersQueue) {
    std::string input;
    bool validInput = false;

    for(int i = 0; i < teamSize; ++i) {
        printInsertPlayerMessage();
        while(!validInput) {
            std::getline(std::cin, input);
            std::string name;
            std::unique_ptr<Player> player;
            
            if(!validatePlayerName(input, name)) {
                printInvalidName();
                continue;
            }
            
            if(!validateClassAndCreatePlayer(input, name, player)) {
                printInvalidClass();
                continue;
            }
            
            playersQueue.push_back(std::move(player));
            validInput = true;
        }
        
        validInput = false;
    }
}

void Mtmchkin::insertIntoLeaderboard(std::unique_ptr<Player> &player, std::deque<std::unique_ptr<Player>> &playersWon,
                                     std::deque<std::unique_ptr<Player>> &playersLost) {
    if(player->getLevel() == Player::MAX_LEVEL) {
        playersWon.push_back(std::move(player));
    }
    
    else if(player->isKnockedOut()) {
        playersLost.push_front(std::move(player));
    }
}

int Mtmchkin::getTeamSize() {
    std::string teamSizeString;
    
    int teamSize = 0;
    bool validInput = false;

    while(!validInput) {
        printEnterTeamSizeMessage();
        try {
            std::getline(std::cin, teamSizeString);
            if(!isNumber(teamSizeString)) {
                printInvalidTeamSize();
                continue;
            }
            
            teamSize = std::stoi(teamSizeString);
            
            if(teamSize < Mtmchkin::TEAM_MIN_SIZE || teamSize > Mtmchkin::TEAM_MAX_SIZE) {
                printInvalidTeamSize();
                continue;
            }
        }
        
        catch(std::invalid_argument& invalidArgument) {
            printInvalidTeamSize();
            continue;
        }
        
        validInput = true;
    }
    
    return teamSize;
}

Mtmchkin::Mtmchkin(const std::string filename):
    m_numberOfRounds(0),
    m_gameOver(false)
{
    printStartGameMessage();
    std::ifstream deckFile(filename);
    if(!deckFile) {
        throw DeckFileNotFound();
    }

    createDeck(deckFile, m_deck);
    int teamSize = getTeamSize();

    createPlayersQueue(teamSize, Mtmchkin::m_playersQueue);
}

void Mtmchkin::playRound() {
    printRoundStartMessage(m_numberOfRounds+1);
    
    int playersPlayed = 0;
    int originalQueueSize = (int)m_playersQueue.size();
    while(playersPlayed < originalQueueSize) {
        std::unique_ptr<Player> currentPlayer = std::move(m_playersQueue.front());
        m_playersQueue.pop_front();
        
        printTurnStartMessage(currentPlayer->getName());
        
        std::unique_ptr<Card> currentCard = std::move(m_deck.front());
        m_deck.pop_front();
        
        currentCard->applyEncounter(*currentPlayer);

        if(currentPlayer->getLevel() == Player::MAX_LEVEL || currentPlayer->isKnockedOut()) {
            insertIntoLeaderboard(currentPlayer, m_playersWon, m_playersLost);
        }
        
        else {
            m_playersQueue.push_back(std::move(currentPlayer));
        }
        
        m_deck.push_back(std::move(currentCard));
        playersPlayed++;
    }
    
    m_numberOfRounds++;
    
    if(m_playersQueue.size() == 0) {
        m_gameOver = true;
    }
}

int Mtmchkin::getNumberOfRounds() const {
    return m_numberOfRounds;
}

bool Mtmchkin::isGameOver() const {
    return m_gameOver;
}

void Mtmchkin::printLeaderBoard() const {
    if(isGameOver()) {
        printGameEndMessage();
    }
    
    printLeaderBoardStartMessage();
    int ranking = 1;

    for(const std::unique_ptr<Player> &currentPlayer : m_playersWon) {
        printPlayerLeaderBoard(ranking, *currentPlayer);
        ranking++;
    }
    
    for(const std::unique_ptr<Player> &currentPlayer : m_playersQueue) {
        printPlayerLeaderBoard(ranking, *currentPlayer);
        ranking++;
    }
    
    for(const std::unique_ptr<Player> &currentPlayer : m_playersLost) {
        printPlayerLeaderBoard(ranking, *currentPlayer);
        ranking++;
    }
}

