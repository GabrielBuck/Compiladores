# Suíte de testes do MiniVisualg

Suíte escrita na **Fase E, antes de existir qualquer código** do compilador. Ela é o contrato
observável que o analisador léxico (Fase F), o parser (Fase G) e a integração (Fase H) devem satisfazer.
**Os testes não serão adaptados para "fazer o código passar"**: se o código divergir, corrige-se o código
ou registra-se uma emenda formal na especificação (nova DEC), nunca o teste em silêncio.

Documento principal, com matrizes, rastreabilidade e cobertura: [`../docs/testes.md`](../docs/testes.md).

## Estrutura

```
testes/
├── manifest.tsv            uma linha por teste (TAB): id, camada, classe, arquivo, esperado, linha, detalhe, rastreabilidade
├── lexico/
│   ├── validos/            LX-V*: programas que tokenizam até o fim sem erro
│   ├── erros/              LX-E*: um erro léxico por arquivo
│   └── esperados/          goldens: LX-V*.tokens.txt e LX-E*.erro.txt
└── sintatico/
    ├── validos/            SY-V*: programas que a GLC aceita
    ├── erros/              SY-E*: léxicamente válidos, rejeitados pela GLC
    └── limites/            SY-L*: aceitos pela GLC, mas semanticamente questionáveis
```

## IDs

| Prefixo | Significado | Esperado no manifest |
|---|---|---|
| `LX-Vnn` | léxico válido | `TOKENS` |
| `LX-Enn` | erro léxico | `ERRO_LEXICO` + linha |
| `SY-Vnn` | sintático válido | `ACEITA` |
| `SY-Enn` | erro sintático | `ERRO_SINTATICO` + linha |
| `SY-Lnn` | limitação consciente | `ACEITA` (= aceito **sintaticamente**) |

O arquivo de cada teste começa pelo seu ID (`LX-V01-minimo.alg`). Nomes de arquivo são ASCII; o
conteúdo é UTF-8 e usa acentos quando necessário.

## Classes de autoridade

- **CORE** — comportamento sustentado diretamente pelo Anexo I ou pelo enunciado.
- **DECISION** — comportamento definido por uma decisão do grupo (`DEC-nn` em `docs/decisoes.md`).
  **Não** é requisito da professora.
- **LIMITATION** — demonstra conscientemente um limite da análise só léxica/sintática (o programa é
  aceito, embora seja semanticamente questionável). **Não** é exemplo oficial de MiniVisualg válido.

## Arquivos esperados

- `lexico/esperados/<arquivo>.tokens.txt` — **golden da Fase F**: a listagem de tokens exata,
  no formato do contrato léxico (`linha# NOME` ou `linha# NOME | atributo`; sem `TOKEN_EOF`). A comparação
  deve ser **linha a linha e literal**, tolerando apenas a diferença de fim de linha CRLF/LF (no Windows
  o Git converte LF em CRLF no checkout).
- `lexico/esperados/<arquivo>.erro.txt` — especifica o **conteúdo** do erro léxico, não o formato do fluxo
  de saída:

  ```
  TIPO=ERRO LÉXICO
  LINHA=<n>
  SEQUENCIA=<texto>
  ```

  Onde a mensagem é impressa (stdout/stderr/arquivo) ainda está em aberto (AMB-14).
- Testes sintáticos só esperam **aceitação ou rejeição**; para os `SY-E`, também a **linha** e o
  **token** do primeiro erro (ver `docs/testes.md` §8).

## Código de retorno

O **exit status** dos casos de erro é **TBD** (AMB-13): nenhum teste exige `exit=0` ou `exit=1`.
Entradas válidas devem terminar com retorno 0.

## Arquivos que terminam sem quebra de linha (de propósito)

`LX-V08-comentario-eof.alg`, `LX-E08-string-nao-fechada-eof.alg`, `SY-E19-falta-fimalgoritmo.alg`.
Não "conserte" esses arquivos adicionando um `\n` final.
