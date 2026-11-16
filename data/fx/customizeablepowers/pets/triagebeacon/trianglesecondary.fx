#############################################################
## TriangleBeaconHeal.fx
#############################################################

FxInfo

##########################################################
Condition
	On	Time
	Time	0

	Event
		Ename	hookup2
		Type	Local
		At	root
		Bhvr	V_COV\Powers\Traps\TriangleSplatOffset.bhvr
	End

	Event
		Type	start
		At	hookup2
		Splat	Generic_GlowTriangle
		Bhvr	V_COV\Powers\Traps\HealingGlowSplat.bhvr
		BhvrOverride
			PrimaryTint	20
			SecondaryTint	50
		End
		LifeSpan	80
	End
End


Condition
	On 	cycle
	Time 	120

	Event
		Type	start
		At	hookup2
		Splat	Generic_GlowTriangle
		Bhvr	V_COV\Powers\Traps\HealingGlowSplat.bhvr
		BhvrOverride
			PrimaryTint	0
			SecondaryTint	70
		End
		LifeSpan	80
	End
End


End