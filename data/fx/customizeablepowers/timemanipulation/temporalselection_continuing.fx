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
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TM_TemporalSelection_Continuing_Loop 100 100 .5
		bhvr	behaviors/Sound/SoundIn2Out10.bhvr
		lifespan 500
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	SwirlAnchor
		Type	Local
		At	Root
		BhvrOverride
			PYRRotateJitter	10 10 10
			PositionOffset	0.0 4.0 0.0
		End
		Part	:Swirl_Explode_Small_Highlights.part
		Part	:Swirl_Explode_Small.part
		Part	:Spiral_Explode_Small.part
	End
End

#############################################################

End