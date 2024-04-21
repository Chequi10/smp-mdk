#include <iostream>
#include "Counter.h"
#include "Smp/ISimulator.h"
#include "Smp/IPublication.h"

// Initialise private fields
void ContadorSmp::Init(void)
{
    // Initialise references
    logger = NULL;
    scheduler = NULL;
    //eventManager = NULL;

    // Turn Count and Reset methods into EntryPoints
    count = new ContadorSmpEntryPoint("Count", "Increment ContadorSmp", this, &ContadorSmp::Punto);
    reset = new ContadorSmpEntryPoint("Reset", "Reset ContadorSmp", this, &ContadorSmp::Reseteo);
}

// Log a message to the logger on Reset
void ContadorSmp::ResetSMP(void)
{
    ContadorOriginal::Reseteo();
    // Send an information message to the logger
    logger->Log(this, "Reset ContadorSmp", Smp::Services::LMK_Information);
}

// Publish fields to environment
void ContadorSmp::Publish(Smp::IPublication *receiver)
{
    if (state == Smp::MSK_Created)
    {
        state = Smp::MSK_Publishing;
        receiver->PublishField("Contador virtual", "Contador del eze", &Contador);
    }
    else
    {
        throw Smp::IModel::InvalidModelState(state, Smp::MSK_Created);
    }
}
// Perform custom configuration.
void ContadorSmp::Configure(Smp::Services::ILogger*) 
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
void ContadorSmp::Connect(Smp::ISimulator *simulator)
{
    if (state == Smp::MSK_Configured)
    {
        state = Smp::MSK_Connected;
        logger = simulator->GetLogger();
        scheduler = simulator->GetScheduler();
        //eventManager = simulator->GetEventManager();
        
        scheduler->AddSimulationTimeEvent(count, 0, this->frequency * 100000000, -1);
        //eventManager->Subscribe(Smp::Services::SMP_EnterStandbyId, reset);
    }
    else
    {
        throw Smp::IModel::InvalidModelState(state, Smp::MSK_Configured);
    }
}


/* Implementation-specific */

extern "C" Smp::IModel* create_ContadorSmp(Smp::String8 name, Smp::IComposite *parent)
{    
    return dynamic_cast<Smp::IModel*>(new ContadorSmp(name,parent));
}

extern "C" void destroy_ContadorSmp( Smp::IModel* obj )
{
    delete dynamic_cast<ContadorSmp*>(obj);
}

void ContadorSmp::ReadInitializationParameters(const YAML::Node& paramsNode)
{    
    this->frequency =  paramsNode["frequency"].as<double>();
}