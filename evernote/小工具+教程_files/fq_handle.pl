#!/usr/bin/perl -w

use strict;
use Cwd;

my $infile=shift;
my $outfile=shift;

if ((!defined $infile) || (!defined $outfile))
{
	print "\n\tperl $0 <infile> <outfile>\n\n";
	exit;
}
########
my %hash;
open L,"$infile" or die"error:can not open file $infile\n";
open O,">$outfile" or die"error:can not open file $outfile\n";
$/=">";
<L>;
while (<L>)
{
	next if (/^$/);
	my ($id,$seq)=split ("\n",$_,2);
	chomp $seq;
	if(not exists $hash{$id})
	{
		print O ">$id\n$seq";
		$hash{$id}=1;
	}else
	{
		next;
	};
};
close L;
close O;

