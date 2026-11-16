#########################################################
## Hamidon
#########################################################

FxInfo

Flags    InheritAlpha

#########################################################

Condition
	
	On	Time
	Time	0

	Event
		Type	Start
		At	FootR
		Sound thornamb4_loop 300 300 .76
	End

	Event
		EName 	HamidonOffset
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset 0.0 24.0 0.0
		End
	End
End

Condition
	On 	Time
	Time 	10	

	Event
		EName 	Hamidon
		Type	Local
		At	Waist
		Part	:Hamidon_Motes1.part
		Part	:Hamidon_Glow1.part
		Part	:Hamidon_Nucleus_Inner_02.part
		Part	:Hamidon_Nucleus_Inner_01.part
		Part	:Hamidon_Nucleus_Outer_01.part
		Part	:Hamidon_Clouds1.part
		Sound Hamidon2_loop 440 440 1.0
	End
	
	Event
		EName 	HamidonCytoplasm
		Type	Start
		At	FootR
		Geom	Hamidon_02
	End
End

#########################################################

End