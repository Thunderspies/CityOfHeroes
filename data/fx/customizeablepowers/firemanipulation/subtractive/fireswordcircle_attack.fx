#############################################################
## FireSwordCircle_Attack.fx
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On 	Time
	Time 	35

#	Event
#		Type	start
#		At	root
#		Part1	CustomizeablePowers\FireManipulation\Swordflash.part
#	End

	Event
		Type	Local
		At	root
		bhvr	CustomizeablePowers\FireManipulation\Infernobubble.bhvr
		Part1	CustomizeablePowers\FireManipulation\swordFlame01.part
		Part2	CustomizeablePowers\FireManipulation\SwordFlame02.part
		Sound fireball1 80 80 .8
	End
End

#############################################################

End