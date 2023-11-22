#!/bin/bash
set -e
cat > /box/freeform.cc
chmod a-w /box/freeform.cc

cd /program
/program/.ppc/grader.py --file "/box/freeform.cc" --binary "/box/freeform" --json "$@"
