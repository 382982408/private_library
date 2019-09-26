#!/usr/bin/perl -w
=head1 Usage

  perl get_vcf_depth.v1.pl <vcf> <conf.txt> <out_file>

=cut

use strict;
use Cwd;
use Getopt::Long;
use Data::Dumper;
use FindBin qw($Bin $Script);
use File::Basename qw(basename dirname);
use lib "$Bin/../models/";
use exon;

###########################################################################Start_Time
my $file=shift;
my $conf_file=shift;
my $out=shift;
die `pod2text $0` if (!$file || !$out || !$conf_file);

#################################
my %parameters;
&exon::initialize($conf_file,\%parameters);
my $language="chinese";
if (exists $parameters{report_language})
{
	$language=$parameters{report_language};
};
######################################

my %clivar_repeat;
my $repeat_file = "/share/chg2master/EXdev/WES_pipe_v4/db/B_disease_database/Clinvar_2018.11_GRCh37.indel.names.repeat";
if(exists $parameters{D89}){
		$repeat_file = $parameters{D89};
}
open F,"<$repeat_file" or die;
while(<F>){
	chomp;next if(/^$/);
	my @line = split(/\t/,$_);
	if($line[1] ne $line[0]){
		$clivar_repeat{$line[1]} = $line[0];
	}
	if($line[2] ne $line[0]){
		$clivar_repeat{$line[2]} = $line[0];
	}
}
close F;

&get_vcf_depth($file,$out,$language);

