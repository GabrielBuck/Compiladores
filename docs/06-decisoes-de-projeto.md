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
**Justificativa:** O professor recomendou arquivo único para facilidade de compilação com o comando simples exigido.
**Impacto:** O arquivo `compilador.c` poderá crescer, exigindo rígida organização interna e uso de protótipos de funções no topo.
