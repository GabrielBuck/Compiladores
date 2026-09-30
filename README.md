# Compiladores — Projeto 1: MiniVisualg (Análise Léxica e Sintática)

Projeto da disciplina de **Compiladores** (Profa. Daniela Cunha).

**Grupo:** Gabriel Nottoli Buck · Julia Andrade · Joao vitor rocha miranda

Implementação, em **C**, das duas primeiras etapas do front-end de um compilador para o
**MiniVisualg**, subconjunto simplificado do Visualg definido exclusivamente pelos exemplos do Anexo I
do enunciado.

## Estado atual

**Fase A (auditoria e requisitos) e Fase A.1 (conferência contra o PDF oficial) concluídas. Nada foi implementado ainda.**
Não há analisador léxico, analisador sintático nem `compilador.c` neste momento.

| Etapa do enunciado | Peso | Situação |
|---|---|---|
| 1 — ERs + GLC | 20% | não iniciada (Fases B–D) |
| 2 — Analisador léxico | 40% | não iniciada (Fase F) |
| 3 — Analisador sintático | 40% | não iniciada (Fase G) |

## Abordagem (decisões do grupo)

- C puro, sem Flex/Bison/Yacc nem bibliotecas externas (`DEC-02`).
- Scanner caractere a caractere: lexemas fixos em tabela declarativa + reconhecedores próprios para
  tokens de classe (ID, inteiro, real, string) (`DEC-05`).
- Parser descendente recursivo preditivo LL(1), uma função por não-terminal (`DEC-03`).
- Léxico e sintático integrados sob demanda: `nextToken()` → `obterToken()`. Isto é **requisito do enunciado**
  (REQ-32, REQ-28); o restante do desenho é do grupo (`DEC-04`).
- Primeiro erro léxico ou sintático encerra o processamento (`DEC-10`).

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
| [decisoes.md](docs/decisoes.md) | Decisões do grupo e ambiguidades do material em aberto. |
| [plano.md](docs/plano.md) | Fases A–J e critérios de saída. |

Previstos nas próximas fases: `especificacao-lexica.md`, `gramatica.md`, `arquitetura.md`, `testes.md`.

## Política de branches

- `main`: base original; não é alterada diretamente.
- `claude/projeto1`: desenvolvimento atual, criado a partir da `main`.
- `dev`: tentativa anterior, mantida só como histórico. **Não é fonte de verdade** e não é usada.
