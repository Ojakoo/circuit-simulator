#include <string>

#include "doctest.h"

#include "resistor.hpp"
#include "capacitor.hpp"
#include "inductor.hpp"


SCENARIO("Testing admittances of passive circuit elements") {

    // passive elements are resistors, inductors and capacitors

    GIVEN("Three different components") {
    
        Resistor R1 = Resistor("R1", 50);
        Capacitor C1 = Capacitor("C1", 0.0001);
        Inductor L1 = Inductor("L1", 0.05);

        WHEN("Angular frequency is 50 Hertz") {

            float w = 50.0;
      
            THEN("Admittance of R1 is correct") {
                CHECK(R1.GetAdmittance() == 0.02f);
            }

            AND_THEN("Admittance of C1 is correct") {
                CHECK(C1.GetAdmittance(w) == std::complex<float>(0.0, 0.005));
            }

            AND_THEN("Admittance of L1 is correct") {
                CHECK(L1.GetAdmittance(w) == std::complex<float>(0.0, 0.4));
            }
        }
    }
}