sub get_vcf_depth		########&get_depth($out\_$len.vcf,$depth);
{
	my ($file,$out,$language)=@_;
	open L,"$file" or die;
	open OUT,">$out" or die;
	while (<L>)
	{
		chomp;
		next if(/^$/);
		if (/^\#/)
		{
			next if($_!~/^\#CHROM\s+/);
			my ($chrori,$posori,$rs,$refori,$altori,$un1,$un2,$un3,$un4,@tmp)=(split/\s+/,$_);
			for (my $j=0;$j<@tmp;$j++)
			{
				($tmp[$j-1])=split /xb|sl|zj|xj|\_/,$tmp[$j-1];
			};
			my $last=join "\t","#chr","start","end","ref","alt","mut_tag","FisherStrand","MQ",@tmp;
			print OUT "$last\n";
			next;
		};

		my ($chrori,$posori,$rs,$refori,$altori,$un1,$un2,$un3,$un4,@depth)=(split/\t/,$_);

#		my ($fs)=$un3=~m/;(FS=[\d+\.]+);/g;
		my $fs=$un3;

		my @ALTS=split /\,/,$altori;

		for (my $k=0;$k<@ALTS ;$k++)
		{
			my $start=$posori;
			my $end=$start;
			my $ref=$refori;
			my $mut=$ALTS[$k];
			my @muttags=();

			if($ref=~/^[ATCG]$/ && $mut=~/^[ATCG]$/)
			{
					my $front_tag = "$chrori\_$start\_$end\_$ref\_$mut";
					push(@muttags,$front_tag);
			}else
			{
					my @REF=split //,$ref;
					my @ALT=split //,$mut;
					while (1)
					{
							if (@REF<1 || @ALT<1)
							{
									 last;
							}elsif ($REF[0] eq $ALT[0])
							{
									$start+=1;
									shift @REF;
									shift @ALT;
							}else
							{
									last;
							};
					};
					while (1)
					{
							if (@REF<1 || @ALT<1)
							{
									 last;
							}elsif($REF[-1] eq $ALT[-1])
							{
									pop @REF;
									pop @ALT;
							}else
							{
									last;
							};
					};
					if(@REF==1 && @ALT==1)
					{
							$end=$start;
							$ref=$REF[0];
							$mut=$ALT[0];
					}elsif(@REF<1 && @ALT>=1)
					{
							$ref="-";
							$mut=join "",@ALT;
							$start-=1;
							$end=$start+1;
					}elsif(@REF>=1 && @ALT<1)
					{
							$mut="-";
							$ref=join "",@REF;
							$end=$start+length($ref)-1;
					}else
					{
							$ref=join "",@REF;
							$mut=join "",@ALT;
							$end=$start+@REF-1;
					};
					my $front_tag = "$chrori\_$start\_$end\_$ref\_$mut";
					push(@muttags,$front_tag);
					######################
					$start = $posori;
					@REF=split //,$refori;
					@ALT=split //,$ALTS[$k];

					while (1)
					{
							if (@REF<1 || @ALT<1)
							{
									 last;
							}elsif($REF[-1] eq $ALT[-1])
							{
									pop @REF;
									pop @ALT;
							}else
							{
									last;
							};
					};
					while (1)
					{
							if (@REF<1 || @ALT<1)
							{
									 last;
							}elsif ($REF[0] eq $ALT[0])
							{
									$start+=1;
									shift @REF;
									shift @ALT;
							}else
							{
									last;
							};
					};
					if(@REF==1 && @ALT==1)
					{
							$end=$start;
							$ref=$REF[0];
							$mut=$ALT[0];
					}elsif(@REF<1 && @ALT>=1)
					{
							$ref="-";
							$mut=join "",@ALT;
							$start-=1;
							$end=$start+1;
					}elsif(@REF>=1 && @ALT<1)
					{
							$mut="-";
							$ref=join "",@REF;
							$end=$start+length($ref)-1;
					}else
					{
							$ref=join "",@REF;
							$mut=join "",@ALT;
							$end=$start+@REF-1;
					};
					my $back_tag = "$chrori\_$start\_$end\_$ref\_$mut";
					#####################
					
					if($front_tag ne $back_tag){
						push(@muttags,$back_tag);
					};
					if((exists $clivar_repeat{$front_tag}) && ($clivar_repeat{$front_tag} ne $back_tag)){
							push(@muttags,$clivar_repeat{$front_tag});		
					}elsif((exists $clivar_repeat{$back_tag}) && ($clivar_repeat{$back_tag} ne $front_tag)){
							push(@muttags,$clivar_repeat{$back_tag});
					}
			};
			########################################

			my $MQ="NA";
			if ($un3=~/MQ=(.*?);/)
			{
				$MQ=$1;
			};

			foreach my $muttag(@muttags)
			{
					my @tags = split(/_/,$muttag);
					print OUT "$tags[0]\t$tags[1]\t$tags[2]\t$tags[3]\t$tags[4]\t$muttags[0]\t$fs\t$MQ";
					foreach my $i (0..@depth-1)
					{
						my($mutdep,$alldep,$ratio,$mut_type)=("NA","NA","NA","Uncov");
						if ($depth[$i]=~/\:/)
						{
							my @INFO=split /\:/,$depth[$i];
							my @DEPTH=split /\,/,$INFO[1];
							my $number=@ALTS+1;
							if (@DEPTH==$number && (defined $INFO[2]) && $INFO[2]=~/^\d+$/)
							{
								$alldep=$INFO[2];
								$mutdep=$DEPTH[$k+1];
								if ($alldep==0)
								{
									$ratio=0;
								}elsif($mutdep=~/^\d+$/)
								{
									$ratio=$mutdep/$alldep;
									$ratio=sprintf("%.4f",$ratio);
									die if($ratio>1);
									if ($mutdep>=2 && $ratio>=0.85)
									{
										$mut_type="Homo";
									}elsif($mutdep>=2 && $ratio>=0.1)
									{
										$mut_type="Hete";
									}else
									{
										$mut_type="Wild";
									};
								}else
								{
									$mutdep="NA";
								};
								
							};
						};
						if ($mutdep=~/\d+/ && $mutdep>=6 && $ratio>=0.1)
						{
							if ($language=~/chinese/)
							{
								print OUT "\t$mutdep/$alldep=$ratio;$mut_type;相对可靠";
							}else
							{
								print OUT "\t$mutdep/$alldep=$ratio;$mut_type;Relatively reliable";
							};
						}elsif($mut_type=~/Hete|Homo/)
						{
							if ($language=~/chinese/)
							{
								print OUT "\t$mutdep/$alldep=$ratio;$mut_type;不可靠";
							}else
							{
								print OUT "\t$mutdep/$alldep=$ratio;$mut_type;Unreliable";
							};
						}else
						{
							if ($language=~/chinese/)
							{
								print OUT "\t$mutdep/$alldep=$ratio;$mut_type;NA";
							}else
							{
								print OUT "\t$mutdep/$alldep=$ratio;$mut_type;NA";
							};
						};
					};
					print OUT "\n";
			};
		};
	};
	close L;
	close OUT;
};
