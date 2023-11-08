#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "ByteCodeInterpreter.h" // inclure le fichier d'en-tête de la classe ByteCodeInterpreter

void executeLoad()
{
  LED led(&PORTA, &DDRA, PA0, PA1);
  ByteCodeInterpreter interpreter;
  interpreter.receiveAndSave();
  while (true)
  {
    led.turnLedGreen();
  }
}

void executeRun()
{
  ByteCodeInterpreter interpreter;
  while (true)
  {
    interpreter.run();
  }
}

int main()
{
  LED led(&PORTA, &DDRA, PA0, PA1);
  ByteCodeInterpreter interpreter;
  interpreter.receiveAndSave(); // cette ligne appelle la fonction receiveAndSave() de la classe ByteCodeInterpreter, fonction bloquante
  while (true)
  {
    interpreter.run(); // cette ligne appelle la fonction run() de la classe ByteCodeInterpreter, qui exécuté chaque instruction du programme stocké dans la mémoire
  }
  // #ifdef ACTIVE_LOAD
  //   executeLoad();
  // #else
  //   executeRun();
  // #endif

  return 0;
}
