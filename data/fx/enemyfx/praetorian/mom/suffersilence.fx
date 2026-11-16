#
FxInfo

Lifespan 225

###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	MoM_Suffer_Loop 100 100 .5
	End
End
	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 0 0
				#Scale 1 1 1
				FadeInLength 30
				FadeOutLength 30
				StartColor 255 120 113
			End
			Geom FX_SufferRing_30
			LifeSpan 0
		End

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 0.2 0
				#Scale 1 1 1
				FadeInLength 30
				FadeOutLength 30
				StartColor 255 120 113
			End
			Part ENEMYFX\Praetorian\MoM\Suffer_Ring_30_inner.part
			Part ENEMYFX\Praetorian\MoM\Suffer_Ring_30.part
			LifeSpan 0
		End

	End

End
