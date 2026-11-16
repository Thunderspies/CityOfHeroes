#############################################################
## Disintegration_Continuing.fx
#############################################################

FxInfo
Flags DontSuppress

#############################################################

Condition
	On 	Time
	Time	0

#########################################################
####################### AUDIO ###########################
#########################################################

	Event
		Type	Local
		At	Root
		Sound 	CoL_TeleportIn_01 100 100 .8
	End

#############################################################

	Event
		EName	TPAnchor
		Type	Local
		At	Chest
		Bhvr	WORLD\Bases\MissionArchitect\WarpIn_Fade.bhvr
		BhvrOverride
			FadeInLength		10
		End
		Flags	AdoptParentEntity
	End

	Event
		Type	Start
		At	TPAnchor
		Part	:Beacon_Border.part
		Part	:Beacon_Swirls.part
		Part	:Beacon_Swirls_Additive.part
		Lifespan 1
	End

	Event
		EName	SpinAnchor
		Type	Start
		At	TPAnchor
		Part	:Beacon_Core.part
		Part	:Beacon_Core_Glow.part
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	Start
		At	TPAnchor
		Part	:Rays_Explode.part
		Part	:Streaks_Explode_Detached.part
		Part	:Streaks_Explode_Short.part
		Lifespan 2
	End
End
#Condition
#	On 	Time
#	Time	0
#	Event
#		Type	Local
#		At	Root
#		SoundNoRepeat 2
#		Sound 	FirstWard_ShadowyOrb2_Loop 35 35 .15
#		Sound 	FirstWard_ShadowyOrb3_Loop 35 35 .15
#		Sound 	FirstWard_ShadowyOrb4_Loop 35 35 .15
#		Sound 	FirstWard_ShadowyOrb5_Loop 35 35 .15
#		bhvr	behaviors/Sound/SoundIn3Out5.bhvr
#	End
#
#	Event
#		Type	Local
#		At	C_Neck
#		Bhvr	WORLD\Bases\MissionArchitect\WarpIn_Fade.bhvr
#		BhvrOverride
#			FadeInLength		300
#		End
#		Flags	AdoptParentEntity
#	End
#
#	Event
#		EName	TPAnchor
#		Type	Local
#		At	Chest
#		Part	:Beacon_Border_Small.part
#	End
#End
#
#Condition
#	On	Time
#	Time	4
#
#	Event
#		Type	Start
#		At	TPAnchor
#		Part	:Beacon_Swirls_Small.part
#		Part	:Beacon_Swirls_Additive_Small.part
#
#	End
#End
#
#Condition
#	On	Time
#	Time	12
#
#	Event
#		EName	SpinAnchor
#		Type	Local
#		At	TPAnchor
#		Part	:Beacon_Core_Small.part
#	End
#End



#########################################################

End