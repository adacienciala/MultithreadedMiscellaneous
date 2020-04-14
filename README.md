# Multithreaded Miscellaneous
Those are the luckiest tasks that I've chosen to save. They were a part of the additional homework for my Operating Systems class about multithreading. It wasn't supposed to be much advanced - *and it's still not* - but I think it's funny enough to save.

## Requirements and installation
To be able to run them, you need to have the ncurses library. Install it with:
```
sudo apt-get install libncurses5-dev libncursesw5-dev
```

Every folder has a makefile, so just compile and then run, using the programs' name - you're ready to go!

## Ex 1 - Memory game
<div align="center">
  <img src="https://user-images.githubusercontent.com/57063056/79117733-35385880-7d8c-11ea-800e-9762b6ed1bf3.gif"/>
</div>

### Running:
```
make
./memory
```

### Description
The rules are simple:
1. Type letters in the presented order. 
2. Use only: *w, s, a, d*. 
3. 10 rounds to win.

Good luck.

## Ex 2 - Stopwatch
<div align="center">
  <img src="https://user-images.githubusercontent.com/57063056/79117735-35385880-7d8c-11ea-8140-c903f2fb3caa.gif"/>
</div>

### Running:
```
make
./stopwatch
```

### Description
Simple stopwatch. No time limit, *get crazy*.

Controls:
* **s** - start
* **p** - toggle pause
* **h/b** - add/minus 1min
* **j/n** - add/minus 1s
* **r** - restart

## Ex 3 - Matrix
<div align="center">
  <img src="https://user-images.githubusercontent.com/57063056/79117729-34072b80-7d8c-11ea-93c5-97d9b52c1d53.gif" width="700"/>
</div>

### Running:
```
make
./matrix
```

### Description
It's just fun to look at, isn't it?

## Ex 4 - Space Race
<div align="center">
  <img src="https://user-images.githubusercontent.com/57063056/79200804-48ecc900-7e37-11ea-976a-c2f2e7633c4d.gif" width="700"/>
</div>

### Running:
```
make
./spaceRace
```

### Description
Space race.\
Move **left and right** to not crash into the enemy's fleet.\
**Up and down** to catch a shield which takes hostile spaceships down.\
Time is running out.\
Godspeed.

## Ex 5 - Cyclists
<div align="center">
  <img src="https://user-images.githubusercontent.com/57063056/79200860-6457d400-7e37-11ea-9e05-e24e5b175d89.gif" width="700"/>
</div>

### Running:
```
make
./cyclists
```

### Description
Cycling race where participants occasionally get and lose motivation. At the top of that... strange accidents may happen.\
Gotta see 'em all!

## Ex 6 - Treasure Hunt
<div align="center">
  <img src="https://user-images.githubusercontent.com/57063056/79200796-47230580-7e37-11ea-92ce-28808ccdc39b.gif" width="700"/>
</div>

### Running:
```
make
./treasureHunt
```

### Description
4 bots trying to score 5 points faster than their oppontents. There are a few surprises on the map, though:
- [x] teleport
- [x] nerf
- [x] boost

## Ex 7 - Exam
<div align="center">
  <img src="https://user-images.githubusercontent.com/57063056/79200802-48543280-7e37-11ea-9411-33052ce066aa.gif" width="700"/>
</div>

### Running:
```
make
./exam
```

### Description
The ultimate challenge - passing Operating Systems' final exam. The luck was not on our side. The proffesors weren't bothered with our critically low chances. This simulator shows how easy it was to fail that class.

PS Jk, it wasn't that hard, most of us passed just fine. *Most.*

## Notes
This repo was used mainly to play around with the readme. Turn a blind eye ;)
