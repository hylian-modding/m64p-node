const fs = require('fs');
const path = require('path');

const name = 'ml64_emu_addon';

let js_dir = path.join(__dirname, '../js');
let dist_dir = path.join(__dirname, '../dist');

if (!fs.existsSync(js_dir))
    fs.mkdirSync(js_dir);

if (!fs.existsSync(dist_dir))
    fs.mkdirSync(dist_dir);

fs.copyFileSync(
    path.join(__dirname, `../build/Release/${name}.node`),
    path.join(__dirname, `../js/${name}.node`)
    );

fs.copyFileSync(
    path.join(__dirname, `../build/Release/${name}.node`),
    path.join(__dirname, `../dist/${name}.node`)
    );

fs.copyFileSync(
    path.join(__dirname, `../src/${name}.d.ts`),
    path.join(__dirname, `../dist/${name}.d.ts`)
    );
