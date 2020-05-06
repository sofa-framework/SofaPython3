#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
.. autosummary::

   cacher

splib.caching.cacher.cacher
*********************
.. autofunction:: cacher

"""

import logging

def extractOptions(kwargsdict):
    
    OptionsStrings = []
    Values = []
        
    for key, value in kwargsdict.items():
        OptionString = key
        OptionValue = value    
        OptionsStrings.append(OptionString)
        Values.append(OptionValue)
    
    return OptionsStrings, Values

def cacher(InputFilePath, OutputDir, OutputFileExtension, kwargsdict, OutputFileName=None):
    """
    This is a first, simple version of a cashing system that takes a file and a list of arguments (through the kwargs) and:
        - generates a hash from the file content, the target file extension and the arguments used generate the target file
        - checks in the target directory if an output has been already generated from the inputs using the hash
        - returns a filename under which the calling function can store or retrieve the generated data (that will be managed by the casher in future calls)
    It has two types of behavior:
        - OneShot --> When an output filename is provided, the old file is replaced by the new one, the filename is 'human-readable'. This is a cache of size 1.
        - Persistent --> Previous files are not overwritten, the files are named with the hash string. In this way the all the generated files with different parameters are 'cached'.
    """
    import os
    import hashlib
    import numpy as np
    
    if not os.path.isdir(OutputDir):
        os.mkdir(OutputDir)
        
    
    OptionsStrings, Values = extractOptions(kwargsdict)
    
    # Hashing
    SortingIdxs = np.argsort(OptionsStrings) # sort options by name to be insensitive to the order they are passed    
    ParametricGeometryFile = open(InputFilePath)
    # Warning: here we are not taking into account that the file could use a large amount of memory
    FileContents = ParametricGeometryFile.read()
    
    # hash the file contents
    FileAndOptionsHashObj = hashlib.sha256(FileContents.encode('utf-8'))
    
    # add the options strings to the hash
    for i in SortingIdxs:
        ArgsForHash = OptionsStrings[i] + '=' + str(Values[i]) + ';'
        FileAndOptionsHashObj.update(ArgsForHash.encode('utf-8'))
    
    # Finally, add output file extension to the hash, so that different target files from the same source will be treated differently (e.g. Surface and Volumetric meshes)
    FileAndOptionsHashObj.update(OutputFileExtension.encode('utf-8'))
    
    # Get the hash string and verify if it was previously generated    
    HashStr = FileAndOptionsHashObj.hexdigest()    
    # OneShot
    if OutputFileName != None: 
        HashFilePath = OutputDir + OutputFileName + OutputFileExtension + '.hash'
        FilePath = OutputDir + OutputFileName + OutputFileExtension 
        if os.path.exists(HashFilePath):
            HashFileRead = open(HashFilePath,'r')
            OldHashStr = HashFileRead.readline()
            if OldHashStr == HashStr+'\n':
                logging.info('Cacher: ' + FilePath + ': Found a file with an identical hash. Returning from cache.')                
                return False, FilePath
        
        # If hash is different or non-existent write hash (+options) info to file                 
        HashFile = open(HashFilePath, 'w+')
        HashFile.write(HashStr+'\n')
        HashFile.write('# Options:\n')
                       
        for i in SortingIdxs:
                HashFile.write('# ' + OptionsStrings[i]+'='+str(Values[i])+';\n')
        HashFile.close()
        return True, FilePath
    
    
    # Persistent
    else:
        HashedFileName = HashStr + OutputFileExtension
        HashedFilePath = OutputDir + HashedFileName
        if os.path.exists(HashedFilePath):    
            logging.info('Cacher: ' + HashedFilePath + ': Found a file with an identical hash. Returning from cache.')                
            return False, HashedFilePath
        else:
            return True, HashedFilePath
