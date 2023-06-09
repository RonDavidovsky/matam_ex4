#ifndef MATAM_EX4_MERCHANT_H
#define MATAM_EX4_MERCHANT_H

#include "Card.h"
#include "../Players/Player.h"
#include "../utilities.h"
#include <string>

// --------------------------------------- Merchant Card Class --------------------------------------- //

class Merchant : public Card {
public:
    /**
     * Constructor for the Merchant Card
     */
    explicit Merchant();
    
    /**
     * Applies encounter according to given rules:
     *      A player can pay 5 coins to buy 1 Health Point boost.
     *      A player can pay 10 coins to but 1 Force Point boost.
     *      A player can choose to leave without action.
     *      If A player has insufficient funds, no action will be taken and the turn will end.
     */
    void applyEncounter(Player &player) const override;
    
    /**
     * Copy contstructor, =operator and destructor.
     */
    Merchant(const Merchant& merchant) = default;
    Merchant& operator=(const Merchant& merchant) = default;
    ~Merchant() override = default;

    
    static const std::string MERCHANT_NAME;
    static const int HEALTH_POTION_OPTION = 1;
    static const int FORCE_BOOST_OPTION = 2;
    static const int LEAVE_OPTION = 0;
    static const char HEALTH_POTION_OPTION_STR = '1';
    static const char FORCE_BOOST_OPTION_STR = '2';
    static const char LEAVE_OPTION_STR = '0';
    static const int HEALTH_POTION_COST = 5;
    static const int FORCE_BOOST_COST = 10;
    static const int HEALTH_TO_BOOST = 1;
    static const int FORCE_TO_BOOST = 1;
private:
    static bool inputCheck(const std::string input);
};

#endif //MATAM_EX4_MERCHANT_H
