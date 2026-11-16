#############################################################
## Burr_Hamidon_01.fx
#############################################################

FxInfo

Flags InheritAlpha

#############################################################

Condition
	On 	Time
	Time 	0
	Event
		Type	local
		At	Hips
//		GEOM	GEO_Seedling_01
		Sound 	HamidonSeed_Idle_Loop 200 200 .8
//		bhvr    behaviors/Echyn/SeedlingSpin01.bhvr
		BhvrOverride
			Scale			0.15 0.15 0.15
			PositionOffset		0 -4 7.3
			PYRrotate 		0 0 0
 			PYRrotateJitter 	0 0 0
			TintGeom	1
		End
	End

	Event
		Type	local
		At	Hips
		Part	:Burr_Hamidon_Glow_01.part
//		Part	:Burr_Hamidon_Outer_01.part
	End
End

#############################################################

End