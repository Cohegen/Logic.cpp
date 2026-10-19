#include <logic/combinational/shifters/LogicalLeftShifter.hpp>
#include <logic/combinational/shifters/LogicalRightShifter.hpp>
#include <logic/combinational/shifters/ArithmeticRightShifter.hpp>
#include <logic/combinational/shifters/BarrelShifter.hpp>

#include <logic/combinational/extenders/ZeroExtender.hpp>
#include <logic/combinational/extenders/SignExtender.hpp>

#include <logic/combinational/encoders/Encoder.hpp>
#include <logic/combinational/encoders/PriorityEncoder.hpp>
#include <logic/combinational/decoders/Decoder.hpp>
#include <logic/combinational/decoders/Decoder2to4.hpp>

#include <logic/combinational/bit_operations/BitSelector.hpp>
#include <logic/combinational/bit_operations/BitSlice.hpp>

#include <logic/signals/BusDriver.hpp>
#include <logic/combinational/multiplexers/Mux.hpp>
#include <logic/combinational/multiplexers/Mux3.hpp>
#include <logic/combinational/multiplexers/Mux3to1.hpp>
#include <logic/combinational/multiplexers/MuxTree.hpp>

#include <iostream>
#include <cassert>

void test_bus_mux()
{
    std::cout << "--- Testing 32-bit Bus Mux (Mux<32>) ---\n";

    logic::Bus<32> busA, busB, busOut;
    logic::Wire select;

    logic::Mux<32> mux32(busA, busB, select, busOut);

    busA.write_value(0x12345678);
    busB.write_value(0xABCDEF01);

    // Select = 0 -> output should be busA
    select.write(logic::LogicState::LOW);
    mux32.evaluate();
    assert(busOut.read_value() == 0x12345678);

    // Select = 1 -> output should be busB
    select.write(logic::LogicState::HIGH);
    mux32.evaluate();
    assert(busOut.read_value() == 0xABCDEF01);

    std::cout << "[PASS] 32-bit Bus Mux Tests\n\n";
}

void test_mux3()
{
    std::cout << "--- Testing Mux3to1 and Mux3<32> ---\n";

    logic::Wire a(logic::LogicState::LOW);
    logic::Wire b(logic::LogicState::HIGH);
    logic::Wire c(logic::LogicState::LOW);
    logic::Wire select0(logic::LogicState::LOW);
    logic::Wire select1(logic::LogicState::LOW);
    logic::Wire bit_out;

    logic::Mux3to1 mux3to1(a, b, c, select0, select1, bit_out);

    c.write(logic::LogicState::HIGH);

    select1.write(logic::LogicState::LOW);
    select0.write(logic::LogicState::LOW);
    mux3to1.evaluate();
    assert(bit_out.read() == logic::LogicState::LOW);

    select0.write(logic::LogicState::HIGH);
    mux3to1.evaluate();
    assert(bit_out.read() == logic::LogicState::HIGH);

    select1.write(logic::LogicState::HIGH);
    select0.write(logic::LogicState::LOW);
    mux3to1.evaluate();
    assert(bit_out.read() == logic::LogicState::HIGH);

    logic::Bus<32> busA, busB, busC, busOut;
    logic::Mux3<32> mux3(busA, busB, busC, select0, select1, busOut);

    busA.write_value(0x11111111);
    busB.write_value(0x22222222);
    busC.write_value(0x33333333);

    select1.write(logic::LogicState::LOW);
    select0.write(logic::LogicState::LOW);
    mux3.evaluate();
    assert(busOut.read_value() == 0x11111111);

    select0.write(logic::LogicState::HIGH);
    mux3.evaluate();
    assert(busOut.read_value() == 0x22222222);

    select1.write(logic::LogicState::HIGH);
    select0.write(logic::LogicState::LOW);
    mux3.evaluate();
    assert(busOut.read_value() == 0x33333333);

    std::cout << "[PASS] Mux3 Tests\n\n";
}

void test_mux_tree()
{
    std::cout << "--- Testing MuxTree (4-to-1 and 32-to-1) ---\n";

    // 4-to-1 MuxTree with 8-bit buses
    std::array<logic::Bus<8>, 4> inputs4;
    std::array<logic::Bus<8>*, 4> input_ptrs4;
    for (std::size_t i = 0; i < 4; ++i)
    {
        inputs4[i].write_value(0x10 * (i + 1));
        input_ptrs4[i] = &inputs4[i];
    }
    logic::Bus<2> sel4;
    logic::Bus<8> out4;
    logic::MuxTree<4, 8> mux_tree4(input_ptrs4, sel4, out4);

    for (std::uint64_t i = 0; i < 4; ++i)
    {
        sel4.write_value(i);
        mux_tree4.evaluate();
        assert(out4.read_value() == 0x10 * (i + 1));
    }

    // 32-to-1 MuxTree with 128-bit buses (as used in VectorRegisterFile)
    std::vector<std::unique_ptr<logic::Bus<128>>> inputs32;
    std::vector<logic::Bus<128>*> input_ptrs32;
    inputs32.reserve(32);
    input_ptrs32.reserve(32);
    for (std::size_t i = 0; i < 32; ++i)
    {
        inputs32.push_back(std::make_unique<logic::Bus<128>>());
        inputs32.back()->write_value(0x1000 + i);
        input_ptrs32.push_back(inputs32.back().get());
    }
    logic::Bus<5> sel32;
    logic::Bus<128> out32;
    logic::MuxTree<32, 128> mux_tree32(input_ptrs32, sel32, out32);

    for (std::uint64_t i = 0; i < 32; ++i)
    {
        sel32.write_value(i);
        mux_tree32.evaluate();
        assert(out32.read_value() == (0x1000 + i));
    }

    std::cout << "[PASS] MuxTree Tests\n\n";
}

