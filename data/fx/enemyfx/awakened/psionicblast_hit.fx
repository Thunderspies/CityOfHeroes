#############################################################
## PsionicBlast_Hit.fx
#############################################################

FxInfo
Lifespan 80

#############################################################

Condition
	On	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	FS_PsionicBlast_Hit_01 100 100 .8
	End

	Event
		Type	Local
		At	Head
		Part	:Head_Glow_Small.part
		Part	:Head_Shockwave_Small.part
		Part	:Head_Rings_Growing_Small.part
		Part	:Head_Rings_Growing2.part
		Part	:Electricity_Continuing.part
		Lifespan	5
	End

End

Condition
	On	Time
	Time	3

	Event
		Type	Local
		At	Head
		BhvrOverride
			StartJitter	0.75 0.75 0.75
		End
		Part	:Head_Rings_Growing_Small.part
		Part	:Head_Glow_Small.part
		Part	:Head_Shockwave_Small.part
		Part	:Electricity_Continuing.part
		Lifespan	5
	End

End

Condition
	On	Time
	Time	6

	Event
		Type	Local
		At	Head
		BhvrOverride
			StartJitter	0.75 0.75 0.75
		End
		Part	:Head_Rings_Growing_Small.part
		Part	:Head_Glow_Small.part
		Lifespan	5
	End

End
#############################################################

End