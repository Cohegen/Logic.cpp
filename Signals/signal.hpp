/*
Modelling a signal--
From digital design we know that a signal can be either high or low on a wire
*/

#pragma once

namespace dd
{
    class Signal{
        public:
        Signal() = default;
        explicit Signal(bool value): m_value(value){}

        bool value() const
        {
            return m_value;
        }

        void set(bool value)
        {
            m_value = value;
        }

        private:
        bool m_value{false};
    };
}
