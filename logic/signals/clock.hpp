/*
Defining a clock whose role will be driving
sequential logic
*/

class Clock
{
    public:

    void tick()
    {
        m_cycle++;

    }

    std::unint64_t cycle() const 
    {
        return m_cycle;
    }

    private:

    std::unit64_t m_cycle{0};

};