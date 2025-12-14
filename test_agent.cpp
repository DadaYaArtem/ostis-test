#include <sc-memory/sc_memory.hpp>
#include <iostream>

#include "problem-solver/cxx/inspection-route-module/keynodes/inspection_route_keynodes.hpp"
#include "problem-solver/cxx/inspection-route-module/agents/find_inspection_route_agent.hpp"

int main()
{
    sc_memory_params params;
    sc_memory_params_clear(&params);
    params.clear = SC_FALSE;
    params.repo_path = "kb.bin";
    params.config_file = "ostis-example-app.ini";
    params.ext_path = "build/Release/extensions";
    
    ScMemory::Initialize(params);
    ScMemoryContext ctx;
    
    std::cout << "=== Testing Inspection Route Agent ===" << std::endl;
    
    // Регистрируем агент
    ctx.SubscribeAgent<FindInspectionRouteAgent>();
    
    // Ищем example_factory
    ScAddr factoryAddr;
    ScIterator3Ptr it3 = ctx.CreateIterator3(
        ScType::ConstNode,
        ScType::ConstPermPosArc,
        ScType::ConstNode
    );
    
    while (it3->Next())
    {
        ScAddr node = it3->Get(0);
        std::string idtf;
        if (ctx.GetElementSystemIdentifier(node, idtf))
        {
            if (idtf == "example_factory")
            {
                factoryAddr = node;
                std::cout << "Found example_factory!" << std::endl;
                break;
            }
        }
    }
    
    if (!factoryAddr.IsValid())
    {
        std::cout << "ERROR: example_factory not found!" << std::endl;
        ScMemory::Shutdown();
        return 1;
    }
    
    // Создаём действие
    ScAction action = ctx.GenerateAction(
        InspectionRouteKeynodes::action_find_inspection_route
    );
    
    action.SetArguments(factoryAddr);
    
    std::cout << "Initiating action..." << std::endl;
    
    // Запускаем действие
    bool success = action.InitiateAndWait(10000);
    
    if (success && action.IsFinishedSuccessfully())
    {
        std::cout << "SUCCESS! Route found!" << std::endl;
        
        ScStructure result = action.GetResult();
        std::cout << "Result structure size: " << result.Size() << " elements" << std::endl;
    }
    else
    {
        std::cout << "FAILED!" << std::endl;
    }
    
    ctx.UnsubscribeAgent<FindInspectionRouteAgent>();
    ScMemory::Shutdown();
    
    return 0;
}
