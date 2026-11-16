#                                                          
FxInfo

	LifeSpan 75

	ClampMinScale 0.875 0.875 0.875
	ClampMaxScale 1.125 1.125 1.125

###########################################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BlackKnights_NetherBlade_Hit 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Chest
			Type Local
			Part :DarkHit.part
			Part :DarkHit1.part
			Part :DarkHit2.part
			Part :DarkHitRaysSmall.part
			Part ENEMYFX\BlackKnights\DarkSmoke_Hit01.part
		End

	End

End
