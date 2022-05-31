#! /usr/bin/env python

import os, sys, math, time
import subprocess
from collections import OrderedDict


def submit_limits(MA, blimit = True, bbias = True , bsigInj = True):
    MA=125
    b_only125 = False
    MTs_limit = [600, 625, 650, 675, 700, 725, 750, 775, 800, 825, 850, 875, 900, 925, 950, 975, 1000, 1025, 1050, 1075, 1100, 1125, 1150, 1175]
    if b_only125:
        MTs_limit = [600, 625, 650, 675, 700]
        
    scriptfolder = "/nfs/dust/cms/user/abenecke/CMSSW_10_2_17/CMSSW_10_2_17/src/UHH2/SingleTth/macros/roofit/datacards/"


    if blimit:
        for MT in MTs_limit:
            run_limits(MA,MT, scriptfolder)


    MTs_bias = [600,700,800,1000]
    rs_bias = [0.0,1.0]
    if bbias:
        for MT in MTs_bias:
            for r in rs_bias:
                for function in range(0,2):
                                
                    run_bias(MA,MT,r,function,scriptfolder)
    

    rs_signal = [0.00, 0.10, 0.25, 0.35, 0.50, 0.75, 1.00]
    if bsigInj:
        for MT in MTs_bias:
            for r in rs_signal:
                run_signal(MA,MT,r,scriptfolder)
    


def run_limits(MA, MT, scriptfolder):
    print "MA=" + str(MA) + " MT="+str(MT)

    workdir = os.path.join(scriptfolder,"workdir_limits_condor")
    ensureDirectory(workdir)

    submissionsettings = derive_from_default_settings("run_combine_condor.sh",'"%i"' % (MT),"limits", scriptfolder, workdir, MA, MT)
    submissionscriptname = os.path.join(workdir, 'submit_combine_condor.sub')
    create_submitfile(settings=submissionsettings, outfilename=submissionscriptname)
    command = 'condor_submit %s' % (submissionscriptname)
    os.system(command)

def run_bias(MA, MT, r, function, scriptfolder):
    print "MA=" + str(MA) + " MT="+str(MT) + " r="+str(r)+" function="+str(function)

    workdir = os.path.join(scriptfolder,"workdir_bias_condor")
    ensureDirectory(workdir)

    submissionsettings = derive_from_default_settings("run_biastest_condor.sh",'"%i %.1f %i %i"' % (MT,r,function, MA),"bias", scriptfolder, workdir, MA, MT)
    submissionscriptname = os.path.join(workdir, 'submit_combine_condor.sub')
    create_submitfile(settings=submissionsettings, outfilename=submissionscriptname)
    command = 'condor_submit %s' % (submissionscriptname)
    os.system(command)

def run_signal(MA, MT, r, scriptfolder):
    print "MA=" + str(MA) + " MT="+str(MT)+" r="+str(r)

    workdir = os.path.join(scriptfolder,"workdir_signal_condor")
    ensureDirectory(workdir)

    submissionsettings = derive_from_default_settings("run_signalinjection_condor.sh",'"%i %.2f %i"' % (MT,r, MA),"signal", scriptfolder, workdir, MA, MT)
    submissionscriptname = os.path.join(workdir, 'submit_combine_condor.sub')
    create_submitfile(settings=submissionsettings, outfilename=submissionscriptname)
    command = 'condor_submit %s' % (submissionscriptname)
    os.system(command)


def derive_from_default_settings(executable,arguments,jobname_base, scriptfolder, workdir, MA, MT):
    submissionsettings = OrderedDict([
                ('executable' ,  os.path.join(scriptfolder, executable)),
                ('output'     ,  os.path.join(workdir, '%s_MA%i_MT%i_$(ClusterId).out'%(jobname_base,MA,MT))),
                ('error'      ,  os.path.join(workdir, '%s_MA%i_MT%i_$(ClusterId).err'%(jobname_base,MA,MT))),
                ('log'        ,  os.path.join(workdir, '%s_MA%i_MT%i_$(ClusterId).log'%(jobname_base,MA,MT))),
                ('environment', 'ClusterId=$(ClusterId);PATH_STORED=%s;LD_LIBRARY_PATH_STORED=%s' % (os.environ['PATH'], os.environ['LD_LIBRARY_PATH'])),
                ('arguments'  , arguments),
                ('transfer_output_files', '""'),
                ('stream_output', 'True'),
                ('stream_error', 'True'),
                ('+RequestRuntime', '3600'),
                ('RequestMemory', '1024'),
                ('RequestDisk', '1048576'),
                ('initialdir', scriptfolder),
                ('getenv', "True"),
                ('JobBatchName', "%s_%i_%i"%(jobname_base,MA,MT)),
                ('queue'      , '')
            ])

    return submissionsettings

def ensureDirectory(dirname):
    """Make directory if it does not exist."""
    if not os.path.exists(dirname):
        os.makedirs(dirname)
    if not os.path.exists(dirname):
        print '--> failed to make directory "%s"'%(dirname)

def create_submitfile(settings, outfilename):
    # 'settings' must be a dictionary, with keys corresponding to the names of the .sub settings and the values corresponding to their values as a single string.

    with open(outfilename, 'wr') as f:
        newlines = []
        for setting in settings:
            line = '%s = %s\n' % (setting, settings[setting])
            if setting == 'queue':
                line = line.replace(' = ', ' ')
            newlines.append(line)
        for l in newlines:
            f.write(l)

# if __name__ == '__main__':
#     main()
