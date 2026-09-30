# Decisões de Projeto

## DEC-001: Separação de Tokens (Espaços vs. Caracteres Adjacentes)
**Problema:** O enunciado cita que os lexemas estão separados por espaços, porém os exemplos incluem `leia(nome)`, onde não há espaços entre `leia`, `(`, `nome` e `)`.
**Opções consideradas:**
1. Usar `strtok` cegamente no espaço e obrigar o código a ser espaçado artificialmente.
2. Construir o scanner caractere a caractere, considerando pontuações e operadores como separadores implícitos além dos espaços.
**Decisão:** Optou-se pela opção 2 (scanner caractere a caractere).
**Justificativa:** É a forma correta e robusta de construir um analisador léxico, aderindo à prática real e comportando a sintaxe dos exemplos anexos no documento do projeto. A leitura puramente baseada em tokens separados por espaços quebraria no primeiro `leia(a)`.
**Impacto:** O léxico terá um laço de leitura com `fgetc` controlando os estados de um autômato para formar identificadores e números, parando corretamente antes de engolir delimitadores.

## DEC-002: Organização de Arquivos
**Problema:** Como estruturar o código C mantendo a simplicidade para avaliação, evitando Makefiles complexos e vários `*.o` no começo.
**Opções consideradas:**
1. Múltiplos arquivos `.c` e `.h`.
2. Arquivo único `compilador.c` com seções bem definidas.
**Decisão:** Arquivo único `compilador.c`.
**Justificativa:** A Profa. Daniela Cunha recomendou arquivo único para facilidade de compilação com o comando simples exigido.
**Impacto:** O arquivo `compilador.c` poderá crescer, exigindo rígida organização interna e uso de protótipos de funções no topo.

## DEC-003: Validação baseada em evidência
**Problema:** Resultados presumidos podem esconder erros de compilação ou execução.
**Decisão:** Nenhuma funcionalidade será marcada como concluída ou validada sem comando executado e resultado observado.
**Justificativa:** O projeto será avaliado por compilação e execução reais usando GCC/MinGW.
**Impacto:** Relatórios de desenvolvimento deverão distinguir claramente: planejado, implementado, compilado e testado.

## DEC-004: Inconsistência dos exemplos de procedimentos
**Problema:** O Anexo I mostra uma disposição confusa onde aparecem blocos `algoritmo`, seguidos por `procedimento`, e depois um novo `algoritmo` em uma disposição que não deixa perfeitamente inequívoca a estrutura completa do programa.
**Decisão:** Não corrigiremos isso usando conhecimento externo de Visualg. Registramos o que está literalmente presente. A definição de se subprogramas ocorrem antes, depois ou no lugar de `var` ficará pendente até a construção da gramática.
**Justificativa:** Aderência estrita à especificação do projeto baseada em evidência.
**Impacto:** A gramática será desenhada para acomodar as evidências, não necessariamente o Visualg completo.

## DEC-005: Tratamento de erro em strings incompletas
**Problema:** Um literal de string que não fecha na mesma linha ou atinge EOF.
**Decisão:** Gerar ERRO LÉXICO.
**Justificativa:** Decisão do grupo. MiniVisualg não tem evidência de suporte para strings multilinha.
**Impacto:** O scanner reportará o erro no momento da leitura da quebra de linha ou final de arquivo.

## DEC-006: Tratamento de números negativos
**Problema:** Existe evidência de `passo -2`. O número negativo `-2` será tratado como o token `NUM_INT` com atributo `-2` ou como token `-` seguido do número `2`?
**Decisão:** Tratar `-` como operador/token separado (TOKEN_MENOS) e deixar o sinal unário para ser resolvido na análise sintática.
**Justificativa:** Preferência arquitetural baseada em práticas comuns, simplificando a ER de literais numéricos para `[0-9]+` e evitando conflitos com operadores de subtração.
**Impacto:** O parser terá uma regra para tratar o menos unário.

## DEC-007: Case Sensitivity
**Problema:** O material não define claramente se MiniVisualg é case-sensitive.
**Decisão:** Ambíguo. Como proposta pendente, as palavras reservadas serão reconhecidas exatamente como nos exemplos (minúsculas). Ficará aguardando eventual esclarecimento futuro.
**Justificativa:** Não devemos pesquisar o Visualg externo. A evidência do Anexo I apresenta predominantemente caixa baixa.
**Impacto:** O léxico pode tratar tudo de forma exata até obtermos instrução contrária.
