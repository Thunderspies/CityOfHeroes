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
		ChildFx	V_COV\Powers\ElectricalMelee\EmpPulse.fx
		lifespan 240
	End
End

#############################################################

Condition
	Event
		Type	local
		At	Chest

		Part	V_COV\Powers\ElectricalMelee\Red_Haze.part
		Part	V_COV\Powers\ElectricalMelee\Red_HaseFade.part
		Part	V_COV\Powers\ElectricalMelee\Red_HaseFade2.part
		Part	V_COV\Powers\ElectricalMelee\Red_HaseFadeLRG.part
		Part	V_COV\Powers\ElectricalMelee\Red_HaseFade2LRG.part
		Part	V_COV\Powers\ElectricalMelee\Red_HaseFadeWhiteLRG2.part
		Part	V_COV\Powers\ElectricalMelee\Red_HaseFade2WhiteLRG2.part
		Sound Sizzle1 60 60 .55
	End
End

#############################################################

End