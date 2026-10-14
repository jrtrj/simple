# Experiment 11 — Shift-reduce parser

**Aim.** Construct a Shift Reduce Parser for a given language.

**Tool.** C only — `gcc`.

Full write-up: `../../report/main.pdf`, Experiment 11.

## Files

| File | What it is |
|---|---|
| `ex11_shift_reduce.c` | the program |
| `input.txt` | the expression, using `i` for `id` |
| `expected_output.txt` | the output, captured from a real run |

## Build and run

```sh
cd ~/lab-exam/programs/ex11
gcc -o ex11 ex11_shift_reduce.c
./ex11 < input.txt
```

## What to look for

- Every Shift and every Reduce is printed with the stack and remaining input, then `Accepted`.
- The grammar is `E → E+E | E*E | (E) | id` — deliberately ambiguous.
- `i+i`, `i*i+i`, `(i+i)*i` → Accepted; `*i` → Rejected.

## Notes

- **The precedence fix is visible in the trace.** On `i+i*i`, when the stack first holds `E+E` with `*` still unread, the parser **shifts** instead of reducing: the trace goes `E+E` → `E+E*` → `E+E*E` → reduce `E*E` → reduce `E+E`. The order of the final reductions *is* the tree `i+(i*i)`.
- Without that one condition (`input[ip] != '*' && input[ip] != '/'` on the `E→E+E` rule) the parser reduces `E+E` the moment the pattern matches and commits to `(i+i)*i`. The bug is asymmetric — `i*i+i` comes out right either way — which is why it is worth tracing rather than eyeballing.
- `reduce()` **returns a boolean**, and the driver calls it in a `while(reduce());` loop — a reduction cascade. There is a second `while(reduce());` after the input ends, to catch a reduction that only becomes possible once the input is exhausted (`i+i` needs it).
- `display` writes `stack[top+1]='\0'` so the array can print as a string, then prints `input + ip` — a pointer into the input at the current position.
- `-` and `/` are not in the grammar: `i-i` is rejected. Added operators need both an `if` branch and a precedence entry.
- This is a hand-coded simulation of a bottom-up parser, not a table-driven LR parser.
