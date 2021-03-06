#include "pch.h"

#include "driver/Driver.h"
#include "driver/CommandLine.h"
#include "basic/Engine.h"

namespace Goto
{
namespace Driver
{

bool ExecuteCompilerEngine(CommandLineContext* CLIContext)
{
    Basic::EngineBuilder engineBuilder;

    try
    {
        //
        // Initialize Options
        //
        engineBuilder.ebCompOptLevel = CLIContext->doOptLevel1.GetValOf<bool>()
                                           ? Basic::EngineOptLevel::MINIMIZED_OPT
                                           : CLIContext->doOptLevel2.GetValOf<bool>()
                                                 ? Basic::EngineOptLevel::SMALL_CODE_OPT
                                                 : CLIContext->doOptLevel3.GetValOf<bool>()
                                                       ? Basic::EngineOptLevel::FULL_OPT
                                                       : Basic::EngineOptLevel::NO_OPT;

        CompileInfo* compInfo = engineBuilder.ebCompInfo;

        compInfo->m_ciSourceFile = std::move(CLIContext->m_clcTargetSourceFile);
        compInfo->m_ciVerbose    = CLIContext->doEnableVerbose.GetValOf<bool>();
        compInfo->m_ciDumpAst    = CLIContext->doEnableASTDump.GetValOf<bool>();
    }
    catch (std::bad_any_cast& bc)
    {
        // Those DriverOption object uses std::any, also GetValOf uses std::any_cast
        return false;
    }

    std::unique_ptr<Basic::Engine> engine = engineBuilder.BuildEngine();
    return engine->StartCompile();
}

} // namespace Driver
} // namespace Goto