#########################################################
##	SStreetSteeam01
##
FxInfo

LifeSpan 0

Input
	InpName	Origin
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	Origin
		Part1	WORLD/lairs/cot/Cauldronflame1.part
		Part2	WORLD/lairs/cot/Cauldronflame2.part
		Part3	WORLD/lairs/cot/CauldronSparks.part
		Part4	WORLD/lairs/cot/CauldronFlameGround.part
		Sound Fireplace_Loop 25.0 25.0 .35

		#Part1	world/City/PhalanxFlame.part

		#Part3	world/City/PhalanxSparks.part
	End
End

End