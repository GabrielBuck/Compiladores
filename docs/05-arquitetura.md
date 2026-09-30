# Arquitetura do Compilador

O compilador segue uma arquitetura baseada no modelo produtor-consumidor orientado pelo analisador sintático.

## Fluxo de Execução
1. `main` recebe o caminho do arquivo e inicializa a leitura.
2. Inicia-se a análise sintática (chamada da função correspondente ao símbolo inicial `Programa()`).
3. O parser requisita tokens chamando `nextToken()`.
4. `nextToken()` chama `obterToken()` (analisador léxico).
5. O analisador léxico lê caracteres do fluxo sob demanda, montando o lexema, identificando o token, validando na Tabela de Símbolos, e retornando-o.
6. O token retornado é simultaneamente formatado e escrito no terminal/arquivo de saída de tokens.
7. O parser avalia a estrutura e continua até o fim.

## Derivação
O registro da derivação (árvore de parsing ou traço das regras de produção) será implementado via logs/impressão a cada função do não-terminal, de forma simplificada sem necessariamente materializar toda a árvore na memória em formato de grafos nesta primeira etapa.
