#pragma once

#include <string>

#include "../application/DerivativeService.h"

namespace calc::ui {

// Interface grafica (SFML 3.0) da calculadora.
//
// Esta classe so conhece o CONTRATO publico do nucleo:
//     CalculationResult DerivativeService::calculate(const std::string&)
// Ela nao inclui Lexer, Parser, Factory nem Formatter e nao contem nenhuma
// regra matematica: le o texto digitado, chama o servico e desenha o que
// o servico devolveu (derivada ou mensagem de erro).
//
// Os tipos da SFML aparecem apenas em CalculatorWindow.cpp; este header
// nao depende da SFML.
class CalculatorWindow {
public:
    explicit CalculatorWindow(const DerivativeService& service);

    // Abre a janela e roda o laco de eventos ate o usuario fechar.
    // Retorna 0 em caso normal e 1 se nenhuma fonte pode ser carregada.
    int run();

private:
    // Chama o servico com o texto atual e guarda o resultado para exibicao.
    void calculate();

    const DerivativeService& service_;
    std::string input_;       // o que o usuario digitou (sem o prefixo f(x) =)
    std::string output_;      // derivada formatada OU mensagem de erro
    bool hasOutput_ = false;  // ja houve algum calculo?
    bool outputIsError_ = false;
};

}  // namespace calc::ui
