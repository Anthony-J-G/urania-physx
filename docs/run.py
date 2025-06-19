import subprocess
import argparse
import os
from pathlib import Path
import shutil


def main(env):
    command = []

    # Ensure proper working directory
    cwd = Path(os.getcwd())    
    if cwd.stem != "docs": 
        # Potentially if you call this from some other folder `docs` this will break, but at that point,
        # if you fuck up that badly it's your own fault.
        print(f"Error: calling `{__file__}` from the wrong working directory")
        exit(1)
    
    if "doxy" in vars(env) and env.doxy:
        command = ["doxygen"]
        subprocess.run(command)    

    if "rebuild" in vars(env) and env.rebuild:
        try:
            shutil.rmtree('_build')
        except FileNotFoundError:
            pass

    command = ["sphinx-autobuild", "source", "_build"]
    if "bind" in vars(env) and env.bind != None:
        command += ["--host", env.bind]
    subprocess.run(command)
    return


if __name__ == "__main__":
    parser = argparse.ArgumentParser(prog='myprogram')
    parser.add_argument('--bind', help='bind autobuild to a specific host address')
    parser.add_argument('--rebuild', type=bool, default=False, action=argparse.BooleanOptionalAction, help="tell autobuild to delete the cached build files and rebuild the docs from scratch")
    parser.add_argument('--doxy', type=bool, default=False, action=argparse.BooleanOptionalAction, help="run doxygen ")
    env = parser.parse_args()

    main(env)
