import os
import subprocess
import platform
import argparse

# arguments
parser = argparse.ArgumentParser()
parser.add_argument('--generate',   action='store_true', help="Generates build project")
parser.add_argument('--build',      action='store_true', help="Builds project")
parser.add_argument('--run',        action='store_true', help="Executes project")

args = parser.parse_args()


# build commands
def get_build_folder():
    build_folder = f'build/{platform.system()}'
    if not os.path.exists(build_folder):
        os.makedirs(build_folder)
    return build_folder

def get_bin_folder():
    return f'bin/{platform.system()}'

def run_executable():
    if platform.system() == "Linux":
        os.environ['MESA_GL_VERSION_OVERRIDE'] = '4.5'

    # TODO: Move Sandbox to shared config file (something equivalent to .uproject)
    # TODO: Add flag that toggles running after building
    os.chdir(get_bin_folder())
    subprocess.run([f'./Sandbox'])

def run_cmake_build():
    try:
        build_folder = get_build_folder() 

        if args.generate:
            subprocess.run(['cmake', '-B', build_folder, '-S', '.'])
        if args.build:
            subprocess.run(['cmake', '--build', build_folder, '--config', 'debug'])
        if args.run:
            run_executable()
    except subprocess.CalledProcessError as e:
        print("CMake build failed with return code {e.returnCode}.")


if __name__ == "__main__":
    run_cmake_build()

