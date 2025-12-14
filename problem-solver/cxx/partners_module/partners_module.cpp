#include "partners_module.hpp"
#include "agent/find_intersection_agent.hpp"
#include "agent/find_union_agent.hpp"
#include "agent/find_unique_partners_agent.hpp"


SC_MODULE_REGISTER(PartnersModule)
  ->Agent<FindIntersectionAgent>()
  ->Agent<FindUnionAgent>()
  ->Agent<FindUniquePartnersAgent>();