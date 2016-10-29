#ifndef TRANSITION_GUARDS
#define TRANSITION_GUARDS

#include <vector>
#include <iostream>
// functors
#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/euml/common.hpp>

#include "events.hpp"

using namespace std;
namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace msm::front;
// for And_ operator
using namespace msm::front::euml;

// guard conditions
struct safe_functA
{
    template <class EVT,class FSM,class SourceState,class TargetState>
    bool operator()(EVT const& evt ,FSM&,SourceState& ,TargetState& )
    {
        if (evt.command_type == FUNCT_A)
        {
            std::cout << "safe_functA true" << std::endl;
            return true;
        }
        std::cout << "safe_functA false" << std::endl;
        return false;
    }
};
struct safe_init_sensors
{
    template <class EVT,class FSM,class SourceState,class TargetState>
    bool operator()(EVT const& evt ,FSM&,SourceState& ,TargetState& )
    {
        if (evt.command_type == INIT_SENSORS)
        {
            std::cout << "safe_init_sensors true" << std::endl;
            return true;
        }
        std::cout << "safe_init_sensors false" << std::endl;
        return false;
    }
};
struct init_functA
{
    template <class EVT,class FSM,class SourceState,class TargetState>
    bool operator()(EVT const& evt ,FSM&,SourceState& ,TargetState& )
    {
        if (evt.command_type == FUNCT_A)
        {
            std::cout << "init_functA true" << std::endl;
            return true;
        }
        std::cout << "init_functA false" << std::endl;
        return false;
    }
};
struct functA_functB
{
    template <class EVT,class FSM,class SourceState,class TargetState>
    bool operator()(EVT const& evt ,FSM&,SourceState& ,TargetState& )
    {
        if (evt.command_type == FUNCT_B)
        {
            std::cout << "functA_functB true" << std::endl;
            return true;
        }
        std::cout << "functA_functB false" << std::endl;
        return false;
    }
};
struct functB_functC
{
    template <class EVT,class FSM,class SourceState,class TargetState>
    bool operator()(EVT const& evt ,FSM&,SourceState& ,TargetState& )
    {
        if (evt.command_type == FUNCT_C)
        {
            std::cout << "functB_functC true" << std::endl;
            return true;
        }
        std::cout << "functB_functC false" << std::endl;
        return false;
    }
};
struct functC_functD
{
    template <class EVT,class FSM,class SourceState,class TargetState>
    bool operator()(EVT const& evt ,FSM&,SourceState& ,TargetState& )
    {
        if (evt.command_type == FUNCT_D)
        {
            std::cout << "functC_functD true" << std::endl;
            return true;
        }
        std::cout << "functC_functD false" << std::endl;
        return false;
    }
};
struct functD_safe
{
    template <class EVT,class FSM,class SourceState,class TargetState>
    bool operator()(EVT const& evt ,FSM&,SourceState& ,TargetState& )
    {
        if (evt.command_type == SAFE_MODE)
        {
            std::cout << "functD_safe true" << std::endl;
            return true;
        }
        std::cout << "functD_safe false" << std::endl;
        return false;
    }
};
struct functD_loading
{
    template <class EVT,class FSM,class SourceState,class TargetState>
    bool operator()(EVT const& evt ,FSM&,SourceState& ,TargetState& )
    {
        if (evt.command_type == LOADING)
        {
            std::cout << "functD_loading true" << std::endl;
            return true;
        }
        std::cout << "functD_loading false" << std::endl;
        return false;
    }
};
struct functD_flightA
{
    template <class EVT,class FSM,class SourceState,class TargetState>
    bool operator()(EVT const& evt ,FSM&,SourceState& ,TargetState& )
    {
        if (evt.command_type == FLIGHT_A)
        {
            std::cout << "functD_flightA true" << std::endl;
            return true;
        }
        std::cout << "functD_flightA false" << std::endl;
        return false;
    }
};
struct loading_functA
{
    template <class EVT,class FSM,class SourceState,class TargetState>
    bool operator()(EVT const& evt ,FSM&,SourceState& ,TargetState& )
    {
        if (evt.command_type == FUNCT_A)
        {
            std::cout << "loading_functA true" << std::endl;
            return true;
        }
        std::cout << "loading_functA false" << std::endl;
        return false;
    }
};
struct flightA_safe
{
    template <class EVT,class FSM,class SourceState,class TargetState>
    bool operator()(EVT const& evt ,FSM&,SourceState& ,TargetState& )
    {
        if (evt.command_type == SAFE_MODE)
        {
            std::cout << "flightA_safe true" << std::endl;
            return true;
        }
        std::cout << "flightA_safe false" << std::endl;
        return false;
    }
};
struct flightA_flightC
{
    template <class EVT,class FSM,class SourceState,class TargetState>
    bool operator()(EVT const& evt ,FSM&,SourceState& ,TargetState& )
    {
        std::cout << "flightA_flightC true" << std::endl;
        return true;
    }
};
struct flightC_flightB
{
    template <class EVT,class FSM,class SourceState,class TargetState>
    bool operator()(EVT const& evt ,FSM&,SourceState& ,TargetState& )
    {
        std::cout << "flightC_flightB true" << std::endl;
        return true;
    }
};
struct flightB_safe
{
    template <class EVT,class FSM,class SourceState,class TargetState>
    bool operator()(EVT const& evt ,FSM&,SourceState& ,TargetState& )
    {
        if (evt.command_type == SAFE_MODE)
        {
            std::cout << "flightB_safe true" << std::endl;
            return true;
        }
        std::cout << "flightB_safe false" << std::endl;
        return false;
    }
};
#endif
