#include "Merchant.h"

const std::string Merchant::MERCHANT_NAME = "Merchant";

Merchant::Merchant():
    Card(MERCHANT_NAME)
{}

bool Merchant::inputCheck(const std::string input) {
    if(input.size() == 1 && (input[0] == HEALTH_POTION_OPTION_STR ||
                             input[0] == LEAVE_OPTION_STR || input[0] == FORCE_BOOST_OPTION_STR)) {
        return true;
    }
    
    return false;
}

void Merchant::applyEncounter(Player &player) const {
    printMerchantInitialMessageForInteractiveEncounter(std::cout, player.getName(), player.getCoins());

    std::string playerStringInput;
    bool validInput = false;
    int playerInput = -1;
    while(!validInput) {
        std::getline(std::cin, playerStringInput);
     
        if (!inputCheck(playerStringInput)) {
            printInvalidInput();
            continue;
        }
        playerInput = std::stoi(playerStringInput);
        validInput = true;
    }
    
    int payment = 0;

    switch (playerInput) {
        case LEAVE_OPTION:
            printMerchantSummary(std::cout, player.getName(), LEAVE_OPTION, payment);
            break;

        case HEALTH_POTION_OPTION:
            if(player.pay(HEALTH_POTION_COST)) {
                payment = HEALTH_POTION_COST;
                player.heal(HEALTH_TO_BOOST);
                printMerchantSummary(std::cout, player.getName(), HEALTH_POTION_OPTION, payment);
            }
            else {
                printMerchantInsufficientCoins(std::cout);
                printMerchantSummary(std::cout, player.getName(), HEALTH_POTION_OPTION, payment);
            }
            break;

        case FORCE_BOOST_OPTION:
            if(player.pay(FORCE_BOOST_COST)) {
                payment = FORCE_BOOST_COST;
                player.buff(FORCE_TO_BOOST);
                printMerchantSummary(std::cout, player.getName(), FORCE_BOOST_OPTION, payment);
            }
            else {
                printMerchantInsufficientCoins(std::cout);
                printMerchantSummary(std::cout, player.getName(), FORCE_BOOST_OPTION, payment);
            }
            break;

        default:
            break;
    }
}
