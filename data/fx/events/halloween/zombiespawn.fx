#############################################################
## ZombieSpawn.fx
#############################################################

FxInfo

Lifespan 300

#############################################################

Condition
	On 	Time
	Time 	1

	Event
		EName 	GroundBurst
		Type	Local
		At	Mystic
		Part3	POWERS\EarthControl\WeaponRocks05.part
		Part4	POWERS\EarthControl\WeaponRocks05.part
		Sound Zombie_Emerge_01 80 80 .85
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	1

	Event
		EName 	GroundBurst
		Type	Local
		At	Mystic
		Part1	:ZombieSpawn_Dust1.part
		Part1	:ZombieSpawn_Dust2.part
		Lifespan 90
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Quake
		Type	local
		At	Root
		BhvrOverride
			Shake			0.125
			ShakeFallOff		0.00625
			ShakeRadius		24
		End
	End
End

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		EName	crack
		Type	start
		At	Mystic
		Bhvr	:ZombieSpawn_splat.bhvr
		Splat	StoneCracks.tga
		Lifespan 300
	End
End

#Condition
#	On 	Time
#	Time 	5
#
#	Event
#		EName	crack
#		Type	start
#		At	Mystic
#		Bhvr	:ZombieSpawn_splat2.bhvr
#		Splat	QuickSand.tga
#		Lifespan 300
#	End
#End

#############################################################

#Condition
#	On 	Time
#	Time 	3
#
#	Event
#		EName 	ChestDust
#		Type	Local
#		At	Chest
#		Part1	POWERS\EarthControl\StalagtiteDustHit.part
#		Part2	POWERS\EarthControl\StalagtiteDustHit2.part
#		#Lifespan 300
#	End
#End

#############################################################

End