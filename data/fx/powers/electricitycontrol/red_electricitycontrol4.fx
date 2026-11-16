#############################################################
## Red_ElectricityControl4.fx
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
		Part	:Red_ElectricityControl01.part
		Part	:Red_ElectricityControl02.part
		Part	:Red_ElectricityInnerGlow.part
		Part	:Red_ElectricityControl01white.part
		Part	:Red_ElectricityControl02white.part
		Sound elechandson 50 50 0.67
	End

	Event
		EName 	Thingy2
		Type	local
		At	HandL
		Part	:Red_ElectricityControl01.part
		Part	:Red_ElectricityControl02.part
		Part	:Red_ElectricityInnerGlow.part
		Part	:Red_ElectricityControl01white.part
		Part	:Red_ElectricityControl02white.part
		Sound elecpunch3 50 50 0.44
	End

	Event
		EName 	Thingy3
		Type	local
		At	HandR
		Part	:Red_ElectricityOuterGlow.part
		Part	:Red_ElectricityArch.part
		Part	:Red_ElectricityRing.part
		Part	:Red_ElectricityArchInvert.part
	End

	Event
		EName 	Thingy4
		Type	local
		At	HandL
		Part	:Red_ElectricityOuterGlow.part
		Part	:Red_ElectricityArch.part
		Part	:Red_ElectricityRing.part
		Part	:Red_ElectricityArchInvert.part
	End
End

#############################################################

End