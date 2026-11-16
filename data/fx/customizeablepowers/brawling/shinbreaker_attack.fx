#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On 	Time
	Time	5
	Event
		Type	Local
		At	Root
		Sound 	Brawling_ShinBreaker_Attack_01 200 100 .8
	End
End


Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	FootR
		Part	:Hand_Trail.part
		Part	:Foot_Trail_Additive_Small.part
		Lifespan 10
	End
End

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	FootR
		Part	:Foot_Buildup_Star.part
		Part	:Foot_Buildup_Ring.part
		Lifespan 5
	End

End
#############################################################

End