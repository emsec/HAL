Search.setIndex({docnames:["base_plugin_interface","boolean_function","boolean_influence","core_utils","data_container","dataflow","endpoint","gate","gate_library","gate_library_manager","gate_type","graph_algorithm","grouping","gui","gui_plugin_interface","hal_py","index","module","net","netlist","netlist_factory","netlist_simulator","netlist_utils","netlist_writer_manager","path","plugin_manager","plugins"],envversion:{"sphinx.domains.c":1,"sphinx.domains.changeset":1,"sphinx.domains.cpp":1,"sphinx.domains.javascript":1,"sphinx.domains.math":2,"sphinx.domains.python":1,"sphinx.domains.rst":1,"sphinx.domains.std":1,"sphinx.ext.intersphinx":1,"sphinx.ext.todo":1,sphinx:55},filenames:["base_plugin_interface.rst","boolean_function.rst","boolean_influence.rst","core_utils.rst","data_container.rst","dataflow.rst","endpoint.rst","gate.rst","gate_library.rst","gate_library_manager.rst","gate_type.rst","graph_algorithm.rst","grouping.rst","gui.rst","gui_plugin_interface.rst","hal_py.rst","index.rst","module.rst","net.rst","netlist.rst","netlist_factory.rst","netlist_simulator.rst","netlist_utils.rst","netlist_writer_manager.rst","path.rst","plugin_manager.rst","plugins.rst"],objects:{"boolean_influence.BooleanInfluencePlugin":{get_boolean_influences_of_gate:[2,1,1,""],get_name:[2,1,1,""],get_version:[2,1,1,""]},"dataflow.DataflowPlugin":{execute:[5,1,1,""],get_name:[5,1,1,""],get_version:[5,1,1,""],name:[5,2,1,""],version:[5,2,1,""]},"graph_algorithm.GraphAlgorithmPlugin":{get_communities:[11,1,1,""],get_communities_fast_greedy:[11,1,1,""],get_communities_spinglass:[11,1,1,""],get_graph_cut:[11,1,1,""],get_name:[11,1,1,""],get_strongly_connected_components:[11,1,1,""],get_version:[11,1,1,""],name:[11,2,1,""],version:[11,2,1,""]},"hal_gui.GuiApi":{deselect:[13,1,1,""],deselectAllItems:[13,1,1,""],deselectGate:[13,1,1,""],deselectModule:[13,1,1,""],deselectNet:[13,1,1,""],getSelectedGateIds:[13,1,1,""],getSelectedGates:[13,1,1,""],getSelectedItemIds:[13,1,1,""],getSelectedItems:[13,1,1,""],getSelectedModuleIds:[13,1,1,""],getSelectedModules:[13,1,1,""],getSelectedNetIds:[13,1,1,""],getSelectedNets:[13,1,1,""],select:[13,1,1,""],selectGate:[13,1,1,""],selectModule:[13,1,1,""],selectNet:[13,1,1,""]},"hal_py.BasePluginInterface":{get_name:[0,1,1,""],get_version:[0,1,1,""],name:[0,2,1,""],version:[0,2,1,""]},"hal_py.BooleanFunction":{Value:[1,0,1,""],__and__:[1,1,1,""],__call__:[1,1,1,""],__eq__:[1,1,1,""],__iand__:[1,1,1,""],__init__:[1,1,1,""],__invert__:[1,1,1,""],__ior__:[1,1,1,""],__ixor__:[1,1,1,""],__ne__:[1,1,1,""],__or__:[1,1,1,""],__str__:[1,1,1,""],__xor__:[1,1,1,""],evaluate:[1,1,1,""],from_string:[1,3,1,""],get_dnf_clauses:[1,1,1,""],get_truth_table:[1,1,1,""],get_variables:[1,1,1,""],is_constant_one:[1,1,1,""],is_constant_zero:[1,1,1,""],is_dnf:[1,1,1,""],is_empty:[1,1,1,""],optimize:[1,1,1,""],optimize_constants:[1,1,1,""],substitute:[1,1,1,""],to_dnf:[1,1,1,""],variables:[1,2,1,""]},"hal_py.BooleanFunction.Value":{name:[1,2,1,""]},"hal_py.CoreUtils":{get_base_directory:[3,5,1,""],get_binary_directory:[3,5,1,""],get_default_log_directory:[3,5,1,""],get_gate_library_directories:[3,5,1,""],get_library_directory:[3,5,1,""],get_plugin_directories:[3,5,1,""],get_share_directory:[3,5,1,""],get_user_config_directory:[3,5,1,""],get_user_share_directory:[3,5,1,""]},"hal_py.DataContainer":{data:[4,2,1,""],delete_data:[4,1,1,""],get_data:[4,1,1,""],get_data_map:[4,1,1,""],set_data:[4,1,1,""]},"hal_py.Endpoint":{__init__:[6,1,1,""],gate:[6,2,1,""],get_gate:[6,1,1,""],get_net:[6,1,1,""],get_pin:[6,1,1,""],is_destination:[6,2,1,""],is_destination_pin:[6,1,1,""],is_source:[6,2,1,""],is_source_pin:[6,1,1,""],net:[6,2,1,""],pin:[6,2,1,""]},"hal_py.GUIPluginInterface":{exec:[14,1,1,""]},"hal_py.Gate":{add_boolean_function:[7,1,1,""],boolean_functions:[7,2,1,""],data:[7,2,1,""],delete_data:[7,1,1,""],fan_in_nets:[7,2,1,""],fan_out_nets:[7,2,1,""],get_boolean_function:[7,1,1,""],get_boolean_functions:[7,1,1,""],get_data:[7,1,1,""],get_data_map:[7,1,1,""],get_fan_in_endpoint:[7,1,1,""],get_fan_in_endpoints:[7,1,1,""],get_fan_in_net:[7,1,1,""],get_fan_in_nets:[7,1,1,""],get_fan_out_endpoint:[7,1,1,""],get_fan_out_endpoints:[7,1,1,""],get_fan_out_net:[7,1,1,""],get_fan_out_nets:[7,1,1,""],get_grouping:[7,1,1,""],get_id:[7,1,1,""],get_input_pins:[7,1,1,""],get_location:[7,1,1,""],get_location_x:[7,1,1,""],get_location_y:[7,1,1,""],get_module:[7,1,1,""],get_name:[7,1,1,""],get_netlist:[7,1,1,""],get_output_pins:[7,1,1,""],get_predecessor:[7,1,1,""],get_predecessors:[7,1,1,""],get_successor:[7,1,1,""],get_successors:[7,1,1,""],get_type:[7,1,1,""],get_unique_predecessors:[7,1,1,""],get_unique_successors:[7,1,1,""],has_location:[7,1,1,""],id:[7,2,1,""],input_pins:[7,2,1,""],is_gnd_gate:[7,1,1,""],is_vcc_gate:[7,1,1,""],location:[7,2,1,""],location_x:[7,2,1,""],location_y:[7,2,1,""],mark_gnd_gate:[7,1,1,""],mark_vcc_gate:[7,1,1,""],module:[7,2,1,""],name:[7,2,1,""],netlist:[7,2,1,""],output_pins:[7,2,1,""],predecessors:[7,2,1,""],set_data:[7,1,1,""],set_location:[7,1,1,""],set_location_x:[7,1,1,""],set_location_y:[7,1,1,""],set_name:[7,1,1,""],successors:[7,2,1,""],type:[7,2,1,""],unique_predecessors:[7,2,1,""],unique_successors:[7,2,1,""],unmark_gnd_gate:[7,1,1,""],unmark_vcc_gate:[7,1,1,""]},"hal_py.GateLibrary":{__init__:[8,1,1,""],add_include:[8,1,1,""],contains_gate_type:[8,1,1,""],contains_gate_type_by_name:[8,1,1,""],create_gate_type:[8,1,1,""],gate_types:[8,2,1,""],get_gate_type_by_name:[8,1,1,""],get_gate_types:[8,1,1,""],get_gnd_gate_types:[8,1,1,""],get_includes:[8,1,1,""],get_name:[8,1,1,""],get_path:[8,1,1,""],get_vcc_gate_types:[8,1,1,""],gnd_gate_types:[8,2,1,""],includes:[8,2,1,""],mark_gnd_gate_type:[8,1,1,""],mark_vcc_gate_type:[8,1,1,""],name:[8,2,1,""],path:[8,2,1,""],vcc_gate_types:[8,2,1,""]},"hal_py.GateType":{ClearPresetBehavior:[10,0,1,""],__init__:[10,1,1,""],add_boolean_function:[10,1,1,""],add_boolean_functions:[10,1,1,""],add_input_pin:[10,1,1,""],add_input_pins:[10,1,1,""],add_output_pin:[10,1,1,""],add_output_pins:[10,1,1,""],add_pin:[10,1,1,""],add_pins:[10,1,1,""],assign_pin_group:[10,1,1,""],assign_pin_type:[10,1,1,""],boolean_functions:[10,2,1,""],config_data_category:[10,2,1,""],config_data_identifier:[10,2,1,""],gate_library:[10,2,1,""],get_boolean_functions:[10,1,1,""],get_clear_preset_behavior:[10,1,1,""],get_config_data_category:[10,1,1,""],get_config_data_identifier:[10,1,1,""],get_gate_library:[10,1,1,""],get_id:[10,1,1,""],get_input_pins:[10,1,1,""],get_name:[10,1,1,""],get_output_pins:[10,1,1,""],get_pin_direction:[10,1,1,""],get_pin_directions:[10,1,1,""],get_pin_groups:[10,1,1,""],get_pin_of_group_at_index:[10,1,1,""],get_pin_type:[10,1,1,""],get_pin_types:[10,1,1,""],get_pins:[10,1,1,""],get_pins_of_direction:[10,1,1,""],get_pins_of_group:[10,1,1,""],get_pins_of_type:[10,1,1,""],get_properties:[10,1,1,""],has_property:[10,1,1,""],id:[10,2,1,""],input_pins:[10,2,1,""],is_lut_init_ascending:[10,1,1,""],lut_init_ascending:[10,2,1,""],name:[10,2,1,""],output_pins:[10,2,1,""],pin_directions:[10,2,1,""],pin_groups:[10,2,1,""],pin_types:[10,2,1,""],pins:[10,2,1,""],properties:[10,2,1,""],set_clear_preset_behavior:[10,1,1,""],set_config_data_category:[10,1,1,""],set_config_data_identifier:[10,1,1,""],set_lut_init_ascending:[10,1,1,""]},"hal_py.GateType.ClearPresetBehavior":{name:[10,2,1,""]},"hal_py.Grouping":{assign_gate:[12,1,1,""],assign_gate_by_id:[12,1,1,""],assign_module:[12,1,1,""],assign_module_by_id:[12,1,1,""],assign_net:[12,1,1,""],assign_net_by_id:[12,1,1,""],contains_gate:[12,1,1,""],contains_gate_by_id:[12,1,1,""],contains_module:[12,1,1,""],contains_module_by_id:[12,1,1,""],contains_net:[12,1,1,""],contains_net_by_id:[12,1,1,""],gate_ids:[12,2,1,""],gates:[12,2,1,""],get_gate_ids:[12,1,1,""],get_gates:[12,1,1,""],get_id:[12,1,1,""],get_module_ids:[12,1,1,""],get_modules:[12,1,1,""],get_name:[12,1,1,""],get_net_ids:[12,1,1,""],get_netlist:[12,1,1,""],get_nets:[12,1,1,""],id:[12,2,1,""],module_ids:[12,2,1,""],modules:[12,2,1,""],name:[12,2,1,""],net_ids:[12,2,1,""],netlist:[12,2,1,""],nets:[12,2,1,""],remove_gate:[12,1,1,""],remove_gate_by_id:[12,1,1,""],remove_module:[12,1,1,""],remove_module_by_id:[12,1,1,""],remove_net:[12,1,1,""],remove_net_by_id:[12,1,1,""],set_name:[12,1,1,""]},"hal_py.HalPath":{__init__:[24,1,1,""],__str__:[24,1,1,""]},"hal_py.Module":{assign_gate:[17,1,1,""],contains_gate:[17,1,1,""],contains_module:[17,1,1,""],data:[17,2,1,""],delete_data:[17,1,1,""],gates:[17,2,1,""],get_data:[17,1,1,""],get_data_map:[17,1,1,""],get_gate_by_id:[17,1,1,""],get_gates:[17,1,1,""],get_grouping:[17,1,1,""],get_id:[17,1,1,""],get_input_nets:[17,1,1,""],get_input_port_name:[17,1,1,""],get_input_port_names:[17,1,1,""],get_input_port_net:[17,1,1,""],get_internal_nets:[17,1,1,""],get_name:[17,1,1,""],get_netlist:[17,1,1,""],get_next_input_port_id:[17,1,1,""],get_next_output_port_id:[17,1,1,""],get_output_nets:[17,1,1,""],get_output_port_name:[17,1,1,""],get_output_port_names:[17,1,1,""],get_output_port_net:[17,1,1,""],get_parent_module:[17,1,1,""],get_submodules:[17,1,1,""],get_type:[17,1,1,""],id:[17,2,1,""],input_nets:[17,2,1,""],input_port_names:[17,2,1,""],internal_nets:[17,2,1,""],is_top_module:[17,1,1,""],name:[17,2,1,""],netlist:[17,2,1,""],next_input_port_id:[17,2,1,""],next_output_port_id:[17,2,1,""],output_nets:[17,2,1,""],output_port_names:[17,2,1,""],parent_module:[17,2,1,""],remove_gate:[17,1,1,""],set_data:[17,1,1,""],set_input_port_name:[17,1,1,""],set_name:[17,1,1,""],set_next_input_port_id:[17,1,1,""],set_next_output_port_id:[17,1,1,""],set_output_port_name:[17,1,1,""],set_parent_module:[17,1,1,""],set_type:[17,1,1,""],submodules:[17,2,1,""],top_module:[17,2,1,""],type:[17,2,1,""]},"hal_py.Net":{add_destination:[18,1,1,""],add_source:[18,1,1,""],data:[18,2,1,""],delete_data:[18,1,1,""],destinations:[18,2,1,""],get_data:[18,1,1,""],get_data_map:[18,1,1,""],get_destinations:[18,1,1,""],get_grouping:[18,1,1,""],get_id:[18,1,1,""],get_name:[18,1,1,""],get_netlist:[18,1,1,""],get_num_of_destinations:[18,1,1,""],get_num_of_sources:[18,1,1,""],get_source:[18,1,1,""],get_sources:[18,1,1,""],id:[18,2,1,""],is_a_destination:[18,1,1,""],is_a_source:[18,1,1,""],is_global_input_net:[18,1,1,""],is_global_output_net:[18,1,1,""],is_unrouted:[18,1,1,""],mark_global_input_net:[18,1,1,""],mark_global_output_net:[18,1,1,""],name:[18,2,1,""],netlist:[18,2,1,""],num_of_destinations:[18,2,1,""],num_of_sources:[18,2,1,""],remove_destination:[18,1,1,""],remove_source:[18,1,1,""],set_data:[18,1,1,""],set_name:[18,1,1,""],sources:[18,2,1,""],unmark_global_input_net:[18,1,1,""],unmark_global_output_net:[18,1,1,""]},"hal_py.Netlist":{clear_caches:[19,1,1,""],create_gate:[19,1,1,""],create_grouping:[19,1,1,""],create_module:[19,1,1,""],create_net:[19,1,1,""],delete_gate:[19,1,1,""],delete_grouping:[19,1,1,""],delete_module:[19,1,1,""],delete_net:[19,1,1,""],design_name:[19,2,1,""],device_name:[19,2,1,""],gate_library:[19,2,1,""],gates:[19,2,1,""],get_design_name:[19,1,1,""],get_device_name:[19,1,1,""],get_free_gate_ids:[19,1,1,""],get_free_grouping_ids:[19,1,1,""],get_free_module_ids:[19,1,1,""],get_free_net_ids:[19,1,1,""],get_gate_by_id:[19,1,1,""],get_gate_library:[19,1,1,""],get_gates:[19,1,1,""],get_global_input_nets:[19,1,1,""],get_global_output_nets:[19,1,1,""],get_gnd_gates:[19,1,1,""],get_grouping_by_id:[19,1,1,""],get_groupings:[19,1,1,""],get_id:[19,1,1,""],get_input_filename:[19,1,1,""],get_module_by_id:[19,1,1,""],get_modules:[19,1,1,""],get_net_by_id:[19,1,1,""],get_nets:[19,1,1,""],get_next_gate_id:[19,1,1,""],get_next_grouping_id:[19,1,1,""],get_next_module_id:[19,1,1,""],get_next_net_id:[19,1,1,""],get_top_module:[19,1,1,""],get_unique_gate_id:[19,1,1,""],get_unique_grouping_id:[19,1,1,""],get_unique_module_id:[19,1,1,""],get_unique_net_id:[19,1,1,""],get_used_gate_ids:[19,1,1,""],get_used_grouping_ids:[19,1,1,""],get_used_module_ids:[19,1,1,""],get_used_net_ids:[19,1,1,""],get_vcc_gates:[19,1,1,""],global_input_nets:[19,2,1,""],global_output_nets:[19,2,1,""],gnd_gates:[19,2,1,""],groupings:[19,2,1,""],id:[19,2,1,""],input_filename:[19,2,1,""],is_gate_in_netlist:[19,1,1,""],is_global_input_net:[19,1,1,""],is_global_output_net:[19,1,1,""],is_gnd_gate:[19,1,1,""],is_grouping_in_netlist:[19,1,1,""],is_module_in_netlist:[19,1,1,""],is_net_in_netlist:[19,1,1,""],is_vcc_gate:[19,1,1,""],mark_global_input_net:[19,1,1,""],mark_global_output_net:[19,1,1,""],mark_gnd_gate:[19,1,1,""],mark_vcc_gate:[19,1,1,""],modules:[19,2,1,""],nets:[19,2,1,""],set_design_name:[19,1,1,""],set_device_name:[19,1,1,""],set_free_gate_ids:[19,1,1,""],set_free_grouping_ids:[19,1,1,""],set_free_module_ids:[19,1,1,""],set_free_net_ids:[19,1,1,""],set_id:[19,1,1,""],set_input_filename:[19,1,1,""],set_next_gate_id:[19,1,1,""],set_next_grouping_id:[19,1,1,""],set_next_module_id:[19,1,1,""],set_next_net_id:[19,1,1,""],set_used_gate_ids:[19,1,1,""],set_used_grouping_ids:[19,1,1,""],set_used_module_ids:[19,1,1,""],set_used_net_ids:[19,1,1,""],top_module:[19,2,1,""],unmark_global_input_net:[19,1,1,""],unmark_global_output_net:[19,1,1,""],unmark_gnd_gate:[19,1,1,""],unmark_vcc_gate:[19,1,1,""],vcc_gates:[19,2,1,""]},"hal_py.NetlistFactory":{create_netlist:[20,5,1,""],load_netlist:[20,5,1,""],load_netlists:[20,5,1,""]},"hal_py.NetlistUtils":{copy_netlist:[22,5,1,""],get_common_inputs:[22,5,1,""],get_complex_gate_chain:[22,5,1,""],get_gate_chain:[22,5,1,""],get_nets_at_pins:[22,5,1,""],get_next_sequential_gates:[22,5,1,""],get_path:[22,5,1,""],get_subgraph_function:[22,5,1,""],remove_buffers:[22,5,1,""],remove_unused_lut_endpoints:[22,5,1,""],rename_luts_according_to_function:[22,5,1,""],replace_gate:[22,5,1,""]},"hal_py.NetlistWriterManager":{write:[23,5,1,""]},"hal_py.plugin_manager":{get_plugin_instance:[25,5,1,""],get_plugin_names:[25,5,1,""],load:[25,5,1,""],load_all_plugins:[25,5,1,""],unload:[25,5,1,""],unload_all_plugins:[25,5,1,""]},"netlist_simulator.Event":{__eq__:[21,1,1,""],__init__:[21,1,1,""],__lt__:[21,1,1,""],affected_net:[21,2,1,""],id:[21,2,1,""],new_value:[21,2,1,""],time:[21,2,1,""]},"netlist_simulator.NetlistSimulator":{add_clock_frequency:[21,1,1,""],add_clock_period:[21,1,1,""],add_gates:[21,1,1,""],generate_vcd:[21,1,1,""],get_gates:[21,1,1,""],get_input_nets:[21,1,1,""],get_output_nets:[21,1,1,""],get_simulation_state:[21,1,1,""],get_simulation_timeout:[21,1,1,""],load_initial_values:[21,1,1,""],load_initial_values_from_netlist:[21,1,1,""],reset:[21,1,1,""],set_input:[21,1,1,""],set_iteration_timeout:[21,1,1,""],set_simulation_state:[21,1,1,""],simulate:[21,1,1,""]},"netlist_simulator.NetlistSimulatorPlugin":{create_simulator:[21,1,1,""],get_name:[21,1,1,""],get_version:[21,1,1,""],name:[21,2,1,""],version:[21,2,1,""]},"netlist_simulator.SignalValue":{name:[21,2,1,""]},"netlist_simulator.Simulation":{__init__:[21,1,1,""],add_event:[21,1,1,""],get_events:[21,1,1,""],get_net_value:[21,1,1,""]},boolean_influence:{BooleanInfluencePlugin:[2,0,1,""]},dataflow:{DataflowPlugin:[5,0,1,""]},graph_algorithm:{GraphAlgorithmPlugin:[11,0,1,""]},hal_gui:{GuiApi:[13,0,1,""]},hal_py:{BasePluginInterface:[0,0,1,""],BooleanFunction:[1,0,1,""],CoreUtils:[3,4,0,"-"],DataContainer:[4,0,1,""],Endpoint:[6,0,1,""],GUIPluginInterface:[14,0,1,""],Gate:[7,0,1,""],GateLibrary:[8,0,1,""],GateLibraryManager:[9,2,1,""],GateType:[10,0,1,""],Grouping:[12,0,1,""],HalPath:[24,0,1,""],Module:[17,0,1,""],Net:[18,0,1,""],Netlist:[19,0,1,""],NetlistFactory:[20,4,0,"-"],NetlistUtils:[22,4,0,"-"],NetlistWriterManager:[23,4,0,"-"],plugin_manager:[25,4,0,"-"]},netlist_simulator:{Event:[21,0,1,""],NetlistSimulator:[21,0,1,""],NetlistSimulatorPlugin:[21,0,1,""],SignalValue:[21,0,1,""],Simulation:[21,0,1,""]}},objnames:{"0":["py","class","Python class"],"1":["py","method","Python method"],"2":["py","attribute","Python attribute"],"3":["py","staticmethod","Python static method"],"4":["py","module","Python module"],"5":["py","function","Python function"]},objtypes:{"0":"py:class","1":"py:method","2":"py:attribute","3":"py:staticmethod","4":"py:module","5":"py:function"},terms:{"1st":7,"2nd":7,"boolean":[7,8,10,15,22,26],"case":[8,10,21,22],"class":[0,1,2,4,5,6,7,8,10,11,12,13,14,17,18,19,21,24],"default":[3,10,19,22,25],"float":2,"function":[2,3,7,8,10,13,15,18,19,20,22,24,25],"int":[5,7,10,11,12,13,17,18,19,21,22],"new":[1,7,8,12,17,18,19,20,21,22],"return":[0,1,2,3,4,5,6,7,8,10,11,12,13,14,17,18,19,20,21,22,23,25],"static":[1,3],"switch":5,"true":[1,4,6,7,8,10,12,13,14,17,18,19,21,22,23,25],"try":3,AND:1,For:[10,17,22],IDs:[11,12,19,22],NOT:1,ONE:[1,21],The:[0,1,2,3,4,5,6,7,8,10,11,12,13,14,17,18,19,20,21,22,23,25],Use:[3,18,21],Useful:21,__and__:1,__call__:1,__eq__:[1,21],__iand__:1,__init__:[1,6,8,10,21,24],__invert__:1,__ior__:1,__ixor__:1,__lt__:21,__ne__:1,__or__:1,__str__:[1,24],__xor__:1,abort:[18,21],about:[7,10,18,19],abov:22,accur:[6,10],act:12,activ:10,add:[4,7,8,10,17,18,19,21],add_boolean_funct:[7,10],add_clock_frequ:21,add_clock_period:21,add_destin:18,add_ev:21,add_gat:21,add_includ:8,add_input_pin:10,add_output_pin:10,add_pin:10,add_sourc:18,added:[10,13,17,21],adding:22,advanc:21,affect:21,affected_net:21,afterward:2,algorithm:[1,26],alia:9,all:[1,4,7,8,10,12,13,17,18,19,21,22,23,25],allow:[12,22],alphabet:1,alreadi:12,also:[1,17],alwai:1,analysi:26,analyz:22,analyze_input:22,ani:[12,22],anoth:[1,12,17],api:26,appear:2,appli:[1,12,17,19,21],applic:19,arg0:[1,10,21,24],arg:[1,13,18,19,20,22,24],ascend:10,assign:[7,10,12,17,18,19],assign_g:[12,17],assign_gate_by_id:12,assign_modul:12,assign_module_by_id:12,assign_net:12,assign_net_by_id:12,assign_pin_group:10,assign_pin_typ:10,associ:[4,6,7,10,12,17,18,19,21,25],automat:[17,19],avail:[7,23],bahavior:10,base:[3,7,15,17,18,22],baseplugininterfac:[0,25],basic:25,been:[10,19],befor:[1,10,13,21,22],beforehand:10,behavior:10,being:11,belong:10,better:22,between:21,binari:3,bit:[1,10],bool:[1,4,5,6,7,8,10,12,13,14,17,18,19,21,22,23,25],boolean_funct:[7,10],boolean_influ:2,booleanfunct:[1,7,10,22],booleaninfluenceplugin:2,both:[1,10,21],bracket:1,buffer:22,build:2,bunction:1,cach:[19,22],call:[10,14,19,25],callabl:[7,12,17,18,19,22],camera:13,can:[1,7,11,12,17,18,19,21,22],candid:22,carri:21,categori:[4,7,10,17,18],caus:21,cell:11,chain:22,chain_typ:22,chang:[10,17],channel:[4,7,17,18],check:[1,6,7,8,10,12,17,18,19],classifi:22,claus:1,clear:[10,13,19],clear_cach:19,clear_current_select:13,clearpresetbehavior:10,clock:21,clock_net:21,cluster:11,code:2,collect:[8,12],combin:[1,22],common:22,commun:11,compil:2,compon:11,compris:[1,6],condit:[12,17,19],config:3,config_data_categori:10,config_data_identifi:10,configur:[3,10],connect:[6,7,11,12,22],consid:[21,22],constant:[1,7],construct:[1,8,21],contain:[1,3,7,8,10,12,15,17,18,19],contains_g:[12,17],contains_gate_by_id:12,contains_gate_typ:8,contains_gate_type_by_nam:8,contains_modul:[12,17],contains_module_by_id:12,contains_net:12,contains_net_by_id:12,content:16,contrast:12,control:21,convert:1,coordin:[7,19],copi:22,copy_netlist:22,core:16,coreutil:3,correctli:22,correspond:[8,17],cp1:10,cp2:10,creat:[5,8,19,20,21],create_g:19,create_gate_typ:8,create_group:19,create_modul:19,create_net:19,create_netlist:20,create_simul:21,current:[3,13,19,21],custom:21,cut:11,cycl:21,dana:26,data:[2,7,10,15,17,18],data_typ:[4,7,17,18],datacontain:[4,7,17,18],dataflow:26,dataflowplugin:5,dataport:2,deep:22,defin:[7,10,22],delet:[4,7,17,18],delete_data:[4,7,17,18],delete_g:19,delete_group:19,delete_modul:19,delete_net:19,deprec:18,depth:11,describ:[1,8,10],deselect:13,deselectallitem:13,deselectg:13,deselectmodul:13,deselectnet:13,design:[12,19],design_nam:19,desir:[22,25],destin:[6,17,18,21],detect:22,determin:[3,13],devic:19,device_nam:19,dict:[1,2,4,7,8,10,11,17,18,21,22],dictionari:[7,17],differ:22,direct:[7,10,17],directori:[3,25],directory_nam:25,disabl:21,disjunct:1,dispatch:23,distanc:11,divice_nam:19,dnf:1,doe:[10,12,21],draw_graph:5,driven:[18,21],durat:21,dure:[1,12,21],dynam:22,each:[1,10,11,12,17,20],edg:21,effic:2,either:17,element:[18,21],empti:[1,7,18,20,22,25],enabl:[17,22],end:21,end_tim:21,endpoint:[7,15,18,22],entir:22,entiti:[4,12],entri:[4,7,17,18],environ:3,equal:[1,21],error:[1,18,19],especi:21,eval:2,evalu:[1,22],event:[4,7,17,18,21],ever:19,everi:22,exampl:[1,7],exec:14,execut:[2,3,5],exist:[4,7,10,13,17,18],expect:22,explicit:[4,7,17,18],explicitli:19,explor:12,exponenti:1,expr:2,express:1,extens:22,extract:1,factori:15,fail:12,fals:[1,4,6,7,8,10,12,13,17,18,19,21,22,23,25],fan:[7,22],fan_in_net:7,fan_out_net:7,fast:11,faster:18,file:[3,8,19,20,21,23,25],file_path:[23,25],filenam:19,fill:22,filter:[7,12,17,18,19,22],find:[3,10,22],first:[1,7,17,18,22],fit:13,fix:22,flip:[2,11],flop:[2,11],follow:19,forc:[4,7,12,17,18],form:[1,22],found:[1,17,22],fpga:21,framework:25,free:17,freed:19,frequenc:21,from:[1,2,4,5,7,8,10,11,12,17,18,19,20,21,22],from_str:1,fulfil:22,func:7,further:11,gate:[2,3,5,6,11,12,13,15,17,18,19,20,21,22],gate_id:[12,13,19],gate_librari:[10,19,20],gate_library_fil:20,gate_typ:[7,8,19],gatelibrari:[8,10,19,20],gatelibrarymanag:9,gatetyp:[7,8,10,19,22],gatetypeproperti:[8,10,22],gener:[2,5,14,21,22],generate_vcd:21,gerner:21,get:[0,1,2,3,4,5,6,7,8,10,11,12,13,17,18,19,21,22,25],get_base_directori:3,get_binary_directori:3,get_boolean_funct:[7,10],get_boolean_influences_of_g:2,get_clear_preset_behavior:10,get_common_input:22,get_commun:11,get_communities_fast_greedi:11,get_communities_spinglass:11,get_complex_gate_chain:22,get_config_data_categori:10,get_config_data_identifi:10,get_data:[4,7,17,18],get_data_map:[4,7,17,18],get_default_log_directori:3,get_design_nam:19,get_destin:18,get_device_nam:19,get_dnf_claus:1,get_ev:21,get_fan_in_endpoint:7,get_fan_in_net:7,get_fan_out_endpoint:7,get_fan_out_net:7,get_free_gate_id:19,get_free_grouping_id:19,get_free_module_id:19,get_free_net_id:19,get_gat:[6,11,12,17,19,21],get_gate_by_id:[17,19],get_gate_chain:22,get_gate_id:12,get_gate_librari:[10,19],get_gate_library_directori:3,get_gate_typ:8,get_gate_type_by_nam:8,get_global_input_net:19,get_global_output_net:19,get_gnd_gat:19,get_gnd_gate_typ:8,get_graph_cut:11,get_group:[7,17,18,19],get_grouping_by_id:19,get_id:[7,10,12,17,18,19],get_includ:8,get_input_filenam:19,get_input_net:[17,21],get_input_pin:[7,10],get_input_port_nam:17,get_input_port_net:17,get_internal_net:17,get_library_directori:3,get_loc:7,get_location_i:7,get_location_x:7,get_modul:[7,12,19],get_module_by_id:19,get_module_id:12,get_nam:[0,2,5,7,8,10,11,12,17,18,21],get_net:[6,12,19],get_net_by_id:19,get_net_id:12,get_net_valu:21,get_netlist:[7,12,17,18],get_nets_at_pin:22,get_next_gate_id:19,get_next_grouping_id:19,get_next_input_port_id:17,get_next_module_id:19,get_next_net_id:19,get_next_output_port_id:17,get_next_sequential_g:22,get_num_of_destin:18,get_num_of_sourc:18,get_output_net:[17,21],get_output_pin:[7,10],get_output_port_nam:17,get_output_port_net:17,get_parent_modul:17,get_path:[8,22],get_pin:[6,10],get_pin_direct:10,get_pin_group:10,get_pin_of_group_at_index:10,get_pin_typ:10,get_pins_of_direct:10,get_pins_of_group:10,get_pins_of_typ:10,get_plugin_directori:3,get_plugin_inst:25,get_plugin_nam:25,get_predecessor:7,get_properti:10,get_share_directori:3,get_simulation_st:21,get_simulation_timeout:21,get_sourc:18,get_strongly_connected_compon:11,get_subgraph_funct:22,get_submodul:17,get_successor:[7,22],get_top_modul:19,get_truth_t:1,get_typ:[7,17],get_unique_gate_id:19,get_unique_grouping_id:19,get_unique_module_id:19,get_unique_net_id:19,get_unique_predecessor:7,get_unique_successor:7,get_used_gate_id:19,get_used_grouping_id:19,get_used_module_id:19,get_used_net_id:19,get_user_config_directori:3,get_user_share_directori:3,get_vari:1,get_vcc_gat:19,get_vcc_gate_typ:8,get_vers:[0,2,5,11,21],getselectedg:13,getselectedgateid:13,getselecteditem:13,getselecteditemid:13,getselectedmodul:13,getselectedmoduleid:13,getselectednet:13,getselectednetid:13,given:[1,2,4,7,8,10,17,18,19,20,21,22,23],global:[7,17,18,19,21],global_input_net:19,global_output_net:19,gnd:[7,8,19],gnd_gate:19,gnd_gate_typ:8,graph:[5,13,26],graph_algorithm:11,graphalgorithmplugin:11,greedi:11,group:[5,7,10,15,17,18,19,22],grouping_id:19,gui:[4,7,15,17,18,26],guiapi:13,guiplugininterfac:14,hal:[3,6,7,10,16,17,18,19,20,21,22,23,25],hal_base_path:3,hal_fil:20,hal_gui:13,hal_path:[3,8,19,20,21,23,25],hal_pi:[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,17,18,19,20,21,22,23,24,25],halpath:[3,8,19,20,21,23,24,25],halpi:2,handl:[1,10,21],happen:21,has:[10,17,18],has_loc:7,has_properti:10,have:[10,19,21],hdl:3,hdl_file:20,help:[6,10],hertz:21,hierarch:[12,17],high:[5,21],highest:19,hold:4,home:3,howev:22,ident:22,identifi:[5,10],ids:[13,19],igraph:11,imped:21,implement:22,improv:22,inc:8,includ:[1,7,8,10,17,18,19,21,22],index:[10,11,16],indic:10,indirect:17,infinit:21,influenc:[21,22,26],info:[4,7,17,18],inform:[7,10,18,19],initi:[6,10,21,22,25],input:[1,2,6,7,10,17,18,19,21,22],input_filenam:19,input_net:17,input_pin:[7,10,22],input_port_nam:17,inspect:21,instal:3,instanc:[1,8,21],instead:18,intend:1,interconnect:22,interfac:[15,25],intern:[10,17,19,21],internal_net:17,interpret:[1,3],invalid:[10,19],invert:10,is_a_destin:18,is_a_sourc:18,is_constant_on:1,is_constant_zero:1,is_destin:6,is_destination_pin:6,is_dnf:1,is_empti:1,is_gate_in_netlist:19,is_global_input_net:[18,19],is_global_output_net:[18,19],is_gnd_gat:[7,19],is_grouping_in_netlist:19,is_input:22,is_lut_init_ascend:10,is_module_in_netlist:19,is_net_in_netlist:19,is_sourc:6,is_source_pin:6,is_top_modul:17,is_unrout:18,is_vcc_gat:[7,19],item:13,iter:21,its:[3,7,8,10,17,18,19,22,25],itself:22,keep:23,kei:[4,7,17,18],known:1,kwarg:[1,13,18,19,20,22,24],lambda:[7,12,17,18,19,22],last:22,layout:7,least:[1,17],leav:[7,18],len:18,level:[4,5,7,17,18],libnetlist_simul:21,librari:[3,10,15,19,20],like:7,limit:11,list:[1,3,5,7,8,10,11,12,13,17,18,19,20,21,22,25],load:[20,21,25],load_all_plugin:25,load_initial_valu:21,load_initial_values_from_netlist:21,load_netlist:20,local:3,locat:[7,23],location_i:7,location_x:7,log:[3,4,7,17,18],log_with_info_level:[4,7,17,18],logic:[1,21,22],look:22,loop:21,loos:17,lut:[10,22],lut_init_ascend:10,mai:[4,7,12,17,18,21,22],make:[10,22],manag:15,map:[2,7,17,21],mark:[7,8,18,19],mark_global_input_net:[18,19],mark_global_output_net:[18,19],mark_gnd_gat:[7,19],mark_gnd_gate_typ:8,mark_vcc_gat:[7,19],mark_vcc_gate_typ:8,match:[1,12,17,19,20],maximum:21,mccluskei:1,member:[1,10,21],memori:11,might:1,mnetid:13,modifi:13,modul:[7,12,13,15,16,19,22],module_id:[12,13,19],most:1,move:17,multi:18,multipl:[7,10],must:[1,10,22],name:[0,1,5,6,7,8,10,11,12,17,18,19,21,22,25],navigate_to_select:13,need:12,neg:7,negat:[1,10],net:[2,6,7,12,13,15,17,19,21,22],net_id:[12,13,19],netlist:[4,5,7,8,11,12,14,15,17,18,26],netlist_simul:21,netlistfactori:20,netlistsimul:21,netlistsimulatorplugin:21,netlistutil:22,netlistwritermanag:23,new_par:17,new_valu:21,new_variable_nam:1,next:[17,19],next_input_port_id:17,next_output_port_id:17,none:[1,7,8,10,12,13,17,18,19,20,21,22,24],normal:1,nullptr:19,num_of_destin:18,num_of_sourc:18,number:[1,11,18,21,22],object:[3,8,14],off:5,old:[1,17,22],old_variable_nam:1,omit:22,one:[1,7,12,17,21,22],onli:[7,12,17,19,21],only_custom_funct:7,oper:[1,5,11],optim:1,optimize_const:1,option:22,order:[1,10,22],ordered_vari:1,other:[17,21,22],otherwis:[1,3,4,6,7,8,10,12,17,18,19,20,21,22,23,25],out:[7,18,22],output:[1,6,7,10,17,18,19,21,22,23],output_net:17,output_path:5,output_pin:[7,10,22],output_port_nam:17,outsid:[17,21],overload:[1,13,18,19,20,22,24],overwrit:[4,7,12,17,18],page:16,pair:[1,22],param:[1,7,13,18,19,20,22],paramet:[1,2,3,4,5,7,8,10,11,12,14,17,18,19,20,21,22,23,25],parent:[3,17,19],parent_modul:17,pars:[1,8],part:[1,8,21],pass:13,path:[3,5,8,15,19,20,21,22,23,25],perform:22,period:21,physic:7,picosecond:21,pin:[6,7,8,10,18,22],pin_direct:10,pin_group:10,pin_map:22,pin_nam:10,pin_typ:10,pindirect:10,pintyp:10,place:1,plain:1,plugin:[3,5,11,15,16,21],plugin_manag:25,plugin_nam:25,point:[11,21],port:[7,17],port_nam:17,posit:[7,13],possibl:1,potenti:[7,18],pre:20,preced:1,predeccessor:22,predecessor:[7,22],present:22,preset:10,previou:[12,13,17],previous:19,print:18,priorit:5,process:[17,21],properti:[8,10,22],provid:[3,21,22],python3:3,quin:1,reach:22,recommend:22,recurs:[17,22],recus:17,reflect:22,regard:[7,17],regist:[5,19],rel:3,relat:12,releas:25,relev:21,remov:[1,12,17,18,19,21,22],remove_buff:22,remove_destin:18,remove_g:[12,17],remove_gate_by_id:12,remove_modul:12,remove_module_by_id:12,remove_net:12,remove_net_by_id:12,remove_sourc:18,remove_unknown_vari:1,remove_unused_lut_endpoint:22,renam:[1,22],rename_luts_according_to_funct:22,repeat:22,replac:[1,22],replace_g:22,repres:[1,7,11,19,21],represent:1,requir:[8,19,22],reserv:19,reset:[7,21],resourc:25,respect:[6,7,22,23],ressourc:25,result:[1,12,17,19,22],rise:21,rtype:[1,18,19,20,22],run:[11,14],runtim:1,rytp:10,said:2,same:10,scc:11,scroll:13,search:[3,16,17,25],see:[6,10],select:13,selectg:13,selectmodul:13,selectnet:13,self:[0,1,2,4,5,6,7,8,10,11,12,13,14,17,18,19,21,24],sequenc:22,sequenti:[21,22],set:[7,8,10,11,12,17,18,19,21,22,25],set_clear_preset_behavior:10,set_config_data_categori:10,set_config_data_identifi:10,set_data:[4,7,17,18],set_design_nam:19,set_device_nam:19,set_free_gate_id:19,set_free_grouping_id:19,set_free_module_id:19,set_free_net_id:19,set_id:19,set_input:21,set_input_filenam:19,set_input_port_nam:17,set_iteration_timeout:21,set_loc:7,set_location_i:7,set_location_x:7,set_lut_init_ascend:10,set_nam:[7,12,17,18],set_next_gate_id:19,set_next_grouping_id:19,set_next_input_port_id:17,set_next_module_id:19,set_next_net_id:19,set_next_output_port_id:17,set_output_port_nam:17,set_parent_modul:17,set_simulation_st:21,set_typ:17,set_used_gate_id:19,set_used_grouping_id:19,set_used_module_id:19,set_used_net_id:19,shall:22,share:3,should:[5,22],show:13,signal:21,signalvalu:21,signatur:[6,10],signific:1,simpl:22,simul:26,sinc:[1,18,19,21],singl:[1,12,21,25],size:5,some:17,sort:[17,21],sourc:[3,6,17,18],source_fil:3,spare:19,specif:[7,11,21,22],specifi:[4,7,8,10,11,12,17,18,19,20,21,22,25],spin:11,spinglass:11,ssc:11,start:[5,11,21,22],start_at_zero:21,start_gat:22,start_tim:21,startup:21,state:[10,21],std:10,stop:22,stop_properti:22,store:[4,7,12,17,18],str:[0,1,2,4,5,6,7,8,10,11,12,17,18,19,20,21,22,24,25],string:[1,10],strongli:11,subgraph:22,subgraph_g:22,submodul:17,subset:22,substitut:1,succ:19,success:[4,7,8,10,12,14,17,18,19,20,21,22,23,25],successor:[7,22],suppli:[1,7,18],support:[1,21],sure:10,tabl:1,take:21,target:[19,22],target_typ:22,task:23,temporarili:12,term:1,termin:11,terminal_gate_typ:11,test:21,than:18,therefor:17,theshold:22,thi:[1,7,8,10,12,17,18,19,21,22],threshold:22,through:22,thu:1,time:[10,17,21],timefram:21,timeout:21,to_dnf:1,toggl:10,top:[17,19],top_modul:[17,19],trace:[4,7,17,18],track:23,translat:2,travers:22,treat:21,tri:5,truth:1,tupl:[1,4,7,10,13,17,18],turn:5,two:[1,21],type:[0,1,2,3,4,5,6,7,8,11,12,13,14,15,17,18,19,20,21,22,23,25],typic:19,undef:10,undefin:[1,21],underli:19,unequ:1,uniqu:[7,10,12,17,18,19,21,25],unique_predecessor:7,unique_successor:7,unix:3,unknown:[7,21],unload:25,unload_all_plugin:25,unmark:[7,18,19],unmark_global_input_net:[18,19],unmark_global_output_net:[18,19],unmark_gnd_g:[7,19],unmark_vcc_g:[7,19],unrout:18,unstructur:12,until:22,usag:22,use:[3,22],used:[1,2,7,10,19,22,23],user:[3,10,22],using:[1,7,20,22],usual:7,util:[1,15],valid:7,valu:[1,4,7,10,17,18,19,21,22],variabl:[1,3,22],variable_nam:1,variant:22,vcc:[7,8,19],vcc_gate:19,vcc_gate_typ:8,vcd:21,vector:[1,3,17],version:[0,5,11,18,21,22],vhdl:8,via:7,view:13,wai:12,walk:1,warn:[1,18],well:[10,17,19],were:7,when:[1,10,21,22],whenev:1,where:[3,5],whether:[1,6,7,8,10,12,17,18,19,21],which:[1,2,7,10,11,13,17,18,22],which_pin:7,wire:21,within:[8,12,17,19,21,22,23],word:22,write:[21,23],writer:15,written:5,xor:1,zero:[1,21],zoom:13},titles:["Base Plugin Interface","Boolean Function","Boolean Influence","Core Utils","Data Container","Dataflow Analysis (DANA)","Endpoint","Gate","Gate Library","Gate Library Manager","Gate Type","Graph Algorithms","Grouping","GUI API","GUI Plugin Interface","HAL Core Documentation","hal_py Documentation","Module","Net","Netlist","Netlist Factory","Netlist Simulator","Netlist Utils","Netlist Writer Manager","HAL Path","Plugin Manager","HAL Plugins Documentation"],titleterms:{"boolean":[1,2],"function":1,algorithm:11,analysi:5,api:13,base:0,contain:4,core:[3,15],dana:5,data:4,dataflow:5,document:[15,16,26],endpoint:6,factori:20,gate:[7,8,9,10],graph:11,group:12,gui:[13,14],hal:[15,24,26],hal_pi:16,indic:16,influenc:2,interfac:[0,14],librari:[8,9],manag:[9,23,25],modul:17,net:18,netlist:[19,20,21,22,23],path:24,plugin:[0,14,25,26],simul:21,tabl:16,type:10,util:[3,22],writer:23}})