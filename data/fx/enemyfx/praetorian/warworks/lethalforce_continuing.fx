#############################################################
## Android_Plasma_WeldingRay_Continuing.fx
#############################################################

FxInfo

#############################################################

Input
	InpName Hips
End

#############################################################


Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound Plasma_Welding_Ray_Hit_Loop 100 100 .25
	End

	Event
		Type	PositOnly
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Continuing_Plasma_Fire_Core.part
		POther	UArmL
	End

	Event
		Type	PositOnly
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Continuing_Plasma_Fire_Core.part
		Part	:Continuing_Plasma_Fire_Core_Detached.part
		POther	UArmR
	End

	Event
		Type	PositOnly
		At	UArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Continuing_Plasma_Fire_Core.part
		Part	:Continuing_Plasma_Fire_Core_Long.part

		POther	Neck
	End

	Event
		Type	PositOnly
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Continuing_Plasma_Fire_Core.part
		Part	:Continuing_Plasma_Fire_Core_Long.part
		POther	Neck
	End

	Event
		Type	PositOnly
		At	Neck
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Continuing_Plasma_Fire_Core_Detached.part
		Part	:Continuing_Plasma_Fire_Core.part
		Part	:Continuing_Trail_Plasma.part
		Part	:Continuing_Plasma_Fire_Core_Long.part
		Part	:Continuing_Plasma_Fire.part
		POther	Hair
	End

	Event
		Type	PositOnly
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Continuing_Plasma_Fire_Core_Chest.part
		Part	:Continuing_Trail_Plasma.part

	End
End

#############################################################

End