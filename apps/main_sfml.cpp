// Ponto de entrada da versao grafica.
// Cria o servico do nucleo e entrega para a interface SFML.
// Para trocar a interface, basta substituir este arquivo e o modulo ui/:
// o nucleo (DerivativeService e tudo abaixo dele) nao muda.
#include "application/DerivativeService.h"
#include "ui/CalculatorWindow.h"

int main() {
    calc::DerivativeService service;
    calc::ui::CalculatorWindow window(service);
    return window.run();
}
