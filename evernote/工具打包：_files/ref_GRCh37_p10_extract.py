#!/share/ofs1a/EXdev/WES_pipe_v4/bin/Anaconda/bin/python3
import re,os,sys
import logging,argparse

def _argparser():
    parser=argparse.ArgumentParser(description='get hg19 gene information')
    parser.add_argument('-i','--input',dest='input',required=True)
    parser.add_argument('-o', '--output',dest='output',required=True)
    parser.add_argument('-v','--version',action='store_true',required=False)
    if len(sys.argv)==1 or ('-v' in sys.argv):
        parser.print_help()
        sys.exit()
    else:
        return parser.parse_args()
def _extract(input,output):
    OUT=open(output,"w")
    OUT.write(f"#chr\tstart\tend\tgene_id\tgene_name\tgene_alias\tgene_type\n")
    for line in open(input,"r"):
        if re.match("#",line):
            continue
            OUT.write(line)
        elif re.match("chr",line):       ###只要1-Y染色体上的
            line=line.strip("\n")
            [chr,r_1,gene,start,end,r_2,r_3,r_4,info]=line.split("\t")
            if gene in "gene":          ###只要基因的
                [gene_id,gene_name,gene_alias,gene_type]=["NA","NA","NA","NA"];
                ############
                re_tmp_1= re.search(r"Dbxref=GeneID:(.*?)[,;]",info)
                if re_tmp_1 != None:
                    gene_id=re_tmp_1.group(1)
                ##########
                re_tmp_1= re.search(r"Name=([^;,]+)",info)
                if re_tmp_1 != None:
                    gene_name=re_tmp_1.group(1)
                ##########
                re_tmp_1 = re.search(r"gene_synonym=([^;]+)", info)
                if re_tmp_1 != None:
                    gene_alias = re_tmp_1.group(1)
                ##########
                re_tmp_1= re.search(r"gene_biotype=([^;]+)",info)
                if re_tmp_1 != None:
                    gene_type=re_tmp_1.group(1)
                ##########
                OUT.write(f"{chr}\t{start}\t{end}\t{gene_id}\t{gene_name}\t{gene_alias}\t{gene_type}\n")
            else:
                continue
    OUT.close()
def _main():
    logging.basicConfig(format='%(levelname)s:%(asctime)s\t%(message)s',level = logging.DEBUG,datefmt = '%Y-%m-%d %H:%M:%S')
    logging.info("work start ...")
    argvs=_argparser()
    _extract(argvs.input,argvs.output);
    logging.info("work end ...")

if __name__ == '__main__':
   _main()
