gcc -o MatComInvasion bullet.c enemy.c globals.c main.c player.c terminal.c visuals.c shield.c mothership.c
sudo cp -f MatComInvasion /usr/local/bin/MatComInvasion
rm -rf MatComInvasion
sudo chmod +x /usr/local/bin/MatComInvasion
