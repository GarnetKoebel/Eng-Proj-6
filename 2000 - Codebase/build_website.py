# This program builds the website into a more URL friendly style so that the project organization structure can be maintained

from os import mkdir, getcwd, rename, listdir, rmdir, system
from os.path import join, isdir
from shutil import copytree, rmtree

def create_dir(dir_name, path):
    try:
        mkdir(join(path, dir_name))

    except FileExistsError:
        print("Directory Already Exists, Emptying Directory")
        rmtree(join(path, dir_name))
        print("Directory Deleted, Creating Emtpy Directory")

    except FileNotFoundError:
        print("Error: Invalid Directory Path")

def copy_dir_tree(path_to_dir):
    copytree(join(base_path, path_to_dir), output_path)

def rename_folders():
    for dir in listdir(output_path): # list all copied folders
        if not isdir(join(output_path, dir)):
            continue # don't rename files
        else:
            dir_name = dir # get dir name as string
            dir_name = dir_name.split('- ')[1] # strip out leading info
            rename(join(output_path, dir), join(output_path, dir_name)) # rename directory

base_path = getcwd()
output_path = join(base_path, "output", "output")

# try to create output directory in case it doesn't exist
print("Creating Output Directory")
create_dir("output", base_path)

# Copy codebase file directories for front-end
print("Copying front-end files")
copy_dir_tree("2100 - Front End")

# Rename copied folders stripping leading organization numbers
print("Renaming files")
rename_folders()

# Upload built website to target
system('scp -r "%s" "%s":"%s"' % (output_path, "pi@192.168.1.202", "/home/pi"))
