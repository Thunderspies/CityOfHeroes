#############################################################
## MachineGunHitLong.fx
#############################################################

FxInfo

LifeSpan 47

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Prime
		Type 	Local
		At	Chest
		Part	:Hit_Flash_Heavy.part
		Part	:Hit_Glow_Heavy.part
		Part	:Hit_Streaks_Heavy.part
		Part	:Hit_Ring_Heavy.part
		Part	:Hit_Ring_Heavy_Hilight.part
		LifeSpan 10
	End
End


Condition
	On 	Time
	Time 	8

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