#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
Lifespan 100

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TM_TemporalSelection_Hit_01 100 100 .8
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PYRRotate	90 0 0
			PositionOffset	0 5.0 8.0
		End
		Part	:Portal_Swirls_Border_Flat.part
		Part	:Portal_Swirls_Border_Additive_Flat.part
		Part	:Portal_Swirls_Border_Highlights_Flat.part
		Part	:Portal_Spiral_Flat.part
		Part	:Portal_Spiral_Additive_Flat.part
		Part	:Portal_Swirls_Border_Flat.part
		Lifespan 20
	End

	Event
		Type	Local
		At	Root
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PYRRotate	90 0 0
			Spin		0.0 0.05 0.0
			PositionOffset	0 5.0 5.0
		End
		Part	:Portal_Border_Smoke.part
		Lifespan 45
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 4.0 10.0
		End
		Part	:Streaks_Forward_Small.part
		Part	:Streaks_Forward_Noise_Small.part

		Lifespan 40
	End
End

Condition
	On	Time
	Time	15
	Event
		Type	Local
		At	Origin
		ChildFX	:Child_BodyGlow.fx
		Lifespan 40
	End
End


#############################################################

End