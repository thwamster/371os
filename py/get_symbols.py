import subprocess
import sys

file_elf = sys.argv[1]
file_input = sys.argv[2]
file_output = sys.argv[3]

symbols = []

with open(file_input) as f:
    for line in f:
        parts = line.strip().split()
        if len(parts) >= 3 and parts[1].lower() == 't':
            address, name = parts[0], parts[2]
            try:
                res = subprocess.check_output(["riscv64-unknown-elf-addr2line", "-e", file_elf, f"0x{address}"],
                                              text=True).strip()
                fi, ln = res.rsplit(':', 1)
                symbols.append((address, name, fi.split('/')[-1], ln))
            except:
                symbols.append((address, name, "unknown", "0"))

with open(file_output, 'w') as f:
    f.write(
        "#include <stdint.h>\n"
        "\n"
        "struct Symbol {\n"
        "\tuint64_t address;\n"
        "\tconst char* name;\n"
        "\tconst char* file;\n"
        "\tint line;\n"
        "};\n"
        "\n"
        "const struct Symbol symbol_table[] = {\n"
    )

    for address, name, file, line in symbols:
        f.write(f"\t{{0x{address}, \"{name}\", \"{file}\", {line}}},\n")

    f.write(
        "\t{0, \"\", \"\", 0}\n"
        "};\n"
        "\n"
    )

    f.write(f"const int symbol_count = {len(symbols)};\n")
