<?php
///////////////////////////
// IPP project - parse.php
// Author: Petr Kaska xkaska01
// 9.3.2023
///////////////////////////


/**
 * The INSTRUCTION class represents a single instruction.
 */
class INSTRUCTION {

    /**
    * @var array The list of argument values for the instruction.
    */
    private $array = [];

    /**
     * Constructor for the INSTRUCTION class.
     *
     * @param array $array The initial list of argument values for the instruction.
     */
    public function __construct($array) {
        $this->array = $array;
    }

    /**
     * Adds an argument value to the list of argument values for the instruction.
     *
     * @param mixed $ArgumentValues The argument value to be added.
     */
    public function addArguments($ArgumentValues) {
        $this->myArrayPush($ArgumentValues);
    }

    /**
     * Private helper function to push an argument value onto the list of argument values for the instruction.
     *
     * @param mixed $value The argument value to be added.
     */
    private function myArrayPush($value) {
        $this->array[] = $value;
    }

    /**
     * Returns the list of argument values for the instruction.
     *
     * @return array The list of argument values for the instruction.
     */
    public function getInstruction() {
        return $this->array;
    }
}


/**
*
*The XMLCREATOR class is responsible for creating XML nodes representing instructions and their arguments.
*
*@class XMLCREATOR
*/
class XMLCREATOR {

    /**
    * @var opCodes the array of operation codes defined below 
    * @var instructionCount count of all instructions, which occures in the code
    *
    */
    private $opCodes;
    private $instructionCount = 1;


    /**
    * Constructor for the XMLCREATOR class.
    *
    * @param opCOdes $array The initial list of argument values for the OpCodes.
    */
    public function __construct($opCodes) {
        $this->opCodes = $opCodes;
    }
  
    /**
    * Creates an XML element for an instruction based on the provided input and line to analyze.
    * @param DOMDocument $input The input document where the instruction element will be created.
    * @param array $lineToAnalyze An array containing the components of the instruction to analyze.
    *  @return DOMElement The created instruction element.
    */
    public function createInstruction($input, $lineToAnalyze) {
        $instuctionXML = $input->createElement("instruction");
        $instuctionXML->setAttribute("order", $this->instructionCount);
        $instuctionXML->setAttribute("opcode", $this->opCodes[$lineToAnalyze[0][1]]);
        $this->instructionCount++;
        return $instuctionXML;
    }
  
    /**
    *Creates and adds a new XML element to an instruction element based on the provided parameters.
    *@param DOMElement $instruct The instruction element to which the new XML element will be added.
    *@param lineToAnalyze The value to add as the text content of the new XML element.
    *@param DOMDocument $dom The XML document to which the new XML element will belong.
    *@param arg The name of the new XML element.
    *@param var The value of the attribute to add to the new XML element.
    *@param type The name of the attribute to add to the new XML element.
    *@return DOMElement The updated instruction element.
    */
    public function createXML($instruct, $lineToAnalyze, $dom, $arg, $var, $type) {
      $arg1 = $dom->createElement($arg, htmlspecialchars($lineToAnalyze, ENT_COMPAT));
      $arg1->setAttribute($type, $var);
      $instruct->appendChild($arg1);
      return $instruct;
    }
}

ini_set("display_errors", "stderr");

define("OK",0);
define("paramError",10);
define("inputFileError",11);
define("outputFileError",12);
define("headerError",21);
define("opCodeError",22);
define("lexSyntaxError",23);
define("allocError",99);

define("TokenHeader", 1);
define("TokenOpCode", 2);
define("TokenLabelType", 3);
define("TokenLabel", 4);
define("TokenVariable", 5);
define("TokenConst", 6);
define("TokenEof", 7);

$headerCount;
$wasHeader = false;

$OpCodes = array(
    0 =>"LINE_JUST_FOR_ITERATION",
    1 =>"MOVE",
    2 =>"NOT",
    3 =>"INT2CHAR",
    4 =>"STRLEN",
    5 =>"TYPE",
    6 =>"POPFRAME",
    7 =>"CREATEFRAME",
    8 =>"PUSHFRAME",
    9 =>"RETURN",
    10 =>"BREAK",
    11 =>"CALL",
    12 =>"LABEL",
    13 =>"JUMP",
    14 =>"PUSHS",
    15 =>"WRITE",
    16 =>"EXIT",
    17 =>"ADD",
    18 =>"SUB",
    19 =>"MUL",
    20 =>"IDIV",
    21 =>"LT",
    22 =>"GT",
    23 =>"EQ",
    24 =>"AND",
    25 =>"OR",
    26 =>"STRI2INT",
    27 =>"CONCAT",
    28 =>"GETCHAR",
    29 =>"SETCHAR",
    30 =>"DEFVAR",
    31 =>"POPS",
    32 =>"READ",
    33 =>"JUMPIFEQ",
    34 =>"JUMPIFNEQ",
    35 =>"DPRINT"
);

