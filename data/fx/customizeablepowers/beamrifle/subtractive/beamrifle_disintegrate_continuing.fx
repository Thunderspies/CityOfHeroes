#############################################################
## MachineGunHitLong.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BR_Disintegrate_HitContinuing_Loop 30 30 .2
	End
End


Condition
	On 	Time
	Time 	0

	Event
		EName	Prime
		Type 	Local
		At	Chest
		Part	:Hit_Star_Continuing.part
		Part	:Hit_Star_Continuing_Color.part
		Part	:Hit_Arcs_Continuing.part
	End

	Event
		EName	Prime
		Type 	Local
		At	Chest
		Part	:Hit_Flash_Scattered.part
	End

	Event
		EName	Prime
		Type 	Local
		At	UArmL
		Part	:Hit_Flash_Continuing.part
	End

	Event
		EName	Prime
		Type 	Local
		At	UArmR
		Part	:Hit_Flash_Continuing.part
	End
End

#############################################################

End