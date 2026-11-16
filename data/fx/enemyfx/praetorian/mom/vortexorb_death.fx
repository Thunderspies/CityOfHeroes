#############################################################
## VortexOrb.fx
#############################################################

FxInfo
	Flags DontSuppress

###########################################################

Condition
On Time
Time 0

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
