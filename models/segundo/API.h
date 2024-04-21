/**
 * @file API.h
 * @author Ing. Acerbo Ezequiel (acerboezequiel@live.com)
 * @brief Modelos SMP2 de un BUS CAN y NODOS CAN
 *        Estas clases se utilizan para formar parate deun entorno de simulacion, que se creo para 
 *        simular un protocolo de comunicacion CAN, donde se crea un bus y varios nodos. 
 *        Tiene la posibilidad de enviar mensajes CAN por puerto serie(empaquetados) a la placa stm32
 *        mediante uno de sus nodos. La placa smt32 tiene un un BUS FISICO y 2 NODOS FISICOS. 
 *        Un nodo virtual maestro creado como un modelo de SMP2 envia en su payload la informacion para 
 *        configurar el id y asdasd de uno de los NODOS FISICOS de la placa. 
 *        Esta aplicacion tiene la posibilidad de tambien recibir por el mismo puerto Serie los
 *        mensajes CAN del puerto fisico. Entonces el modelo de BUS CAN virtual en SMP2, muestra el NODO FISICO como
 *        asi tambien los NODOS CAN esclavo virtuales. 
 * 
 * @version 0.1
 * @date 2024-04-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "SERIE.h"
#include "Smp/IModel.h"
#include "Smp/IEntryPoint.h"
#include "Smp/Services/ILogger.h"
#include "Smp/Services/IScheduler.h"
#include "Smp/Services/IEventManager.h"

#include "IYAMLConfigurable.h"

// creo la clase NODO antes, para que se pueda crear modelos de nodos dentro del BUS_CAN
class NODO;

class BUS_CAN : public SERIE,
                public virtual Smp::IModel,
                public IYAMLConfigurable
{

private:
    char *name;                           ///< Name of model.
    Smp::ModelStateKind state;            ///< Model state.
    Smp::IComposite *parent;              ///< Parent component.
    Smp::Services::ILogger *logger;       ///< Logger service.
    Smp::Services::IScheduler *scheduler; ///< Scheduler service.
    // Smp::Services::IEventManager *eventManager; ///< EventManager service.
    void Init(); ///< Init private fields.

public:
    BUS_CAN(Smp::String8 name, Smp::IComposite *parent)
    {
        Init();
        this->name = strdup(name);
        this->state = Smp::MSK_Created;
        this->parent = parent;
    }

    virtual ~BUS_CAN()
    {
        if (name)
            free(name);
    }

    // IModel methods
    virtual Smp::String8 GetName() const { return name; }
    virtual Smp::String8 GetDescription() const { return "BUS_CAN Model"; }
    virtual Smp::IComposite *GetParent() const { return parent; }
    virtual Smp::ModelStateKind GetState() const { return state; }
    virtual void Publish(Smp::IPublication *receiver);
    virtual void Configure(Smp::Services::ILogger *logger);
    virtual void Connect(Smp::ISimulator *simulator);

    // Non-SMP2
    void ReadInitializationParameters(const YAML::Node &paramsNode);
    void loadNODOx(NODO *node1);
    void inyecto_nodo();

public:
    double rede;

    std::vector<NODO *> numeros_nodos;
    std::vector<std::string> nombre_numeros_nodos;
};

/// This clall create an Smp model.
class NODO : public SERIE,
             public virtual Smp::IModel,
             public IYAMLConfigurable
{
private:
    /// Private helper class for entry points
    class NODOEntryPoint : public Smp::IEntryPoint
    {
    private:
        char *name;                     ///< Name of entry point.
        char *description;              ///< Description.
        NODO *publisher;                ///< Entry point publisher.
        void (NODO::*entryPoint)(void); ///< Instance method.

    public:
        /// Constructor with name, description, publisher and entry point.
        NODOEntryPoint(
            Smp::String8 name,
            Smp::String8 description,
            NODO *publisher,
            void (NODO::*entryPoint)(void))
        {
            this->name = strdup(name);
            this->description = strdup(description);
            this->publisher = publisher;
            this->entryPoint = entryPoint;
        }

        virtual Smp::String8 GetName() const { return name; }

        virtual Smp::String8 GetDescription() const { return description; }

        virtual Smp::IComponent *GetOwner() const { return publisher; }

        void Execute() const { (publisher->*entryPoint)(); }
    };

private:
    char *name;                           ///< Name of model.
    Smp::ModelStateKind state;            ///< Model state.
    Smp::IComposite *parent;              ///< Parent component.
    Smp::Services::ILogger *logger;       ///< Logger service.
    Smp::Services::IScheduler *scheduler; ///< Scheduler service.
    // Smp::Services::IEventManager *eventManager; ///< EventManager service.
    NODOEntryPoint *starting; ///< Entry point for start service.
    NODOEntryPoint *sending;  ///< Entry point for send message.
    void Init();              ///< Init private fields.

public:
    NODO(Smp::String8 name, Smp::IComposite *parent) : SERIE()
    {
        Init();
        this->name = strdup(name);
        this->state = Smp::MSK_Created;
        this->parent = parent;
    }
    virtual ~NODO()
    {
        if (name)
            free(name);
    }

    virtual void ResetSMP(void); ///< Reset method logs a message

    // IModel methods
    virtual Smp::String8 GetName() const { return name; }
    virtual Smp::String8 GetDescription() const { return "NODO Model"; }
    virtual Smp::IComposite *GetParent() const { return parent; }
    virtual Smp::ModelStateKind GetState() const { return state; }
    virtual void Publish(Smp::IPublication *receiver);
    virtual void Configure(Smp::Services::ILogger *logger);
    virtual void Connect(Smp::ISimulator *simulator);

    // Implementation-specific
    /**
     * @brief 
     * 
     * @param paramsNode 
     */
    void ReadInitializationParameters(const YAML::Node &paramsNode);
    uint32_t GetId() const { return this->id_yml; }
    void SetBus(BUS_CAN *bus) { this->m_bus = bus; }
    void cargo_mensaje();
    uint32_t GetIdb() const { return this->id_yml; }
    void send_CAN();

    /**
     * @brief 
     * 
     */
    void receive_CAN();

    void startup();

private:
    // FIXME: explain
    uint32_t id_yml;
    uint32_t canid_yml;
    uint32_t dlc_yml;
    uint32_t payload0_yml;
    uint32_t payload1_yml;
    BUS_CAN *m_bus;
    uint8_t status_yml;
};
