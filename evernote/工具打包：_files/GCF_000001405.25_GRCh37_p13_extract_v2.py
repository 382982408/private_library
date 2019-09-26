#!/usr/bin/env python
import re,os,sys
import logging,argparse

def _argparser():
	parser=argparse.ArgumentParser(description='get hg19 exon information')
	parser.add_argument('-i','--input',dest='input',required=True)
	parser.add_argument('-o', '--output',dest='output',required=True)
	parser.add_argument('-v','--version',action='store_true',required=False)
	if len(sys.argv)==1 or ('-v' in sys.argv):
		parser.print_help()
		sys.exit()
	else:
		return parser.parse_args()

class newdict(dict):
	def __missing__(self, key):
		value=self[key]=type(self)()	#type(self)() # retain local pointer to value
		return (value)			  # faster to return than dict lookup
		
def extract_exon(input,output):
	all_info=newdict()
	strand=newdict()
	for line in open(input,"r"):
		if re.match("#",line):
			continue
			OUT.write(line)
		elif re.match("NC_0000",line):		 ###只要1-Y染色体上的
			line=line.strip("\n")
			[chr,r_1,gene,start,end,r_2,gene_strand,r_4,info]=line.split("\t")
			if (gene in "exon") and ("transcript" in info):		   
				gene_name=re.split(";",re.split("gene=",info)[1])[0]
				strand[gene_name]=gene_strand
				transcript=re.split(";",re.split("transcript_id=",info)[1])[0]
				exon_info=[chr,start,end]
				if len(all_info[gene_name][transcript])==0:
					all_info[gene_name][transcript]=[exon_info]
				else:
					all_info[gene_name][transcript].append(exon_info)
			else:
				continue
	####
	OUT=open(output+"_exoninfo.txt","w")
	OUT.write("#chr\tstart\tend\texon_num\tgene\ttranscript\tstrand\n")
	for gene in sorted(all_info):
		for transcript in sorted(all_info[gene]):
			num=len(all_info[gene][transcript])
			if strand[gene] in "+":
				for i in range(0,num):
					[chr,start,end]=all_info[gene][transcript][i]
					OUT.write(f"{chr}\t{start}\t{end}\texon:{i+1}\t{gene}\t{transcript}\t{strand[gene]}\n")
			else:
				for i in range(0,num):
					[chr,start,end]=all_info[gene][transcript][i]
					OUT.write(f"{chr}\t{start}\t{end}\texon:{num-i}\t{gene}\t{transcript}\t{strand[gene]}\n")
	OUT.close()

def extract_cds(input,output):
	all_info=newdict()
	strand=newdict()
	for line in open(input,"r"):
		if re.match("#",line):
			continue
			OUT.write(line)
		elif re.match("NC_0000",line):		 ###只要1-Y染色体上的
			line=line.strip("\n")
			[chr,r_1,gene,start,end,r_2,gene_strand,r_4,info]=line.split("\t")
			if (gene in "CDS") and ("protein_id" in info):	   
				gene_name=re.split(";",re.split("gene=",info)[1])[0]
				strand[gene_name]=gene_strand
				transcript=re.split(";",re.split("protein_id=",info)[1])[0]
				exon_info=[chr,start,end]
				if len(all_info[gene_name][transcript])==0:
					all_info[gene_name][transcript]=[exon_info]
				else:
					all_info[gene_name][transcript].append(exon_info)
			else:
				continue
	####
	OUT=open(output+"_cdsinfo.txt","w")
	OUT.write("#chr\tstart\tend\tcds_num\tgene\tprotein_id\tstrand\n")
	for gene in sorted(all_info):
		for transcript in sorted(all_info[gene]):
			num=len(all_info[gene][transcript])
			if strand[gene] in "+":
				for i in range(0,num):
					[chr,start,end]=all_info[gene][transcript][i]
					OUT.write(f"{chr}\t{start}\t{end}\tcds:{i+1}\t{gene}\t{transcript}\t{strand[gene]}\n")
			else:
				for i in range(0,num):
					[chr,start,end]=all_info[gene][transcript][i]
					OUT.write(f"{chr}\t{start}\t{end}\tcds:{num-i}\t{gene}\t{transcript}\t{strand[gene]}\n")
	OUT.close()
	
def extract_gene(input,output):
    OUT=open(output+"_geneinfo.txt","w")
    OUT.write(f"#chr\tstart\tend\tgene_id\tgene_name\tgene_alias\tgene_type\tgene_strand\n")
    for line in open(input,"r"):
        if re.match("#",line):
            continue
            OUT.write(line)
        elif re.match("NC_0000",line):       ###只要1-Y染色体上的
            line=line.strip("\n")
            [chr,r_1,gene,start,end,r_2,gene_strand,r_4,info]=line.split("\t")
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
                OUT.write(f"{chr}\t{start}\t{end}\t{gene_id}\t{gene_name}\t{gene_alias}\t{gene_type}\t{gene_strand}\n")
            else:
                continue
    OUT.close()
	
def _main():
	logging.basicConfig(format='%(levelname)s:%(asctime)s\t%(message)s',level = logging.DEBUG,datefmt = '%Y-%m-%d %H:%M:%S')
	argvs=_argparser()
	logging.info("work start ...")	
#	extract_gene(argvs.input,argvs.output);
#	extract_exon(argvs.input,argvs.output);
	extract_cds(argvs.input,argvs.output);
	logging.info("work end ...")

if __name__ == '__main__':
   _main()
