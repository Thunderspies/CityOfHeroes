##############################################################
## AreaLightning.fx
#############################################################
FxInfo
LifeSpan	160
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Hookup1
		Type	local
		At	root

	End
End

Condition
	On 	cycle
	Time 	7

	Event
		Ename	Hookup2
		Type	local
		At	Hookup1
		BhvrOverride
			FadeOutLength		280
			StartJitter		120.0 0.0 120.0
		End

		geom	Centerdummy
		LifeSpan	68
	End
End

Condition
	On 	cycle
	Time 	7	#3

	Event
		Type	local
		At	Hookup2
		Altpiv	1

		BhvrOverride
			FadeOutLength		280
			StartJitter		120.0 0.0 120.0

		End

		ChildFx	:Reichsman_LightningRod.fx


		LifeSpan	35
	End

End