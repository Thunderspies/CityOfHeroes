#########################################################
##	Lightning Bolt
##
FxInfo


Input  
	Inpname	WepL
End

Input  
	Inpname	WepR
End

LifeSpan	180

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	WepL
		Part1	:BluePowerpunch1.part
		Part2	:BluePowerpunch2.part
		Part3   :BluePowerpunchGlow.part
		Sound BluePowerup7 100 100 .5
	End


	Event
		EName 	Prime2
		Type	Local 
		At	WepR
		Part1	:BluePowerpunch1.part
		Part2	:BluePowerpunch2.part
		Part3   :BluePowerpunchGlow.part
		Sound forcefield5_loop 60 60 .33
	End

		Event
		EName 	Prime3
		Type	Local 
		At	WepL
		Part1	:BluePowerpunch3.part
		Part2	:BluePowerpunch2.part
		Part3   :BluePowerpunchRings.part
	End
	

	Event
		EName 	Prime4
		Type	Local 
		At	WepR
		Part1	:BluePowerpunch3.part
		Part2	:BluePowerpunch2.part
		Part3   :BluePowerpunchRings.part

	End

End		

End		