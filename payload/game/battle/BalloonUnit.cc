#include "BalloonUnit.hh"

#include "game/system/RaceConfig.hh"
#include "game/ui/TeamColors.hh"

namespace Battle {

void BalloonUnit::onAdd(u32 r4, u8 playerId, u8 r6, u8 r7) {
    REPLACED(onAdd)(r4, playerId, r6, r7);

    GXColor color{255, 255, 255, 255};
    const auto &raceScenario = System::RaceConfig::Instance()->raceScenario();
    if (raceScenario.spMaxTeamSize >= 2) {
        color = UI::TeamColors::Get(raceScenario.players[playerId].spTeam);
    }
    SP_LOG("%u %u %u %u", playerId, color.r, color.g, color.b);

    auto resMdl = m_drawMdl->resMdl();
    auto resMat = resMdl.GetResMat(0);
    auto resTevColor = resMat.GetResMatTevColor();
    resTevColor.GXSetTevKColor(GX_KCOLOR0, color);
    resTevColor.DCStore(true);
}

} // namespace Battle
