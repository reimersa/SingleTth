#!/bin/zsh

echo "Jobname: " $1

first=$1
last=$(( $first+9 ))
cmd="root -l -b -q 'make_all.C($first, $last)'"

echo ${cmd}

cat > job.$1.submit <<- _EOF_
#!/bin/zsh
#
# (only accept jobs with correct resources)
#$ -w e
#
# (stderr and stdout are merged together to stdout)
#$ -j y
#$ -o /nfs/dust/cms/user/kogler/Pythia8/pythia8210/ttbar/output
#
# (send mail on job's end and abort)
#$ -m aes
#$ -M roman.kogler@desy.de
#
# (put log files into current working directory)
#$ -cwd
#
# (use CMS project)
#$ -P cms
#
#$ -l h_vmem=2G
#
# (run on SLD6 machines)
#$ -l distro=sld6
#
# (run in hamburg)
#$ -l site=hh
#
# (give the job a name)
#$ -N ntuple_${1}

# source my login script to get the environment variables
source ~/.zshrc
ini6
cd /afs/desy.de/products/root/amd64_rhel60/5.34.00
source bin/thisroot.sh
echo "ROOT version:"
which root
root-config --version

# execute Ntuple generation
cd /nfs/dust/cms/user/kogler/Pythia8/pythia8210/ttbar/NtupleWriter
${cmd}

exit 0
_EOF_

echo "submitting job..."
qsub -q short.q job.$1.submit

echo "moving submit script to directory ""sub"""
mkdir -p sub
mv job.$1.submit sub
