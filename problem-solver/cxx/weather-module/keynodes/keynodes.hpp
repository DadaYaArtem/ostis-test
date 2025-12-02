#pragma once

#include <sc-memory/sc_keynodes.hpp>

class Keynodes : public ScKeynodes
{
public:
    static inline ScKeynode const concept_woken_up_human{"concept_woken_up_human"};
    static inline ScKeynode const action_update_weather{"action_update_weather"};
    static inline ScKeynode const nrel_home_town{"nrel_home_town"};
    static inline ScKeynode const nrel_weather{"nrel_weather"};
};