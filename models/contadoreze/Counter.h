#include "ContadorOriginal.h"
#include "Smp/IModel.h"
#include "Smp/IEntryPoint.h"
#include "Smp/Services/ILogger.h"
#include "Smp/Services/IScheduler.h"
#include "Smp/Services/IEventManager.h"

#include "IYAMLConfigurable.h"

/// This class turns the ContadorOriginal into an Smp model.
class ContadorSmp : 
    public ContadorOriginal, 
    public virtual Smp::IModel,
    public IYAMLConfigurable
{
    private:
        /// Private helper class for entry points
        class ContadorSmpEntryPoint : public Smp::IEntryPoint
        {
            private:
                char* name; ///< Name of entry point.
                char* description; ///< Description.
                ContadorSmp* publisher; ///< Entry point publisher.
                void (ContadorSmp::*entryPoint)(void); ///< Instance method.
            
            public:
                /// Constructor with name, description, publisher and entry point.
                ContadorSmpEntryPoint(
                    Smp::String8 name,
                    Smp::String8 description,
                    ContadorSmp* publisher,
                    void (ContadorSmp::*entryPoint)(void) )
                {
                    this->name = strdup(name);
                    this->description = strdup(description);
                    this->publisher = publisher;
                    this->entryPoint = entryPoint;
                }

                virtual Smp::String8 GetName() const { return name; }
                
                virtual Smp::String8 GetDescription() const { return description; }
                
                virtual Smp::IComponent* GetOwner() const { return publisher; }
                
                void Execute() const { (publisher->*entryPoint)(); }
        };

private:
    char* name; ///< Name of model.
    Smp::ModelStateKind state; ///< Model state.
    Smp::IComposite *parent; ///< Parent component.
    Smp::Services::ILogger *logger; ///< Logger service.
    Smp::Services::IScheduler *scheduler; ///< Scheduler service.
    //Smp::Services::IEventManager *eventManager; ///< EventManager service.
    ContadorSmpEntryPoint *count; ///< Entry point for Count.
    ContadorSmpEntryPoint *reset; ///< Entry point for Reset.
    void Init(); ///< Init private fields.


    // FIXME: explain
    double frequency;

public:
    ContadorSmp(Smp::String8 name, Smp::IComposite *parent) : ContadorOriginal()
    {
        Init();
        this->name = strdup(name);
        this->state = Smp::MSK_Created;
        this->parent = parent;
    }
    
    virtual ~ContadorSmp()
    {
        if (name) free(name);
    }
    
    virtual void ResetSMP(void); ///< Reset method logs a message
    
    // IModel methods
    virtual Smp::String8 GetName() const { return name; }
    virtual Smp::String8 GetDescription() const { return "ContadorSmp Model"; }
    virtual Smp::IComposite* GetParent() const { return parent; }
    virtual Smp::ModelStateKind GetState() const { return state; }
    virtual void Publish(Smp::IPublication* receiver);
    virtual void Configure(Smp::Services::ILogger* logger);
    virtual void Connect(Smp::ISimulator* simulator);


    // Implementation-specific
    void ReadInitializationParameters(const YAML::Node& paramsNode);
};