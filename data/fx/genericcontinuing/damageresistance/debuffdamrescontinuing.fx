#########################################################
## Generic Continuing - Debuff Damage Resistance
#########################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

Lifespan 5400

#########################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	CrossHairNode
		Type	Create
		Inherit	Position Scale
		Update	Position
		At	Chest
		ChildFX Genericcontinuing\DamageResistance\DeBuffDamRes.fx
		Lifespan 200
	End
End

Condition
	On	Cycle
	Time	120

	Event
		ENAME	CrossHairNode
		Type	Create
		Inherit	Position Scale
		Update	Position
		At	Chest
		ChildFX Genericcontinuing\DamageResistance\DeBuffDamRes.fx
		Lifespan 200
	End
End

#########################################################

End