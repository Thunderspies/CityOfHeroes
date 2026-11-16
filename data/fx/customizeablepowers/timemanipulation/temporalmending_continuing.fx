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
		Sound 	TM_TemporalMending_Continuing_Loop 100 100 .15
		bhvr	behaviors/Sound/SoundIn2Out10.bhvr
		lifespan 500
	End
End

Condition
	On	Cycle
	Time	4

	Event

		Type	Local
		At	Root
		BhvrOverride
			PYRRotateJitter	5 5 5
		End
		Part	:Heal_Swirl_Flat.part
		Lifespan 15
	End

End

Condition
	On	Cycle
	Time	15

	Event

		Type	PositOnly
		At	Root
		CHILDFX	:CHILD_TemporalMending_Continuing.fx
		Lifespan 15
	End
End
#############################################################

End