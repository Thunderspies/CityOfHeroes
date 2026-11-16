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
		Part	:thinSwordflash.part
		Part1	:Swordflash.part
		
	End

	Event
		EName 	Prime1
		Type	start 
		At	root
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	:swordFlame01.part
		Part2	:SwordFlame02.part
		Part	:IceCrystals.part
		Part	:IceCrystals2.part
		Part	:SnowFlakes.part
		Sound IceCircle2 80 80 .8
		
	End
End


End
##################################

