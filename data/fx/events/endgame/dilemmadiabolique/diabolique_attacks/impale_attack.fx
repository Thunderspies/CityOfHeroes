#
FxInfo
Lifespan 250

###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DD_Impale_Attack_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			Ename	SpinAnchor
			At Root
			Type StartPositOnly
			BhvrOverride
				Spin	0.0 0.15 0.0
			End
			Lifespan 230
		End
		Event
			At SpinAnchor
			Type Local
			Bhvr :Impale_RainPricks.bhvr
			BhvrOverride
				StartColor 10 30 50
				Alpha	255
			End
			Geom FX_Impale_RainColumn2
			Lifespan 135

		End
		Event
			At SpinAnchor
			Type Local
			Bhvr :Impale_RainPricks_Mid.bhvr
			BhvrOverride
				StartColor 10 30 50
				Alpha	255
			End
			Geom FX_Impale_RainColumn2
			Lifespan 130

		End

End




Condition
	On	Time
	Time	5

		Event
			At SpinAnchor
			Type Local
			Bhvr :Impale_Rain_Mid.bhvr
			BhvrOverride
				StartColor 20 70 90
				Alpha	255
			End
			Geom FX_Impale_RainColumn
			Lifespan 130
		End

		Event
			At SpinAnchor
			Type Local
			Bhvr :Impale_Rain.bhvr
			BhvrOverride
				StartColor 20 70 90
				Alpha	255
			End
			Geom FX_Impale_RainColumn
			Lifespan 130
		End
End

Condition
	On	Time
	Time	7

		Event
			At	SpinAnchor
			Type	Local
			Part	:Impale_Sparkles_Rain.part
			Part	:Impale_Sparkles_Rain_Subtractive.part
			Part	:Impale_Rain_Ground.part
			Lifespan 115
		End
End


Condition
	On	Time
	Time	120

		Event
			At	Origin
			Type	Local
			CHILDFX	:Impale_Crystal_Pet.fx
			Lifespan 300
		End
End

Condition

	On	Time
	Time	210

		Event
			At	Root
			Type	Local
			CHILDFX	:Sentinel_ConsumingBlast_Pet.fx
			Lifespan 150
		End
End

###########################################################

End