#############################################################
## DronePlasmaBeam_Hit.fx
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
		Part	:Hit_Flash.part
		Part	:Hit_Glow_Drone.part
		Part	:Hit_Shockwave_Drone.part
		Part	:Hit_Smoke.part
		Part	:Hit_Smoke_Lingering.part
		Part	:Hit_Streak_Drone.part
	End
End

Condition
	On 	Time
	Time 	7

	Event
		Type	StartPositOnly
		At	Chest
		Part	:HIt_Flash_Drone.part
		Part	:Hit_Glow_Drone.part
		Part	:Hit_Glow_Drone_Small.part
		Part	:Hit_Streak_Drone_Small.part
	End
End

Condition
	On 	Time
	Time 	14

	Event
		Type	StartPositOnly
		At	Chest
		Part	:Hit_Flash.part
		Part	:Hit_Glow_Drone.part
		Part	:Hit_Shockwave_Drone.part
		Part	:Hit_Smoke.part
		Part	:Hit_Smoke_Lingering.part
		Part	:Hit_Streak_Drone.part
	End
End

Condition
	On 	Time
	Time 	21

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