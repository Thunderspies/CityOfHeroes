
FxInfo
Condition
	On 	Time
	Time 	0

	Event
		Ename	Center
		Type	Start 
		At	Chest
		LifeSpan 	5
		#Sound	Ice7 100 80 .6
		Part1	:GlassDust.part
		Part1	:GlassDustExplosion.part
	End

End

Condition
	On 	Time
	Time 	0
	Repeat 		12
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
	Repeat 		8
	RepeatJitter	2

	Event
		Type	Start 
		At	Center
		LifeSpan 	100
		ChildFX	:GlassShardExplode.fx
	End

End

Condition
	On 	Time
	Time 	0
	Repeat 		4
	RepeatJitter	1

	Event
		Type	Start 
		At	Center
		LifeSpan 	100
		ChildFX	:GlassPanelSmall.fx
	End

End

End