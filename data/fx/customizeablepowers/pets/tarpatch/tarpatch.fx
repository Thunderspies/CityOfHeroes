#########################################################
## Tar Patch
#########################################################

FxInfo

#########################################################


Condition
	On	Time
	Time	0

	Event
		ENAME	BaseTrail
		Type	start
		At	root
		Part1	:DarkTarRing.Part
		Part1	:DarkPuddle.part
		Part2	:DarkPuddle2.part
		Part4	:DarknessTendrill.Part
		Sound Tar_loop 80 80 .53
	End
End

Condition
	On	Time
	Time	0

	Event
		EName 	burst
		Type	start
		At	Root
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	:PitBust1.part
		Part2	:PitBust2.part
		Sound grexplo2 80 80 1.0
	End

	Event
		ENAME	Base
		Type	start
		At	root
		Bhvr 	Behaviors/DarkScale2.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_FX_DarkWave
	End
End

Condition
	On	Time
	Time	30

	Event
		ENAME	BaseTrail
		Type	start
		At	root
		Part3	:TarBubblePopping.part
		Part5	:TarBubblePoppingFlat.part
		Sound miasmaburst4 80 80 .62
	End
End

#########################################################

End