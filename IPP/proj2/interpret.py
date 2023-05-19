
from enum import IntEnum, Enum
import re, argparse as ag, os as o , sys as s, xml.etree.ElementTree as ET

import err as err, Structs, commands as c, stats

class ValueType(Enum):
    NIL = "NIL"
    VALUE = 0
    TYPE = 1

FLAG = False
def debug(string):
    if(FLAG):
        print(string)

sorted_inst = []

def print_stats(arguments ,stats):

    if arguments.argsLine.stats != None:
        try:
            stats_file = open(arguments.argsLine.stats, "w")
        except:
            exit(err.ErrorHandling.paramsErr)

        order = []
        for arg in s.argv[1:]:
            if arg.startswith('-'):
                order.append(arg)
            else:
                order.append(f"'{arg}'")

        for arg in order:
            if arg == "--vars":
                stats_file.write(str(stats.give_max_vars()) + "\n")
            elif arg == "--insts":
                stats_file.write(str(stats.give_num_of_instructions()) + "\n")
            elif arg == "--hot":
                stats_file.write(str(stats.give_hot())+ "\n")
            elif arg == "--frequent":
                stats_file.write(str(stats.give_freq()) + "\n")
            elif arg == "--print":
                stats_file.write(str(arguments.argsLine.pr) + "\n")
            elif arg == "--eol":
                stats_file.write("\n")

class ProcessArgs:

    def __init__(self) -> None:
        self.input_to_execute = None
        self.input_to_read = None
        self.argsLine = None
        self.parse = None
        self.Ex = None    

    def parseArguments(self) -> None:
        self.parse = ag.ArgumentParser(description="IPPcode2023 interpret", add_help=False)
        self.parse.add_argument("-h", "--help", action="store_true")
        self.parse.add_argument("-s", "--source", help="program input file", action="store", dest="src")
        self.parse.add_argument("-i", "--input", help="input for program", action="store", dest="input")
        self.parse.add_argument("--stats", help="write statistics", action="store", dest="stats")
        self.parse.add_argument("--vars", help="max count of vars", action="store_true")
        self.parse.add_argument("--hot", help="writes order of the most used instruction", action="store_true")
        self.parse.add_argument("--print", help="writes string", action="store", dest="pr")
        self.parse.add_argument("--insts", help="count of instructions", action="store_true")
        self.parse.add_argument("--frequent", help="count of instructions", action="store_true")
        self.parse.add_argument("--eol", help="prints new line", action="store_true")

        self.argsLine = self.parse.parse_args()


                             
        if self.argsLine.stats is None and (self.argsLine.vars or self.argsLine.hot or self.argsLine.insts or self.argsLine.pr):
            debug("chyba zadani argumentu u statistiky")

            exit(err.ErrorHandling.paramsErr)

        if self.argsLine.help:
            if(len(s.argv) == 2):
                self.parse.print_help()
                debug("HELP PRINTED")
                exit(err.ErrorHandling.OK)
            else:
                debug("ARGUMENTS ERR")
                exit(err.ErrorHandling.paramsErr)

    

        if self.argsLine.src is None and self.argsLine.input is None:
            isInput = False
            isSource = False
        elif self.argsLine.src != None and self.argsLine.input == None:
            self.input_to_execute = self.argsLine.src
            self.input_to_read = s.stdin
            self.sourceBool = True
            isInput = False
            isSource = True
        
        elif self.argsLine.src == None and self.argsLine.input != None:
            self.input_to_read = open(self.argsLine.input, "r")
            self.input_to_execute = s.stdin
            isInput = True
            isSource = False
        elif self.argsLine.src != None and self.argsLine.input != None:
            self.input_to_read = open(self.argsLine.input, "r")
            self.input_to_execute = self.argsLine.src
            isInput = True
            isSource=  True
    
        try:
            if isSource:
                self.Ex = o.path.exists(self.argsLine.src)
                if not self.Ex:
                    raise err.CustomException("No source", err.ErrorHandling.stdinErr)
            if isInput:
                self.ex = o.path.exists(self.argsLine.input)
                if not self.Ex:
                    raise err.CustomException("No source", err.ErrorHandling.stdinErr)
        except err.CustomException as e:
            err.CustomException.print_error()
            exit(e.error_code)

