#########################################################
##	SStreetSteeam01
##
FxInfo

LifeSpan 0

Input
	InpName	root
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	start
		At	root
		Part1	WORLD/fire/fire_flame1.part
		Part2	WORLD/fire/Phlanxspark1.part
		Part3	WORLD/fire/fire_flame2.part

	End

End

End