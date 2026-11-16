#############################################################
## Mercenary Mastermind Assault Rifle - Slug
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Origin
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer_CoveringFire.fx
		LifeSpan 15
	End
End

Condition
	On 	Cycle
	Time 	3

	Event
		Type	Start
		At	Origin
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_Tracer_CoveringFire.fx
		LifeSpan 15
	End
End

#############################################################

End