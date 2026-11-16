#############################################################
## Disintegration_Continuing.fx
#############################################################

FxInfo
Flags DontSuppress
Lifespan 240

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On 	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound 	Apparitions_CrushingFate_Loop 100 100 .8
	End

	Event
		EName	CrushingHateGeo
		Type	PositOnly
		At	Chest
		Bhvr 	:ForceBubble_Disintegration.bhvr
		BhvrOverride
			Alpha		255
			StartColor	255 50 50
			PyrRotateJitter	180.0 180.0 0.0
			FadeInLength	20
			FadeOutLength	20
			Spin 		0.1 0.1 0
		End
		Geom	Tintable_Dark_EarthGraspHit
	End

	Event
		EName	CrushingHateSwirls
		Type	PositOnly
		At	Chest
		Bhvr 	:ForceBubble_Disintegration.bhvr
		BhvrOverride
			Alpha		255
			StartColor	20 0 0
			PyrRotateJitter	-180.0 -180.0 0.0
			FadeInLength	20
			FadeOutLength	20
			Spin 		0.1 0.1 0
		End
		Part	:InducedHorror_Swirls.part
		Part	:InducedHorror_Swirls_Additive.part
		Part	:InducedHorror_Border.part
		Geom	Tintable_Dark_EarthGraspHit
	End
End

Condition
	On	Death

	Event
		EName	CrushingHateGeo
		Type	DESTROY
	End

	Event
		EName	CrushingHateSwirls
		Type	DESTROY
	End

	Event
		Ename	Explosion
		Type	PositOnly
		At	Chest
		Part	:InducedHorror_Streaks_Exploding.part
		Part	:InducedHorror_Streaks_Exploding_Additive.part

		Part	:InducedHorror_Swirls_Exploding.part
		Part	:InducedHorror_Border_Exploding.part
		Part	:InducedHorror_Smoke.part
		Part	:InducedHorror_Smoke_Lingering.part
		Lifespan 5
	End
End

#########################################################

End