class InstructOnLineCheck:
    argdictionary = dict()

    def __init__(self, count_of_local_frames, one_instruction, is_temporary_frame, dict_of_variables):
        self.count_of_local_frames = count_of_local_frames
        self.one_instruction = one_instruction
        self.arg = []
        self.dict_of_variables = dict_of_variables
        self.is_temporary_frame = is_temporary_frame
        self.check_operand_arguments()

    def check_operand_arguments(self):
        self.arg[:] = sorted( self.one_instruction, key=lambda x: x.tag)
        if not  self.one_instruction.get("opcode").upper() in Structs.instruction_num_of_arguments[len( self.one_instruction)]:
            debug("neznama instrukce")
            exit(32)

        args = sorted( self.one_instruction, key=lambda x: x.tag) 
        for i, argG in enumerate(args[:3], start=1):
            self.argdictionary[i] =  {"type":argG.get("type").upper(), "value":argG.text}
            self.argument_check(i, argG.get("type").upper(), argG.text) 

    def argument_check(self, number, str, value):
        if(self.type_check(Structs.instruction_formats[ self.one_instruction.get("opcode").upper()][number - 1], str)):
            debug("chyba v typechecku")
            exit(err.ErrorHandling.badOperandErr)
        
        self.check_frame(value, str, number)
        self.check_conversion(value, str, number)
        self.delete_escape_seq(value, str, number)
        
        if  self.argdictionary[number]["value"]  == None and  self.one_instruction.get("opcode").upper() != "TYPE": 
            debug("neznamy typ")
            exit(56)

    def delete_escape_seq(self, value, string_value, number):
        if string_value != "STRING":
            return

        if value is None:
            value = ""
        else:
            value = re.sub(r"\\\d{3}", lambda match: chr(int(match.group(0)[1:])), value) # swap from "\\065\\066" to "AB"

        self.argdictionary[number] = {"type": string_value, "value": value}


    def check_conversion(self, value, str, number):
        if str == "INT":
            try:
                self.argdictionary[number] =  {"type": str,"value": int((value))}
            except:
                debug("CHYBA CHECK CONVERSION")
                exit(err.ErrorHandling.unexpectedStructureErr)
        elif str == "BOOL":
            debug("BOOOL")
            if value == True or value == "true":
                self.argdictionary[number] =  {"type": str,"value": True}
            else:
                self.argdictionary[number] =  {"type": str,"value": False}
        elif str == "NIL":
            debug("jsem v check_conversion NIL")
            self.argdictionary[number] =  {"type": str,"value": ValueType.NIL.value}
        elif str == "FLOAT":
            try:
                self.argdictionary[number] =  {"type": str,"value": float.fromhex(value)}
            except:
                debug("CHYBA V CHECK CONVERSION")
                exit(err.ErrorHandling.unexpectedStructureErr)
        else:
            return

    def check_frame(self, value, str, count):
        try:
            if str == "VAR" and not  self.one_instruction.get("opcode").upper() == "DEFVAR":
                if value.startswith("TF") and not self.is_temporary_frame:
                    raise err.CustomException("frame ",55)
                elif value.startswith("LF") and self.count_of_local_frames == 0:
                    raise err.CustomException("frame",55)
                elif not value in self.dict_of_variables:
                    raise err.CustomException("variable does not exists",54)
                elif not count == 1:
                    var_value, var_type = self.dict_of_variables[value]
                    self.argdictionary[count] = {"type": var_type, "value": var_value}
        except err.CustomException as e:
            err.CustomException.print_error()
            exit(e.error_code)

    def type_check(self, typeR, typeT):
        if typeR == typeT or typeR == "VAR":
            return False
        elif typeR == "SYM":
            if typeT == "VAR" or typeT == "INT" or typeT == "STRING" or typeT == "BOOL" or typeT == "NIL" or typeT == "FLOAT":
                return False
        elif typeR == "LABEL" and typeT == "LABEL":
            return False
        elif typeR == "TYPE":
            if typeT == "INT" or typeT == "STRING" or typeT == "BOOL" or typeT == "FLOAT":
                return False
        return True

