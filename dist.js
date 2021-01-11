const { execSync } = require('child_process')
const { copyFileSync } = require('fs');

execSync('npm run rebuild', { stdio: 'inherit' });
execSync('mkdir dist', { stdio: 'inherit' });
copyFileSync('bin/m64p.node', 'dist/m64p.node');
copyFileSync('src/Ts/m64p.d.ts', 'dist/m64p.d.ts');
