#pragma once

#include "game/ui/GlobalContext.hh"
#include "game/ui/RegisteredPadManager.hh"
#include "game/ui/SaveManagerProxy.hh"
#include "game/ui/Section.hh"

namespace UI {

class SectionManager {
public:
    Section *currentSection();
    SectionId nextSectionId() const;
    SectionId lastSectionId() const;
    RegisteredPadManager &registeredPadManager();
    SaveManagerProxy *saveManagerProxy();
    GlobalContext *globalContext();
    void setNextSection(SectionId sectionId, Page::Anim anim);
    void REPLACED(createSection)();
    REPLACE void createSection();
    void REPLACED(destroySection)();
    REPLACE void destroySection();
    void calc();
    void REPLACED(startChangeSection)(s32 delay, u32 color);
    REPLACE void startChangeSection(s32 delay, u32 color);
    void transitionToError(u32 errorCode);

    static SectionManager *Instance();

private:
    Section *m_currentSection;
    u8 _04[0x0c - 0x04];
    SectionId m_nextSectionId;
    SectionId m_lastSectionId;
    u8 _14[0x34 - 0x14];
    RegisteredPadManager m_registeredPadManager;
    SaveManagerProxy *m_saveManagerProxy;
    u8 _94[0x98 - 0x94];
    GlobalContext *m_globalContext;

    static SectionManager *s_instance;
};
static_assert(sizeof(SectionManager) == 0x9c);

} // namespace UI
