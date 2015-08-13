#!/usr/bin/python

from __future__ import print_function
import sys
sys.dont_write_bytecode = True # prevent creation of .pyc files
import re

###
### This file has helper functions to get parameters out of paramILS results
### file.
###

#Parses a ParamILS result file, returns the parameters in ParamILS format
def getResultFileParams(resultFilename):
    with open(resultFilename, 'r') as resultFile:
        rawArgs = re.search(r'Active parameters: (.*)',
                            resultFile.read()).group(1)
    #Break in to individual args
    rawArgs = rawArgs.split(', ')
    #Add SMACK wrapper's expected "-" to the front
    rawArgs = ['-' + arg for arg in rawArgs]
    #Break arg and value into two pieces
    rawArgs = [smplArg for cmplxArg in rawArgs for smplArg in cmplxArg.split('=')]
    return rawArgs

###Parses a ParamILS result file, returns the cutoff time used for experiment
def getResultFileCutoffTime(resultFilename):
    with open(resultFilename, 'r') as resultFile:
        cutoff = re.search(r'.*cutoff (.*)', resultFile.read()).group(1)
    return int(float(cutoff))

###Reads a ParamILS scenario file, returns the key/value pairs as a python dict
def getScenarioDictionary(scenarioFilename):
    scenarioDict = dict()
    with open(scenarioFilename, 'r') as f:
        lines = f.readlines()
    for line in lines:
        tokens = line.split('=',1)
        scenarioDict[tokens[0].strip()] = tokens[1].strip()
    return scenarioDict
