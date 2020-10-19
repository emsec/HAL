#/usr/bin/python3
import json

from tabulate import tabulate
from pylatex import Document, Section, Subsection, Subsubsection, Command, Tabular, Table, Package, LongTable, MultiColumn, MultiRow
from pylatex.utils import italic, NoEscape

# our imports
from utils import *



# gather data from json
def generate_overview(json_map_to_design, doc):

    overview_data = dict()

    for design_type, json_paths in json_map_to_design.items():
        overview_data[design_type] = dict()

        for json_file_path in json_paths:
            with open(json_file_path) as json_file:
                data = json.load(json_file)
                for design_name in data:
                    if design_name not in overview_data[design_type]:
                        overview_data[design_type][design_name] = dict()
                        
                    
                    sizes = "no_sizes"
                    if "sizes" in json_file_path:
                        sizes = "sizes"

                    total_time = data[design_name]["total_time"]
                    total_gates = data[design_name]["all_gates"]
                    seq_gate = data[design_name]["sequential_gates"]
                    print(total_gates)
                    print(total_time)
                    print(seq_gate)

                    overview_data[design_type][design_name][sizes] = dict()
                        
                    overview_data[design_type][design_name][sizes]
                    overview_data[design_type][design_name][sizes]["iterations"] = -1
                    for iteration_str in data[design_name]:
                        if "voting" in iteration_str:
                            iteration = int(iteration_str[iteration_str.find("_") + 1:])
                            nmi     = data[design_name][iteration_str]["result"]["NMI"]
                            purity  = data[design_name][iteration_str]["result"]["purity"]


                            if iteration > overview_data[design_type][design_name][sizes]["iterations"]:
                                overview_data[design_type][design_name][sizes]["iterations"] = iteration
                                overview_data[design_type][design_name][sizes]["nmi"] = nmi
                                overview_data[design_type][design_name][sizes]["purity"] = purity
                                overview_data[design_type][design_name][sizes]["total_time"] = total_time
                                overview_data[design_type][design_name][sizes]["total_gates"] = total_gates
                                overview_data[design_type][design_name][sizes]["seq_gates"] = seq_gate

    print(overview_data)

    # Generate data table
    with doc.create(Table(position='H')):
        with doc.create(LongTable("lrrrrrrrr", booktabs=True)) as data_table:
            data_table.add_hline()
            data_table.add_row([MultiRow(2, data="Design"), MultiRow(2, data="Synthesizer"), MultiRow(2, data="Sequential Gates"), MultiColumn(3, data="Automated"), MultiColumn(3, data="Defined Sizes")])
            data_table.add_row(["", "", "", "NMI", "Purity", "Run Time", "NMI", "Purity", "Run Time"])
            data_table.add_hline()
            data_table.add_hline()

            for design_type in sorted(overview_data.keys(), key=str.casefold):
                design_new = True
                for design_name in sorted(overview_data[design_type].keys(), key=str.casefold):
                    number_of_different_designs = len(overview_data[design_type])
                    synthesizer = design_name[design_name.rfind('_') + 1:]

                    if "no_sizes" in overview_data[design_type][design_name]:
                        #iterations  = overview_data[design_type][design_name]["no_sizes"]["iterations"]
                        nmi         = overview_data[design_type][design_name]["no_sizes"]["nmi"]
                        purity      = overview_data[design_type][design_name]["no_sizes"]["purity"]
                        total_time  = overview_data[design_type][design_name]["no_sizes"]["total_time"]
                        #total_gates = overview_data[design_type][design_name]["no_sizes"]["total_gates"]
                        seq_gates   = overview_data[design_type][design_name]["no_sizes"]["seq_gates"]
                        
                    else:
                        #iterations  = 88.88
                        nmi         = 88.88
                        purity      = 88.88
                        total_time  = 88.88
                        #total_gates = 8888
                        seq_gates   = 8888

                    if "sizes" in overview_data[design_type][design_name]:
                        #iterations_sizes  = overview_data[design_type][design_name]["sizes"]["iterations"]
                        nmi_sizes         = overview_data[design_type][design_name]["sizes"]["nmi"]
                        purity_sizes      = overview_data[design_type][design_name]["sizes"]["purity"]
                        total_time_sizes  = overview_data[design_type][design_name]["sizes"]["total_time"]
                        #total_gates_sizes = overview_data[design_type][design_name]["sizes"]["total_gates"]
                        seq_gates_sizes   = overview_data[design_type][design_name]["sizes"]["seq_gates"]
                    else:
                        #iterations_sizes  = 88.88
                        nmi_sizes         = 88.88
                        purity_sizes      = 88.88 
                        total_time_sizes  = 88.88
                        #total_gates_sizes = 8888
                        seq_gates_sizes   = 8888


                    if design_new:
                        data_table.add_row([(MultiRow(number_of_different_designs, data=design_type)), synthesizer, seq_gates, round_float(nmi), round_float(purity), round_float_1(total_time) + " s", round_float(nmi_sizes), round_float(purity_sizes), round_float_1(total_time_sizes) + " s"])
                        design_new = False
                    else:
                        data_table.add_row(['',synthesizer, seq_gates, round_float(nmi), round_float(purity), round_float_1(total_time) + " s", round_float(nmi_sizes), round_float(purity_sizes), round_float_1(total_time_sizes) + " s"])
                data_table.add_hline()



