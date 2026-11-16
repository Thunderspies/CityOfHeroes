#############################################################
## MachineGunHitLong.fx
#############################################################

FxInfo

LifeSpan 47

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BR_SingleShot_Hit_01 100 100 .8
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	Prime
		Type 	Local
		At	Chest
		Part	:Hit_Star.part
		Part	:Hit_Flash_Small.part
		Part	:Hit_Arcs.part
		LifeSpan 10
	End
End

#############################################################

End