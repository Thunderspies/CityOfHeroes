#############################################################
## TriangleBeaconHeal.fx
#############################################################

FxInfo

##########################################################

Condition
	Event
		Ename	hookup
		Type	Local
		At		root
		Bhvr	V_COV\Powers\Traps\TriangleSplatOffset.bhvr
	End

	Event
		Ename	Top
		Type	Local
		At		hookup
		Bhvr	V_COV\Powers\Traps\trispin.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Tintable_TriageHealSpinnerTop02

	End

	Event
		Ename	Base
		Type	Local
		At		hookup
		BhvrOverride
			TintGeom	1
			PyrRotate 0 1.35 0
		End
		geom	Tintable_TriageBaseHeal
	End

	Event
		Ename	Base
		Type	Local
		At		hookup
		BHVR	V_COV\Powers\Traps\TriBaseEnergyOffset.bhvr
		Part	:GreenEnergy.part
		Part	:GreenEnergyScreenFacing.part
	End

	Event
		Type	start
		At		hookup
		Splat	Generic_GlowTriangle
		Bhvr	V_COV\Powers\Traps\HealingGlowSplat.bhvr
		BhvrOverride
			PrimaryTint	70
			SecondaryTint	0
		End
		LifeSpan	80
	End


End

Condition
	On 	cycle
	Time 	60 #120

	Event
		Type	start
		At		hookup
		Splat	Generic_GlowTriangle
		Bhvr	V_COV\Powers\Traps\HealingGlowSplat.bhvr
		BhvrOverride
			PrimaryTint	70
			SecondaryTint	0
		End
		LifeSpan	80
	End
End

#############################################################

End