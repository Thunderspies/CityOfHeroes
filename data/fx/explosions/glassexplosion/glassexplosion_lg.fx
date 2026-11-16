
FxInfo
Condition
	On 	Time
	Time 	0

	Event
		Ename	Center
		Type	Start 
		At	Chest
		LifeSpan 	3
		Sound GlassExploLarge 100 100 .8
		Part1	:GlassDust.part
		Part1	:GlassDustExplosion.part
		Part1	:GlassDustExplosion.part
	End

End

Condition
	On 	Time
	Time 	0
	Repeat 		9
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
		LifeSpan 	100
		ChildFX	:GlassShardExplode.fx
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
		LifeSpan 	100
		ChildFX	:GlassShard.fx
	End

End

Condition
	On 	Time
	Time 	0
	Repeat 		5
	RepeatJitter	1

	Event
		Type	Start 
		At	Center
		LifeSpan 	100
		ChildFX	:GlassPanelSmallExplode.fx
	End

End

Condition
	On 	Time
	Time 	0
	Repeat 		3
	RepeatJitter	1

	Event
		Type	Start 
		At	Center
		LifeSpan 	100
		ChildFX	:GlassPanelExplode.fx
	End

End

Condition
	On 	Time
	Time 	0
	Repeat 		1

	Event
		Type	Start 
		At	Center
		LifeSpan 	100
		ChildFX	:GlassPanel.fx
	End

End

End