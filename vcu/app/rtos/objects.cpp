#include "rtos/objects.hpp"
#include "tasks/can_dispatch.hpp"
#include "tasks/can_recovery.hpp"
#include "tasks/control_loop.hpp"
#include "tasks/pdu_heartbeat.hpp"
#include "tasks/pedals.hpp"
#include "tasks/polling.hpp"


namespace rtos {

tasks::CANDispatchTask CANBus1DispatchTask;
tasks::CANDispatchTask CANBus2DispatchTask;
tasks::ControlLoopTask ControlLoopTask;
tasks::PedalsTask PedalsTask;
tasks::PollingTask PollingTask;
tasks::CANRecoveryTask CANRecoveryTask;
tasks::PDUHeartbeatTask PDUHeartbeatTask;


Queue<drivers::can::Message, 64> CANBus1RxQueue;
Queue<drivers::can::Message, 64> CANBus2RxQueue;

} // namespace rtos
