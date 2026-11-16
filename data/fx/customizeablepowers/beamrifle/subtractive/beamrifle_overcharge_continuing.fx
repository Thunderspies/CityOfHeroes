#############################################################
## MachineGunHitLong.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Prime
		Type 	Local
		At	Chest
		Part	:Hit_Ring_Heavy_Continuing.part
		Part	:Hit_Ring_Heavy_Continuing_Hilight.part
		Part	:Hit_Arcs_Continuing.part
		Part	:Hit_Flash_Scattered.part
	End
End

#############################################################

End