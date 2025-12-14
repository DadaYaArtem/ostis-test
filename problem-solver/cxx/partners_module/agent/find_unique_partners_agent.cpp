#include "find_unique_partners_agent.hpp"
#include "keynodes/partners_keynodes.hpp"
#include "utils.hpp"
#include <sc-memory/sc_memory.hpp>

using namespace std;

ScAddr FindUniquePartnersAgent::GetActionClass() const
{
  return PartnersKeynodes::action_find_unique_partners;
}

ScResult FindUniquePartnersAgent::DoProgram(ScAction & action)
{
  // Получаем аргументы действия
  auto const & [argsAddr] = action.GetArguments<1>();
  
  if (!m_context.IsElement(argsAddr))
    return action.FinishWithError();
  
  // Извлекаем две компании из аргументов
  ScAddr const company1 = resolve_arg(m_context, argsAddr, PartnersKeynodes::rrel_1);
  ScAddr const company2 = resolve_arg(m_context, argsAddr, PartnersKeynodes::rrel_2);
  
  if (!company1.IsValid() || !company2.IsValid())
    return action.FinishWithError();
  
  // Создаём структуру для результата
  ScStructure result = m_context.GenerateStructure();
  
  // Собираем партнёров первой компании
  ScIterator5Ptr it1 = m_context.CreateIterator5(
    company1,
    ScType::ConstCommonArc,
    ScType::Unknown,
    ScType::ConstPermPosArc,
    PartnersKeynodes::nrel_partner
  );
  
  // Для каждого партнёра company1
  while (it1->Next())
  {
    ScAddr const partner = it1->Get(2);
    
    // Проверяем, есть ли этот партнёр у company2
    ScIterator5Ptr check = m_context.CreateIterator5(
      company2,
      ScType::ConstCommonArc,
      partner,
      ScType::ConstPermPosArc,
      PartnersKeynodes::nrel_partner
    );
    
    // Если НЕТ у company2 - это уникальный партнёр company1
    if (!check->Next())
    {
      result << partner;
    }
  }
  
  // Устанавливаем результат
  action.SetResult(result);
  
  return action.FinishSuccessfully();
}
