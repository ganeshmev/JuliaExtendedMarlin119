# Julia 2018 Marlin

3 September 2018 - [Marlin](https://github.com/MarlinFirmware/Marlin/tree/7b594ee4a2feba8872d86efff16f414d93dc01c7) 1.1.9

Commit - 7b594ee

[G-code](http://marlinfw.org/meta/gcode/)

## Build Variants

| Variant                  | Variant Code               | Short Code | Build Code |
|--------------------------|----------------------------|------------|-----------:|
| Julia Basic              | JULIA_2018_GLCD            | J18GX      | 0          |
| Julia Intermediate       | JULIA_2018_GLCD_HB         | J18GB      | 1          |
| Julia Advanced           | JULIA_2018_RPI             | J18RX      | 2          |
| Julia Extended           | JULIA_2018_RPI_E           | J18RE      | 3          |
| Julia Pro Single         | JULIA_2018_PRO_SINGLE      | J18PS      | 4          |
| Julia Pro Dual           | JULIA_2018_PRO_DUAL        | J18PD      | 5          |
| Julia Pro Single ABL     | JULIA_2018_PRO_SINGLE_A    | J18PT      | 6          |
| Julia Pro Dual ABL       | JULIA_2018_PRO_DUAL_A      | J18PE      | 7          |

## Migration Process

1. Clone Marlin to `src/Marlin` .
2. Copy overriding files to `src/common`.
3. Modify overriding files.
4. Commit source to git.

## Build Environment Initialization

1. Install [Node.js](https://nodejs.org/en/download/).
2. Change to project root in terminal.
3. Execute `npm i` to install *Node.js* package dependencies.

## Code Compilation

1. Generate intermediates using build script in **Git Bash for Windows**.    
    `./scripts/build <variant_code>`.
1. Export hex files using Arduino IDE.


## Directory Structure

- `src/` - Source root
  - `Marlin/` - [Marlin](https://github.com/MarlinFirmware/Marlin)
  - `common/` - Files common to all variants
- `scripts/` - Build automation rellated scripts
- `staging/` - Marlin + common + variant files
- `output/` - Hex files
- ~~`build.ps1` - Build automation PowerShell script~~ **redundant**
- `watcher.js` - Build automation Node.js script