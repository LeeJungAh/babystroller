<?php
include("db.php");

$id = $_POST['id'];
$password = $_POST['password'];

$query_search = "select * from Login where user_id = '".$id."' AND user_password = '".$password."'";
$query_exec = mysql_query($query_search) or die(mysql_error());
$rows = mysql_num_rows($query_exec);

 if($rows == 0) {
 echo "No Such User Found";
 }
 else  {
    echo "User Found";
}
?>