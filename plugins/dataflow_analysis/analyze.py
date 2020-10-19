#!/usr/bin/python3

import os
import argparse
import getpass
import json
import sys

from tabulate import tabulate
from pylatex import Document, Section, Subsection, Subsubsection, Command, Tabular, Table, Package, LongTable, MultiColumn, MultiRow
from pylatex.utils import italic, NoEscape

# our imports
from configuration import *

from generate_overview import *
#from analysis.generate_iteration_table import *
#from analysis.analyse_start_passes import *
#from analysis.find_top10 import *
#from analysis.pass_combinations import *


from utils import *


### parser

parser = argparse.ArgumentParser(description='Call the HAL dataflow plugin.')
parser.add_argument('--re_execute', action='store_true',
                    help='if set does executes all HAL netlists again')
parser.add_argument('--overview', action='store_true',
                    help='generates just the overview table')
parser.add_argument('--rebuild', action='store_true',
                    help='if set will remove build folder and rebuild cmake')
parser.add_argument('--rebuild_debug', action='store_true',
                    help='if set will remove build folder and rebuild cmake with debug and execute with gdb')
parser.add_argument('--debug', action='store_true',
                    help='if set will execute with gdb')





### MAIN ####
if __name__ == "__main__":
    args = parser.parse_args()

    synthesizers = set(x for x in netlists)
    designs = set(x for y in netlists for x in netlists[y])

    expect(os.path.isdir(path_to_core_collection),
           "core collection needs to be placed in: " + path_to_core_collection)
    expect(os.path.isdir(path_to_core_collection),
           "hal needs to be placed in: " + path_to_core_collection)

    if args.re_execute:
        # ---- BUILD HAL ----
        if args.rebuild or args.rebuild_debug:
            print("will recreate cmake")
            # delete everything in build folder
            os.system('rm -rf ' + path_to_hal_build + "/*")
            os.chdir(path_to_hal_build)
            if args.rebuild:
                os.system(
                    "cmake ../ -GNinja -DBUILD_DOCUMENTATION=OFF -DPL_DATAFLOW=ON -DCMAKE_BUILD_TYPE=RelWithDebInfo -DWITH_GUI=OFF")
            else:
                os.system(
                    "cmake ../ -GNinja -DBUILD_DOCUMENTATION=OFF -DPL_DATAFLOW=ON -DCMAKE_BUILD_TYPE=Debug -DWITH_GUI=OFF")

        expect(os.path.isdir(path_to_hal_build),
               "path to hal build '{}' does not exist".format(path_to_hal_build))

        os.chdir(path_to_hal_build)

        return_ninja = os.system('ninja')
        expect(return_ninja == 0,
               "error in build: return code: " + str(return_ninja))

        failed_configs = list()
        for synth in synthesizers:
            for design in netlists[synth]:
                # only run netlists for paper
                if design in netlists_paper and synth != "wordrev":
                    print("starting: {} {}".format(design, synth))
                    # run without sizes
                    if not execute_hal(synth, design, args.rebuild_debug, args.debug, ""):
                        failed_configs.append((synth, design))
                    # run with sizes
                    if not execute_hal(synth, design, args.rebuild_debug, args.debug, netlists_paper[design]):
                        failed_configs.append((synth, design))

        if failed_configs:
            print("WARNING: some combinations failed, most likely their files do not exist in the core collection:")
            for synth, design in failed_configs:
                print("  "+design+" "+synth)



    jsons_of_netlist = []

    for root, dirs, files in os.walk(path_dataflow_out):
        for file in files:
            if file.endswith(".json"):
                if "others" not in root:
                    jsons_of_netlist.append(os.path.join(root, file))



    json_map_to_design = {}

    for json_file_path in jsons_of_netlist:
        with open(json_file_path) as json_file:
            data = json.load(json_file)
            for design_name in data:
                if design_name == "test":
                    continue
                json_map_to_design[design_name[:design_name.rindex("_")]] = []

    # create substrings for design_names, so we can create nice chapters with _vivado, _synopsys...
    for json_file_path in jsons_of_netlist:
        with open(json_file_path) as json_file:
            data = json.load(json_file)
            for design_name in data:
                if design_name == "test":
                    continue
                json_map_to_design[design_name[:design_name.rindex("_")]].append(
                    json_file_path)




    # create latex report
    geometry_options = {
        "margin": "2.54cm",
        "includeheadfoot": True
    }

    doc = Document(page_numbers=True, geometry_options=geometry_options)

    doc.packages.append(Package('float'))


    doc.preamble.append(Command('title', 'Report Dataflow Analysis'))
    doc.preamble.append(Command('author', 'Nils, Max, Sebus'))
    doc.preamble.append(Command('date', NoEscape(r'\today')))
    doc.append(NoEscape(r'\maketitle'))

    doc.append(NoEscape(r'\tableofcontents'))


    doc.append(NoEscape(r'\newpage'))

    top_x = 20

    with doc.create(Section("Overview")):
        generate_overview(json_map_to_design, doc)
        '''
        if not args.overview:
            with doc.create(Subsection("Top " + str(top_x) + " of Best and Worst Overall First Layer Passes")):
                find_top_x_of_best_worst_non_start_layers(json_map_to_design, doc, top_x)
            with doc.create(Subsection("Top " + str(top_x) + " of Best and Worst Pass Combinations")):
                find_top_x_of_best_worst_pass_combination(json_map_to_design, doc, top_x)
        '''


    doc.append(NoEscape(r'\newpage'))

    '''
    # generate correct sections
    for design, json_file_paths in json_map_to_design.items():
        with doc.create(Section(design)):
            for json_file_path in json_file_paths:
                with open(json_file_path) as json_file:
                    data = json.load(json_file)
                    # detailed analysis
                    for design_name in data:
                        with doc.create(Subsection(design_name)):
                            with doc.create(Subsubsection("Iteration Table")):
                                generate_iteration_table(data, design_name, doc)
                                doc.append(NoEscape(r'\newpage'))
                            if not args.overview:
                                with doc.create(Subsubsection("Start passes change over time")):
                                    analyze_start_passes(data, design_name, doc)
                                    doc.append(NoEscape(r'\newpage'))
                                with doc.create(Subsubsection("top5, bottom5 combinations for all start passes")):
                                    find_top_pass_combinations(data, design_name, doc)
        doc.append(NoEscape(r'\newpage'))
    '''

    # doc.generate_pdf(clean_tex=False)
    doc.generate_pdf(path_dataflow_out + '/report', clean_tex=False)
    print("created latex files: " + path_dataflow_out)
