#########################################################
##	Fireweapon
########################################################
FxInfo

########################################################

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

		ChildFX	ENEMYFX\V_LongBow\LB_ThrusterM.fx
#		Geom	Testing_TargetB
		Sound Thruster_loop 60 60 .1
	End

End



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

		ChildFX	ENEMYFX\V_LongBow\LB_ThrusterSidesM.fx
#		Geom	Testing_TargetB
	End

End




End
