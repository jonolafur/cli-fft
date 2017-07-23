#!/usr/bin/env python3.5

import subprocess, sys
from datetime import datetime, timezone

git_cmd = ['git', '-C', sys.argv[1], 'describe', '--all', '--long', '--dirty']

result = subprocess.run(git_cmd,  stdout=subprocess.PIPE)

version_string = result.stdout.decode('utf-8').strip('\n')

now = str(datetime.now(timezone.utc).astimezone() )

in_file  = open(sys.argv[2]+'/version.h.tmpl')
out_file = open(sys.argv[3]+'/version.h', 'wt')

for line in in_file:
   line = line.replace('$GIT_DESCRIBE$', version_string)
   line = line.replace('$WCNOW$', now)
   out_file.write( line )

in_file.close()
out_file.close()




