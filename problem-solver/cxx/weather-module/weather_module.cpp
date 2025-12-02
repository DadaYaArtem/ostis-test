#include "weather_module.hpp"

#include "agents/update_weather_agent.hpp"

SC_MODULE_REGISTER(WeatherModule)
->Agent<UpdateWeatherAgent>();