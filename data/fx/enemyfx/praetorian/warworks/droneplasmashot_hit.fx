#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 300

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	StartPositOnly
		At	Chest
		Part	:HIt_Flash_Drone.part
		Part	:Hit_Glow_Drone.part
		Part	:Hit_Glow_Drone_Small.part
		Part	:Hit_Streak_Drone_Small.part
	End
End

#############################################################

End