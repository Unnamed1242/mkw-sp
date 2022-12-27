#include "CtrlRaceDebugPanel.hh"

#include "game/kart/KartObjectManager.hh"
#include "game/kart/KartRollback.hh"
#include "game/kart/VehiclePhysics.hh"
#include "game/system/RaceManager.hh"
#include "game/system/SaveManager.hh"

#include <sp/cs/RaceClient.hh>

namespace UI {

class WStringWriter {
public:
    WStringWriter();
    ~WStringWriter();

    const wchar_t *data() const;

    void writePlayer(u32 playerId);
    void writeOnline(u32 playerId);

private:
    void write(const wchar_t *restrict format, ...);
    void write(const char *name, const Vec3 &v);
    void write(const char *name, const Vec3 &v, const Vec3 &w);

    std::array<wchar_t, 256> m_buffer{};
    size_t m_offset = 0;
};

WStringWriter::WStringWriter() = default;

WStringWriter::~WStringWriter() = default;

const wchar_t *WStringWriter::data() const {
    return m_buffer.data();
}

void WStringWriter::writePlayer(u32 playerId) {
    write(L"F %u\n", System::RaceManager::Instance()->frameId());
    auto *object = Kart::KartObjectManager::Instance()->object(playerId);
    write("P", *object->getPos());
    write("EV", *object->getVehiclePhysics()->externalVel());
    write("IV", *object->getVehiclePhysics()->internalVel());
    write("MRV", *object->getVehiclePhysics()->movingRoadVel());
    write("MWV", *object->getVehiclePhysics()->movingWaterVel());
}

void WStringWriter::writeOnline(u32 playerId) {
    u32 frameId = System::RaceManager::Instance()->frameId();
    if (auto *raceClient = SP::RaceClient::Instance()) {
        s32 drift = raceClient->drift();
        auto frame = raceClient->frame();
        if (frame) {
            write(L"F/D/SF/CF %u %d %u %u\n", frameId, drift, frame->id, frame->clientId);
        } else {
            write(L"F/D %u %d\n", frameId, drift);
        }
    } else {
        write(L"F %u\n", frameId);
    }
    auto *object = Kart::KartObjectManager::Instance()->object(playerId);
    if (auto *rollback = object->getKartRollback()) {
        write("P/RP", *object->getPos(), rollback->posDelta());
    } else {
        write("P", *object->getPos());
    }
    write("EV", *object->getVehiclePhysics()->externalVel());
    write("IV", *object->getVehiclePhysics()->internalVel());
}

void WStringWriter::write(const wchar_t *restrict format, ...) {
    if (m_offset >= m_buffer.size()) {
        return;
    }

    va_list args;
    va_start(args, format);
    s32 result = vswprintf(m_buffer.data() + m_offset, m_buffer.size() - m_offset, format, args);
    va_end(args);

    if (result < 0) {
        m_offset = m_buffer.size();
    } else {
        m_offset += result;
    }
}

void WStringWriter::write(const char *name, const Vec3 &v) {
    write(L"%s %.2f %.2f %.2f\n", name, v.x, v.y, v.z);
}

void WStringWriter::write(const char *name, const Vec3 &v, const Vec3 &w) {
    write(L"%s %.2f %.2f %.2f %.2f %.2f %.2f\n", name, v.x, v.y, v.z, w.x, w.y, w.z);
}

CtrlRaceDebugPanel::CtrlRaceDebugPanel() = default;

CtrlRaceDebugPanel::~CtrlRaceDebugPanel() = default;

void CtrlRaceDebugPanel::calcSelf() {
    u32 playerId = getPlayerId();
    auto *saveManager = System::SaveManager::Instance();
    auto setting = saveManager->getSetting<SP::ClientSettings::Setting::DebugPanel>();
    WStringWriter writer;
    switch (setting) {
    case SP::ClientSettings::DebugPanel::Player:
        writer.writePlayer(playerId);
        break;
    case SP::ClientSettings::DebugPanel::Online:
        writer.writeOnline(playerId);
        break;
    default:
        break;
    }
    MessageInfo info{};
    info.strings[0] = writer.data();
    setMessageAll(6602, &info);
}

void CtrlRaceDebugPanel::load() {
    m_localPlayerId = 0;

    const char *groups[] = {
        nullptr,
        nullptr,
    };
    LayoutUIControl::load("game_image", "DebugPanel", "DebugPanel", groups);
}

} // namespace UI
