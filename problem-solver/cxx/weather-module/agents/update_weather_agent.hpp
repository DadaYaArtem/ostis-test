#pragma once

#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_agent.hpp>

using HumanWakeUpEvent = ScEventAfterGenerateOutgoingArc<ScType::ConstPermPosArc>;

class UpdateWeatherAgent : public ScAgent<HumanWakeUpEvent>
{
public:
  UpdateWeatherAgent();

  ScAddr GetEventSubscriptionElement() const override;

  ScTemplate GetInitiationConditionTemplate(HumanWakeUpEvent const & event) const override;

  ScAddr GetActionClass() const override;

  ScResult DoProgram(HumanWakeUpEvent const & event, ScAction & action) override;
};