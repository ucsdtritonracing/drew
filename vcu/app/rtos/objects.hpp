#pragma once

#include "tasks/can_dispatch.hpp"
#include "tasks/can_recovery.hpp"
#include "tasks/control_loop.hpp"
#include "tasks/pdu_heartbeat.hpp"
#include "tasks/pedals.hpp"
#include "tasks/polling.hpp"
#include "rtos/queue.hpp"
#include "drivers/can/can_utils.hpp"


namespace rtos {

extern tasks::CANDispatchTask CANBus1DispatchTask;
extern tasks::CANDispatchTask CANBus2DispatchTask;
extern tasks::ControlLoopTask ControlLoopTask;
extern tasks::PedalsTask PedalsTask;
extern tasks::PollingTask PollingTask;
extern tasks::CANRecoveryTask CANRecoveryTask;
extern tasks::PDUHeartbeatTask PDUHeartbeatTask;

extern Queue<drivers::can::Message, 64> CANBus1RxQueue;
extern Queue<drivers::can::Message, 64> CANBus2RxQueue;

} // namespace rtos
