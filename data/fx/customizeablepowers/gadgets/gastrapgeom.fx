#############################################################
## GasTrapGeom.fx
#############################################################

FxInfo
LifeSpan	4700

#################################################################################

Condition
	On 	Time
	Time 	0


	Event
		Ename	hookup
		Type	local
		At	root
		Bhvr	V_COV\Powers\Traps\PoisonOffset.bhvr

	End

	Event
		EName	minepivot
		Type	start
		At	hookup
		Sound mine2_loop 13 13 .12
		Geom	gadgetHookup
	End

	Event
		EName	mine
		Type	start
		At	minepivot
		altpiv	1
		Bhvr	V_COV\Powers\Traps\GreenGlassMineScale.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_PoisonGlassBall
	End
#########################################################################################

	Event
		Ename	Polegear
		Type	start
		At	minepivot
		altpiv	1

		Bhvr	V_COV\Powers\Traps\PoleScale.bhvr
		BhvrOverride
			StartColor 0 0 0
		End
		Geom	CloakingPole
	End

#########################################################################################


End

End