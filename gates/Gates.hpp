#pragma once 

namespace logic {
    class Gate{
        public:
            virtual ~Gate() noexcept = default;
            virtual void evaluate()=0;
    };

}
