#include "rtos/objects.hpp"


namespace rtos {

tasks::CANDispatchTask CANBus1DispatchTask;
tasks::CANDispatchTask CANBus2DispatchTask;
tasks::CANTxTask CANBus1TxTask;
tasks::CANTxTask CANBus2TxTask;
tasks::ControlLoopTask ControlLoopTask;
tasks::PedalsTask PedalsTask;
tasks::PollingTask PollingTask;
tasks::CANRecoveryTask CANRecoveryTask;
tasks::PDUHeartbeatTask PDUHeartbeatTask;
tasks::BroadcastTask BroadcastTask;
tasks::WheelsTask WheelsTask;


Queue<drivers::can::Message, 64> CANBus1RxQueue;
Queue<drivers::can::Message, 64> CANBus2RxQueue;

} // namespace rtos
