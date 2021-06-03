const chalk = require('chalk');
const path = require('path');
const readlineSync = require('readline-sync');
const fs = require('fs-extra');
const exec = require('child_process').exec;
const chokidar = require('chokidar');
const moment = require('moment');


function i(s) { console.log(s); }
function e(s) { console.log( chalk.red(s) ); }
function s(s) { console.log( chalk.green(s) ); }
function m(s) { console.log( chalk.gray(s) ); }
function p() { readlineSync.keyInPause("Press any key to exit."); }
function fe(d) { return fs.existsSync(d); }
function mkdirne(d) { if ( !fe(d) ) { fs.mkdirSync(d); } }
function cdir(d) { fs.emptyDirSync(d); }

const args = process.argv.slice(2);

if (args.length != 1) {
	e("Staging path not specified!");
	process.exit(1);
}

const  pwd = __dirname;
const dir_src = path.join( pwd, "src" );
const dir_src_common = path.join( dir_src, "common" );

const staging_name = args[0]; //"Julia2018Marlin";
const dir_staging = path.join( pwd, "staging", staging_name );
const dir_output = path.join( pwd, "output" );

const path_ino = path.join(dir_staging, staging_name + ".ino");
const path_hex = path_ino + ".mega.hex";

// Make intermediate dir if not exists
mkdirne( dir_staging );

// Make output dir if not exists
mkdirne( dir_output );

s( "Fracktal Works Marlin Build Automation" );
m( "PWD: " + pwd );

// Check for Marlin or Exit
if ( !fs.existsSync(dir_src_common) ) {
    e( "Source not found." );
    p();
    return;
}

m("STG: " + dir_staging);
m("INO: " + path_ino);
m("Hex: " + path_hex);

// Open ino file
exec( path_ino );
s( "Opening INO file" );

// Watch src variant dir for changes
s( "Watching source dir.." );

function copyData(srcPath, destPath) {
    fs.readFile(srcPath, function (err, data) {
		if (err) throw err;
		//Do your processing, MD5, send a satellite to the moon, etc.
		fs.writeFile (destPath, data, function(err) {
			if (err) throw err;
			m(' Overwritten: ' + destPath );
		});
	});
}

function onFileEvent(filePath) {
	try {
		m(" " + moment().format("DD-MM-YYYY HH:mm:ss") + " Changed: " + filePath);
		
		if ( filePath.indexOf(".hex") < 0 ) {
			// fs.copySync( filePath, dir_staging, { "overwrite": true } );
			copyData( filePath, path.join(dir_staging, path.basename( filePath )) );
		} else {
			const path_hex_dest = path.join( dir_output, staging_name + "_mega_" + moment().format("DDMMYYYY_HHmmss") + ".hex");
			fs.copySync( filePath, path_hex_dest, { "overwrite": true } );
		}
	} catch(ex) {
		e(ex);
	}
}

const watcher = chokidar.watch( [ dir_src_common, path_hex ] , { "persistent": true, "ignoreInitial": true } );
watcher.on('add', onFileEvent);
watcher.on('change', onFileEvent);


function exitHandler() {
	process.stdin.resume();//so the program will not close instantly
	
	if (watcher)
		watcher.close();
    process.exit();
}

//do something when app is closing
process.on('exit', exitHandler.bind(null));

//catches ctrl+c event
process.on('SIGINT', exitHandler.bind(null));

// catches "kill pid" (for example: nodemon restart)
process.on('SIGUSR1', exitHandler.bind(null));
process.on('SIGUSR2', exitHandler.bind(null));

//catches uncaught exceptions
process.on('uncaughtException', exitHandler.bind(null));