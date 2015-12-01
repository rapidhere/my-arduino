#include "framework.h"
#include "commondata.h"

namespace rapid {

void blink(uint32 interval_ms) {
    digitalWrite(LED_PIN, HIGH);
    delay(interval_ms);
    digitalWrite(LED_PIN, LOW);
    delay(interval_ms);
}

} // namespace rapid
