//
// Created by 222ro on 11-Jun-22.
//

#ifndef MATAM_EX4_WIZARD_H
#define MATAM_EX4_WIZARD_H

#include "Player.h"
#include "../utilities.h"
#include <string>

// --------------------------------------- Wizard Player Class --------------------------------------- //

class Wizard: public Player {
public:
    /**
     * Constructor of Wizard Class.
     *
     * Sets the parameters of the Player class.
     * @param name - name of the player
     */
    explicit Wizard(const std::string &name);

    Wizard(const Wizard& wizard) = default;

    Wizard& operator=(const Wizard& wizard) = default;

    ~Wizard() override = default;

    /**
     * doubles the hp amount added to the Wizard.
     *
     * @param HPToHeal - Amount og HP to double
     */
    void heal(int HPToHeal) override;

    /**
     * Outputs Wizard's Parameters according to given structure
     */
    void printInfo(std::ostream &output) const override;
    
    static const std::string JOB_NAME;
};

#endif //MATAM_EX4_WIZARD_H
