#include "match3plugin_plugin.h"
#include "match3.h"

#include <qqml.h>

void Match3PluginPlugin::registerTypes(const char *uri)
{
    // @uri Match3
    qmlRegisterType<Match3>(uri, 1, 0, "Match3");
}
