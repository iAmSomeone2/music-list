#!/usr/bin/env python3
# -*- coding: utf-8 -*-

__author__ = "Brenden Davidson"

import os

def find_license():
    license_txt = ""

    with open("LICENSE", "r") as file:
        license_txt = file.read()

    return license_txt

def locate_cpp_files():
    file_list = []
    for root, dirs, files in os.walk(u"."):
        for file in files:
            file = str(file)
            ext = file.split('.')[-1]
            if ext == "cpp" or ext == "hpp":
                file_path = os.path.join(root, file)
                file_list.append(file_path)

    return file_list

def prepend_license(license_txt, file_path):
    file_txt = ""

    with open(file_path, "r") as file:
        file_txt = file.read()

    out_txt = "/*\n"
    license_lines = license_txt.split("\n")

    for line in license_lines:
        out_txt += "  " + line + "\n"

    out_txt += "*/\n" + file_txt

    with open(file_path, "w") as file:
        file.write(out_txt)

if __name__ == "__main__":
    # Locate license file and load into mem
    license_txt = find_license()

    # Generate list of cpp headers and source files
    cpp_files = locate_cpp_files()

    # Prepend the license to the beginning of each file.
    for path in cpp_files:
        prepend_license(license_txt, path)
