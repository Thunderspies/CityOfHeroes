#############################################################
## HEADER
#############################################################

FxInfo

Lifespan 0

#############################################################
Condition
	On 	Time
	Time	10
	Event
		Type	Local
		At	Root
		Sound 	Brawling_AssassinsStrike_Windup_01 100 100 .8
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	WepR
		Part	:Hand_Buildup_Ring_Continuing_Smokey.part
		Part	:Hand_Buildup_Ring_Continuing.part
	End
End

#############################################################

End