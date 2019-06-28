from setuptools import setup
import os


def find_stubs(package):
    stubs = []
    for root, dirs, files in os.walk(package):
        for file in files:
            path = os.path.join(root, file).replace(package + os.sep, '', 1)
            stubs.append(path)
    return dict(package=stubs)


setup(
    name='SofaRuntime-stubs',
    maintainer="SofaRuntime Developers",
    maintainer_email="example@python.org",
    description="PEP 561 type stubs for SofaRuntime",
    version='1.0',
    packages=['SofaRuntime-stubs'],
    # PEP 561 requires these
    install_requires=['SofaRuntime'],
    package_data=find_stubs('SofaRuntime-stubs'),
)