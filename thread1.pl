#!/usr/bin/perl
use strict;
use warnings;
use LWP::UserAgent;
use HTTP::Request::Common;

my $url = 'http://127.0.0.1:8080';

my $ua = LWP::UserAgent->new;

while (my $data = <STDIN>) {
    # Usunięcie niechcianych znaków sprzed stdin
    chomp $data;

    # Utworzenie prośby POST z danymi z stdin
    my $request = POST $url, Content => { message => $data };

    # Wysłanie prośby POST
    $ua->request($request);
}
