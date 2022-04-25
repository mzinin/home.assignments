#include "main.hpp"

#include "engine/matching_engine.hpp"
#include "types/event.hpp"

std::vector<std::string> run(std::vector<std::string> const& input)
{
    MatchingEngine engine;
    for (const auto& str : input)
    {
        engine.process(Event{str});
    }
    return engine.report();
}
