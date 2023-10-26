  #include <stdint.h>
  #include <avr/io.h>
  class Bouton{
    public:
    Bouton(uint8_t int_N_p);
    ~Bouton();

    void setRisingEdge();
    void setFallingEdge();
    void setAnyEdge();
    void enableInterrupt();
    void disableInterrupt();
    void reset();

    private:
        uint8_t int_N;
  };