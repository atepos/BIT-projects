import sys as s
from enum import Enum

import err as err

class ValueType(Enum):
    NIL = "NIL"
    VALUE = 0
    TYPE = 1

FLAG = False
def debug(string):
    if(FLAG):
        print(string)

def determine_datatype(value):
    if type(value) == int:
        return "INT"
    elif type(value) == float:
        return "FLOAT"
    elif type(value) == bool:
        return "BOOL"
    elif type(value) == str:
        return "STRING"
    else:
        return "unknown"
    
def StackPoP(stack):
    if not stack: 
        print("empty stacks ")
        raise err.CustomException("Instruction", err.ErrorHandling.valueMissingErr)
    return stack.pop()

def getTypeStack(arg, dict_of_variables):
    if arg[1] == "VAR":
        varType = dict_of_variables[arg[0]][ValueType.TYPE.value]
        if varType == None:
            raise err.CustomException("Instruction", err.ErrorHandling.valueMissingErr)
        return dict_of_variables[arg[0]][ValueType.TYPE.value]
    else:
        return arg[1]
    
def getValueStack(arg, dict_of_variables):
    if arg[1] == "VAR":
        varType = dict_of_variables[arg[0]][ValueType.VALUE.value]
        if varType == None:
            raise err.CustomException("Instruction", err.ErrorHandling.valueMissingErr)
        return dict_of_variables[arg[0]][ValueType.VALUE.value]
    else:
        return arg[0]

