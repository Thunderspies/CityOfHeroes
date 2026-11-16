#############################################################
## SynapticOverload_Hit.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname		Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	HandGlow1
		Type	Local
		At	Head
		Part	:Hand_Glow.part
		Part	:Hand_Ring.part
		Part	:Hand_Ring_Burst.part
		Part	:Body_Sparks.part
		Part	:Hit_Flash_Small.part
		Lifespan 5
	End

	Event
		EName	HandGlow1
		Type	Local
		At	Head
		Sound	EC_SynapticOverload_Continuing_Loop 100 100 .25
		Part	:Hand_Electricity_Arcs_Consistant.part
		Part	:Hand_Electricity_Arcs_Consistant2.part
		Part	:Head_Ring_Continuing.part
		ChildFX	GenericContinuing/ConfuseContinuing.fx
	End
End

#############################################################

End
