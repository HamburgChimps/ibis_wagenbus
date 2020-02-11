#include "TinyIbis.h"

namespace TinyIBIS {

    t_telegram MatchDS001(std::string_view sv) noexcept {
        if (auto match = ctre::match<DS001_re>(sv)) {
            return t_telegram::DS001;
        }
        return t_telegram::UKNOWN;
    };

    t_telegram MatchDS003(std::string_view sv) noexcept {
        if (auto match = ctre::match<DS003_re>(sv)) {
            return t_telegram::DS003;
        }        return t_telegram::UKNOWN;
        return t_telegram::UKNOWN;
    };

    t_telegram MatchDS010E(std::string_view sv) noexcept {
        if (auto match = ctre::match<DS010E_re>(sv)) {
            return t_telegram::DS010E;
        }
        return t_telegram::UKNOWN;
    };

    static const t_fmatch MatchFns[3] = {
        &MatchDS001,
        &MatchDS003,
        &MatchDS010E
    };

    Ibis::Ibis(/* args */)
        : parity(0), isParity(false), curTermOffset(0) {
        parity = PARITY_SEED;
        buffer[0] = 0;
    };

    t_telegram Ibis::encode(char c) {
        if (curTermOffset >= IBIS_MAX_FIELD_SIZE) {
            // Buffer "overflow" - reset and return
            reset();
            return t_telegram::UKNOWN;
        }

        if (isParity) { // A parity byte follows a terminator
            if (parity == (uint8_t)c) {
                t_telegram tt = GetTelegramType(buffer);
                reset();
                return tt;
            }
            reset();
            return t_telegram::UKNOWN;
            // Telegram END
        }
        parity ^= (uint8_t)c;
        if (c == _TERMINATOR) { // We use a terminator to sync
            isParity = true;
            buffer[curTermOffset] = '\0'; // End String
            return t_telegram::UKNOWN;
        }
        buffer[curTermOffset] = c;
        curTermOffset += 1;
        return t_telegram::UKNOWN;
    }

    void Ibis::reset() {
        curTermOffset = 0;
        parity = PARITY_SEED;
        isParity = false;
        isStart = false;
        memset(buffer, '\0', IBIS_MAX_FIELD_SIZE);
    }

    t_telegram GetTelegramType(char *t) {
        Serial.printf("\nGet telegram type for: %s\n", t);
        const size_t se = sizeof(MatchFns) / sizeof(MatchFns[0]);
        const std::string_view sv = std::string_view{t};
        for (int i = 0; i < se; i++) {
            const t_telegram tt = MatchFns[i](sv);
            if (tt != t_telegram::UKNOWN) {
                return tt;
            };
        }
        return t_telegram::UKNOWN;
    }

} // namespace TinyIBIS
