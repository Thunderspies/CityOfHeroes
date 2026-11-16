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
		ChildFx	V_COV\Powers\ElectricalMelee\Blue_EmpPulse.fx
		lifespan 240
	End
End

#############################################################

Condition
	Event
		Type	local
		At	Chest
		Part	:Blue_Haze.part
		Part	:Blue_HazeFade.part
		Part	:Blue_HazeFade2.part
		Part	:Blue_HazeFadeLRG.part
		Part	:Blue_HazeFade2LRG.part
		Part	:Red_HaseFadeWhiteLRG2.part
		Part	:Red_HaseFade2WhiteLRG2.part
		Sound Sizzle1 60 60 .55
	End
End

#############################################################

End