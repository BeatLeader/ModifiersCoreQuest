#include "logger.hpp"

// Returns a logger, useful for printing debug messages
Paper::ConstLoggerContext<19UL> getLogger()
{
    static auto fastContext = Paper::Logger::WithContext<"ModifiersCoreQuest">();
    return fastContext;
}