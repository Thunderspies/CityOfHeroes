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
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_SingleShot.fx
		Lifespan 2
	End
End

#############################################################

Condition
	On 	Cycle
	Time 	4
	Random	1

	Event
		Type	Start
		At	Origin
		ChildFX	ENEMYFX\V_PraetorianResistance\PowerRifle\Child_SingleShot.fx
		Lifespan 2
	End
End

#############################################################

End