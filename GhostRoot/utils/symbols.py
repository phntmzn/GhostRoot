
# Python script to find _realhost symbol in mach_kernel using nm or jtool
import subprocess
import re
import sys

def extract_realhost_symbol(kernel_path):
    try:
        # Try extracting symbol using nm
        result = subprocess.run(['nm', '-g', kernel_path], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        if result.returncode != 0:
            print("[-] nm failed, trying jtool...")
            result = subprocess.run(['jtool', '--symbols', kernel_path], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
            if result.returncode != 0:
                print("[-] jtool failed too.")
                return None

        # Look for _realhost symbol
        for line in result.stdout.splitlines():
            if '_realhost' in line:
                match = re.search(r'(0x)?[0-9a-fA-F]+', line)
                if match:
                    print(f"[+] Found _realhost at {match.group(0)}")
                    return match.group(0)
        print("[-] _realhost not found.")
    except Exception as e:
        print(f"[!] Error: {e}")
    return None

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <mach_kernel>")
        sys.exit(1)
    extract_realhost_symbol(sys.argv[1])
