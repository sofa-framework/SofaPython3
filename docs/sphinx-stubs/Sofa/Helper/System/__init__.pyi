import Sofa.Helper.System
import typing

__all__ = [
    "FileRepository"
]


class FileRepository():
    """
    Helper class to find files in a list of directories.

    Each file is searched as follow:

    1: Using the specified filename in current directory, or in the specified directory.
    If the filename does not start with "/", "./", or "../" :
    2: In the directory path specified using addFirstPath method.
    3: In the directory path specified using an environment variable (default to SOFA_DATA_PATH).
    4: In the default directories relative to the main executable (default to ../share).
    5: In the directory path specified using addLastPath method.

    For file name starting with '/', './' or '../' only the first step is used.
    A path is considered as a concatenation of directories separated by : on linux / mac and ; on windows
    """
    def __init__(self, envVar: str = 'SOFA_DATA_PATH', relativePath: str = None, fileKeyMaps: typing.Dict[str, typing.List[str]] = {}) -> None: 
        """
        FileRepository(envVar = 'SOFA_DATA_PATH', relativePath = None, iniFilePath = None)
        :param envVar: Every directory paths found in this environment variable will be automatically added to this file repository
        :param relativePath:
        :param iniFilePath: Every directory paths defined in this INI file will be added to this file repository
        """
    def addFirstPath(self, path: str) -> None: 
        """
        Adds a path to the front of the set of paths.
        """
    def addLastPath(self, path: str) -> None: 
        """
        Adds a path to the back of the set of paths.
        """
    def clear(self) -> None: 
        """
        Remove all known paths.
        """
    @staticmethod
    def entrySeparator() -> str: 
        """
        OS-dependant character separing entries in list of paths.
        """
    @typing.overload
    def findFile(self, filename: str) -> bool: 
        """
        Find file using the stored set of paths.
        :param basefile: override current directory by using the parent directory of the given file
        :param filename: requested file as input, resolved file path as output
        :return: true if the file was found in one of the directories, false otherwise


        Find file using the stored set of paths.
        :param basefile: override current directory by using the parent directory of the given file
        :param filename: requested file as input, resolved file path as output
        :return: true if the file was found in one of the directories, false otherwise
        """
    @typing.overload
    def findFile(self, filename: str, basedir: str) -> bool: ...
    def findFileFromFile(self, filename: str, basefile: str) -> bool: 
        """
        Find file using the stored set of paths.
        :param basefile: override current directory by using the parent directory of the given file
        :param filename: requested file as input, resolved file path as output
        :return: true if the file was found in one of the directories, false otherwise
        """
    @typing.overload
    def getFile(self, filename: str) -> str: 
        """
        Alias for findFile, but returning the resolved file as the result. Less informative for errors, but sometimes easier to use.


        Alias for findFile, but returning the resolved file as the result. Less informative for errors, but sometimes easier to use.
        """
    @typing.overload
    def getFile(self, filename: str, basedir: str) -> str: ...
    def getFirstPath(self) -> str: 
        """
        Get the first path into the set of paths.
        """
    def getPaths(self) -> typing.List[str]: 
        """
        Get the list of directory paths included in this file repository.
        """
    def getPathsJoined(self) -> str: 
        """
        Get the list of directory paths included in this file repository into a string separated by Sofa.Helper.System.FileRepository.entrySeparator().
        """
    def print(self) -> None: 
        """
        Print the list of path to std::cout.
        """
    @staticmethod
    def relativeToPath(path: str, refPath: str) -> str: 
        """
        Returns a string such as refPath + string = path if path contains refPath.
        Otherwise returns path.
        """
    def removePath(self, path: str) -> None: 
        """
        Remove a path of the set of paths.
        """
    @property
    def direct_access_protocol_prefix(self) -> str:
        """
        A protocol like http: or file: which will bypass the file search if found in the filename of the findFile* functions that directly returns the path as if the function succeeded
        Use case: add the prefix ram: as the direct protocol, this way the FileRepository will not try to look for the file on the hard disk and will directly return
        then the inherited FileAccess singleton enhanced with the capacity to find ram file will deliver a correct stream to this in-ram virtual file

        :type: str
        """
    @direct_access_protocol_prefix.setter
    def direct_access_protocol_prefix(self, arg1: str) -> None:
        """
        A protocol like http: or file: which will bypass the file search if found in the filename of the findFile* functions that directly returns the path as if the function succeeded
        Use case: add the prefix ram: as the direct protocol, this way the FileRepository will not try to look for the file on the hard disk and will directly return
        then the inherited FileAccess singleton enhanced with the capacity to find ram file will deliver a correct stream to this in-ram virtual file
        """
    pass
