#!/usr/bin/perl -w
use strict;
use Cwd;
use Getopt::Long;
use Data::Dumper;
use FindBin qw($Bin $Script);
use File::Basename qw(basename dirname);
###########################################################################Start_Time


my $bam=shift;

if (!$bam )
{
	print STDERR "\n\t perl $0 <./rmdup.bam> \n\n";
	exit;
};
##################
my $score=0;

my $samtools="/share/chg2master/EXdev/WES_pipe_v4//bin/samtools";
my $reference="/share/chg2master/EXdev/WES_pipe_v4/db/J_hg19_reference/hg19_reference_with_NC.fasta";
#my $smn1_region="chr5:70247773-70247921";
#my $smn2_region="chr5:69372353-69372501";

my $smn1_region="chr5:70247773-70247921";
my $smn2_region="chr5:69372353-69372501";
##################
#print "$samtools view $bam $smn1_region -T $reference|cut -f 3,4,6,10,16\n";
#die;


my $all_reads.=`$samtools view $bam $smn1_region -T $reference|cut -f 3,4,6,10,16`;
$all_reads.=`$samtools view $bam $smn2_region -T $reference|cut -f 3,4,6,10,16`;


#print "$all_reads\n";
#die;
####################
my %depth;
foreach my $a ("chr5:70247773","chr5:69372353","chr5:70247901","chr5:69372481","chr5:70247921","chr5:69372501")
{
	foreach my $b ("A","T","C","G","tot")
	{
		if (not exists $depth{$a}{$b}) 
		{
			$depth{$a}{$b}=0;
		}
	}
}
####################

foreach my $read (split /\n/,$all_reads)
{
	my ($chr,$start,$match,$seq,$other_blat)=split /\t/,$read;
	next if($match!~/M/);

=cut
	my $end=$start+length($seq);
	if ($start > 70000000)
	{
	}else
	{
		next if(!defined $other_blat || $other_blat!~/chr/);
		my ($tmp_chr,$tmp_start,$tmp_match)=split /\,/,(split /\:/,$other_blat)[2];
		$tmp_start=~s/\+\-//g;
		my $tmp_end=$tmp_start+length($seq);

		if ($tmp_chr=~/chr5/ && $tmp_start > 70000000)
		{
			$start=$tmp_start;
			$match=$tmp_match;
		}else
		{
			next;
		};
	};
	######################
=cut
	&get_point_depth($chr,$start,$match,$seq,\%depth);
};
#print Dumper \%depth;
#die;



my $out_content;

my $depth_1c=$depth{"chr5:70247773"}{C}+$depth{"chr5:70247773"}{G}+$depth{"chr5:69372353"}{C}+$depth{"chr5:69372353"}{G};
my $depth_1t=$depth{"chr5:70247773"}{T}+$depth{"chr5:70247773"}{A}+$depth{"chr5:69372353"}{T}+$depth{"chr5:69372353"}{A};
my $depth_1tot=$depth{"chr5:70247773"}{tot}+$depth{"chr5:69372353"}{tot};

my $depth_2t=$depth{"chr5:70247901"}{T}+$depth{"chr5:69372481"}{T};
my $depth_2tot=$depth{"chr5:70247901"}{tot}+$depth{"chr5:69372481"}{tot};

my $depth_3a=$depth{"chr5:70247921"}{A}+$depth{"chr5:70247921"}{T}+$depth{"chr5:69372501"}{A}+$depth{"chr5:69372501"}{T};
my $depth_3g=$depth{"chr5:70247921"}{C}+$depth{"chr5:70247921"}{G}+$depth{"chr5:69372501"}{C}+$depth{"chr5:69372501"}{G};
my $depth_3tot=$depth{"chr5:70247921"}{tot}+$depth{"chr5:69372501"}{tot};

print "#chr5:70247773(C)\tchr5:70247773(T)\tchr5:70247773(tot)\tchr5:70247901(T)\tchr5:70247901(tot)\tchr5:70247921(A)\tchr5:70247921(G)\tchr5:70247921(tot)\n";
print "$depth_1c\t$depth_1t\t$depth_1tot\t$depth_2t\t$depth_2tot\t$depth_3a\t$depth_3g\t$depth_3tot\n";
#########################################
sub get_point_depth
{
	my($chr,$start,$match,$seq,$hash)=@_;

	#############
	my @base=split //,$seq;
	my @num=$match=~m/\d+/g;
	my @tag=$match=~m/\D+/g;
	my @true_base;
	my $now=0;
	my $match_score=0;
	for (my $i=0;$i<@num ;$i++)
	{
		if ($tag[$i]=~/M/)
		{
			push @true_base,@base[$now..$num[$i]-1];
			$now+=$num[$i];
			$match_score+=$num[$i];
		}elsif ($tag[$i]=~/D/)
		{
			for (my $j=0;$j<$num[$i] ;$j++)
			{
				push @true_base,"S";
			};
		}elsif ($tag[$i]=~/I/)
		{
			$now+=$num[$i];
		}elsif ($tag[$i]=~/S/)
		{
			$now+=$num[$i];
		}else
		{
			print "\n\terror !!!\n\n";
			die;
		};
	};
	if ($match_score/length($seq) > $score)
	{
		foreach my $j (0..@true_base-1)
		{
			my $pos=$start+$j;
			if (not exists $hash->{"$chr:$pos"}{$true_base[$j]})
			{
				$hash->{"$chr:$pos"}{$true_base[$j]}=1;
			}else
			{
				++$hash->{"$chr:$pos"}{$true_base[$j]};
			};
			++$hash->{"$chr:$pos"}{tot};
		};
	}else
	{
		###read 比对太差，不要了
	};
};
