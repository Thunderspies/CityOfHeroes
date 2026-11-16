#############################################################
## VortexOrb.fx
#############################################################

FxInfo
	Flags DontSuppress

###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	MoM_Orb_Loop 100 100 .5
	End
End

Condition
		On Time
		Time 0

	Event
			At Hips
			Type Local
			EName OrbAnchor
			Bhvr :ForceBubble_Disintegration.bhvr
			BhvrOverride
				PositionOffset 0 2 0
				PyrRotateJitter 180 180 0
				Spin 0.05 0.05 0
				Alpha 255
				FadeInLength 20
				FadeOutLength 20
				StartColor 231 221 119
			End
			Geom Tintable_Dark_EarthGraspHit
	End

	Event
			At OrbAnchor
			Type PositOnly
			EName OrbSwirls
			Flags OneAtATime
			Bhvr :ForceBubble_Disintegration.bhvr
			BhvrOverride
				PositionOffset 0 5 0
				PyrRotateJitter -180 -180 0
				Spin 0.050 0.05 0
				Alpha 255
				FadeInLength 20
				FadeOutLength 20
				StartColor 255 255 255
			End
			Part :VotexOrb_Orb.part
			Part :VotexOrb_Trail.part
			Part :VortexOrb_Swirls_Additive.part
			Part :VortexOrb_Swirls.part
			Part :VortexOrb_Swirls_02.part
			Part :VortexOrb_Border.part

			Geom Tintable_Dark_EarthGraspHit
			ChildFX	:VortexOrb_Flash_Child.fx
	End
End

Condition
		On Death

	Event
			Type Destroy
			EName CrushingHateGeo
	End

	Event
			Type Destroy
			EName CrushingHateSwirls
	End

	Event
			At Hips
			Type PositOnly
			EName Explosion
			Part :VortexOrb_Streaks_Exploding.part
			Part :VortexOrb_Streaks_Exploding_Additive.part
			Part :VortexOrb_Swirls_Exploding.part
			Part :VortexOrb_Border_Exploding.part
			Part :VortexOrb_Smoke.part
			Part :VortexOrb_Smoke_Lingering.part
			LifeSpan 5
	End
End


End
