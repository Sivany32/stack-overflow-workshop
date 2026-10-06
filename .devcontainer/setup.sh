#!/usr/bin/env bash
# Runs once when the Codespace / devcontainer is created.
set -euo pipefail

echo "== Installing toolchain =="
sudo apt-get update
sudo apt-get install -y gcc g++ gdb make python3 python3-pip file curl

echo "== Installing GEF (lightweight gdb enhancement) =="
# GEF is a single script sourced from ~/.gdbinit — fast to install, no build step.
bash -c "$(curl -fsSL https://gef.blah.cat/sh)" || \
  echo "GEF install failed (network?). Plain gdb still works."

# Optional: pwndbg instead of GEF. Uncomment if you prefer it (slower to install).
# git clone https://github.com/pwndbg/pwndbg ~/pwndbg && (cd ~/pwndbg && ./setup.sh)

echo "== Making addresses predictable (best effort) =="
# -no-pie already fixes code addresses; this also calms stack ASLR if permitted.
echo 0 | sudo tee /proc/sys/kernel/randomize_va_space >/dev/null 2>&1 || \
  echo "(Could not disable ASLR in this sandbox — fine, -no-pie covers the code.)"

echo "== Building challenges =="
make

echo ""
echo "Setup complete. Try:  gdb -q ./bin/01_find_stack"
