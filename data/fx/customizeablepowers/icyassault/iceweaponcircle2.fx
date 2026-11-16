#########################################################
##	Fireweapon
########################################################
FxInfo


Input  
	Inpname	root
End

LifeSpan	100

Condition
	On 	Time
	Time 	35

	Event
		EName 	SwordSpot3
		Type	start
		At	root
		Part	CustomizeablePowers\IcyAssault\thinSwordflash.part
		Part1	CustomizeablePowers\IcyAssault\Swordflash.part
		
	End

	Event
		EName 	Prime1
		Type	start 
		At	root
		bhvr	CustomizeablePowers\IcyAssault\Infernobubble.bhvr
		Part1	CustomizeablePowers\IcyAssault\swordFlame01.part
		Part2	CustomizeablePowers\IcyAssault\SwordFlame02.part
		Part	CustomizeablePowers\IcyAssault\IceCrystals.part
		Part	CustomizeablePowers\IcyAssault\IceCrystals2.part
		Part	CustomizeablePowers\IcyAssault\SnowFlakes.part
		Sound IceCircle2 80 80 .8
		
	End
End


End
##################################

