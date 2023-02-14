#pragma once

#include "game/obj/Obj.hh"

namespace Battle {

class BalloonUnit : public Geo::ObjEntity {
public:
    BalloonUnit(u8 balloonId, u8 teamId);
    ~BalloonUnit() override;
    // ...

    void REPLACED(onAdd)(u32 r4, u8 playerId, u8 r6, u8 r7);
    REPLACE void onAdd(u32 r4, u8 playerId, u8 r6, u8 r7);

private:
    u8 _0b0[0x1e4 - 0x0b0];
};
static_assert(sizeof(BalloonUnit) == 0x1e4);

} // namespace Battle
