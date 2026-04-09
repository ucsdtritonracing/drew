@echo off & goto :windows
:<<BATCH
@echo off
:windows
python "%~dp0pre_generation.py"
exit /b
BATCH

#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"
python pre_generation.py