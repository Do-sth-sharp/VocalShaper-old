#include "ProjectIO.h"
#include <libJModule.h>
#include <libVocalShaper.h>

ProjectIO::ProjectIO()
{
    //以下获取全局设置
    juce::var* config = nullptr;
    bool ok = false;
    jmadf::CallInterface<const juce::String&, juce::var*&, bool&>(
        "WuChang.JMADF.GlobalConfigs", "GetReference",
        "config", config, ok
        );
    if (ok && (config != nullptr)) {
        if ((*config)["ProjectExtension"].isString()) {
            this->projectExtension = (*config)["ProjectExtension"].toString();
        }
    }
}

bool ProjectIO::read(vocalshaper::ProjectProxy* project) const
{
    if (!project) {
        return false;
    }
    
    juce::String fileUrl = project->getPath() + "/" + project->getName()
        + this->projectExtension;
    juce::File file(fileUrl);
    juce::FileInputStream istream(file);

    return vocalshaper::files::vsp3::readProject(project, istream);
}

bool ProjectIO::write(vocalshaper::ProjectProxy* project) const
{
    if (!project) {
        return false;
    }

    juce::File dir(project->getPath());
    if (!dir.exists()) {
        dir.createDirectory();
    }

    juce::String fileUrl = project->getPath() + "/" + project->getName()
        + this->projectExtension;
    juce::File file(fileUrl);
    juce::FileOutputStream ostream(file);
    
    return vocalshaper::files::vsp3::writeProject(project, ostream);
}
