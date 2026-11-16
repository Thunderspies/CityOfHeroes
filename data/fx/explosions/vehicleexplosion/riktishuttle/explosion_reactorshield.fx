#########################################################
## Rikti Transport Shuttle - Persistent FX
#########################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start 
		At	Origin
		Sound GlassExploLarge 100 100 .8
		LifeSpan 	3
	End
End

#########################################################

Condition
	On 	Time
	Time 	0
	Repeat 		1
	RepeatJitter	1

	Event
		Type	Start 
		At	Origin
		part	:ReactorShield_Shards.part
	End
End

#########################################################

Condition
	On		Time
	Time		0
	Repeat 		8
	RepeatJitter	2

	Event
		Type	Start 
		At	Origin
		ChildFX	:ReactorShield_ShardsSmall.fx
		LifeSpan 	90
		LifespanJitter	30
	End
End

Condition
	On		Time
	Time		0
	Repeat		4
	RepeatJitter	2

	Event
		Type	Start 
		At	Origin
		ChildFX	:ReactorShield_ShardsMedium.fx
		LifeSpan 	90
		LifespanJitter	30
	End
End

Condition
	On		Time
	Time		0
	Repeat		2
	RepeatJitter	1

	Event
		Type	Start 
		At	Origin
		ChildFX	:ReactorShield_ShardsLarge.fx
		LifeSpan 	90
		LifespanJitter	30
	End
End

#########################################################

End