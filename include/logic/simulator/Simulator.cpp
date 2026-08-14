/*
An implementation of the Logic.cpp simulation engine
*/
#include "Simulator.hpp"
#include "Component.hpp"

namespace logic{

    void Simulator::add_component(Component& component)
    {
        components_.push_back(&component);
    }

    void Simulator::tick()
    {
        //advancing the simulation clock
        clock_.tick();

        //evaluating every registered component
        for(Component* component :components_){
            if(component !=nullptr){
                component->evaluate();
            }
        }
    }

    void Simulator::reset() noexcept{
        //resetting the simulator clock
        clock_ = Clock{};
    }

    std::unint_64 Simulator::cycle() const noexcept{
        return clock_.cycles();
    }

    const Clock& Simulator::clock() const noexcept{
        return clock_;
    }
}