$flagLexer = false;
$flagScanner = false;

function debugScanner($output){
    global $flagScanner;
    if($flagScanner){
        echo $output;
    }
}

function debugLexer($output){
    global $flagLexer;
    if($flagLexer){
        echo $output;
    }
}

/**
*Displays help information for the script.
*It prints out the different flags that can be passed as command line arguments, along with a brief description of what each flag does.
*After displaying the help information, it exits the script with a status code of OK.
*@return void
*/
function help()
{
    echo("help:\n");
    echo("--h Prints help\n");
    echo("--stats Select file to print Stat\n");
    echo("--loc Prints to stat count of lines with code\n");
    echo("--comments Prints to stat count of lines with comments\n");
    echo("--jumps Prints to stat count of return instructions and jump instructions\n");
    exit(OK);
}

/**
*Checks the command line arguments passed to the script and performs appropriate actions.
*If the "--help" flag is passed, it displays help information. Otherwise, it sets the $isStats variable to false.
*@param array $argv An array of command line arguments
*@param int $argc The number of command line arguments
*@return void
*/
function arg_check($argv, $argc){
    $isStats = false;
    for($i = 1; $i < $argc; $i++){
        if($argv[$i] == "--help")
        {
            if($argc > 2)
            {
                fwrite(STDERR, "ERROR: param\n");
                exit(paramError);
            }else
            {
                help();
            }
        }
    }
}

/**
*Checks if the given variable is of a valid type according to the language specification.
*@param string $var The variable to check.
*@return bool Returns true if the variable is of a valid type, false otherwise.
*/
function isType($var){
    return (((preg_match("/^STRING@/", strtoupper($var)) && !preg_match("/(\\\\[\d]{0,2}($|\p{M}|\p{L}|\p{S}|\p{Z}\p{P}|\p{C}| ))/", strtoupper($var)))) || preg_match("/^INT@[+|-]?\d+\b$|^INT@[+|-]?[\d]+(?:_[\d]+)*\b$|^INT@[+|-]?0[x|X][0-9a-fA-F]+(?:_[A-Fa-f0-9]+)*\b$|^INT@0x[A-Fa-f0-9]+\b$|^INT@[+|-]?0[o|O][0-7]+(?:_[0-7]+)*\b$|INT@0[o|O][0-7]+\b$/", strtoupper($var)) || preg_match("/^BOOL@(TRUE|FALSE)$\b/", strtoupper($var)) || preg_match("/NIL@NIL\b/", strtoupper($var)) ? true : false);
}


