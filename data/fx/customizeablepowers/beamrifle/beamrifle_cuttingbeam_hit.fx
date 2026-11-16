#############################################################
## MachineGunHitLong.fx
#############################################################

FxInfo

LifeSpan 20

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BR_CuttingBeam_Hit_01 100 100 .5
	End
End

Condition
	On	Time
	Time	0

	Event
		Type 	Local
		At	Chest
		Part	:Hit_Slash.part
		LifeSpan 20
	End
End

Condition
	On	Time
	Time	3

	Event
		Type 	Local
		At	Chest
		Part	:Hit_Slash.part
		#Part	:Hit_Star.part
		Part	:Hit_Arcs.part
		LifeSpan 20
	End
End

Condition
	On	Time
	Time	8

	Event
		Type 	Local
		At	Chest
		Part	:Hit_Slash.part
		#Part	:Hit_Star.part
		Part	:Hit_Flash_Small.part
		LifeSpan 20
	End
End

#############################################################

End