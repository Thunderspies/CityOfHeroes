#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo

LifeSpan 250

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	Root
		sound Plasma_Spray 200 200 .8
	End

	Event
		EName	Spike
		Type	Local
		At	C_Neck
		Part	:Spray_Plasma_Jet.part
		Part	:Spray_Plasma_Body.part
		Part	:Spray_Plasma_Plume.part
		Lifespan 110
	End

	Event
		EName	Spike
		Type	Local
		At	C_Head
		Part	:Spray_Plasma_Jet.part
		Part	:Spray_Plasma_Body.part
		Part	:Spray_Plasma_Plume.part
		Lifespan 110
	End
End

#############################################################

End