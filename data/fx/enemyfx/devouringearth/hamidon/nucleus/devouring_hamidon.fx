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
		Type	Local
		At	Origin
		#Sound	ThornAmb5_loop 480 200 .95
	End
End

Condition
	On 	Time
	Time 	10	
	
	Event
		EName 	Hamidon
		Type	local
		At	Waist
		Part	:Devouring_Hamidon_Glow1.part
		Part	:Devouring_Hamidon_Glow2.part
		Part	:Devouring_Hamidon_Glow3.part
		Part	:Devouring_Hamidon_Nucleus_Inner_02.part
		Part	:Devouring_Hamidon_Nucleus_Inner_01.part
		Part	:Devouring_Hamidon_Nucleus_Outer_01.part
		#Part	:Devouring_Hamidon_Cloud1.part
		Part	:Devouring_Hamidon_Cloud2.part
		Sound Hamidon2_loop 440 440 1.0
	End
	
	Event
		EName 	Hamidon
		Type	Start
		At	FootR
		Geom	Hamidon_01
		Sound thornamb4_loop 300 300 .76
	End
End

#########################################################

End