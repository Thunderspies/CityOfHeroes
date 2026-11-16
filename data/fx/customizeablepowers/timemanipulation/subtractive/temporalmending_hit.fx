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
	On	Time
	Time	0
	Event
		EName	SwirlAnchor
		Type	Local
		At	Root
		Lifespan 15
	End

	Event
		Type	Local
		At	Root
		CHILDFX	:CHILD_BodyGlow_Heal.fx
		Lifespan 80
	End
End

Condition
	On	Cycle
	Time	2

	Event

		Type	Local
		At	SwirlAnchor
		BhvrOverride
			PYRRotateJitter	5 5 5
		End
		Part	:Heal_Swirl_Flat_Explode.part
		Lifespan 10
	End

End

#############################################################

End