void test_shifters()
{
    std::cout << "--- Testing Shifters & Barrel Shifter ---\n";
    
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

    in4.write_value(0b0101);
    zext.evaluate();
    sext.evaluate();
    assert(out_zero.read_value() == 5);
    assert(out_sign.read_value() == 5);

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
    logic::Encoder enc(in4, out2, valid);

    in4.write_value(0b0100);
    enc.evaluate();
    assert(valid.read() == logic::LogicState::HIGH);
    assert(out2.read_value() == 2);

    logic::Bus<8> in8;
    logic::Bus<3> out3;
    logic::Wire p_valid;
    logic::PriorityEncoder<8, 3> penc(in8, out3, p_valid);

    in8.write_value(0b00101100);
    penc.evaluate();
    assert(p_valid.read() == logic::LogicState::HIGH);
    std::cout << "Priority Encoder Highest Active Line (0b00101100): " << out3.read_value() << "\n";
    assert(out3.read_value() == 5);

    std::cout << "[PASS] Encoder Tests\n\n";
}

void test_decoders()
{
    std::cout << "--- Testing Decoders (Decoder<N> & Decoder2to4) ---\n";

    // 1. 2-to-4 Decoder with Enable Wire
    logic::Bus<2> addr2;
    logic::Wire enable(logic::LogicState::HIGH);
    logic::Bus<4> out4;
    logic::Decoder2to4 dec2to4(addr2, enable, out4);

    for (std::uint64_t i = 0; i < 4; ++i)
    {
        addr2.write_value(i);
        dec2to4.evaluate();
        assert(out4.read_value() == (1ULL << i));
        assert(out4[i].read() == logic::LogicState::HIGH);
    }

    // Disabled state
    enable.write(logic::LogicState::LOW);
    dec2to4.evaluate();
    assert(out4.read_value() == 0);
    for (std::size_t i = 0; i < 4; ++i)
    {
        assert(out4[i].read() == logic::LogicState::LOW);
    }

    // Unknown enable state
    enable.write(logic::LogicState::UNKNOWN);
    dec2to4.evaluate();
    for (std::size_t i = 0; i < 4; ++i)
    {
        assert(out4[i].read() == logic::LogicState::UNKNOWN);
    }

    // 2. 3-to-8 Decoder without Enable Wire (always enabled)
    logic::Bus<3> addr3;
    logic::Bus<8> out8;
    logic::Decoder<3> dec3to8(addr3, out8);

    for (std::uint64_t i = 0; i < 8; ++i)
    {
        addr3.write_value(i);
        dec3to8.evaluate();
        assert(out8.read_value() == (1ULL << i));
        assert(out8[i].read() == logic::LogicState::HIGH);
    }

    // 3. 4-to-16 Decoder (Decoder4to16 alias)
    logic::Bus<4> addr4;
    logic::Bus<16> out16;
    logic::Decoder4to16 dec4to16(addr4, out16);

    addr4.write_value(13);
    dec4to16.evaluate();
    assert(out16.read_value() == (1ULL << 13));
    assert(out16[13].read() == logic::LogicState::HIGH);

    // 4. Partial Decoder: 4-to-10 BCD Decoder
    logic::Bus<4> bcd_in;
    logic::Bus<10> bcd_out;
    logic::Decoder<4, 10> bcd_dec(bcd_in, bcd_out);

    bcd_in.write_value(9);
    bcd_dec.evaluate();
    assert(bcd_out.read_value() == (1ULL << 9));
    assert(bcd_out[9].read() == logic::LogicState::HIGH);

    // Out of range (12 >= 10) -> all outputs LOW
    bcd_in.write_value(12);
    bcd_dec.evaluate();
    assert(bcd_out.read_value() == 0);

    // 5. Unknown input address handling
    bcd_in[0].write(logic::LogicState::UNKNOWN);
    bcd_dec.evaluate();
    for (std::size_t i = 0; i < 10; ++i)
    {
        assert(bcd_out[i].read() == logic::LogicState::UNKNOWN);
    }

    std::cout << "[PASS] Decoder Tests\n\n";
}

void test_bit_operations()
{
    std::cout << "--- Testing BitSelector & BitSlice ---\n";

    logic::Bus<8> bus_in;
    logic::Bus<8> bit_idx;
    logic::Wire selected_bit;
    logic::BitSelector<8> selector(bus_in, bit_idx, selected_bit);

    bus_in.write_value(0b10101100); // Bit 3 is 1, Bit 4 is 0
    bit_idx.write_value(3);
    selector.evaluate();
    assert(selected_bit.read() == logic::LogicState::HIGH);

    bit_idx.write_value(4);
    selector.evaluate();
    assert(selected_bit.read() == logic::LogicState::LOW);

    // BitSlice test (extract 4 bits starting from index 2)
    logic::Bus<4> slice_out;
    logic::BitSlice<8, 2, 4> slicer(bus_in, slice_out); // Bits 2..5 (1011 -> 0b1011 = 11)
    slicer.evaluate();
    std::cout << "BitSlice<8, 2, 4> of 0b10101100: 0x" << std::hex << slice_out.read_value() << "\n";
    assert(slice_out.read_value() == 0b1011);

    std::cout << "[PASS] Bit Operations Tests\n\n";
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

    test_bus_mux();
    test_mux3();
    test_mux_tree();
    test_shifters();
    test_extenders();
    test_encoders();
    test_decoders();
    test_bit_operations();
    test_bus_driver();

    std::cout << "ALL EXPANDED COMPONENT TESTS PASSED SUCCESSFULLY!\n";
    return 0;
}
