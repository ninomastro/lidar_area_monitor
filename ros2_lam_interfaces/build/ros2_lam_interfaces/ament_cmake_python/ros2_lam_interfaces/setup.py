from setuptools import find_packages
from setuptools import setup

setup(
    name='ros2_lam_interfaces',
    version='0.0.1',
    packages=find_packages(
        include=('ros2_lam_interfaces', 'ros2_lam_interfaces.*')),
)
