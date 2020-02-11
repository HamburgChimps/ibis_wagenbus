#ifndef _TinyIbis_h
#ifdef __cplusplus
#define _TinyIbis_h
#define _TINYIBIS_VERSION 0.1
#define PARITY_SEED 0x7F
#define _TERMINATOR 0x0D
#include "Arduino.h"
#include "ctre.hpp"



namespace TinyIBIS {
    const size_t IBIS_MAX_FIELD_SIZE = 50;

    enum class t_telegram {
        UKNOWN,
        DS001, 
        DS003,
        DS010E
    };
    static constexpr ctll::fixed_string DS001_re =  ctll::fixed_string{ "l([0-9]{3})"};
    static constexpr ctll::fixed_string DS003_re =  ctll::fixed_string{ "^hP([0-9]{4})$"};
    static constexpr ctll::fixed_string DS010E_re =  ctll::fixed_string{ "^xV(/-|/+)([0-9]{3})$"};


    typedef t_telegram (*t_fmatch) (std::string_view) noexcept;
    
    t_telegram GetTelegramType(char *t);

    class Ibis {
      private:
        // parsing state variables
        uint8_t parity = PARITY_SEED;
        bool isParity = false;
        bool isStart = false;
        uint8_t curTermOffset;
        char buffer[IBIS_MAX_FIELD_SIZE];
        void reset();
      public:
        Ibis();
        t_telegram encode(char c);
    };

} // namespace TinyIBIS
#endif // __cplusplus
#endif /* _TinyIbis_h */
