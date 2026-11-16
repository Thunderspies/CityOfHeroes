#############################################################
## Lunge_Attack.fx
#############################################################

FxInfo

Flags InheritAlpha

Lifespan 35

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time 	6

	Event
		EName	Streak8
		Type	local
		At	HandR
		Bhvr	CustomizeablePowers\Spines\Spines\PoisonSpikeScale.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_PoisonSpikeRight
	End
End

#############################################################

End