<?php
/* 
ServeurStats
6 oct 2008 
*/

$db_host = "localhost";
$db_world = "character";
$db_user = "root";
$db_pass = "";

$connect = mysql_connect($db_host,$db_user,$db_pass);
mysql_select_db($db_world, $connect);

$quoi = $_GET['quoi'];

$valeur=0;

$valrow = mysql_query ("SELECT * FROM serveurstats");
$numrow = mysql_num_rows($valrow);
//$result = mysql_query("SELECT * FROM table1", $link);
//$num_rows = mysql_num_rows($result);

$numaffiche = 60;
$numrow = $numrow - $numaffiche;

$val = mysql_query ("SELECT * FROM serveurstats ORDER BY tagStat ASC LIMIT $numrow,$numaffiche");

while($resultat = mysql_fetch_array($val)) { 
	$valeur++;

	$arr[$valeur]['player'] = $resultat['player'];
	$arr[$valeur]['gm'] = $resultat['gm'];
	$arr[$valeur]['alliance'] = $resultat['alliance'];
	$arr[$valeur]['horde'] = $resultat['horde'];
	$arr[$valeur]['accept'] = $resultat['accept'];
	$arr[$valeur]['peak'] = $resultat['peak'];	
}

$taille_x = 230; // 790
$taille_y = 100;
$marge = 5;
$echelle_x = 4;
$echelle_y = 5;

header ("Content-type: image/png");
$image = imagecreate($taille_x,$taille_y);

$noir = imagecolorallocate($image, 0, 0, 0);
$orange = imagecolorallocate($image, 255, 128, 0);
$bleu = imagecolorallocate($image, 0, 0, 255);
$rouge = imagecolorallocate($image, 255, 0, 0);
$blanc = imagecolorallocate($image, 255, 255, 255);
$jaune = imagecolorallocate($image, 234, 249, 62);

//axe x
$x1_axe_x = 0 + $marge ;
$y1_axe_x = $taille_y - $marge;
$x2_axe_x = $taille_x - $marge;
$y2_axe_x = $taille_y - $marge;
ImageLine ($image, $x1_axe_x, $y1_axe_x, $x2_axe_x, $y2_axe_x, $blanc);

//axe y
$x1_axe_y = 0 + $marge;
$y1_axe_y = 0 + $marge;
$x2_axe_y = 0 + $marge;
$y2_axe_y = $taille_y - $marge;
ImageLine ($image, $x1_axe_y, $y1_axe_y, $x2_axe_y, $y2_axe_y, $blanc);

$i=0;
foreach ($arr as $valgraph) {
	$i++;
	if ($i != 1) {
	
		if ( $quoi == 'player' ) { $couleur = imagecolorallocate($image, 234, 249, 62); } //jaune 
		if ( $quoi == 'gm' ) { $couleur = imagecolorallocate($image, 234, 249, 62); }
		if ( $quoi == 'alliance' ) { $couleur = imagecolorallocate($image, 0, 0, 255); }
		if ( $quoi == 'horde' ) { $couleur = imagecolorallocate($image, 255, 0, 0); }
		if ( $quoi == 'accept' ) { $couleur = imagecolorallocate($image, 234, 249, 62); }
		if ( $quoi == 'peak' ) { $couleur = imagecolorallocate($image, 234, 249, 62); }
	
		// point origine ... -1
		$x1 = ((($i-2) * $echelle_x) + $marge);
		$y1 = ($taille_y - $marge) - (($arr[($i-1)][$quoi]) * $echelle_y);
		// point destination ...
		$x2 = ((($i-1) * $echelle_x) + $marge);
		$y2 = ($taille_y - $marge) - (($arr[$i][$quoi]) * $echelle_y);
		ImageLine ($image, $x1, $y1, $x2, $y2, $couleur);
	}
}

imagepng($image);

?>