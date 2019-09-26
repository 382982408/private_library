#!/usr/bin/env python
import sys,os,re,time,argparse,logging,subprocess

file_gff="/share/ofs1a/EXdev/database/hg_19_gff/GCF_000001405.25_GRCh37.p13_genomic.gff"
hg19_fa="/share/ofs1a/EXdev/WES_pipe_v4/db/J_hg19_reference/hg19_reference_with_NC.fasta"
samtools="/share/ofs1a/EXdev/WES_pipe_v4/bin/samtools"
NC2chr={"NC_000001":"chr1","NC_000002":"chr2","NC_000003":"chr3","NC_000004":"chr4","NC_000005":"chr5",\
"NC_000006":"chr6","NC_000007":"chr7","NC_000008":"chr8","NC_000009":"chr9","NC_000010":"chr10","NC_000011":"chr11",\
"NC_000012":"chr12","NC_000013":"chr13","NC_000014":"chr14","NC_000015":"chr15","NC_000016":"chr16","NC_000017":"chr17",\
"NC_000018":"chr18","NC_000019":"chr19","NC_000020":"chr20","NC_000021":"chr21","NC_000022":"chr22","NC_000023":"chrX","NC_000024":"chrY"}

HELP="""
    This program is used to get the genome position of some variant from the gff file !
"""

class _newdict(dict):
    def __missing__(self, key):
        value=self[key]=type(self)()    #type(self)() # retain local pointer to value
        return (value)            # faster to return than dict lookup

def _argparses():
    parser = argparse.ArgumentParser(description='To anno genome position from gff !')
    parser.add_argument('-g',required=True,dest='gene_pos',help="gene_position:  GLUD1P7:c.1 (SpecialNotice:Focus only on variations in coding regions !)")
    parser.add_argument('-c',required=False,dest='chr_pos',help="chr_position:  chr1:1000000")
    parser.add_argument('-v','--version',required=False,action='store_true',help="virsion information")
    if len(sys.argv) == 1:
        parser.print_help()
        sys.exit(0)
    elif ('-v' in sys.argv) or ('--version' in sys.argv):
        print(HELP)
        sys.exit(0)
    else:
        return parser.parse_args()

def _get_gene_info(gene_name):
    gene_info=_newdict()
    rna_name=_newdict()
    for line in open(file_gff,"r"):
        if line[0] == "#":continue
        line=line.strip("\n")
        line_arr=line.split("\t")
        line_arr[0]=line_arr[0].split(".")[0]
        if ("exon" in line_arr[2]) and ("Parent=" in line_arr[8]) and ("transcript_id=" in line_arr[8]):
            parent_name=line_arr[8].split("Parent=")[1].split(";")[0]
            nm_name=line_arr[8].split("transcript_id=")[1].split(";")[0]
            rna_name[parent_name]=nm_name
        if (not "CDS" in line_arr[2]) or (not "gene=" in line_arr[8]) or (not "protein_id=" in line_arr[8]):continue
        tmp_gene_name=line_arr[8].split("gene=")[1].split(";")[0]
        tmp_np=line_arr[8].split("protein_id=")[1].split(";")[0]
        parent_name=line_arr[8].split("Parent=")[1].split(";")[0]
        tmp_nm=f"{rna_name[parent_name]}({tmp_np})"
        if tmp_gene_name == gene_name:
            if len(gene_info[tmp_nm])==0:
                gene_info[tmp_nm]["info"]=[]
                gene_info[tmp_nm]["info"].append([line_arr[0],line_arr[3],line_arr[4]])
                gene_info[tmp_nm]["count"]=0
                gene_info[tmp_nm]["strand"]=line_arr[6]
                gene_info[tmp_nm]["length"]=0
            else:
                gene_info[tmp_nm]["info"].append([line_arr[0],line_arr[3],line_arr[4]])
                gene_info[tmp_nm]["count"]+=1
                gene_info[tmp_nm]["length"]+=int(line_arr[4])-int(line_arr[3])+1
    return gene_info

def _get_fasta(position):
    [tmp_chr,tmp_start,*rubish]=re.split(r":|-",position)
    if "NC" in tmp_chr:
        tmp_chr=NC2chr[tmp_chr]
    tmp_fa=subprocess.getoutput(f"{samtools} faidx {hg19_fa} {tmp_chr}:{tmp_start}-{tmp_start}|tail -1")
    tmp_fa=tmp_fa.strip("\n")
    return tmp_fa
    
