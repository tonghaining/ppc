#!/bin/bash
set -e
cat > /box/freeform.cu
chmod a-w /box/freeform.cu

# Fix nvprof
cat > "/etc/nsswitch.conf" <<EOT
passwd:         files systemd
EOT

cd /program
/program/.ppc/grader.py --file "/box/freeform.cu" --binary "/box/freeform" --json "$@"
