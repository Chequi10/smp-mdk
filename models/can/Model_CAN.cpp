#include <iostream>
#include "Model_CAN.h"
#include "Smp/ISimulator.h"
#include "Smp/IPublication.h"
#include <boost/format.hpp>

/* aca creo la clase para el BUS_CAN pero el CONNECT es el que tiene el AddSimulationTimeEvent */
SERIE can;

void BUS_CAN::Init(void)
{
    // Initialise references
    logger = NULL;
    scheduler = NULL;
    // eventManager = NULL;
}

// Publish fields to environment
void BUS_CAN::Publish(Smp::IPublication *receiver)
{
    if (state == Smp::MSK_Created)
    {
        state = Smp::MSK_Publishing;
    }
    else
    {
        throw Smp::IModel::InvalidModelState(state, Smp::MSK_Created);
    }
}
// Perform custom configuration.
void BUS_CAN::Configure(Smp::Services::ILogger *)
{
    if (state == Smp::MSK_Publishing)
    {
        state = Smp::MSK_Configured;
    }
    else
    {
        throw Smp::IModel::InvalidModelState(state, Smp::MSK_Publishing);
    }
}

// Get access to services, and register entry points.
void BUS_CAN::Connect(Smp::ISimulator *simulator)
{
    if (state == Smp::MSK_Configured)
    {
        state = Smp::MSK_Connected;
        logger = simulator->GetLogger();
        scheduler = simulator->GetScheduler();

        for (auto &nombre_nodo : this->nombre_numeros_nodos)
        {
            std::cout << "    CONECTANDO NODOS :     " << nombre_nodo << "\n";

            NODO *node1 = dynamic_cast<NODO *>(simulator->GetModel(nombre_nodo.c_str()));
            assert(node1);
            this->loadNODOx(node1);
        }
    }
    else
    {
        throw Smp::IModel::InvalidModelState(state, Smp::MSK_Configured);
    }
}

// cargo los datos del nodo al bus
void BUS_CAN::loadNODOx(NODO *node1)
{
    node1->SetBus(this);
    this->numeros_nodos.push_back(node1);
}

// injecto los datos del bus a la rede CANBUS
void BUS_CAN::inyecto_nodo()
{
    for (auto &node1 : this->numeros_nodos)
    {
        if (node1->GetId() != can.Message_send->originator_id)
        {
            node1->receive_CAN();
        }
    }

    //  delete can.Message_send;
}

/* Implementation-specific */

extern "C" Smp::IModel *create_BUS_CAN(Smp::String8 name, Smp::IComposite *parent)
{
    return dynamic_cast<Smp::IModel *>(new BUS_CAN(name, parent));
}

extern "C" void destroy_BUS_CAN(Smp::IModel *obj)
{
    delete dynamic_cast<BUS_CAN *>(obj);
}

void BUS_CAN::ReadInitializationParameters(const YAML::Node &paramsNode)
{
    std::cout << "INICIANDO BUS_CAN: " << paramsNode["nodes"].size() << std::endl;
    for (YAML::const_iterator cantidad = paramsNode["nodes"].begin();
         cantidad != paramsNode["nodes"].end(); ++cantidad)
    {
        std::cout << "AGREGANDO NODOS: " << cantidad->as<std::string>() << "\n";
        this->nombre_numeros_nodos.push_back(cantidad->as<std::string>());
    }
}

// Initialise private fields
void NODO::Init(void)
{
    // Initialise references
    logger = NULL;
    scheduler = NULL;
    // eventManager = NULL;

    // Turn Count and Reset methods into EntryPoints

    starting = new NODOEntryPoint("arranque", "se pone en marcha NODO", this, &NODO::startup);
    sending = new NODOEntryPoint("envio", "envio de dato del NODO", this, &NODO::send_CAN);
}

// Log a message to the logger on Reset
void NODO::ResetSMP(void)
{
    SERIE::stop();
    // Send an information message to the logger
    logger->Log(this, "Reseteo NODO", Smp::Services::LMK_Information);
}

// Publish fields to environment
void NODO::Publish(Smp::IPublication *receiver)
{
    if (state == Smp::MSK_Created)
    {
        state = Smp::MSK_Publishing;
        //  receiver->PublishField("Nodo virtual", "datos del nodo", &speed);
    }
    else
    {
        throw Smp::IModel::InvalidModelState(state, Smp::MSK_Created);
    }
}
// Perform custom configuration.
void NODO::Configure(Smp::Services::ILogger *)
{
    if (state == Smp::MSK_Publishing)
    {
        state = Smp::MSK_Configured;
    }
    else
    {
        throw Smp::IModel::InvalidModelState(state, Smp::MSK_Publishing);
    }
}

// Get access to services, and register entry points.
void NODO::Connect(Smp::ISimulator *simulator)
{
    if (state == Smp::MSK_Configured)
    {
        state = Smp::MSK_Connected;
        logger = simulator->GetLogger();
        scheduler = simulator->GetScheduler();
        // eventManager = simulator->GetEventManager();
        if (this->status_yml != 0)
        {
            scheduler->AddSimulationTimeEvent(starting, 0, 1000000000, 0);
            scheduler->AddSimulationTimeEvent(sending, 0,   500000000, -1);
        }
        // eventManager->Subscribe(Smp::Services::SMP_EnterStandbyId, reset);
    }
    else
    {
        throw Smp::IModel::InvalidModelState(state, Smp::MSK_Configured);
    }
}

void NODO::startup()
{
    can.start();
}

void NODO::send_CAN()
{
    std::cout << this->GetName() << " : Message Sending  id: " << this->id_yml << " payload: " << uint32_t{can.Message_send->data[0]} << std::endl;
    can.Message_send->originator_id = this->id_yml;
    can.Message_send->id = this->id_yml;
    can.Message_send->canid = this->canid_yml;
    can.Message_send->dlc = this->dlc_yml;
    can.Message_send->data[0] = this->payload0_yml;
    can.Message_send->data[1] = this->payload1_yml;

    can.write_some();
    this->cargo_mensaje();
}

void NODO::receive_CAN()
{
    uint32_t id_virtual;
    id_virtual = this->id_yml;
    std::cout << this->GetName() << " : Message received id: " << this->id_yml << " payload: " << this->payload0_yml << std::endl;

    if (can.Message_receive->id == 0x34)
    {
        std::cout << "NODO_FISICO    : Message receiveD id: " << can.Message_receive->id << " payload: " << uint32_t{can.Message_receive->data[0]} << std::endl;
        can.Message_receive->id = 0;
    }
}

void NODO::cargo_mensaje()
{
    can.Message_send->originator_id = this->id_yml;
    assert(this->m_bus);
    this->m_bus->inyecto_nodo();
}

/* Implementation-specific */

extern "C" Smp::IModel *create_NODO(Smp::String8 name, Smp::IComposite *parent)
{
    return dynamic_cast<Smp::IModel *>(new NODO(name, parent));
}

extern "C" void destroy_NODO(Smp::IModel *obj)
{
    delete dynamic_cast<NODO *>(obj);
}

void NODO::ReadInitializationParameters(const YAML::Node &paramsNode)
{

    this->id_yml = paramsNode["id"].as<uint32_t>();
    this->canid_yml = paramsNode["canid"].as<uint32_t>();
    this->dlc_yml = paramsNode["dlc"].as<uint32_t>();
    this->payload0_yml = paramsNode["payload0"].as<uint32_t>();
    this->payload1_yml = paramsNode["payload1"].as<uint32_t>();
    this->status_yml = paramsNode["status"].as<uint8_t>();
}
