#############################################################
## Disintegration_Continuing.fx
#############################################################

FxInfo
Flags DontSuppress

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On 	Time
	Time	10
	Event
		Type	Local
		At	Root
		Sound 	CoL_TeleportOut_01 100 100 .8
	End
End

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	C_Neck
		Bhvr	WORLD\Bases\MissionArchitect\WarpOut_Fade.bhvr
		BhvrOverride
			FadeOutLength		10
		End
		Flags	AdoptParentEntity
		Lifespan 18
	End
End

Condition
	On	Time
	Time	15
	Event
		Type	Start
		At	Root
		BhvrOverride
			Alpha		255
			Scale		5.0 5.0 5.0
			PositionOffset	0.0 0.0 -1.0
		End
	End

	Event
		EName	CrushingHateSwirls
		Type	Start
		At	Hips
		BhvrOverride
			PYRRotate	90 0 0
			Spin		0.0 0.5 0.0
		End
		Part	:Beacon_Border.part
		Part	:Beacon_Swirls.part
		Part	:Beacon_Swirls_Additive.part
		Lifespan 1
	End

	Event
		EName	SpinAnchor
		Type	Start
		At	Hips
		BhvrOverride
			PYRRotate	90 0 0
			Spin		0.0 0.01 0.0
		End
		Part	:Beacon_Core.part
		Part	:Beacon_Core_Glow.part
	End
End

Condition
	On	Time
	Time	18

	Event
		Type	Start
		At	Hips
		Part	:Rays_Explode.part
		Part	:Streaks_Explode_Detached.part
		Part	:Streaks_Explode_Short.part
		Lifespan 2
	End
End

#########################################################

End