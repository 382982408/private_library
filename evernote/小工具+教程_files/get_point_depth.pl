#!/usr/bin/perl -w
use strict;
use Cwd;
use Getopt::Long;
use Data::Dumper;
use FindBin qw($Bin $Script);
use File::Basename qw(basename dirname);
###########################################################################Start_Time
my $samtools="/share/nas9/luozw/bin/samtools";
$samtools="/share/chg2master/EXdev/WES_pipe_v4/bin/samtools";

my $point=shift;
my $bam=shift;
if (!$point || !$bam)
{
	print STDERR "\n\t perl $0 <chr5:70247773-70247773> <./rmdup.bam> \n\n";
	exit;
};

my %hash;

&get_point_depth($bam,$point,\%hash);
foreach my $sample (keys %hash)
{
	foreach my $base (keys %{$hash{$sample}})
	{
		$hash{$sample}{tot}+=$hash{$sample}{$base};
	};
};

print Dumper \%hash;
#########################################
sub get_point_depth
{
	my($bam,$pos,$hash)=@_;
	my @ARR=split /\n/,`$samtools view $bam $pos|cut -f 4,6,10 `;
	my ($chr,$start,$end)=split /[:-]/,$pos;
	foreach ("A","T","C","G")
	{
		if (not exists $hash->{$pos}{$_})
		{
			$hash->{$pos}{$_}=0;
		};
	};
	foreach my $read (@ARR)
	{
		my @arr=split /\t/,$read;
		next if($arr[1]=~/\*/);
		my @base=split //,$arr[2];
		#############
		my @num=$arr[1]=~m/\d+/g;
		my @tag=$arr[1]=~m/\D+/g;
		my @true_base;
		my $now=0;
		for (my $i=0;$i<@num ;$i++)
		{
			if ($tag[$i]=~/M/)
			{
				push @true_base,@base[$now..$num[$i]-1];
				$now+=$num[$i];
			}elsif ($tag[$i]=~/D/)
			{
				for (my $j=0;$j<$num[$i] ;$j++)
				{
					push @true_base,"loss";
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
		}
		next if(!defined $true_base[$start-$arr[0]]);
		++$hash->{$pos}{$true_base[$start-$arr[0]]};
	};
};
