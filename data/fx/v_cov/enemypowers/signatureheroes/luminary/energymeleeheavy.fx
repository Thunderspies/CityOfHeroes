#########################################################
##	Lightning Bolt
##
FxInfo


#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	Local
#		At	Root
#		ChildFX	:EnergyMeleeAttached.fx
#	End
#
#End


Condition
	On 	Time
	Time 	10

	Event
		EName 	hand2
		Type	Local 
		At	WepL
		#Part1	:Largepowerpunch1.part
		Part3   :LargepowerpunchGlow.part
		Sound PunchA 100 100 .6

		LifeSpan	15

	End


	Event
		EName 	hand
		Type	Local 
		At	WepL
		Part1	:Largepowerpunch3.part

	End
	

	Event
		EName 	hand2
		Type	Local 
		At	WepR
		#Part1	:Largepowerpunch1.part
		Part3   :LargepowerpunchGlow.part

		LifeSpan	15

	End


	Event
		EName 	hand
		Type	Local 
		At	WepR
		Part1	:Largepowerpunch3.part


	End


End

Condition
	On 	Time
	Time 	24

	Event
		EName 	hand
		Type	Local 
		At	WepL
		Part3   :LargepowerpunchRings.part
		Part2	:Largepowerpunch2.part
		LifeSpan	5

	End
	
	Event
		EName 	hand
		Type	Local 
		At	WepR
		Part3   :LargepowerpunchRings.part
		Part2	:Largepowerpunch2.part
		LifeSpan	5

	End

End

Condition
	On 	Time
	Time 	55

	Event
		EName 	all
		Type	Destroy

	End



End		

End		