class ExecuteCommands:  
    temp = {}
    call_jump = []
    data_stack = []
    stack_of_variables = [] 

    def MOVE(self, args, stats):
        stats.increment_instruction_count()
        return [args[1]["value"], args[1]["type"]]
    
    def DEFVAR(self, args, dict_of_variables, is_temporary_frame, count_of_local_frames, stats):
        stats.increment_instruction_count()
        if args[0]["value"] in dict_of_variables:
            debug("DEFVAR")
            raise err.CustomException("Instruction", err.ErrorHandling.semanticErr)
        if args[0]["value"].startswith("TF") and is_temporary_frame:
            dict_of_variables[args[0]["value"]] = [None, None]
        elif args[0]["value"].startswith("LF") and count_of_local_frames > 0:
            dict_of_variables[args[0]["value"]] = [None, None]
        elif args[0]["value"].startswith("GF"):
            dict_of_variables[args[0]["value"]] = [None, None]
        else:
            debug("DEFVAR")
            raise err.CustomException("Instruction", err.ErrorHandling.labelnonErr)
        return dict_of_variables

    def CREATEFRAME(self, is_temporary_frame, dict_of_variables, stats):
        stats.increment_instruction_count()
        if is_temporary_frame:
            [dict_of_variables.pop(variable) for variable in list(dict_of_variables.keys()) if variable.startswith("TF")]
        return dict_of_variables
    
    def PUSHFRAME(self, creating_temporary_frame, dict_of_variables, stats):
        stats.vars(dict_of_variables)
        stats.increment_instruction_count()
        if not creating_temporary_frame:
            debug("pushframe")
            raise err.CustomException("Instruction", err.ErrorHandling.labelnonErr)

        for variable in list(dict_of_variables.keys()):
            if variable.startswith("LF"):
                self.temp = {}
                self.temp[variable] = dict_of_variables.pop(variable)  

        self.stack_of_variables.append(self.temp)

        for variable in list(dict_of_variables.keys()):
            new_key = variable.replace("TF","LF")
            dict_of_variables[new_key] = dict_of_variables.pop(variable)

        return dict_of_variables
    
    def POPFRAME(self, is_temporary_frame, dict_of_variables, count_of_local_frames,stats):
        stats.vars(dict_of_variables)
        stats.increment_instruction_count()
        if count_of_local_frames == 0:
            raise err.CustomException("Instruction", err.ErrorHandling.labelnonErr)
        if is_temporary_frame == True:
            [dict_of_variables.pop(var) for var in list(dict_of_variables.keys()) if var.startswith("TF")]
        for var in list(dict_of_variables.keys()):
            newKey = var.replace("LF", "TF")
            dict_of_variables[newKey] = dict_of_variables.pop(var)

        dict_of_variables.update(self.stack_of_variables.pop())
        return dict_of_variables

    def CALL(self, args, label_positions, iterator,stats):
        stats.increment_instruction_count()
        if not args[0]["value"] in label_positions:
            raise err.CustomException("Instruction", err.ErrorHandling.semanticErr)
        self.call_jump.append(iterator)
        iterator = label_positions[args[0]["value"]] 

        return iterator

    def RETURN(self, iterator,stats):
        stats.increment_instruction_count()
        if not self.call_jump:
            raise err.CustomException("Instruction", err.ErrorHandling.valueMissingErr) 
        iterator = self.call_jump.pop()
        return iterator
    
    def ADD(self, args, dict_of_variables,stats):
        stats.increment_instruction_count()
        if not ((args[1]["type"] == args[2]["type"]) and (args[1]["type"] in ["INT", "FLOAT"])):
            debug("add chyba")
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = args[1]["value"] + args[2]["value"]        
        dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = args[2]["type"]
        return dict_of_variables
    
    def SUB(self, args, dict_of_variables,stats):
        stats.increment_instruction_count()
        if not ((args[1]["type"] == args[2]["type"]) and (args[1]["type"] in ["INT", "FLOAT"])):
            debug("SUB chyba")
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = args[1]["value"] - args[2]["value"]
        dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = args[2]["type"]
        return dict_of_variables
    
    def MUL(self, args, dict_of_variables,stats):
        stats.increment_instruction_count()
        if not ((args[1]["type"] == args[2]["type"]) and (args[1]["type"] in ["INT", "FLOAT"])):
            debug("MUL chyba")
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = args[1]["value"] * args[2]["value"]
        dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = args[2]["type"]
        return dict_of_variables
    
    def IDIV(self, args, dict_of_variables,stats):
        stats.increment_instruction_count()
        if not ((args[1]["type"] == args[2]["type"]) and (args[1]["type"] in ["INT", "FLOAT"])):
            debug("IDIV chyba")
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        if args[2]["value"] == 0:
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandValueErr)
        dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = args[1]["value"] // args[2]["value"]
        dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = args[2]["type"]
        return dict_of_variables
    
    def DIV(self, args, dict_of_variables,stats):
        stats.increment_instruction_count()
        if args[2]["value"] == 0:
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandValueErr)
        if args[1]["type"] != "FLOAT" or args[2]["type"] != "FLOAT": 
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)

        dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = args[1]["value"] / args[2]["value"]
        dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = args[2]["type"]

        return dict_of_variables

    def LT(self, args, dict_of_variables,stats):
        stats.increment_instruction_count()
        if args[1]["value"] == "NIL" or args[2]["type"] == "NIL":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        if args[1]["type"] != args[2]["type"]:
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        if args[1]["value"] < args[2]["value"]:
            dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = True
        else:
            dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = False

        dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = "BOOL"
        return dict_of_variables
    
    def GT(self, args, dict_of_variables,stats):
        stats.increment_instruction_count()
        if args[1]["value"] == "NIL" or args[2]["type"] == "NIL":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        if args[1]["type"] != args[2]["type"]:
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        if args[1]["value"] > args[2]["value"]:
            dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = True
        else:
            dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = False 
            
        dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = "BOOL"
        return dict_of_variables
           
    def EQ(self, args, dict_of_variables,stats):
        stats.increment_instruction_count()
        if args[1]["type"] != args[2]["type"]:
            if args[1]["type"] == "NIL" or args[2]["type"] == "NIL":
                pass
            else:
                raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
                
        if args[1]["value"] == args[2]["value"]:
            dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = True
        else:
            dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = False
        dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = "BOOL"
        return dict_of_variables
    
    def AND(self, args, dict_of_variables,stats):       
        stats.increment_instruction_count() 
        if args[1]["type"] != "BOOL":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        if args[2]["type"] != "BOOL":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)

        if args[1]["value"] == False and args[2]["value"] == False:
            dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = False
        elif args[1]["value"] == True and args[2]["value"] == False:       
            dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = False
        elif args[1]["value"] == False and args[2]["value"] == True:
            dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = False
        elif args[1]["value"] == True and args[2]["value"] == True:
            dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = True
        dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = "BOOL"
        return dict_of_variables

    def OR(self, args, dict_of_variables,stats):
        stats.increment_instruction_count()
        if args[1]["type"] != "BOOL":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        if args[2]["type"] != "BOOL":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)

        if args[1]["value"] == False and args[2]["value"] == False:
            dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = False
        elif args[1]["value"] == True and args[2]["value"] == False:       
            dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = True
        elif args[1]["value"] == False and args[2]["value"] == True:
            dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = True
        elif args[1]["value"] == True and args[2]["value"] == True:
            dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = True
        dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = "BOOL"
        return dict_of_variables


    def NOT(self, args, dict_of_variables,stats):
        stats.increment_instruction_count()
        if args[1]["type"] != "BOOL":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        if args[1]["value"] == False:
            dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = True
        elif args[1]["value"] == True:
            dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = False
        dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = "BOOL"
        
        return dict_of_variables
    
    
    def INT2CHAR(self, args, dict_of_variables,stats):
        stats.increment_instruction_count()
        if args[1]["type"] != "INT":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        if not 0 < args[1]["value"] < 256:
            raise err.CustomException("Instruction", err.ErrorHandling.stringErr)
        dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = chr(args[1]["value"])
        dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = "STRING"
        return dict_of_variables
    
    def STR2INT(self, args, dict_of_variables,stats):
        stats.increment_instruction_count()
        if args[1]["type"] != "STRING" or args[2]["type"] != "INT":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        if not 0 < args[2]["value"] < len(args[1]["value"]):
            raise err.CustomException("Instruction", err.ErrorHandling.stringErr)
        dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = ord(args[1]["value"][args[2]["value"]])
        dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = "INT"
        return dict_of_variables
    
    def READ(self, args, arg, dict_of_variables, arguments,stats):
        stats.increment_instruction_count()
        args[1]["type"] = (arg[1].text).upper()
        inputV = arguments.input_to_read.readline()
        is_EOF = False
        if not inputV:
            is_EOF = True
        if is_EOF == False and inputV[-1] == "\n":
            inputV = inputV[:-1]
        if args[1]["type"] == "BOOL":
            if inputV.lower() == "true":
                dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = True
                dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = "BOOL"
            else:
                dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = False
                dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = "BOOL"
        elif args[1]["type"] == "INT":
            try:
                dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = int(inputV)
                dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = "INT"
            except:
                dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = ValueType.NIL.value
                dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = "NIL"
        elif args[1]["type"] == "FLOAT":
            try:
                dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = float.fromhex(inputV)
                dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = "FLOAT"
            except:
                dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = ValueType.NIL.value
                dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = "NIL"
        elif args[1]["type"] == "STRING" and not is_EOF:
            dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = inputV
            dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = "STRING"
        else:
            dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = ValueType.NIL.value
            dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = "NIL"
        return dict_of_variables
    
    def WRITE(self, args, dict_of_variables,stats):
        stats.increment_instruction_count()
        if args[0]["type"] == "VAR":
            if dict_of_variables[args[0]["value"]][ValueType.VALUE.value] == None:
                debug("WRITE")
                raise err.CustomException("Instruction", err.ErrorHandling.valueMissingErr)
            args[0]["value"] = dict_of_variables[args[0]["value"]][ValueType.VALUE.value]

        if determine_datatype(args[0]["value"]) == "FLOAT":
            print(float.hex(args[0]["value"]), end="")
            return 
        
        value = str(args[0].get("value", ""))
        value = value.lower() if value in ("True", "False") else value
        value = "" if value == "NIL" else value
        args[0]["value"] = value
        print(args[0]["value"], end="")

    def CONCAT(self, args, dict_of_variables,stats):
        stats.increment_instruction_count()
        if args[1]["type"] != "STRING" or args[2]["type"] != "STRING":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = args[1]["value"] + args[2]["value"]
        dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = "STRING"
        return dict_of_variables

    def STRLEN(self, args, dict_of_variables,stats):
        stats.increment_instruction_count()
        if args[1]["type"] != "STRING":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        dict_of_variables[args[0]["value"]] = [len(args[1]["value"]), "INT"]
        return dict_of_variables

    def GETCHAR(self, args, dict_of_variables,stats):
        stats.increment_instruction_count()
        if args[1]["type"] != "STRING" or args[2]["type"] != "INT":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        leng = len(args[1]["value"]) - 1
        if args[2]["value"] > leng or args[2]["value"] < 0:
            raise err.CustomException("Instruction", err.ErrorHandling.stringErr)
        dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = args[1]["value"][args[2]["value"]]
        dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = "STRING"
        return dict_of_variables

    def SETCHAR(self, args, dict_of_variables ,stats):
        stats.increment_instruction_count()
        if dict_of_variables[args[0]["value"]][ValueType.VALUE.value] == None:
            raise err.CustomException("Instruction", err.ErrorHandling.valueMissingErr)
        if dict_of_variables[args[0]["value"]][ValueType.TYPE.value] != "STRING":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        if args[1]["type"] != "INT" or args[2]["type"] != "STRING":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        string = dict_of_variables[args[0]["value"]][ValueType.VALUE.value]
        if len(args[2]["value"]) == 0:
            raise err.CustomException("Instruction", err.ErrorHandling.stringErr)

        len_of_arg = len(dict_of_variables[args[0]["value"]][ValueType.VALUE.value]) - 1
        if len_of_arg < args[1]["value"] or 0 > args[1]["value"]:
            raise err.CustomException("Instruction", err.ErrorHandling.stringErr)
        dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = string[:args[1]["value"]] + args[2]["value"][0] + string[args[1]["value"] + 1:]
        return dict_of_variables 
    
    def TYPE(self, args, dict_of_variables,stats):
        stats.increment_instruction_count()
        if args[1]["type"] in ("INT","BOOL","STRING","NIL"):
            dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = args[1]["type"].lower()
            dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = "STRING"
        else:
            dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = ""
            dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = "STRING"
        return dict_of_variables
    
    def JUMP(self, args, label_positions, iterator,stats):
        stats.increment_instruction_count()
        if not args[0]["value"] in label_positions:
            raise err.CustomException("Instruction", err.ErrorHandling.semanticErr)
        iterator = label_positions[args[0]["value"]]
        return iterator

    def JUMPIFEQ(self, args, label_positions, iterator,stats):
        stats.increment_instruction_count()
        if not args[0]["value"] in label_positions:
            raise err.CustomException("Instruction", err.ErrorHandling.semanticErr)
        if not args[1]["type"] == args[2]["type"]:
            if args[1]["type"] == "NIL" or args[2]["type"] == "NIL":
                return iterator
            else:
                raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
                
        if (args[1]["value"] == args[2]["value"]):
            iterator = label_positions[args[0]["value"]]
        return iterator
    
    def JUMPIFNEQ(self, args, label_positions, iterator,stats):
        stats.increment_instruction_count()
        if not args[0]["value"] in label_positions:
             raise err.CustomException("Instruction", err.ErrorHandling.semanticErr)
        if not args[1]["type"] == args[2]["type"]:
            if args[1]["type"] == "NIL" or args[2]["type"] == "NIL":
                iterator = label_positions[args[0]["value"]]
                return iterator
            else:
                raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
                
        if (args[1]["value"] != args[2]["value"]):
            iterator = label_positions[args[0]["value"]]
        return iterator

    def EXIT(self, args, dict_of_variables,stats):
        stats.increment_instruction_count()
        if args[0]["type"] == "VAR":
            args[0]["type"] = dict_of_variables[args[0]["value"]][ValueType.TYPE.value]
            args[0]["value"] = dict_of_variables[args[0]["value"]][ValueType.VALUE.value]
        if args[0]["value"] == None:
            raise err.CustomException("Instruction", err.ErrorHandling.valueMissingErr)
        if args[0]["type"] != "INT":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        if args[0]["value"] < 0 or args[0]["value"] > 49:
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandValueErr)
        exit(args[0]["value"])

    def DPRINT(self, args, dict_of_variables):
        if args[0]["type"] == "VAR":
            args[0]["type"] = dict_of_variables[args[0]["value"]][ValueType.TYPE.value]
            args[0]["value"] = dict_of_variables[args[0]["value"]][ValueType.VALUE.value]
        if args[0]["value"] == None:
            raise err.CustomException("Instruction", err.ErrorHandling.valueMissingErr)
        print(args[0]["value"], file = s.stderr)

    def BREAK(self):
        print("STAV INTERPRETU",file= s.stderr)

    def FLOAT2INT(self, args, dict_of_variables,stats):
        stats.increment_instruction_count()
        if args[1]["type"] != "FLOAT":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = int(args[1]["value"])
        dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = "INT"
        return dict_of_variables
    
    def INT2FLOAT(self, args, dict_of_variables,stats):
        stats.increment_instruction_count()
        if args[1]["type"] != "INT":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = float(args[1]["value"])
        dict_of_variables[args[0]["value"]][ValueType.TYPE.value] = "STRING"
        return dict_of_variables
    
    def POPS(self, args, dict_of_variables,stats):
        stats.increment_instruction_count()
        if not self.data_stack: 
            debug("empty stacks ")
            raise err.CustomException("Instruction", err.ErrorHandling.valueMissingErr) 
        poppedData = self.data_stack.pop()
        dict_of_variables[args[0]["value"]][ValueType.VALUE.value] = poppedData[ValueType.VALUE.value]
        dict_of_variables[args[0]["value"]][ValueType.TYPE.value]  = poppedData[ValueType.TYPE.value]
        return dict_of_variables

    def CLEARS(self,stats):
        stats.increment_instruction_count()
        self.data_stack.clear()

    def ADDS(self, dict_of_variables,stats):
        stats.increment_instruction_count()
        arg1 = StackPoP(self.data_stack)
        arg2 = StackPoP(self.data_stack)

        if getTypeStack(arg1, dict_of_variables) not in ["INT", "FLOAT"] or getTypeStack(arg2, dict_of_variables) not in ["INT", "FLOAT"]:
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)

        tmp = []
        tmp.append(getValueStack(arg2, dict_of_variables) + getValueStack(arg1, dict_of_variables))
        tmp.append(getTypeStack(arg1, dict_of_variables))

        self.data_stack.append([tmp[0],tmp[1]])

    def SUBS(self, dict_of_variables,stats):
        stats.increment_instruction_count()
        arg1 = StackPoP(self.data_stack)
        arg2 = StackPoP(self.data_stack)
        if getTypeStack(arg1, dict_of_variables) not in ["INT", "FLOAT"] or getTypeStack(arg2, dict_of_variables) not in ["INT", "FLOAT"]:
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)

        tmp = []
        tmp.append(getValueStack(arg2, dict_of_variables) - getValueStack(arg1, dict_of_variables))
        tmp.append(getTypeStack(arg1, dict_of_variables))
        self.data_stack.append([tmp[0],tmp[1]])


    def MULS(self, dict_of_variables,stats):
        stats.increment_instruction_count()
        arg1 = StackPoP(self.data_stack)
        arg2 = StackPoP(self.data_stack)
        if getTypeStack(arg1, dict_of_variables) not in ["INT", "FLOAT"] or getTypeStack(arg2, dict_of_variables) not in ["INT", "FLOAT"]:
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
    
        tmp = []
        tmp.append(getValueStack(arg2, dict_of_variables) * getValueStack(arg1, dict_of_variables))
        tmp.append(getTypeStack(arg1, dict_of_variables))
        self.data_stack.append([tmp[0],tmp[1]])

    def IDIVS(self, dict_of_variables,stats):
        stats.increment_instruction_count()
        arg1 = StackPoP(self.data_stack)
        arg2 = StackPoP(self.data_stack)

        if getTypeStack(arg1, dict_of_variables) not in ["INT", "FLOAT"] or getTypeStack(arg2, dict_of_variables) not in ["INT", "FLOAT"]:
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        if getValueStack(arg1, dict_of_variables) == 0:
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandValueErr)

        tmp = []
        tmp.append(getValueStack(arg2, dict_of_variables) // getValueStack(arg1, dict_of_variables))
        tmp.append(getTypeStack(arg1, dict_of_variables))
        self.data_stack.append([tmp[0],tmp[1]])

    def LTS(self, dict_of_variables,stats):
        stats.increment_instruction_count()
        arg1 = StackPoP(self.data_stack)
        arg2 = StackPoP(self.data_stack)

        if getTypeStack(arg1, dict_of_variables) == "NIL" or getTypeStack(arg2, dict_of_variables)== "NILL":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)

        if getTypeStack(arg1, dict_of_variables) != getTypeStack(arg2, dict_of_variables):
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        
        if getValueStack(arg2, dict_of_variables) < getValueStack(arg1, dict_of_variables):
            self.data_stack.append([True, "BOOL"])
        else:
            self.data_stack.append([False, "BOOL"])

    def GTS(self, dict_of_variables,stats):
        stats.increment_instruction_count()
        arg1 = StackPoP(self.data_stack)
        arg2 = StackPoP(self.data_stack)

        if getTypeStack(arg1, dict_of_variables) == "NIL" or getTypeStack(arg2, dict_of_variables)== "NILL":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)

        if getTypeStack(arg1, dict_of_variables) != getTypeStack(arg2, dict_of_variables):
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        
        if getValueStack(arg2, dict_of_variables) > getValueStack(arg1, dict_of_variables):
            self.data_stack.append([True, "BOOL"])
        else:
            self.data_stack.append([False, "BOOL"])

    def EQS(self, dict_of_variables,stats):
        stats.increment_instruction_count()
        arg1 = StackPoP(self.data_stack)
        arg2 = StackPoP(self.data_stack)

        if getTypeStack(arg1, dict_of_variables) != getTypeStack(arg2, dict_of_variables):
            if getTypeStack(arg1, dict_of_variables) == "NIL" or getTypeStack(arg2, dict_of_variables) == "NIL":
                pass
            else:
                raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)

        if getValueStack(arg2, dict_of_variables) == getValueStack(arg1, dict_of_variables):
            self.data_stack.append([True, "BOOL"])
        else:
            self.data_stack.append([False, "BOOL"])

    def ANDS(self, dict_of_variables,stats):
        stats.increment_instruction_count()
        arg1 = StackPoP(self.data_stack)
        arg2 = StackPoP(self.data_stack)
        if getTypeStack(arg1, dict_of_variables) != "BOOL":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        if getTypeStack(arg2, dict_of_variables) != "BOOL":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)

        if getValueStack(arg1, dict_of_variables) == False and getValueStack(arg2, dict_of_variables) == False:
            self.data_stack.append([False, "BOOL"])

        if getValueStack(arg1, dict_of_variables) == True and getValueStack(arg2, dict_of_variables) == False:
            self.data_stack.append([False, "BOOL"])

        if getValueStack(arg1, dict_of_variables) == False and getValueStack(arg2, dict_of_variables) == True:
            self.data_stack.append([False, "BOOL"])

        if getValueStack(arg1, dict_of_variables) == True and getValueStack(arg2, dict_of_variables) == True:
            self.data_stack.append([True, "BOOL"])

    def ORS(self, dict_of_variables,stats):
        stats.increment_instruction_count()
        arg1 = StackPoP(self.data_stack)
        arg2 = StackPoP(self.data_stack)
        if getTypeStack(arg1, dict_of_variables) != "BOOL":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        if getTypeStack(arg2, dict_of_variables) != "BOOL":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)

        if getValueStack(arg1, dict_of_variables) == False and getValueStack(arg2, dict_of_variables) == False:
            self.data_stack.append([False, "BOOL"])

        if getValueStack(arg1, dict_of_variables) == True and getValueStack(arg2, dict_of_variables) == False:
            self.data_stack.append([True, "BOOL"])

        if getValueStack(arg1, dict_of_variables) == False and getValueStack(arg2, dict_of_variables) == True:
            self.data_stack.append([True, "BOOL"])

        if getValueStack(arg1, dict_of_variables) == True and getValueStack(arg2, dict_of_variables) == True:
            self.data_stack.append([True, "BOOL"])

    def NOTS(self, dict_of_variables,stats):
        stats.increment_instruction_count()
        arg1 = StackPoP(self.data_stack)
        if getTypeStack(arg1, dict_of_variables) != "BOOL":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)

        if getValueStack(arg1, dict_of_variables) == True:
            self.data_stack.append([False, "BOOL"])
        else:
            self.data_stack.append([True, "BOOL"])

    def INT2CHARS(self, dict_of_variables,stats):
        stats.increment_instruction_count()
        arg1 = StackPoP(self.data_stack)
        if getTypeStack(arg1, dict_of_variables) != "INT":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        if not 0 < getValueStack(arg1, dict_of_variables) < 256:
            raise err.CustomException("Instruction", err.ErrorHandling.stringErr)

        self.data_stack.append([chr(getValueStack(arg1, dict_of_variables)), "STRING"])

    def STRI2INTS(self, dict_of_variables,stats):
        stats.increment_instruction_count()
        arg1 = StackPoP(self.data_stack)
        arg2 = StackPoP(self.data_stack)

        if getTypeStack(arg2, dict_of_variables) != "STRING" or getTypeStack(arg1, dict_of_variables) != "INT":
            raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)
        if not 0 < getValueStack(arg1, dict_of_variables) < len(getValueStack(arg2, dict_of_variables)):
            raise err.CustomException("Instruction", err.ErrorHandling.stringErr)

        self.data_stack.append([ord(getValueStack(arg2, dict_of_variables)[getValueStack(arg1, dict_of_variables)]), "INT"])

    def JUMPIFEQS(self, label_positions, args, iterator, dict_of_variables,stats):
        stats.increment_instruction_count()
        arg1 = StackPoP(self.data_stack)
        arg2 = StackPoP(self.data_stack)

        if not args[0]["value"] in label_positions:
            debug("JUMPIFEQS")
            raise err.CustomException("Instruction", err.ErrorHandling.semanticErr)


        if getTypeStack(arg1, dict_of_variables) != getTypeStack(arg2, dict_of_variables):
            if getTypeStack(arg1, dict_of_variables) == "NIL" or getTypeStack(arg2, dict_of_variables) == "NIL":
                return iterator
            else:
                raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)

        if (getValueStack(arg1, dict_of_variables) == getValueStack(arg2, dict_of_variables)):
            iterator = label_positions[args[0]["value"]]
        return iterator

    def JUMPIFNEQS(self, label_positions, args, iterator, dict_of_variables,stats):
        stats.increment_instruction_count()
        arg1 = StackPoP(self.data_stack)
        arg2 = StackPoP(self.data_stack)

        if not args[0]["value"] in label_positions:
            debug("JUMPIFNEQS")

            raise err.CustomException("Instruction", err.ErrorHandling.semanticErr)

        if getTypeStack(arg1, dict_of_variables) != getTypeStack(arg2, dict_of_variables):
            if getTypeStack(arg1, dict_of_variables) == "NIL" or getTypeStack(arg2, dict_of_variables) == "NIL":
                return label_positions[args[0]["value"]]    
            else:
                raise err.CustomException("Instruction", err.ErrorHandling.badOperandErr)

        if (getValueStack(arg1, dict_of_variables) != getValueStack(arg2, dict_of_variables)):
            iterator = label_positions[args[0]["value"]]
        return iterator
    
    def PUSHS(self,args, dict_of_variables,stats):
        stats.increment_instruction_count()
        if args[0]["type"] == "VAR":
            valueL = dict_of_variables[args[0]["value"]][ValueType.VALUE.value]
            if valueL == None:
                raise err.CustomException("Instruction", err.ErrorHandling.valueMissingErr)
            self.data_stack.append([args[0]["value"],args[0]["type"]])
        else:
            self.data_stack.append([args[0]["value"],args[0]["type"]])   