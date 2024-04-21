#include "Red_CAN.h"


// Initialise private fields
void Bus::Init(void)
{
    // Initialise references
    logger = NULL;
    scheduler = NULL;
    // eventManager = NULL;
}

// Publish fields to environment
void Bus::Publish(Smp::IPublication *receiver)
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
void Bus::Configure(Smp::Services::ILogger *)
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
void Bus::Connect(Smp::ISimulator *simulator)
{
    if (state == Smp::MSK_Configured)
    {
        state = Smp::MSK_Connected;
        logger = simulator->GetLogger();
        scheduler = simulator->GetScheduler();

        for (auto &nodeName : this->m_nodeNames)
        {
            std::cout << "Bus. Connecting node: " << nodeName << "\n";

            Node *node = dynamic_cast<Node *>(simulator->GetModel(nodeName.c_str()));
            assert(node);
            this->Attach(node);
        }
    }
    else
    {
        throw Smp::IModel::InvalidModelState(state, Smp::MSK_Configured);
    }
}

void Bus::Attach(Node *node)
{
    node->SetBus(this);
    this->m_nodes.push_back(node);
}

void Bus::Inject(Message *msg)
{
    for (auto &node : this->m_nodes)
    {
        if (node->GetId() != msg->originator_id)
        {
            node->Receive(msg);
        }
    }

    delete msg;
}

/* Implementation-specific */

extern "C" Smp::IModel *create_Bus(Smp::String8 name, Smp::IComposite *parent)
{
    return dynamic_cast<Smp::IModel *>(new Bus(name, parent));
}

extern "C" void destroy_Bus(Smp::IModel *obj)
{
    delete dynamic_cast<Bus *>(obj);
}

void Bus::ReadInitializationParameters(const YAML::Node &paramsNode)
{
    std::cout << "Initializing bus. Nodes: " << paramsNode["nodes"].size() << std::endl;
    for (YAML::const_iterator it = paramsNode["nodes"].begin();
         it != paramsNode["nodes"].end(); ++it)
    {
        std::cout << "Adding node: " << it->as<std::string>() << "\n";
        this->m_nodeNames.push_back(it->as<std::string>());
    }
}

/* Node */

// Initialise private fields
void Node::Init(void)
{
    // Initialise references
    logger = NULL;
    scheduler = NULL;
    // eventManager = NULL;

    sync = new NodeEntryPoint("Sync", "Send SYNC messages", this, &Node::Sync);
}

// Publish fields to environment
void Node::Publish(Smp::IPublication *receiver)
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
void Node::Configure(Smp::Services::ILogger *)
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
void Node::Connect(Smp::ISimulator *simulator)
{
    if (state == Smp::MSK_Configured)
    {
        state = Smp::MSK_Connected;
        logger = simulator->GetLogger();
        scheduler = simulator->GetScheduler();
        // eventManager = simulator->GetEventManager();

        if (this->m_isMaster)
        {
            this->scheduler->AddSimulationTimeEvent(sync, 0, 1000000000, -1);
        }
    }
    else
    {
        throw Smp::IModel::InvalidModelState(state, Smp::MSK_Configured);
    }
}

void Node::Transmit(Message *msg)
{
    msg->originator_id = this->m_Can_Id;

    assert(this->m_bus);
    this->m_bus->Inject(msg);

    // char cadena[11]="PKT!80asd\n";

}

void Node::Receive(Message *msg)
{
    std::cout << "Recibiendo Mensajes en: " <<this->GetName() << std::endl;
    std::cout << "LEN: "<< this->m_Len  << std::endl;
    std::cout << "CAN_ID: " << this->m_Can_Id <<  std::endl;
    std::cout << "PAYLOAD: " << msg->Payload[1] << "\n" <<  std::endl;
}


void Node::Sync()
{
    std::cout << "==========Enviando Mensajes SYNC desde: " <<  this->GetName()<< "\n" <<std::endl;

    Message *msg = new Message();
    // char cadena[11]="PKT!80asd\n";
    msg->Can_id = 0x0BADBABE;
    msg->Len = this->m_Len;
    msg->Payload[0] = 'P';
    msg->Payload[1] = m_Payload;
    msg->Payload[2] = 'T';
    msg->Payload[3] = '!';
    msg->Payload[4] = m_Len;
    msg->Payload[5] = '0';
    msg->Payload[6] = m_Payload;
    msg->Payload[7] = m_Payload;

    for (size_t i = 0; i < 7; i++)
    {
        rx_buffer[i] = msg->Payload[i];
    }
    this->Transmit(msg);
stm32canbus_serialif can("/dev/ttyACM0", 115200);
std::cout << "Ingrese menor a 3 para ver mensajes CAN:\nIngrese mayor a 3 para encender led rojo:" << std::endl;
can.start();
while(1)
{

can.write_some();
can.stop();
}


}

/* Implementation-specific */

extern "C" Smp::IModel *create_Node(Smp::String8 name, Smp::IComposite *parent)
{
    return dynamic_cast<Smp::IModel *>(new Node(name, parent));
}

extern "C" void destroy_Node(Smp::IModel *obj)
{
    delete dynamic_cast<Node *>(obj);
}

void Node::ReadInitializationParameters(const YAML::Node &paramsNode)
{
    this->m_Can_Id = paramsNode["Can_Id"].as<uint32_t>();
    this->m_isMaster = paramsNode["master"].as<bool>();
    this->m_Len = paramsNode["Len"].as<uint32_t>();
    this->m_Payload = paramsNode["Payload"].as<uint32_t>();
}