/**
*Scanner function that reads input from STDIN and generates a sequence of tokens.
*Tokens are generated based on the grammar rules specified in the project documentation.
*@global instructions - array of instructions generated by the scanner
*@global wasHeader - flag indicating if the header has been encountered yet
*@global headerCount - counter for the number of headers encountered
*@return array - array of tokens representing the input instructions
*/
function scanner(){
    global $instructions;
    global $wasHeader;
    global $headerCount;
    $final = array();
    $inst  = new INSTRUCTION($final);
    $tmp; 
    $StartOfLine = true;
    
    while(1){

        // if Eof -> end 
        if(($line = fgets(STDIN)) == false){
            $inst->addArguments(array(TokenEof));
            debugScanner("END of file \n");
            return $inst->getInstruction();
        }

        // if comment -> SKIP
        if(preg_match("/^\s*#/", $line)){
            continue;
        }
        // if newline -> SKIP
        if(preg_match("/^\s*$/", $line)){
            continue;
        }

        $splitLex = preg_split("/\s+/", explode("#", $line)[0]); 

        // Remove any leading or trailing empty strings
        $splitLex = array_filter($splitLex, function($value) {
            return $value !== "";
        });

        // Re-index the array to remove any gaps caused by the filtering
        $splitLex = array_values($splitLex);
        break;
    }
    foreach($splitLex as $lex){
       
        if(!preg_match("/@/", $lex)){// Operation code, label, header or label type

            if(preg_match("/^(BOOL|STRING|NIL|INT)$/", strtoupper($lex))){
                $inst->addArguments(array(TokenLabelType, $lex));
                debugScanner("LABELTYPE :\n");
            }
            else{
                // Header 
                if(preg_match("/^\.IPPCODE23$/i", strtoupper($lex))){
                    $inst->addArguments(array(TokenHeader));
                    $wasHeader = true;
                    $headerCount++;
                    debugScanner("HEADER\n");
                    if($headerCount > 1){
                        fwrite(STDERR, "ERROR: OpCode\n");
                        exit(opCodeError);
                    }
                    break;
                }

                if(!$wasHeader){
                    fwrite(STDERR, "ERROR: Header\n");
                    exit(headerError);
                }
               
                // Opcode
                $OpCodeNum = isOpCode($lex);
                if($OpCodeNum){
                    if($StartOfLine){
                        $inst->addArguments(array(TokenOpCode, $OpCodeNum));
                        debugScanner("INSTRUCTION :");
                        
                    }
                    else{
                        $inst->addArguments(array(TokenLabel, $lex));
                        debugScanner("Label :\n");
                    }
                }
                else
                {
                    // LABEL
                    if(preg_match("/^[A-Z_\-$&?!*%][A-Z\d_\-%&!?*$]*$/", strtoupper($lex))){
                        $inst->addArguments(array(TokenLabel, $lex));
                        debugScanner("LABEL :\n");
                    }
                    else{
                        fwrite(STDERR, "ERROR: OpCode\n");
                        exit(lexSyntaxError);
                    }
                }
            }
           
        } 
        else{// variable or constant
            //constant 
            if (!preg_match("/^(string|int|bool|nil)/", $lex)) 
            {
                //variable
                if(preg_match("/^(GF|TF|LF)@[a-zA-Z_\-$&*%?!][a-zA-Z0-9\_$-&*%?!]*$/", $lex))
                {
                    debugScanner("variable\n");
                    $inst->addArguments(array(TokenVariable, $lex));
                }else{
                    fwrite(STDERR, "ERROR: Lex or Syn\n");
                    exit(lexSyntaxError);
                }
            }
            else{
                //check condition for int, bool and string
                if(preg_match("/^nil@/", $lex) && !preg_match("/nil$/", $lex)){
                    fwrite(STDERR, "ERROR: Lex or Syn\n");
                    exit(lexSyntaxError);
                }
                
                if(isType($lex))
                {
                    $inst->addArguments(array_merge(array(TokenConst) ,explode("@", $lex, 2)));
                    debugScanner("const\n");
                    
                }
                else{
                    fwrite(STDERR, "ERROR: Lex or Syn\n");
                    exit(lexSyntaxError);
                }
            }
        }
        $StartOfLine = false;
    }
    return $inst->getInstruction();
}


/**
* Function isOpCode()
*This function checks if the given lexeme is a valid opcode by comparing it with the list of available opcodes.
*
*@param oneLex The lexeme to check if it is an opcode
*@return int Returns the index of the opcode if it is found, otherwise returns 0.
*/
function isOpCode($oneLex){
    $num = $found = 0;

    global $OpCodes;

    foreach($OpCodes as $OpCode){
        if(!strcmp($OpCode, strtoupper($oneLex))){
            $found = 1;
            break;
        }
        $num = $num + 1;
    }
    
    return $found ? $num : 0;
}

/**
*Function IsInArray()
*Checks if a variable is present in an array
*@param var The variable to check
*@param array The array to check for the variable
*@return bool True if the variable is present in the array, false otherwise
*/
function IsInArray($var, $array){
    foreach($array as $el){
        if($var == $el){
            return true;
        }
    }
    return false;
}


