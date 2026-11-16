#########################################################
##	Fireweapon
########################################################
FxInfo

########################################################

Condition
	On 		Time
	Time		1
	

	Event
		EName 	ChildThruster
		Type	local
		At	Chest

		ChildFX	ENEMYFX\V_LongBow\LB_SkiffThruster.fx
#		Geom	Testing_TargetB
		Sound Thruster_loop 60 60 .1
	End

	Event
		EName 	ChildIntake
		Type	local
		At	Chest

		ChildFX	ENEMYFX\V_LongBow\LB_SkiffIntake.fx
#		Geom	Testing_TargetB

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

#		ChildFX	ENEMYFX\V_LongBow\LB_ThrusterSides.fx
#		Geom	Testing_TargetB
	End

End




End
