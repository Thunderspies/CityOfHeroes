#############################################################
## Child_FieldFlashes.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Cycle
	Time 	2
	Chance	0.25

	Event
		Type	Local
		At	Root
		ChildFX	:Child_RandomElectircalArc1.fx
		Lifespan 10
	End
End

Condition
	On 	Cycle
	Time 	3
	Chance	0.375

	Event
		Type	Local
		At	Root
		ChildFX	:Child_RandomElectircalArc1.fx
		Lifespan 10
	End
End

Condition
	On 	Cycle
	Time 	5
	Chance	0.5

	Event
		Type	Local
		At	Root
		ChildFX	:Child_RandomElectircalArc1.fx
		Lifespan 10
	End
End

#############################################################

End