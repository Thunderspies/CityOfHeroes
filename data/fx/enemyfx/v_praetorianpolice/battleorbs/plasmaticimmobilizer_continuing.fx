#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo

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
		At	Root
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
		End
		Part	:BurnPatch_Sparks_Small.part
		Lifespan 300
		Sound IDF_BattleOrb_PlasmaImmobilizer_Hit_01 100 100 .8
	End

End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	Root
		Part	:Coals_Flat_Additive_Small.part
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 -1.0 0.0
		End
		Part	:Explosion_Fire_Small.part
		Part	:Explosion_Fire_Base.part
	End

	Event
		Type	Start
		At	Root
		Bhvr	behaviors\Splat_RuneCircle.bhvr
		BhvrOverride
			Alpha		255
			FadeInLength	10
			FadeOutLength	40
			StartColor	20 20 20
			Scale		15.0 10.0 15.0
		End
		Splat	BlurredOilSplatDark.tga
	End
End

#############################################################

End