#########################################################
##	Fireweapon
########################################################
FxInfo

Flags InheritAlpha DontSuppress

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Rocketpack
		Type	local
		At	Back
		Geom	FX_Longbow_Rocketpack
		BhvrOverride
			Scale			1 1 1
			PositionOffset		0 0 0
			PYRrotate 		0 0 0
			TintGeom		1
		End
	End
End


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
#		Geom	TempPower_Rocketpack.geo
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
