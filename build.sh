cd Game
gcc -o MatComInvasion bullet.c enemy.c globals.c main.c player.c terminal.c visuals.c shield.c mothership.c sounds.c
sudo cp -f MatComInvasion /usr/local/bin/MatComInvasion
cd ..
sudo cp -r -f Sounds /usr/local/bin/
sudo touch -f /usr/local/bin/highscore.txt
rm -rf MatComInvasion
sudo chmod +x /usr/local/bin/MatComInvasion
sudo chmod u+s /usr/local/bin/MatComInvasion