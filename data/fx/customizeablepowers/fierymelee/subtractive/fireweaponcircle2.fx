#############################################################
## FireWeaponCircle2.fx
#############################################################

FxInfo
LifeSpan	100

#############################################################

Condition
	On 	Time
	Time 	35

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
		bhvr	CustomizeablePowers\FieryMelee\Infernobubble.bhvr
		Part1	:swordFlame01.part
		Part2	:SwordFlame01_Alpha.part
		Part2	:SwordFlame02.part

		Sound fireball1 80 80 .8

	End
End


End
##################################

