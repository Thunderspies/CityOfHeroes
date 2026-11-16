#############################################################
## HEADER
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time	0

	Event
		Type	start
		At	Root
		ChildFx	:Child_EmpPulse.fx
		lifespan 240
	End
End

#############################################################

Condition
	On 	Time
	Time	0

	Event
		Type	local
		At	Chest
		Part	:Haze.part
		Part	:HaseFade.part
		Part	:HaseFade2.part
		Part	:HaseFadeLRG.part
		Part	:HaseFade2LRG.part
		Part	:HaseFadeWhiteLRG2.part
		Part	:HaseFade2WhiteLRG2.part
		Sound Sizzle1 60 60 0.55
	End
End

#############################################################

End