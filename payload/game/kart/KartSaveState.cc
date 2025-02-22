#include "KartSaveState.hh"

#include <cstring>

namespace Kart {

KartSaveState::KartSaveState(KartAccessor accessor, VehiclePhysics *physics, KartItem *item) {
    save(accessor, physics, item);
}

void KartSaveState::save(KartAccessor accessor, VehiclePhysics *physics, KartItem *item) {
    m_externalVel = physics->m_externalVel;
    m_internalVel = physics->m_internalVel;
    m_mainRot = physics->m_mainRot;
    m_pos = physics->m_pos;

    m_internalSpeed = accessor.move->m_internalSpeed;
    m_boostState.m_types = accessor.move->m_boost.m_types;
    m_boostState.m_boostMultipler = accessor.move->m_boost.m_boostMultipler;
    m_boostState.m_boostAcceleration = accessor.move->m_boost.m_boostAcceleration;
    m_boostState.m_1c = accessor.move->m_boost.m_1c;
    m_boostState.m_boostSpeedLimit = accessor.move->m_boost.m_boostSpeedLimit;

    m_item = *item;

    for (u8 i = 0; i < accessor.settings->tireCount; i++) {
        m_wheelPhysics[i].m_realPos = accessor.tire[i]->m_wheelPhysics->m_realPos;
        m_wheelPhysics[i].m_lastPos = accessor.tire[i]->m_wheelPhysics->m_lastPos;
        m_wheelPhysics[i].m_lastPosDiff = accessor.tire[i]->m_wheelPhysics->m_lastPosDiff;
    }
}

void KartSaveState::reload(KartAccessor accessor, VehiclePhysics *physics, KartItem *item) {
    physics->m_externalVel = m_externalVel;
    physics->m_internalVel = m_internalVel;
    physics->m_mainRot = m_mainRot;
    physics->m_pos = m_pos;

    accessor.move->m_internalSpeed = m_internalSpeed;
    accessor.move->m_boost.m_types = m_boostState.m_types;
    accessor.move->m_boost.m_boostMultipler = m_boostState.m_boostMultipler;
    accessor.move->m_boost.m_boostAcceleration = m_boostState.m_boostAcceleration;
    accessor.move->m_boost.m_1c = m_boostState.m_1c;
    accessor.move->m_boost.m_boostSpeedLimit = m_boostState.m_boostSpeedLimit;

    *item = m_item;

    for (u8 i = 0; i < accessor.settings->tireCount; i++) {
        accessor.tire[i]->m_wheelPhysics->m_realPos = m_wheelPhysics[i].m_realPos;
        accessor.tire[i]->m_wheelPhysics->m_lastPos = m_wheelPhysics[i].m_lastPos;
        accessor.tire[i]->m_wheelPhysics->m_lastPosDiff = m_wheelPhysics[i].m_lastPosDiff;
    }
}

} // namespace Kart
