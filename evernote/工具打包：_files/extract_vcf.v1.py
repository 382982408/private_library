#!/usr/bin/env python
import os,sys,re,getopt,gzip

opts,r_para=getopt.getopt(sys.argv[1:],"hi:o:")				########short_parameters	
#opts,rubish=getopt.getopt(sys.argv[1:],"h",["i=","o="]);		########long_parameters
def help(prog):
	print (("\nUsage:\n\tpython %s <-i:inputfile> <-o:outputfile>\n\n") % (prog));
	sys.exit(0);
def mut_revise_from_head(tmp_chr,tmp_start,tmp_end,tmp_ref,tmp_alt,num,tmp_info):
	tmp_start=int(tmp_start);
	tmp_end=int(tmp_end);
	while 1:
		if len(tmp_ref)<1 or len(tmp_alt)<1:
			break;
		elif tmp_ref[0] == tmp_alt[0]:
			tmp_ref=tmp_ref[1:];
			tmp_alt=tmp_alt[1:];
			tmp_start+=1;
		else:
			break;
	while 1:
		if len(tmp_ref)<1 or len(tmp_alt)<1:
			break;
		elif tmp_ref[-1] == tmp_alt[-1]:
			tmp_ref=tmp_ref[0:-1];
			tmp_alt=tmp_alt[0:-1];
		else:
			break;
	if  len(tmp_ref)==1 and len(tmp_alt)==1:
		tmp_end=tmp_start;
	elif len(tmp_ref)<1 and len(tmp_alt)>=1:
		tmp_start-=1;
		tmp_end=tmp_start+1;
		tmp_ref="-";
	elif len(tmp_ref)>=1 and len(tmp_alt)<1:
		tmp_end=tmp_start+len(tmp_ref)-1;
		tmp_alt="-";
	else:
		tmp_end=tmp_start+len(tmp_ref)-1;
	return(tmp_chr+"\t"+str(tmp_start)+"\t"+str(tmp_end)+"\t"+tmp_ref+"\t"+tmp_alt+"\t"+str(num)+"\t"+tmp_info);
def mut_revise_from_tail(tmp_chr,tmp_start,tmp_end,tmp_ref,tmp_alt,num,tmp_info):
	tmp_start=int(tmp_start);
	tmp_end=int(tmp_end);
	while 1:
		if len(tmp_ref)<1 or len(tmp_alt)<1:
			break;
		elif tmp_ref[-1] == tmp_alt[-1]:
			tmp_ref=tmp_ref[0:-1];
			tmp_alt=tmp_alt[0:-1];
		else:
			break;
	while 1:
		if len(tmp_ref)<1 or len(tmp_alt)<1:
			break;
		elif tmp_ref[0] == tmp_alt[0]:
			tmp_ref=tmp_ref[1:];
			tmp_alt=tmp_alt[1:];
			tmp_start+=1;
		else:
			break;
	if  len(tmp_ref)==1 and len(tmp_alt)==1:
		tmp_end=tmp_start;
	elif len(tmp_ref)<1 and len(tmp_alt)>=1:
		tmp_start-=1;
		tmp_end=tmp_start+1;
		tmp_ref="-";
	elif len(tmp_ref)>=1 and len(tmp_alt)<1:
		tmp_end=tmp_start+len(tmp_ref)-1;
		tmp_alt="-";
	else:
		tmp_end=tmp_start+len(tmp_ref)-1;
	return(tmp_chr+"\t"+str(tmp_start)+"\t"+str(tmp_end)+"\t"+tmp_ref+"\t"+tmp_alt+"\t"+str(num)+"\t"+tmp_info);

#############
Dict_para={};
for key,value in opts:
	if key=="-i":
		Dict_para['i']=value;
	elif key=="-o":
		Dict_para['o']=value;
if 'i' in Dict_para and 'o' in Dict_para:
	print ("work start...\n");
else:
	help(sys.argv[0]);
	sys.exit(0);
####################
OUT=open(Dict_para['o'],"w");
for LINE in open(Dict_para['i'],"r"):
	if re.match("#",LINE):
		OUT.write(LINE);
	else:
		(chr,start,rs,ref,alt,*tmp)=LINE.split("\t");
		info="\t".join(tmp)
		ALTS=alt.split(",");
		num=1;
		for tmp_alt in ALTS:
			info_1=mut_revise_from_head(chr,start,start,ref,tmp_alt,num,info);
			info_2=mut_revise_from_tail(chr,start,start,ref,tmp_alt,num,info);
			if info_1 == info_2:
				OUT.write(info_1);
			else:
				OUT.write(info_1);
				OUT.write(info_2);
			num+=1;
			continue;
OUT.close();
print ("work end...\n");
