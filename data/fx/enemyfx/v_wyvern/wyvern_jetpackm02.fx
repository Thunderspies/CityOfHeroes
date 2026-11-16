#########################################################
##	Fireweapon
########################################################
FxInfo

################################################################################
################################  Engines Idle  ################################
################################################################################

Condition
	On 		Time
	Time		0

	Event
		EName 	ThrusterIdle
		Type	local
		At	Chest

		ChildFX	ENEMYFX\V_Wyvern\Wyvern_ThrusterM02Idle.fx
	End

	Event
		EName	SidesIdle
		Type	Local
		At	Chest

		ChildFX	ENEMYFX\V_Wyvern\Wyvern_ThrusterSidesIdle02.fx
	End

End

################################################################################

#Condition
#	On 		TriggerBits
#	TriggerBits	FLY
#	DoMany		1
#
#	Event
#		EName 	ChildSidesIdle
#		Type	local
#		At	Chest
#		While	FLY
##		Until	Forward
#		Flags	OneAtATime
##		BhvrOverride
##			behavior
##			FadeOutLength	130
##		End
#
#		ChildFX	ENEMYFX\V_Wyvern\Wyvern_ThrusterSidesIdle02.fx
##		Geom	Testing_TargetB
#	End
#End


################################################################################
################################  Engines Air   ################################
################################################################################




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

		ChildFX	ENEMYFX\V_Wyvern\Wyvern_ThrusterM02.fx
		#Geom	Testing_TargetB
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
		While	FLY Forward
#		Until	Forward
		Flags	OneAtATime

#		Bhvr	ENEMYFX\V_Wyvern\Wyvern_B_Fade.bhvr
		ChildFX	ENEMYFX\V_Wyvern\Wyvern_ThrusterSides02.fx
#		Geom	Testing_TargetB
	End

End




End

