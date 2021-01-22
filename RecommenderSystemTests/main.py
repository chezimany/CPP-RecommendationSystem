import difflib
import subprocess
import sys
import time


TESTS = ["RecommendByContent", "PredictMovieScoreForUserS",
         "PredictMovieScoreForUserM", "RecommendByCF"]
ARGS = {"Small": "movies_small.txt ranks_small.txt", "Medium":
    "movies_features.txt ranks_matrix.txt",
        "Big": "movies_big.txt ranks_big.txt"}
USE_VAL = False


def check_valgrind_file(val_file):
    check_lines = -1

    with open(val_file, 'r') as fVal:
        for line in fVal.readlines():
            if "LEAK SUMMARY:" in line:
                check_lines = 0

            if 0 <= check_lines < 5:
                check_lines += 1
                if ": 0 bytes in 0 blocks" not in line:
                    print("\tValgrind test - FAIL")
                    print(
                        "\tMemory leak found, more info in '" + val_file + "'\n")
                    return 0

            if "ERROR SUMMARY:" in line and "ERROR SUMMARY: 0 errors from" not in line:
                print("\tValgrind test - FAIL")
                print("\tError found, more info in '" + val_file + "'")
                return 0

    print("\tValgrind test - PASS")
    return 1


def cmp_files(cmp_file, out_file):
    with open(cmp_file, 'r') as fCmp:
        with open(out_file, 'r') as fOut:
            diff = difflib.ndiff(fOut.readlines(), fCmp.readlines())

            for i, line in enumerate(diff):
                if line.startswith("- ") or line.startswith(
                        "+ ") or line.startswith("? "):
                    print("\tOutput test - FAIL")
                    print(
                        "\tDifference found in '" + out_file + "' at line " + str(
                            i))
                    return 0

            print("\tOutput test - PASS")
            return 1


def run_test(file_name, args, possible_val=False):
    cmp_file = f"cmp/{file_name}_{args}.txt"
    val_file = f"val/{file_name}_{args}_val.txt"
    output_file = f"out/{file_name}_{args}_output.txt"

    print(f"\t{args}:")
    with open(output_file, 'w') as fOut:
        if USE_VAL and possible_val:
            program = "valgrind --leak-check=full --log-file=" + val_file + \
                      " " + file_name + " " + ARGS[args]
        else:
            program = file_name + " " + ARGS[args]
        subprocess.run(program, stdout=fOut, stderr=subprocess.STDOUT,
                       shell=True, cwd="in")

    test_result = cmp_files(cmp_file, output_file)
    val_result = 0
    if USE_VAL and possible_val:
        val_result = check_valgrind_file(f"in/{val_file}")

    return test_result, val_result


def compile_c(file_name):
    CREATE_GCC_EDGE_CASE = f"g++ -Wextra -Wall -Wvla -g -std=c++17  -Wno-unused-parameter" \
                           f" {file_name}.cpp" \
                           f" RecommenderSystem.cpp RecommenderSystem.h -o {file_name}"
    return subprocess.run(CREATE_GCC_EDGE_CASE, text=True, shell=True,
                          cwd="in").returncode


if __name__ == '__main__':
    if "-val" in sys.argv:
        USE_VAL = True
    print("\n------- COMPILATION START -------")
    for test in TESTS:
        if compile_c(test) != 0:
            print("\nProgram failed compiling")
            exit(1)
    print(
        "------- COMPILATION END - The section above should have no warnings nor errors -------")

    print("\n------- TEST START -------")
    num_test_success = 0
    num_val_success = 0
    print("Test RecommendByContent:")
    (t, v) = run_test(TESTS[0], "Small", True)
    num_test_success += t
    num_val_success += v
    print("\n")
    (t, v) = run_test(TESTS[0], "Big", True)
    num_test_success += t
    num_val_success += v
    print("\n")

    print("Test PredictMovieScoreForUser:")
    start = time.time()
    (t, v) = run_test(TESTS[1], "Small", True)
    end = time.time()
    print(f"\tYour time - {end - start}")
    print("\n")

    num_test_success += t
    num_val_success += v
    start = time.time()
    (t, v) = run_test(TESTS[2], "Medium", True)
    end = time.time()
    print(f"\tYour time - {end - start}")
    print("\n")
    num_test_success += t
    num_val_success += v

    print("Test RecommendByCF:")
    start = time.time()
    (t, v) = run_test(TESTS[3], "Small", True)
    end = time.time()
    print(f"\tYour time - {end - start}")
    num_test_success += t
    num_val_success += v
    print("\n")

    start = time.time()
    (t, v) = run_test(TESTS[3], "Big", False)
    end = time.time()
    print(f"\tYour time - {end - start}")
    num_test_success += t
    num_val_success += v
    print("\n")



    print("------- TEST END -------\n")
    print(
        f"Test Pass: {num_test_success}/{6}")
    if USE_VAL:
        print(
            f"Valgrind Pass: {num_val_success}/{5}")
