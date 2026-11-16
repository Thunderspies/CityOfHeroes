#############################################################
## Child_MudPots.fx
#############################################################

FxInfo

Lifespan 19

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	Origin
		Part	CustomizeablePowers\StoneArmor\Clay_B3.part
		Lifespan 19
	End

	Event
		Type	start
		At	Origin
		Part	CustomizeablePowers\StoneArmor\SmallSpecks.part
		Part	CustomizeablePowers\StoneArmor\SmallSpecks2.part
		Part	CustomizeablePowers\StoneArmor\Clay_Flat.part
		Part	CustomizeablePowers\StoneArmor\Clay_Flat2.part
		Part	CustomizeablePowers\StoneArmor\Clay_ADifuse.part
		Part	CustomizeablePowers\StoneArmor\Clay_a2.part
		Part	CustomizeablePowers\StoneArmor\Clay_a.part
		Part	CustomizeablePowers\StoneArmor\Clay_B.part
		Part	CustomizeablePowers\StoneArmor\Clay_C.part
		Lifespan 8
	End
End

#############################################################

End