"""
setup.py file for SWIG example
"""

from distutils.core import setup, Extension


ddcutil_module = Extension('_ddcutil',
                           sources=['ddcutil_wrap.c'],
                           libraries=['ddcutil']
                           )

setup(
    name = 'ddcutil',
    version = '0.1',
    author = "JK",
    description = """Simple swig ddcutil from docs""",
    ext_modules = [ddcutil_module],
    py_modules = ["ddcutil"],
)
