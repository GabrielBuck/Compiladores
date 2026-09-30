# Compiladores — Projeto 1: MiniVisualg (Análise Léxica e Sintática)

Projeto da disciplina de **Compiladores** (Profa. Daniela Cunha).

**Grupo:** Gabriel Nottoli Buck · Julia Andrade · Joao vitor rocha miranda

Implementação, em **C**, das duas primeiras etapas do front-end de um compilador para o
**MiniVisualg**, subconjunto simplificado do Visualg definido exclusivamente pelos exemplos do Anexo I
do enunciado.

## Estado atual

**Concluídas — somente documentação:** Fase A (auditoria e requisitos), Fase A.1 (conferência contra o
PDF oficial), Fase B (especificação léxica) e Fase C (gramática). Não há analisador léxico, analisador
sintático nem `compilador.c` neste momento. A próxima fase é a **D (FIRST/FOLLOW, nullable e validação
LL(1))**.

| Etapa do enunciado | Peso | Situação |
|---|---|---|
| 1 — ERs + GLC | 20% | **ERs: concluídas documentalmente** (`docs/especificacao-lexica.md`). **GLC: concluída documentalmente** (`docs/gramatica.md`). **FIRST/FOLLOW: pendente** (Fase D) |
| 2 — Analisador léxico | 40% | **não implementado** (especificação pronta; implementação na Fase F) |
| 3 — Analisador sintático | 40% | **não implementado** (gramática pronta; implementação na Fase G) |

## Abordagem (decisões do grupo)

- C puro, sem Flex/Bison/Yacc nem bibliotecas externas (`DEC-02`).
- Scanner caractere a caractere: lexemas fixos em tabela declarativa + reconhecedores próprios para
  tokens de classe (ID, inteiro, real, string) (`DEC-05`).
- Parser descendente recursivo preditivo LL(1), uma função por não-terminal (`DEC-03`).
- Léxico e sintático integrados sob demanda: `nextToken()` → `obterToken()`. Isto é **requisito do enunciado**
  (REQ-32, REQ-28); o restante do desenho é do grupo (`DEC-04`).
- Primeiro erro léxico ou sintático encerra o processamento (`DEC-10`). O código de retorno nesse caso
  **ainda não está decidido** (`AMB-13`).
- Léxico case-sensitive; 50 nomes de token; `<` e `>` isolados e `.` isolado são erro léxico
  (`DEC-13`, `DEC-16`, `DEC-24`).
- Gramática: sub-rotinas antes do principal (`DEC-28`); `OU` aceito no mesmo nível de `E`
  (`DEC-31`, `DEC-32`); sem subtração, `-` só em `passo -2` (`DEC-33`, `DEC-34`).

## Compilação e execução (quando houver código)

```
gcc -Wall -Wno-unused-result -g -Og compilador.c -o compilador
./compilador programa.alg          # Linux/macOS/MinGW shell
.\compilador.exe programa.alg      # Windows
```

O nome do arquivo MiniVisualg é sempre recebido por linha de comando.

## Documentação (`docs/`)

| Arquivo | Conteúdo |
|---|---|
| [requisitos.md](docs/requisitos.md) | Requisitos rastreáveis, com origem (enunciado / aula / grupo). |
| [especificacao-minivisualg.md](docs/especificacao-minivisualg.md) | Inventário do que o Anexo I confirma, menciona ou não cobre. |
| [especificacao-lexica.md](docs/especificacao-lexica.md) | **Contrato léxico congelado (Fase B):** tokens, ERs, atributos, formato de saída, erros. |
| [gramatica.md](docs/gramatica.md) | **GLC congelada (Fase C):** 91 produções em BNF, sem recursão à esquerda, fatorada; cobertura do Anexo I; derivações manuais. |
| [decisoes.md](docs/decisoes.md) | Decisões do grupo e ambiguidades do material em aberto. |
| [plano.md](docs/plano.md) | Fases A–J e critérios de saída. |

Previstos nas próximas fases: FIRST/FOLLOW (Fase D, em `gramatica.md` ou documento próprio),
`arquitetura.md`, `testes.md`.

## Política de branches

- `main`: base original; não é alterada diretamente.
- `claude/projeto1`: desenvolvimento atual, criado a partir da `main`.
- `dev`: tentativa anterior, mantida só como histórico. **Não é fonte de verdade** e não é usada.
