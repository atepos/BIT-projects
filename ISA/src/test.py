import subprocess
import difflib

green_color_code = "\033[92m"
red_color_code = "\033[91m"
reset_color_code = "\033[0m"

test1 = """Authoritative:Yes,Recursive:Yes,Truncated:No
Question section (1)
 www.fit.vut.cz.,A, IN 
Answer section (1)
 www.fit.vut.cz., A, IN, 14400, 147.229.9.26
Authority section (0)
Additional section (0)"""

def run_dns_program(arguments):
    cmd = ['./dns'] + arguments
    process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    stdout, stderr = process.communicate()
    return process.returncode, stdout.strip()  

def check_output( program_output, expected_output, exit_code, expected_exit_code, test_name):
    result = (program_output == expected_output) and (exit_code == expected_exit_code)
    status = "Passed" if result else "Failed"
    if result:
        print(f"Test {test_name} :{green_color_code} {status}{reset_color_code}")
    else:
        print(f"Test {test_name} : {red_color_code}{status}{reset_color_code}")
        print(f"Expected Output: {expected_output}")
        print(f"Program Output: {program_output}")
        print(f"Expected Exit Code: {expected_exit_code}")
        print(f"Actual Exit Code: {exit_code}")
        print("")


def run_tests(test_cases):
    for (arguments, expected_output, expected_exit_code, test_name) in test_cases:
        exit_code, program_output = run_dns_program(arguments)
        check_output(program_output, expected_output, exit_code, expected_exit_code, test_name)

if __name__ == "__main__":
    test_cases = [
        (["-k", "-s", "8.8.8.8", "www.seznam.cz"], "Error: bad arguments", 1, "Unknown flag -k"),  #nepodporovany argument
        (["-s", "8.8.8.8", "-p", "-p", "53", "www.seznam.cz"], "Error: bad arguments", 1, "Same flag entered multiple times"),  # dvakrat zadany jeden a ten stejny argument
        (["-s", "8.8.8.8", "-m", "-6", "www.seznam.cz"], "Error: bad arguments", 1, "Two query flags entered at the same time"), # dva argumenty dotazu zadane zaroven
        (["-s", "8.8.8.8", "-x", "vutbr.cz"], "Error: Invalid IP address", 1, "Ip address not valid"),  
        (["-s", "8.8.8.8", "-p", "www.seznam.cz"], "Error: bad arguments", 1, "Flag parametr is missing"),  
        (["-s", "8.8.8.adsd", "www.seznam.cz"], "Error: creating Socket", 1, "Invalid server address"),
        (["-s", "8.8.8.8", "jaksemaasdfasdfasdfasdfasdfasdfasdfsadfasdfasdfasdfsadfasdfasdasdasdad"], "Error: DNS Query Format", 1, "Too long label"),
        (["-r", "-s", "kazi.fit.vutbr.cz", "www.fit.vut.cz"], test1, 0, "\"-r -s kazi.fit.vutbr.cz www.fit.vut.cz:\""),
    ]

    run_tests(test_cases)


