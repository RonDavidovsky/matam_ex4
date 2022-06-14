#include "Merchant.h"

Merchant::Merchant() : Card(MERCHANT_NAME)
{}

void Merchant::applyEncounter(Player &player) const {
    printMerchantInitialMessageForInteractiveEncounter(std::cout, player.getName(), player.getCoins());

    std::string playerStringInput;
    // TODO Catch thrown exceptions.
    std::getline(std::cin, playerStringInput);
    int playerInput = std::stoi(playerStringInput);

    int payment = 0;

    std::cin >> playerInput;

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
                printMerchantSummary(std::cout, player.getName(), LEAVE_OPTION, payment);
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
                printMerchantSummary(std::cout, player.getName(), LEAVE_OPTION, payment);
            }
            break;

        default:
            break;
    }
}