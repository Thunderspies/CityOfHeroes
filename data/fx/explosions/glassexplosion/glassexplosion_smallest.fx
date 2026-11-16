
FxInfo
Condition
	On 	Time
	Time 	0

	Event
		Ename	Center
		Type	Start 
		At	Root
		LifeSpan 	5
		#Sound	Ice7 100 80 .6
		Part1	:GlassDust.part
		Part1	:GlassDustExplosion.part
	End

End

Condition
	On 	Time
	Time 	0
	Repeat 		6
	RepeatJitter	3

	Event
		Type	Start 
		At	Center
		LifeSpan 	150
		ChildFX :GlassCube.fx
	End

End

Condition
	On 	Time
	Time 	0
	Repeat 		3
	RepeatJitter	2

	Event
		Type	Start 
		At	Center
		LifeSpan 	200
		ChildFX	:GlassShardExplodeSmallest.fx
	End

End

Condition
	On 	Time
	Time 	0
	Repeat 		2
	RepeatJitter	1

	Event
		Type	Start 
		At	Center
		LifeSpan 	200
		ChildFX	:GlassPanelSmallest.fx
	End

End

End