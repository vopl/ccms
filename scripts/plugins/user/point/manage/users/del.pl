

foreach my $i(268..650)
{
	system("wget -Oout \"http://localhost:8080/userAdmin/users/del?id=${i}&backUrl=/userAdmin/users\"");
}