@echo off
cls
echo.
echo                    Vmaps Extracteur et Assembleur : Bienvenue !
echo.
echo Vous avez besoin d'au moins de 2 GO d'espace disque. Tapez, CTRL+C pour arreter l'operation
echo Appuyez sur ENTREE pour demarrer . . .
pause>nul
cls
echo.
echo.
echo.
vmapextract_v3.exe
echo.
echo.
echo.
echo Appuyez sur une touche pour continuer . . .
pause>nul)
cls
echo Les Vmaps ont ete extraites, maintenant il est temps de les assembler.
echo Appuyez sur une touche pour continuer . . .
pause>nul
md vmaps
vmap_assembler.exe buildings vmaps
echo.
echo.
echo.
echo Appuyez sur une touche pour continuer . . .
pause>nul)
cls
echo.
echo.
echo.
echo Termine ! Copiez le repertoire vmaps dans votre repertoire WaadAscent
echo Appuyer sur une touche pour continuer . . .
pause>nul
