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
    juce::ScopedWriteLock locker(project->getLock());

    juce::String fileUrl = project->getPath() + "/" + project->getName()
        + this->projectExtension;
    juce::File file(fileUrl);
    juce::FileInputStream istream(file);
    if (!istream.openedOk()) {
        return false;
    }

    std::unique_ptr<vocalshaper::files::vsp3::Project> ptrProto
        = std::make_unique<vocalshaper::files::vsp3::Project>();
    vocalshaper::ProjectMeta::MetaObject meta;

    if (!vocalshaper::files::vsp3::FilePacker::unpackData(istream, ptrProto.get())) {
        return false;
    }

    if (!vocalshaper::files::vsp3::ProtoConverter::parseFromProto(
        ptrProto.get(), project->getPtr(), &meta)) {
        return false;
    }

    project->getMeta()->resetMeta(meta);

    return true;
}

bool ProjectIO::write(vocalshaper::ProjectProxy* project) const
{
    if (!project) {
        return false;
    }
    juce::ScopedReadLock locker(project->getLock());

    std::unique_ptr<vocalshaper::files::vsp3::Project> ptrProto
        = std::make_unique<vocalshaper::files::vsp3::Project>();
    if (!vocalshaper::files::vsp3::ProtoConverter::serilazeToProto(
        project->getPtr(), &project->getMeta()->toMeta(), ptrProto.get())) {
        return false;
    }

    juce::String fileUrl = project->getPath() + "/" + project->getName()
        + this->projectExtension;
    juce::File file(fileUrl);
    juce::FileOutputStream ostream(file);
    if (!ostream.openedOk()) {
        return false;
    }
    if (!ostream.truncate().wasOk()) {
        return false;
    }

    if (!vocalshaper::files::vsp3::FilePacker::packData(ptrProto.get(), ostream)) {
        return false;
    }

    return true;
}
