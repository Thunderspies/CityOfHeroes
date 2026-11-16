#############################################################
## Android_Plasma_WeldingRay_Continuing.fx
#############################################################

FxInfo

LifeSpan 180

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
		# Sound Plasma_Welding_Ray_Hit_Loop 100 100 .15
	End

	Event
		Type	Local
		At	SpadL
		Part	:Continuing_Fire.part
	End

	Event
		Type	Local
		At	SpadR
		Part	:Continuing_Fire.part
	End

	Event
		Type	Local
		At	Chest
		Part	:Continuing_Fire.part
		Part	:Continuing_Fire_Rings.part
		Part	:Continuing_Trail_Plasma.part
	End

	Event
		Type	Local
		At	Hips
		Part	:Continuing_Fire.part
	End
End

#############################################################

End