class ParseT:
    
    sorted_inst = []
    def __init__(self, XML_tree_to_be_parsed, stats, label_positions) -> None:
        self.statts = stats
        self.label_positions = label_positions
        if o.stat(XML_tree_to_be_parsed).st_size == 0:
            debug("empty file here")
            exit(err.ErrorHandling.OK)
        try:
            self.tree = ET.parse(XML_tree_to_be_parsed)
        except ET.ParseError:
            debug("no path")
            err.CustomException.print_error()
            exit(err.ErrorHandling.stdinXMLBadformatErr)
        self.XML_root = self.tree.getroot()
    
    def check_xml_and_process_instructions(self) -> None:

        try:
            # Iterate over each instruction in the XML document
            for instruction in self.XML_root:
                if instruction:
                    if "order" in instruction.attrib and "opcode" in instruction.attrib:
                        self.statts.hot({"key":instruction.attrib["opcode"] ,"order": instruction.attrib["order"], "count": 1})
                        self.statts.freq({"key":instruction.attrib["opcode"] ,"order": instruction.attrib["order"], "count": 1})
            # Ensure that the instruction tag is "instruction"
                if not instruction.tag == "instruction":
                    raise err.CustomException("Instruction ", err.ErrorHandling.unexpectedStructureErr) 
                # Ensure that the instruction has an "opcode" and an "order" attribute
                if not "opcode" in instruction.attrib:
                    raise err.CustomException("Bad oppcode", err.ErrorHandling.unexpectedStructureErr) 

                if not "order" in instruction.attrib:
                    raise err.CustomException("bad order", err.ErrorHandling.unexpectedStructureErr) 
                # Count the number of arguments in the instruction
                num_args = 0
            
                for arg in instruction:

                    # Ensure that each argument has a tag of "arg1", "arg2", or "arg3"
                    if not re.match("^arg(1|2|3)$", arg.tag):
                        raise err.CustomException("Bad args numbers", err.ErrorHandling.unexpectedStructureErr)
                    # Ensure that each argument has a "type" attribute
                    if not "type" in arg.attrib:
                        raise err.CustomException("Type not specified", err.ErrorHandling.unexpectedStructureErr) 

                    num_args += 1
                    # Ensure that the root tag is "program"
            if self.XML_root.tag != "program":
                raise err.CustomException("Program not specified", err.ErrorHandling.unexpectedStructureErr) 
            # Ensure that the root element has a "language" attribute with a value of "IPPcode22"
            if not "language" in self.XML_root.attrib:
                raise err.CustomException("Langue not specified", err.ErrorHandling.unexpectedStructureErr) 
                
            if self.XML_root.get("language") != "IPPcode23":
                raise err.CustomException("Program must be IPPcode23", err.ErrorHandling.unexpectedStructureErr) 
                    
        # If any of the assertions fail, exit the program with exit code 32
        except err.CustomException as e:
            err.CustomException.print_error()
            exit(e.error_code)

        try:
            # Sort the instructions by order number
            sorted_inst[:] = sorted(self.XML_root, key=lambda child: (child.tag, int(child.get("order"))))
            # Check for non-positive or duplicated order numbers
            
            order_numbers = []
            for instruction in sorted_inst:
                order = int(instruction.get("order"))
                assert order >= 1, "Non-positive order number detected"
                order_numbers.append(order)
            assert len(order_numbers) == len(set(order_numbers)), "Duplicated order number detected"
            debug("DUPLICATE NUMBER")
        except:
            debug("DUPLICATE NUMBER OR NON POSIITVE num")
            err.CustomException.print_error()
            exit(err.ErrorHandling.unexpectedStructureErr)

        try:
            instruction_counter = 0
            for ins in self.XML_root:

                if ins.get("opcode") == "LABEL":

                    current_label = ins[0].text

                    if current_label in self.label_positions:
                        debug("MULTIPLE LABELS")
                        raise err.CustomException("Multiple labels with same name", err.ErrorHandling.semanticErr)
                    self.label_positions[current_label] = instruction_counter
                instruction_counter += 1
        except err.CustomException as e:
            err.CustomException.print_error()
            exit(e.error_code)

