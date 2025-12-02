#include "update_weather_agent.hpp"

#include "keynodes/keynodes.hpp"


UpdateWeatherAgent::UpdateWeatherAgent()
{
    m_logger = utils::ScLogger(utils::ScLogger::ScLogType::File, "logs/UpdateWeatherAgent.log", utils::ScLogLevel::Debug);
}

ScAddr UpdateWeatherAgent::GetEventSubscriptionElement() const
{
    return Keynodes::concept_woken_up_human;
}

ScTemplate UpdateWeatherAgent::GetInitiationConditionTemplate(HumanWakeUpEvent const & event) const
{
    ScTemplate templ;
    templ.Triple(
        Keynodes::concept_woken_up_human,
        ScType::VarPermPosArc,
        ScType::VarNode
    );
    return templ;
}

ScAddr UpdateWeatherAgent::GetActionClass() const
{
    return Keynodes::action_update_weather;
}

ScResult UpdateWeatherAgent::DoProgram(HumanWakeUpEvent const & event, ScAction & action)
{
    ScAddr const & humanAddr = event.GetArcTargetElement();

    ScTemplate templateToSearchHomeTown;
    templateToSearchHomeTown.Quintuple(
        humanAddr,
        ScType::VarCommonArc,
        ScType::VarNode >> "_home_town",
        ScType::VarPermPosArc,
        Keynodes::nrel_home_town
    );
    
    ScTemplateSearchResult homeTownResult;
    if (!m_context.SearchByTemplate(templateToSearchHomeTown, homeTownResult))
    {
        m_logger.Warning("Home town not specified for found human");
        return action.FinishUnsuccessfully();
    }

    ScAddr const & homeTownAddr = homeTownResult[0]["_home_town"];

    ScTemplate templateToSearchTownWeather;
    templateToSearchTownWeather.Quintuple(
        homeTownAddr,
        ScType::VarCommonArc >> "_arc_to_weather",
        ScType::VarNodeLink >> "_weather",
        ScType::VarPermPosArc,
        Keynodes::nrel_weather
    );

    ScTemplateSearchResult weatherResult;
    if (m_context.SearchByTemplate(templateToSearchTownWeather, weatherResult))
    {
        ScAddr const & arcToWeatherAddr = weatherResult[0]["_arc_to_weather"];
        ScAddr const & weatherAddr = weatherResult[0]["_weather"];

        m_context.EraseElement(arcToWeatherAddr);
        m_context.EraseElement(weatherAddr);
    }


    /*
        TODO(artem): Implement retrieving weather data from some Weather API
    */
    std::string const & weather = "2 degree";
    ScAddr const & newWeatherLinkAddr = m_context.GenerateLink(ScType::ConstNodeLink);
    m_context.SetLinkContent(newWeatherLinkAddr, weather);

    ScTemplate templateToGenerateTownWeather;
    templateToGenerateTownWeather.Quintuple(
        homeTownAddr,
        ScType::VarCommonArc,
        newWeatherLinkAddr,
        ScType::VarPermPosArc,
        Keynodes::nrel_weather
    );

    ScTemplateGenResult genResult;
    m_context.GenerateByTemplate(templateToGenerateTownWeather, genResult);

    m_logger.Debug("Weather updated");
    return action.FinishSuccessfully();

}