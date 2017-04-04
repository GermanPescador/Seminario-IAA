#include <iostream>
#include <vector>

using namespace std;

int main (void){
    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Tablas de probabilidad
    
    vector<vector<int> >  health;
    vector<vector<int> >  weapon;
    vector<vector<int> >  oppWeapon;
    vector<vector<int> >  heardNoise;
    vector<vector<int> >  nearEnemies;
    vector<vector<int> >  packWeapon;
    vector<vector<int> >  packHealth;
    vector<int>           estadoT;
    vector<vector<int> >  estadoTMas1;
    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    //Matriz Health
    
    health.resize(6);
    
    for(int i = 0; i < health.size(); i++){
        
        health[i].resize(2);
        
    }
    
    health[0][0] = 75;
    health[0][1] = 25;
    
    health[1][0] = 60;
    health[1][1] = 40;
    
    health[2][0] = 30;
    health[2][1] = 70;
    
    health[3][0] = 80;
    health[3][1] = 20;
    
    health[4][0] = 15;
    health[4][1] = 85;
    
    health[5][0] = 55;
    health[5][1] = 45;
    
    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    //Matriz Weapon
    
    weapon.resize(6);

    for(int i = 0; i < weapon.size(); i++){
        
        weapon[i].resize(2);
        
    }
    
    weapon[0][0] = 99;
    weapon[0][1] = 1;
    
    weapon[1][0] = 10;
    weapon[1][1] = 90;
    
    weapon[2][0] = 40;
    weapon[2][1] = 60;
    
    weapon[3][0] = 80;
    weapon[3][1] = 20;
    
    weapon[4][0] = 30;
    weapon[4][1] = 70;
    
    weapon[5][0] = 45;
    weapon[5][1] = 55;

    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    //Matriz OpponentsWeapon
    
    oppWeapon.resize(6);

    for(int i = 0; i < oppWeapon.size(); i++){
        
        oppWeapon[i].resize(2);
        
    }
    
    oppWeapon[0][0] = 40;
    oppWeapon[0][1] = 60;
    
    oppWeapon[1][0] = 70;
    oppWeapon[1][1] = 30;
    
    oppWeapon[2][0] = 60;
    oppWeapon[2][1] = 40;
    
    oppWeapon[3][0] = 50;
    oppWeapon[3][1] = 50;
    
    oppWeapon[4][0] = 70;
    oppWeapon[4][1] = 30;
    
    oppWeapon[5][0] = 50;
    oppWeapon[5][1] = 50;

    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
   //Matriz HeardNoise
    
    heardNoise.resize(6);

    for(int i = 0; i < heardNoise.size(); i++){
        
        heardNoise[i].resize(2);
        
    }
    
    heardNoise[0][0] = 70;
    heardNoise[0][1] = 30;
    
    heardNoise[1][0] = 60;
    heardNoise[1][1] = 40;
    
    heardNoise[2][0] = 65;
    heardNoise[2][1] = 35;
    
    heardNoise[3][0] = 30;
    heardNoise[3][1] = 70;
    
    heardNoise[4][0] = 60;
    heardNoise[4][1] = 40;
    
    heardNoise[5][0] = 80;
    heardNoise[5][1] = 20;

    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    //Matriz NearEnemies
    
    nearEnemies.resize(6);

    for(int i = 0; i < nearEnemies.size(); i++){
        
        nearEnemies[i].resize(2);
        
    }
    
    nearEnemies[0][0] = 30;
    nearEnemies[0][1] = 70;
    
    nearEnemies[1][0] = 45;
    nearEnemies[1][1] = 55;
    
    nearEnemies[2][0] = 60;
    nearEnemies[2][1] = 40;
    
    nearEnemies[3][0] = 40;
    nearEnemies[3][1] = 60;
    
    nearEnemies[4][0] = 80;
    nearEnemies[4][1] = 20;
    
    nearEnemies[5][0] = 20;
    nearEnemies[5][1] = 80;

    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Matriz PackWeapon
    
    packWeapon.resize(6);

    for(int i = 0; i < packWeapon.size(); i++){
        
        packWeapon[i].resize(2);
        
    }
    
    packWeapon[0][0] = 50;
    packWeapon[0][1] = 50;
    
    packWeapon[1][0] = 90;
    packWeapon[1][1] = 10;
    
    packWeapon[2][0] = 45;
    packWeapon[2][1] = 55;
    
    packWeapon[3][0] = 60;
    packWeapon[3][1] = 40;
    
    packWeapon[4][0] = 30;
    packWeapon[4][1] = 70;
    
    packWeapon[5][0] = 40;
    packWeapon[5][1] = 60;

    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Matriz PackHealth
    
    packHealth.resize(6);

    for(int i = 0; i < packHealth.size(); i++){
        
        packHealth[i].resize(2);
        
    }
    
    packHealth[0][0] = 70;
    packHealth[0][1] = 30;
    
    packHealth[1][0] = 20;
    packHealth[1][1] = 80;
    
    packHealth[2][0] = 90;
    packHealth[2][1] = 10;
    
    packHealth[3][0] = 60;
    packHealth[3][1] = 40;
    
    packHealth[4][0] = 20;
    packHealth[4][1] = 80;
    
    packHealth[5][0] = 45;
    packHealth[5][1] = 55;

    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Matriz S_tMas1
    
    estadoTMas1.resize(6);

    for(int i = 0; i < estadoTMas1.size(); i++){
        
        estadoTMas1[i].resize(6);
        
    }
    
    estadoTMas1[0][0] = 80;
    estadoTMas1[0][1] = 2;
    estadoTMas1[0][2] = 5;
    estadoTMas1[0][3] = 1;
    estadoTMas1[0][4] = 10;
    estadoTMas1[0][5] = 2;
    
    estadoTMas1[1][0] = 1;
    estadoTMas1[1][1] = 70;
    estadoTMas1[1][2] = 2;
    estadoTMas1[1][3] = 10;
    estadoTMas1[1][4] = 1;
    estadoTMas1[1][5] = 16;
    
    estadoTMas1[2][0] = 1;
    estadoTMas1[2][1] = 2;
    estadoTMas1[2][2] = 80;
    estadoTMas1[2][3] = 7;
    estadoTMas1[2][4] = 7;
    estadoTMas1[2][5] = 3;
    
    estadoTMas1[3][0] = 4;
    estadoTMas1[3][1] = 4;
    estadoTMas1[3][2] = 1;
    estadoTMas1[3][3] = 70;
    estadoTMas1[3][4] = 1;
    estadoTMas1[3][5] = 20;
    
    estadoTMas1[4][0] = 1;
    estadoTMas1[4][1] = 1;
    estadoTMas1[4][2] = 10;
    estadoTMas1[4][3] = 1;
    estadoTMas1[4][4] = 85;
    estadoTMas1[4][5] = 2;
    
    estadoTMas1[5][0] = 40;
    estadoTMas1[5][1] = 4;
    estadoTMas1[5][2] = 3;
    estadoTMas1[5][3] = 3;
    estadoTMas1[5][4] = 30;
    estadoTMas1[5][5] = 20;

    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

    
    return(0);
    
}