/**
*Function lexer()
*
*This function analyzes the input code using the scanner function and generates an XML representation of the code with specific format.
*
*@global array $OpCodes An array containing all the supported instructions
*@global array $instructions An array containing all the instructions
*
*@return DOMDocument A DOM document containing the XML representation of the input code
*/
function lexer(){
    global $OpCodes;
    global $instructions;

    $xmlCreator = new XMLCreator($OpCodes);
    $dom = new DOMDocument();
    $dom->encoding = "UTF-8";
    $dom->xmlVersion = "1.0";
    $dom->formatOutput = true;
    $root = $dom->createElement("program");
    $root->setAttribute("language", "IPPcode23");
    $dom->AppendChild($root);

    
    // HEader check
    $lineToAnalyze = scanner();
    if(count($lineToAnalyze, 0) == 1 && $lineToAnalyze[0][0] == TokenHeader){
        debugLexer("lexer {HEADER}\n");
    }
    else{
        debugLexer("lexer {HEADER} MISSING\n");
        exit(lexSyntaxError);
    }

    while(1){
        $lineToAnalyze = scanner();
        $TokenType1 = $lineToAnalyze[1][0];
        $TokenType2 = $lineToAnalyze[2][0];
        $argsOnLine = count($lineToAnalyze);

        if($lineToAnalyze[0][0] == TokenEof){
            debugLexer("LEXER: EOF\n");
            break;
        }

        if($lineToAnalyze[0][0] == TokenOpCode){

            $instruct = $xmlCreator->createInstruction($dom, $lineToAnalyze);

            switch($lineToAnalyze[0][1]){
                // OP <var> <symbol>
                case 1: // MOVE
                case 2: // NOT
                case 3: // INT2CHAR
                case 4: // STRLEN
                case 5: // TYPE
                    if($argsOnLine == 3 && $TokenType1 == TokenVariable && (($TokenType2) == TokenVariable || $TokenType2 == TokenConst)){
                        $instruct = $xmlCreator->createXML($instruct, $lineToAnalyze[1][1], $dom, "arg1", "var", "type");
                    
                        if($TokenType2 != TokenVariable){
                            $instruct = $xmlCreator->createXML($instruct, $lineToAnalyze[2][2], $dom, "arg2", $lineToAnalyze[2][1], "type");
                        }
                        else{
                            $instruct = $xmlCreator->createXML($instruct, $lineToAnalyze[2][1], $dom, "arg2", "var", "type");
                        }
                        break;
                    }
                    else{
                        debugLexer("MOVE, NOT");
                        fwrite(STDERR, "ERROR: LexERR\n");
                        exit(lexSyntaxError);
                    }
                // OP
                case 6: //POPFRAME
                case 7: //CREATEFRAME
                case 8: //PUSHFRAME
                case 9: //RETURN
                case 10: //BREAK
                    if($argsOnLine == 1){
                        break;
                    }
                    else{
                        debugLexer("POPFRAME, CREATEFRAME");
                        fwrite(STDERR, "ERROR: LexERR\n");
                        exit(lexSyntaxError);
                    }
                // OP <label>
                case 11: //CALL
                case 12: //LABEL
                case 13: //JUMP
                    if(($argsOnLine == 2 && $TokenType1 == TokenLabel) || ($argsOnLine == 2 && $TokenType1 == TokenLabelType)){
                        
                        $instruct = $xmlCreator->createXML($instruct, $lineToAnalyze[1][1], $dom, "arg1", "label", "type");
                       
                        break;
                    }
                    else{
                        debugLexer("CALL");
                        fwrite(STDERR, "ERROR: LexERR\n");
                        exit(lexSyntaxError);
                    }
                // OP <symb> 
                case 14: //PUSHS
                case 15: //WRITE
                case 16: //EXIT
                case 35: //DPRINT
                    if(($argsOnLine == 2 && $TokenType1 == TokenVariable) || ($argsOnLine == 2 && $TokenType1 == TokenConst)){
                        if($TokenType1 != TokenVariable){
                            $instruct = $xmlCreator->createXML($instruct, $lineToAnalyze[1][2], $dom, "arg1", $lineToAnalyze[1][1], "type");
                        }
                        else{
                            $instruct = $xmlCreator->createXML($instruct, $lineToAnalyze[1][1], $dom, "arg1", "var", "type");
                        }
                        break;
                    }
                    else{
                        debugLexer("PUSHS");
                        fwrite(STDERR, "ERROR: LexERR\n");
                        exit(lexSyntaxError);
                    }
                // OP <var> <symb> <symb>
                case 17: //ADD
                case 18: //SUB
                case 19: //MUL
                case 20: //IDIV
                case 21: //LT
                case 22: //GT
                case 23: //EQ
                case 24: //AND
                case 25: //OR
                case 26: //STRI2INT
                case 27: //CONCAT
                case 28: //GETCHAR
                case 29: //SETCHAR
                    if(($argsOnLine == 4) && $TokenType1 == TokenVariable && ($TokenType2 == TokenConst || $TokenType2 == TokenVariable) && ($lineToAnalyze[3][0] == TokenConst || $lineToAnalyze[3][0] == TokenVariable)){
                        $instruct = $xmlCreator->createXML($instruct, $lineToAnalyze[1][1], $dom, "arg1", "var", "type");

                        if($TokenType2 == TokenVariable){
                            $instruct = $xmlCreator->createXML($instruct, $lineToAnalyze[2][1], $dom, "arg2", "var", "type");
                        }
                        elseif($TokenType2 == TokenConst)
                        {
                            $instruct = $xmlCreator->createXML($instruct, $lineToAnalyze[2][2], $dom, "arg2", $lineToAnalyze[2][1], "type");
                        }

                        if($lineToAnalyze[3][0] == TokenVariable){
                            $instruct = $xmlCreator->createXML($instruct, $lineToAnalyze[3][1], $dom, "arg3", "var", "type");
                        }
                        elseif($lineToAnalyze[3][0] == TokenConst)
                        {
                            $instruct = $xmlCreator->createXML($instruct, $lineToAnalyze[3][2], $dom, "arg3", $lineToAnalyze[3][1], "type");
                        }
                        break;
                    }
                    else{
                        debugLexer("ADD");
                        fwrite(STDERR, "ERROR: LexERR\n");
                        exit(lexSyntaxError);
                    }
                // OP <var>
                case 30: //DEFVAR
                case 31: //POPS
                    if(($argsOnLine == 2) && ($TokenType1 == TokenVariable)){
                        debugLexer("DEFVAR, POPS - good");
                        $instruct = $xmlCreator->createXML($instruct, $lineToAnalyze[1][1], $dom, "arg1", "var", "type");
                        break;
                    }
                    else{
                        debugLexer("DEFVAR");
                        fwrite(STDERR, "ERROR: LexERR\n");
                        exit(lexSyntaxError);
                    }
                // OP <var> <type>
                case 32: // READ
                    if(($argsOnLine == 3) && ($TokenType1 == TokenVariable && $TokenType2 == TokenLabelType)){
                        debugLexer("READ - good");
                        $instruct = $xmlCreator->createXML($instruct, $lineToAnalyze[1][1], $dom, "arg1", "var", "type");
                        $instruct = $xmlCreator->createXML($instruct, $lineToAnalyze[2][1], $dom, "arg2", "type", "type");
                        break;
                    }
                    else{
                        debugLexer("READ");
                        fwrite(STDERR, "ERROR: LexERR\n");
                        exit(lexSyntaxError);
                    }
                // OP <label> <symb> <symb>
                case 33: //JUMPIFEQ
                case 34: //JUMPIFNEQ
                    if(($argsOnLine == 4) && ($TokenType1 == TokenLabelType || $TokenType1 == TokenLabel) && ($TokenType2 == TokenVariable || $TokenType2 == TokenConst) && ($lineToAnalyze[3][0] == TokenVariable || $lineToAnalyze[3][0] == TokenConst)){
                        $instruct = $xmlCreator->createXML($instruct, $lineToAnalyze[1][1], $dom, "arg1", "label", "type");

                        if($TokenType2 == TokenVariable){
                            debugLexer("JUMPIFEQ - Good 1");
                            $instruct = $xmlCreator->createXML($instruct, $lineToAnalyze[2][1], $dom, "arg2", "var", "type");
                        }
                        elseif($TokenType2 == TokenConst){
                            debugLexer("JUMPIFEQ - Good 1");
                            $instruct = $xmlCreator->createXML($instruct, $lineToAnalyze[2][2], $dom, "arg2", $lineToAnalyze[2][1], "type");
                        }

                        if($lineToAnalyze[3][0] == TokenVariable){
                            debugLexer("JUMPIFEQ - Good 2");
                            $instruct = $xmlCreator->createXML($instruct, $lineToAnalyze[3][1], $dom, "arg3", "var", "type");
                        }
                        elseif($lineToAnalyze[3][0] == TokenConst){
                            debugLexer("JUMPIFEQ - Good 2");
                            $instruct = $xmlCreator->createXML($instruct, $lineToAnalyze[3][2], $dom, "arg3", $lineToAnalyze[3][1], "type");
                        }

                        break;
                    }
                    else{
                        debugLexer("JUMPIFEQ");
                        fwrite(STDERR, "ERROR: LexERR\n");
                        exit(lexSyntaxError);
                    }
                default:
                    debugLexer("UNEXPECTED");
                    fwrite(STDERR, "ERROR: LexERR\n");
                    exit(lexSyntaxError);
            }
        }
        else{
            exit(opCodeError);
        }
        $root->AppendChild($instruct);
    }
    echo $dom->saveXML();
}

// Call the arg_check function with the command line arguments
arg_check($argv, $argc);

// Call the lexer function
lexer();

// Exit the program with a success status code
exit(OK);

?>
