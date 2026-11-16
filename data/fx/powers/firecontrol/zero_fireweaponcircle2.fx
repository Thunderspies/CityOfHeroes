#########################################################
##	Fireweapon
########################################################
FxInfo


Input  
	Inpname	root
End

LifeSpan	65

Condition
	On 	Time
	Time 	0

	Event
		EName 	SwordSpot3
		Type	start
		At	root
		Part1	:Swordflash.part
		
	End

	Event
		EName 	Prime1
		Type	Local 
		At	root
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	powers/firecontrol/swordFlame01.part
		Part2	powers/firecontrol/SwordFlame02.part
		Sound fireball1 80 80 .8
		
	End
End


End
##################################

