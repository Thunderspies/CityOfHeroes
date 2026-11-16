
FxInfo

Lifespan	200

Condition
	On 	Time
	Time 	0

	Event
		Ename	center
		Type	Start 
		At	Chest
		BhvrOverride
			PositionOffset	0 .3 0
		End
		#Sound	Ice7 100 80 .6
		#Part1	:GlassDust.part
		
		LifeSpan 	3
	End

End

Condition
	On 	Time
	Time 	0
	Repeat 		12
	RepeatJitter	3

	Event
		Type	Start 
		At	center
		LifeSpan 	150
		ChildFX :GlassCube.fx
	End

End

Condition
	On 	Time
	Time 	0
	Repeat 		4
	RepeatJitter	2

	Event
		Type	Start 
		At	center
		LifeSpan 	100
		ChildFX	:GlassShard.fx
	End

End

Condition
	On 	Time
	Time 	0
	Repeat 		2
	RepeatJitter	1

	Event
		Type	Start 
		At	center
		LifeSpan 	150
		ChildFX	:GlassPanelSmall.fx
	End

End

End