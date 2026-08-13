#include "combinational/shifters/Shifters.hpp"
#include "combinational/extenders/Extenders.hpp"
#include "combinational/decoders/Encoders.hpp"
#include "signals/BusDriver.hpp"
#include <iostream>
#include <cassert>

void test_shifters()
{
    std::cout << "--- Testing Shifters & Barrel Shifter ---\n";
    
    // 8-bit bus shifters
    logic::Bus<8> in, shift_amt, out_lls, out_lrs, out_ars;
    logic::LogicalLeftShifter<8> lls(in, shift_amt, out_lls);
    logic::LogicalRightShifter<8> lrs(in, shift_amt, out_lrs);
    logic::ArithmeticRightShifter<8> ars(in, shift_amt, out_ars);

    in.write_value(0b11001001); // 0xC9 (-55 in 2's complement)
    shift_amt.write_value(2);

    lls.evaluate();
    lrs.evaluate();
    ars.evaluate();

    std::cout << "Logical Left Shift (0b11001001 << 2):  0b" << std::hex << out_lls.read_value() << "\n";
    assert(out_lls.read_value() == 0b00100100);

    std::cout << "Logical Right Shift (0b11001001 >> 2): 0b" << std::hex << out_lrs.read_value() << "\n";
    assert(out_lrs.read_value() == 0b00110010);

    std::cout << "Arithmetic Right Shift (0b11001001 >> 2): 0b" << std::hex << out_ars.read_value() << "\n";
    assert(out_ars.read_value() == 0b11110010);

    // Barrel Shifter
    logic::Bus<3> shift3;
    logic::Bus<2> mode;
    logic::Bus<8> out_barrel;
    logic::BarrelShifter<8, 3> barrel(in, shift3, mode, out_barrel);

    shift3.write_value(3);
    mode.write_value(3); // Rotate Right
    barrel.evaluate();
    std::cout << "Barrel Rotate Right (0b11001001 ROR 3): 0b" << std::hex << out_barrel.read_value() << "\n";
    assert(out_barrel.read_value() == 0b00111001);

    std::cout << "[PASS] Shifter Tests\n\n";
}

void test_extenders()
{
    std::cout << "--- Testing Zero & Sign Extenders ---\n";

    logic::Bus<4> in4;
    logic::Bus<8> out_zero, out_sign;
    logic::ZeroExtender<4, 8> zext(in4, out_zero);
    logic::SignExtender<4, 8> sext(in4, out_sign);

    // Positive number: 0b0101 (5)
    in4.write_value(0b0101);
    zext.evaluate();
    sext.evaluate();
    assert(out_zero.read_value() == 5);
    assert(out_sign.read_value() == 5);

    // Negative number: 0b1011 (-5 in 4-bit 2's complement)
    in4.write_value(0b1011);
    zext.evaluate();
    sext.evaluate();
    std::cout << "Zero Extended (0b1011): 0x" << std::hex << out_zero.read_value() << "\n";
    assert(out_zero.read_value() == 0b00001011);

    std::cout << "Sign Extended (0b1011): 0x" << std::hex << out_sign.read_value() << "\n";
    assert(out_sign.read_value() == 0b11111011);

    std::cout << "[PASS] Extender Tests\n\n";
}

void test_encoders()
{
    std::cout << "--- Testing Encoders & Priority Encoder ---\n";

    logic::Bus<4> in4;
    logic::Bus<2> out2;
    logic::Wire valid;
    logic::Encoder4to2 enc(in4, out2, valid);

    in4.write_value(0b0100); // Input line 2 active
    enc.evaluate();
    assert(valid.read() == logic::LogicState::HIGH);
    assert(out2.read_value() == 2);

    // Priority Encoder (8 lines -> 3 bits)
    logic::Bus<8> in8;
    logic::Bus<3> out3;
    logic::Wire p_valid;
    logic::PriorityEncoder<8, 3> penc(in8, out3, p_valid);

    in8.write_value(0b00101100); // Highest active bit is line 5
    penc.evaluate();
    assert(p_valid.read() == logic::LogicState::HIGH);
    std::cout << "Priority Encoder Highest Active Line (0b00101100): " << out3.read_value() << "\n";
    assert(out3.read_value() == 5);

    std::cout << "[PASS] Encoder Tests\n\n";
}

void test_bus_driver()
{
    std::cout << "--- Testing Tri-State Buffer & Bus Driver ---\n";

    logic::Wire input(logic::LogicState::HIGH);
    logic::Wire enable(logic::LogicState::LOW);
    logic::Wire output;
    logic::TriStateBuffer tri_buf(input, enable, output);

    tri_buf.evaluate();
    assert(output.read() == logic::LogicState::HIGH_IMPEDANCE);

    enable.write(logic::LogicState::HIGH);
    tri_buf.evaluate();
    assert(output.read() == logic::LogicState::HIGH);

    // Controlled Bus Driver
    logic::Bus<8> bus_in, bus_out;
    logic::Wire bus_en(logic::LogicState::LOW);
    logic::BusDriver<8> bus_driver(bus_in, bus_en, bus_out);

    bus_in.write_value(0xDE);
    bus_driver.evaluate();
    assert(bus_out[0].read() == logic::LogicState::HIGH_IMPEDANCE);

    bus_en.write(logic::LogicState::HIGH);
    bus_driver.evaluate();
    assert(bus_out.read_value() == 0xDE);

    std::cout << "[PASS] Bus Driver Tests\n\n";
}

int main()
{
    std::cout << "=======================================\n";
    std::cout << " Running Logic.cpp Expanded Unit Tests \n";
    std::cout << "=======================================\n\n";

    test_shifters();
    test_extenders();
    test_encoders();
    test_bus_driver();

    std::cout << "ALL EXPANDED COMPONENT TESTS PASSED SUCCESSFULLY!\n";
    return 0;
}
