#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 340

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
		End
		Part	:BurnPatch_Sparks_Small.part
		Lifespan 300
	End

End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	Root
#		BhvrOverride
#			PositionOffset	0.0 -0.2 0.0
#		End
		Part	:Coals_Flat_Additive_Small.part
		Lifespan 250
	End

	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
		End
		Part	:Explosion_Fire_Small.part
		Part	:Explosion_Fire_Base.part
		Lifespan 300
	End

	Event
		Type	Start
		At	Hips
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			Alpha		255
			FadeInLength	10
			FadeOutLength	40
			StartColor	20 20 20
			Scale		20.0 10.0 20.0
		End
		Splat	BlurredOilSplatDark.tga
		Lifespan 320
	End

	Event
		Type	Start
		At	Hips
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			Alpha		255
			FadeInLength	10
			FadeOutLength	40
			StartColor	20 20 20
			Scale		20.0 10.0 20.0
		End
		Splat	Energy_ConeB3.tga
		Lifespan 340
	End
End

#############################################################

End