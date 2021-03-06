#include "basic/Engine.h"
#include "basic/FileUtils.h"
#include "basic/Debug.h"

namespace Goto
{
namespace Basic
{

//
// Engine Builder
//
std::unique_ptr<Engine> EngineBuilder::BuildEngine()
{
    return std::move(std::unique_ptr<Engine>(new Engine(this)));
    ;
}

EngineBuilder::EngineBuilder()
{
    ebCompInfo = new CompileInfo();
}

//
// Engine Implements
//
Engine::Engine(EngineBuilder* builder)
{
    m_egCompStage = EnginePhase::STAGE_PRE_INIT;
    m_egCompOpts  = builder->ebCompOptLevel;
    m_egCompInfo  = std::shared_ptr<CompileInfo>(builder->ebCompInfo);
}

EnginePhase Engine::GetEnginePhase() const
{
    return m_egCompStage;
}

EnginePhase Engine::SetEnginePhase(EnginePhase newStage)
{
    std::swap(m_egCompStage, newStage);
    return newStage;
}

EngineOptLevel Engine::GetEngineOptLevel() const
{
    return m_egCompOpts;
}

//
// Compile Section
//
bool Engine::StartCompile()
{
    //
    // Initializing stage
    //
    this->SetEnginePhase(EnginePhase::STAGE_PST_INIT);
    const std::string& compSrcName    = m_egCompInfo->m_ciSourceFile;
    const std::string& compSrcPath    = m_egCompInfo->m_ciSourcePath;
    std::string        compTargetPath = "";

    //
    // Source finding stage
    //
    if (fuFileExists(compSrcName))
    {
        // Check that file exists on current path
        // Or we are finding on path included file path

        compTargetPath = compSrcName;
    }

    if (fuFileExists(compSrcPath, compSrcName))
    {
        // We are getting full path of file.

        compTargetPath = fuGetFullFilePath(compSrcPath, compSrcName);
    }

    if (compTargetPath.empty())
    {
        // We couldn't find source file even we tried to find on absolute path
        noway_assert(true, "[INTERNAL] Failed to find source file");
        return false;
    }

    //
    // Source reading stage
    //
    FileViewer sourceFileReader = FileViewer(compTargetPath);

    if (sourceFileReader.GetFileSize() == 0)
    {
        noway_assert(true, "[INTERNAL] Size of target source file is zero");
        return false;
    }

    //
    // Source tokenize and lexing stage.
    //
    this->SetEnginePhase(EnginePhase::STAGE_TOKENLIZE);

    return true;
}

} // namespace Basic
} // namespace Goto