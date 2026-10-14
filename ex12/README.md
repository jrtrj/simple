# Experiment 12 — Intermediate code generation

**Aim.** Implement Intermediate code generation for simple expressions.

**Tool.** C only — `gcc`.

Full write-up: `../../report/main.pdf`, Experiment 12.

## Files

| File | What it is |
|---|---|
| `ex12_intermediate_code.c` | the program |
| `input.txt` | the expression, single-character operands |
| `expected_output.txt` | the output, captured from a real run |

## Build and run

```sh
cd ~/lab-exam/programs/ex12
gcc -o ex12 ex12_intermediate_code.c
./ex12 < input.txt
```

## What to look for

- `a+b*c` → `t1 = b * c`, `t2 = a + t1`, `Result = t2`. The multiplication is emitted first because it must be evaluated first.
- Two stages: infix to postfix (shunting-yard, using the precedence table), then postfix to three-address code (a stack walk that emits one instruction per operator).
- More verified: `a*b+c` → `t1=a*b, t2=t1+c`; `a+b*c-d` → `t1=b*c, t2=a+t1, t3=t2-d`; `a-b-c` → `t1=a-b, t2=t1-c` (left-associative).

## Notes

- **Parentheses break it, and not gracefully.** `precedence('(')` returns 0, so `(` is pushed onto the operator stack and treated as an operator; it ends up on the operand stack of Part B and the program copies uninitialised memory. Verified garbage on `(a+b)*c`. Do not demonstrate this on a bracketed expression.
- Operands are **single characters** only — `isalnum` tests one character, so `ab+cd` is read as four operands. Multi-character names need a real tokeniser, i.e. Experiment 1.
- A `generate` helper and three globals used to sit in the file as dead code. They have been removed; if you meet a copy that still has them, they play no part.
- Each emitted instruction is really a **quadruple** (operator, operand 1, operand 2, result) — that is how three-address code is stored in a real compiler.
- Anything that is not alphanumeric is treated as an operator, so a stray `$` would be pushed onto the operator stack.
