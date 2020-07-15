[![DOI](https://zenodo.org/badge/169076171.svg)](https://zenodo.org/badge/latestdoi/169076171) [![pipeline status](https://gitlab.com/swallat/hal/badges/master/pipeline.svg)](https://gitlab.com/swallat/hal/commits/master) [![macOS Workflow](https://github.com/emsec/hal/workflows/Github%20CI/badge.svg?branch=master)](https://github.com/emsec/hal/actions?query=branch%3Amaster+workflow%3A%22Github+CI%22) [![coverage report](https://codecov.io/gh/emsec/hal/branch/master/graph/badge.svg)](https://codecov.io/gh/emsec/hal) [![CodeFactor](https://www.codefactor.io/repository/github/emsec/hal/badge)](https://www.codefactor.io/repository/github/emsec/hal) [![Codacy Badge](https://api.codacy.com/project/badge/Grade/6070b197c3644c03bb3f0ec79d641675)](https://app.codacy.com/app/emsec/hal?utm_source=github.com&utm_medium=referral&utm_content=emsec/hal&utm_campaign=Badge_Grade_Settings) [![Doc: C++](https://img.shields.io/badge/doc-c%2B%2B-orange)](https://emsec.github.io/hal/doc/) [![Doc: Python](https://img.shields.io/badge/doc-python-red)](https://emsec.github.io/hal/pydoc/)

# Welcome to HAL!

[HAL](http://eprint.iacr.org/2017/783) \[/hel/\] is a comprehensive reverse engineering and manipulation framework for gate-level netlists focusing on efficiency, extendability, and portability. HAL comes with a full-fledged plugin system, allowing to introduce arbitrary functionalities to the core.

![HAL Screenshot](https://raw.githubusercontent.com/emsec/hal/master/hal_screenshot.png "HAL Screenshot")

HAL is currently under active developement by the Embedded Security group of the [Max Planck Institute for Security and Privacy](https://www.mpi-sp.org). Apart from multiple research projects, it is also used in our university lecture [Introduction to Hardware Reverse Engineering](https://www.ei.ruhr-uni-bochum.de/studium/lehrveranstaltungen/832/).

## Features

- Natural directed graph representation of netlist elements and their connections
- Support for custom gate libraries using the `liberty` gate library format
- High performance thanks to optimized C++ core
- Modularity: write your own Python or C++ Plugins for efficient netlist analysis and manipulation
- Ships with a selection of plugins for a variety of purposes such as
  - [DANA](https://eprint.iacr.org/2020/751.pdf) for data flow analsysis
  - [igraph](https://igraph.org) integration for graph analysis
- A feature-rich GUI allowing for visual netlist inspection and interactive analysis
  - Includes features such as hierarchization and isolation/cone view
- An integrated Python shell to exploratively interact with netlist elements and to interface plugins from the GUI
- A set of state-of-the-art benchmarks for the evaluation of netlist reverse engineering techniques available in a seperate [repository](https://github.com/emsec/hal-benchmarks).

## API Documentation

The C++ documentation is available [here](https://emsec.github.io/hal/doc/).
The Python documentation can be found [here](https://emsec.github.io/hal/pydoc/).

## Install Instructions

### Ubuntu

HAL releases are available via it's own ppa. You can find it here: [ppa:sebastian-wallat/hal](https://launchpad.net/~sebastian-wallat/+archive/ubuntu/hal)

### macOS

Use the following commands to install hal via [homebrew](https://brew.sh/index_de).

```bash
brew tap emsec/hal
brew install hal
```

### Build Instructions

If you want to build HAL locally on your machine, run the following commands:

1. `git clone https://github.com/emsec/hal.git && cd hal`
2. To install all neccessary dependencies execute `./install_dependencies.sh`
3. `mkdir build && cd build`
4. `cmake .. `
5. `make`

Optionally you can install HAL:

`make install`

#### Build on macOS

Please make sure to use a compiler that supports OpenMP. You can install one from e.g. Homebrew via: `brew install llvm`.

To let cmake know of the custom compiler use following command.

`cmake .. -DCMAKE_C_COMPILER=/usr/local/opt/llvm/bin/clang -DCMAKE_CXX_COMPILER=/usr/local/opt/llvm/bin/clang++`


## Quick Start

Install or build HAL and start the GUI via `hal -g`. You can list all available options via `hal [--help|-h]`.
We included some example netlists in `examples` together with the implementation of the respective example gate library in `plugins/example_gate_library`.
For instructions to create your own gate library and other useful tutorials, take a look at the [wiki](https://github.com/emsec/hal/wiki).

Load a library from the `examples` directory and start exploring the graphical representation.
Use the integrated Python shell or the Python script window to interact. Both feature (limited) autocomplete functionality.

Let's list all lookup tables and print their Boolean functions:
```python
for gate in netlist.get_gates():
    if "LUT" in gate.type.name:
        print("{} (id {}, type {})".format(gate.name, gate.id, gate.type.name))
        print("  {}-to-{} LUT".format(len(gate.input_pins), len(gate.output_pins)))
        boolean_functions = gate.boolean_functions
        for name in boolean_functions:
            print("  {}: {}".format(name, boolean_functions[name]))
        print("")
```
For the example netlist `fsm.vhd` this prints:
```text
FSM_sequential_STATE_REG_0_i_3_inst (id 4, type LUT6)
  6-to-1 LUT
  O: (!I1 & !I2 & I3 & !I4 & I5) | (I0 & !I2) | (I0 & I1) | (I0 & I3) | (I0 & I4) | (I0 & I5)

FSM_sequential_STATE_REG_0_i_2_inst (id 3, type LUT6)
  6-to-1 LUT
  O: (I2 & I3 & I4 & !I5) | (I1 & !I5) | (I1 & !I4) | (I1 & !I3) | (I0 & I1) | (I1 & I2)

FSM_sequential_STATE_REG_1_i_3_inst (id 6, type LUT6)
  6-to-1 LUT
  O: (!I1 & I4 & !I5) | (!I1 & !I3 & I4) | (I0 & I4 & !I5) | (I0 & !I3 & I4) | (!I1 & I2 & I4) | (I0 & I2 & I4) | (!I2 & !I5) | (!I2 & !I4) | (!I2 & !I3) | (!I0 & !I4) | (!I0 & !I2) | (!I0 & !I1) | (I1 & !I4) | (I1 & !I2) | (I0 & I1) | (I3 & !I5) | (I3 & !I4) | (!I0 & I3) | (I1 & I3) | (I2 & I3) | (!I4 & I5) | (!I3 & I5) | (!I0 & I5) | (I1 & I5) | (I2 & I5)

FSM_sequential_STATE_REG_1_i_2_inst (id 5, type LUT6)
  6-to-1 LUT
  O: (!I0 & I1 & !I2 & I3 & I4 & !I5) | (I0 & !I2 & I3 & I4 & I5)

OUTPUT_BUF_0_inst_i_1_inst (id 18, type LUT1)
  1-to-1 LUT
  O: !I0

OUTPUT_BUF_1_inst_i_1_inst (id 20, type LUT2)
  2-to-1 LUT
  O: (I0 & !I1) | (!I0 & I1)
```

## Benchmarks
A benchmark suite with modern open-source hardware designs for ASICs and FPGAs can be found here: [hal-benchmarks](https://github.com/emsec/hal-benchmarks)

## Contributing

You are welcome to contribute to the development of HAL. Feel free to submit a new pull request via github. Please consider running the static checks + clang format before that. You can also install these checks as git hooks before any commit.

### Run static checks and clang format locally
To install clang-format hook install [git-hooks](https://github.com/icefox/git-hooks) and run:

`git hooks --install`

Start Docker build via:
`docker-compose run --rm hal-build`

### Generate Changelog

`git log $(git describe --tags --abbrev=0)..HEAD --pretty=format:"%s" --no-merges`

## Citation

If you use HAL in an academic context, please cite the framework using the reference below:
```latex
@misc{hal,
    author = {{Embedded Security Group}},
    publisher = {{Max Planck Institute for Security and Privacy}},
    title = {{HAL - The Hardware Analyzer}},
    year = {2019},
    howpublished = {\url{https://github.com/emsec/hal}},
}
```

Feel free to also include the original [paper](http://eprint.iacr.org/2017/783)
```latex
@article{2018:Fyrbiak:HAL,
      author    = {Marc Fyrbiak and
                   Sebastian Wallat and
                   Pawel Swierczynski and
                   Max Hoffmann and
                   Sebastian Hoppach and
                   Matthias Wilhelm and
                   Tobias Weidlich and
                   Russell Tessier and
                   Christof Paar},
  title     	= {{HAL-} The Missing Piece of the Puzzle for Hardware Reverse Engineering,
               	  Trojan Detection and Insertion},
  journal	= {IEEE Transactions on Dependable and Secure Computing},
  year		= {2018},
  publisher	= {IEEE},
  howpublished 	= {\url{https://github.com/emsec/hal}}
}
```

## Contact and Support

For all kinds of inquiries, please contact us using our dedicated e-mail address: [hal@csp.mpg.de](mailto:hal@csp.mpg.de).

## Licensing

HAL is licensed under MIT License to encourage collaboration with other research groups and contributions from the industry. Please refer to the license file for further information.

## Disclaimer

HAL is at most alpha-quality software. Use at your own risk. We do not encourage any malicious use of our toolkit.
