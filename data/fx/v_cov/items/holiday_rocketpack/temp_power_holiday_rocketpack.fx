#############################################################
## Temp_power_Holiday_Rocketpack.fx
#############################################################

FXInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	jetpack
		Type	local
		At	Back
		Geom	GEO_Back_Holiday_RocketPack
	End
End

#############################################################

Condition
	On 		TriggerBits
	TriggerBits	FLY
	DoMany		1

	Event
		EName 	ChildThruster
		Type	local
		At	Chest
		While	FLY
		Flags	OneAtATime
		ChildFX	ENEMYFX\V_LongBow\LB_Thruster_xmas.fx
		#Sound	xmasdrone_loop 70 30 .6
	End
End

#############################################################

Condition
	On 		TriggerBits
	TriggerBits	FLY
	DoMany		1

	Event
		EName 	ChildThrusterSides
		Type	local
		At	Chest
		While	FLY
		Until	Forward
		Flags	OneAtATime
		ChildFX	ENEMYFX\V_LongBow\LB_ThrusterSides_xmas.fx
	End
End

#############################################################

End