class Interpret():

    global sorted_inst
    def __init__(self):
        self.creating_temporary_frame = False
        self.count_of_local_frames = 0
        self.label_positions = {}
        self.iterator = 0
        self.is_temporary_frame = False
        self.dict_of_variables = {}
        self.stats_count_of_instructions = 0
        self.statss = stats.Stats(self.stats_count_of_instructions)
        self.arguments = ProcessArgs()
        self.arguments.parseArguments()
        self.parser = ParseT(self.arguments.input_to_execute,self.statss, self.label_positions)
        self.com = c.ExecuteCommands()
    stats_count_of_instructions = 0
    def run(self):
        self.parser.check_xml_and_process_instructions()
        while True:
            if self.iterator < len(sorted_inst):
                one_instruction = sorted_inst[self.iterator]
            else:
                self.statss.vars(self.dict_of_variables)
                print_stats(self.arguments, self.statss)
                debug("USPECH!!!")
                exit(err.ErrorHandling.OK)


            self.arg = []
            self.arg[:] = sorted(one_instruction, key=lambda x: x.tag)
            ins = InstructOnLineCheck(self.count_of_local_frames, one_instruction, self.is_temporary_frame, self.dict_of_variables)
            args = []
            for i in range(1, 4):
                if i in ins.argdictionary and "type" in ins.argdictionary[i] and "value" in ins.argdictionary[i]:
                    args.append({"value": ins.argdictionary[i]["value"], "type": ins.argdictionary[i]["type"]})

            operating_code = one_instruction.get("opcode").upper()
            if operating_code in Structs.calling_func:
                func = Structs.calling_func[operating_code]
                try:
                    if operating_code == "MOVE":
                        self.dict_of_variables[args[0]["value"]] = func(args, self.statss)
                    elif operating_code in ["STRI2INT","INT2CHAR","ADD", "SUB", "MUL", "IDIV", "DIV", "AND", "OR", "NOT", "LT", "GT", "EQ", "POPS", "FLOAT2INT", "INT2FLOAT", "SETCHAR", "GETCHAR", "TYPE", "STRLEN", "CONCAT"]:
                        self.dict_of_variables = func(args, self.dict_of_variables, self.statss)
                    elif operating_code in ["STRI2INTS","INT2CHARS","ANDS", "ADDS","SUBS", "MULS", "IDIVS", "LTS", "GTS", "EQS", "ORS", "NOTS"]:
                        func(self.dict_of_variables, self.statss)
                    elif operating_code == "CLEARS":
                        func(self.statss)
                    elif operating_code in ["JUMPIFEQS","JUMPIFNEQS"]:
                        self.iterator = func(self.label_positions, args, self.iterator, self.dict_of_variables, self.statss)
                    elif operating_code == "BREAK":
                        func()
                    elif operating_code == "DEFVAR":
                        self.dict_of_variables = func(args, self.dict_of_variables, self.is_temporary_frame, self.count_of_local_frames, self.statss)
                    elif operating_code == "RETURN":
                        self.iterator = func(self.iterator, self.statss)
                    elif operating_code in ["JUMPIFEQ", "JUMPIFNEQ", "CALL", "JUMP"]:
                        self.iterator = func(args, self.label_positions, self.iterator, self.statss)
                    elif operating_code in ["EXIT","WRITE", "PUSHS"]:
                        func(args, self.dict_of_variables, self.statss)
                    elif operating_code == "PUSHFRAME":
                        self.dict_of_variables = func(self.creating_temporary_frame, self.dict_of_variables, self.statss)
                        self.count_of_local_frames += 1
                        self.creating_temporary_frame = False
                        self.is_temporary_frame = False      
                    elif operating_code == "POPFRAME":
                        self.dict_of_variables = func(self.is_temporary_frame, self.dict_of_variables,self.count_of_local_frames, self.statss) 
                        self.count_of_local_frames -= 1
                        self.creating_temporary_frame = True
                        self.is_temporary_frame = True
                    elif operating_code == "READ":
                        self.dict_of_variables = func(args, self.arg, self.dict_of_variables, self.arguments, self.statss)
                    elif operating_code == "CREATEFRAME":
                        self.dict_of_variables = func(self.is_temporary_frame, self.dict_of_variables, self.statss)
                        self.creating_temporary_frame = True
                        self.is_temporary_frame = True
                    elif operating_code == "DPRINT":
                        func(args, self.dict_of_variables)

                except err.CustomException as e:
                    err.CustomException.print_error()
                    exit(e.error_code)

            self.iterator = self.iterator + 1 

if __name__ == "__main__":
    interpreter = Interpret()
    interpreter.run()