def _get_cds_info(gene_info,gene_input,gene,origin_cds_pos,origin_intro_pos,pos_index):
    for nm in sorted(gene_info.keys()):
        cds_pos=origin_cds_pos
        nm_count=gene_info[nm]["count"]
        nm_strand=gene_info[nm]["strand"]
        cds_length=gene_info[nm]["length"]
        cds_info=gene_info[nm]["info"]
        [cds_start,cds_end,cds_num]=[0,0,0]
        if "-" in nm_strand:
            for [tmp_chr,tmp_start,tmp_end] in cds_info:
                cds_num+=1
                cds_start=cds_end+1
                cds_end+=int(tmp_end)-int(tmp_start)+1
                if cds_pos >=cds_start and cds_pos<=cds_end:
                    real_pos=int(tmp_end)-(cds_pos-cds_start)
                    if (origin_intro_pos >0) and ("down" in pos_index):
                        if real_pos==int(tmp_start):
                            real_pos=real_pos-origin_intro_pos
                        else:
                            ###位置不在变体边缘，不是同一个变体
                            break
                    elif (origin_intro_pos >0) and ("up" in pos_index):
                        if real_pos==int(tmp_end):
                            real_pos=real_pos+origin_intro_pos
                        else:
                            ###位置不在变体边缘，不是同一个变体
                            break
                    base=_get_fasta(f"{tmp_chr}:{real_pos}")
                    print (f"{tmp_chr}:{real_pos}({base})\tcds{cds_num}:{tmp_chr}:{tmp_start}-{tmp_end}\t{nm};tot_cds={nm_count};strand={nm_strand}\t{gene_input}")
                    break
                else:
                    continue ####只要是在CDS以内,一定在上面的情况里面才对
        elif "+" in nm_strand:
            for [tmp_chr,tmp_start,tmp_end] in cds_info:
                cds_num+=1
                cds_start=cds_end+1
                cds_end+=int(tmp_end)-int(tmp_start)+1
                if cds_pos >=cds_start and cds_pos<=cds_end:
                    real_pos=cds_pos-cds_start+int(tmp_start)
                    if (origin_intro_pos >0) and ("down" in pos_index):
                        if real_pos==int(tmp_end):
                            real_pos=real_pos+origin_intro_pos
                        else:
                            ###位置不在变体边缘，不是同一个变体
                            break
                    elif (origin_intro_pos >0) and ("up" in pos_index):
                        if real_pos==int(tmp_start):
                            real_pos=real_pos-origin_intro_pos
                        else:
                            ###位置不在变体边缘，不是同一个变体
                            break
                    base=_get_fasta(f"{tmp_chr}:{real_pos}")
                    print (f"{tmp_chr}:{real_pos}({base})\tcds{cds_num}:{tmp_chr}:{tmp_start}-{tmp_end}\t{nm};tot_cds={nm_count};strand={nm_strand}\t{gene_input}")
                    break
                else:
                    continue ####只要是在CDS以内,一定在上面的情况里面才对
        else:
            print (f"error:gene_strand is error:{nm_strand}\n")
            sys.exit(0)

def _gene_convert(gene_pos):
    [gene,other_info]=re.split(r":c.",gene_pos)
    gene_info=_get_gene_info(gene)
    if other_info[0]==r"-":
        ###以5'utr定位
        print ("error:utr in 5'\n")
        sys.exit(0)
    elif other_info[0]==r"*":
        ###以3'utr定位
        print ("error:utr in 3'\n")
        sys.exit(0)
    else:
        ###以cds定位
        pos_index="down"
        if '+' in other_info:
            ###3‘内含子
            [cds_pos,intro_pos]=re.split(r"\+",other_info)
            pos_index="down"
        elif '-' in other_info:
            ###5'内含子
            [cds_pos,intro_pos]=re.split(r"\-",other_info)
            pos_index="up"
        else:
            ###在cds上
            [cds_pos,intro_pos]=[other_info,0]
            pos_index="down"
#        print (f"{cds_pos}\t{intro_pos}\t{pos_index}")
#        sys.exit(0)
        _get_cds_info(gene_info,gene_pos,gene,int(cds_pos),int(intro_pos),pos_index)

def _chr_convert(chr_pos):
    print ("here is chr_convert!")

def _main():
    logging.basicConfig(format='%(levelname)s %(asctime)s,line %(lineno)s:\t%(message)s',level=logging.DEBUG,datefmt='%Y-%m-%d %H:%M:%S')
    argvs = _argparses()
#    print (argvs)
#    logging.info("work start")
    if argvs.gene_pos != None:
        _gene_convert(argvs.gene_pos)
    elif argvs.chr_pos != None:
        _chr_convert(argvs.chr_pos)
    else:
        print ("error:input error !!!\n")

if __name__ == '__main__':
    _main()

