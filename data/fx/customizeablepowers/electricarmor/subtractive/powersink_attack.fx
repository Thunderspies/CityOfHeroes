#############################################################
## PowerSink_Attack.fx
#############################################################

FxInfo

Lifespan 50

#############################################################

Condition
	On 	Time
	Time 	23

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part	:ElectricityControl01.part
		Part	:ElectricityControl02.part
		Part	:ElectricityInnerGlow.part
		Part	:ElectricityOuterGlow.part
		Part	:ElectricityArch.part
		Part	:ElectricityRing.part
		Sound elechandson 50 50 0.67
	End

	Event
		EName 	Thingy2
		Type	local
		At	HandL
		Part	:ElectricityControl01.part
		Part	:ElectricityControl02.part
		Part	:ElectricityInnerGlow.part
		Part	:ElectricityOuterGlow.part
		Part	:ElectricityArch.part
		Part	:ElectricityRing.part
		Sound elecpunch3 50 50 0.44
	End
End

#############################################################

##Condition
##	On 	Time
##	Time 	0
##
##	Event
##		EName	ChestOffset
##		Type	Local
##		At	Root
##		Geom	RootToChestAdjustment
##	End
##End
##
##Condition
##	On 	Time
##	Time 	23
##
##	Event
##		Type	Local
##		At	ChestOffset
##		AltPiv	1
##		Part	:PowerSink_ContractingRing1.part
##		Part	:PowerSink_ContractingRing2.part
##	End
##End

#############################################################

End