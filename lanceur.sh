#!/bin/sh
#SBATCH --job-name=job
#SBATCH --partition=normal            # submission queue (normal or long or bigmem or bigpu or quadgpu)
#SBATCH --time=1-1:00:00            # 1-1 means one day and one hour
#SBATCH --mail-type=ALL    # Type can be BEGIN, END, FAIL, ALL(any statchange).
#SBATCH --output=job_seq-%j.out        # if --error is absent, includes also the errors
#SBATCH --mem=8G    # T-tera, G-giga, M-mega
#SBATCH --nodes=4   # 4 cpus CPU Numbers
#SBATCH --mail-user=votre_mail@domain.precis
echo "-----------------------------------------------------------"
echo "hostname                     =   $(hostname)"
echo "SLURM_JOB_NAME               =   $SLURM_JOB_NAME"
echo "SLURM_SUBMIT_DIR             =   $SLURM_SUBMIT_DIR"
echo "SLURM_JOBID                  =   $SLURM_JOBID"
echo "SLURM_JOB_ID                 =   $SLURM_JOB_ID"
echo "SLURM_NODELIST               =   $SLURM_NODELIST"
echo "SLURM_JOB_NODELIST           =   $SLURM_JOB_NODELIST"
echo "SLURM_TASKS_PER_NODE         =   $SLURM_TASKS_PER_NODE"
echo "SLURM_JOB_CPUS_PER_NODE      =   $SLURM_JOB_CPUS_PER_NODE"
echo "SLURM_TOPOLOGY_ADDR_PATTERN  = $SLURM_TOPOLOGY_ADDR_PATTERN"
echo "SLURM_TOPOLOGY_ADDR          =   $SLURM_TOPOLOGY_ADDR"
echo "SLURM_CPUS_ON_NODE           =   $SLURM_CPUS_ON_NODE"
echo "SLURM_NNODES                 =   $SLURM_NNODES"
echo "SLURM_JOB_NUM_NODES          =   $SLURM_JOB_NUM_NODES"
echo "SLURMD_NODENAME              =   $SLURMD_NODENAME"
echo "SLURM_NTASKS                 =   $SLURM_NTASKS"
echo "SLURM_NPROCS                 =   $SLURM_NPROCS"
echo "SLURM_MEM_PER_NODE           =   $SLURM_MEM_PER_NODE"
echo "SLURM_PRIO_PROCESS           =   $SLURM_PRIO_PROCESS"
echo "-----------------------------------------------------------"


instances="instances"

for fichier in "$instances"/*; do
  
  max=0
  while read -r line; do
    for x in $line; do
      if [ "$x" -gt "$max" ]; then
        max="$x"
      fi
    done
  done < "$fichier"
  ./main "16" "$fichier" "$max"
  max=0
done