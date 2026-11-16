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
		EName 	hand1
		Type	Local 
		At	WepL
		Bhvr	behaviors/genericParticleFade.bhvr
		Part5	:Largepowerpunch3.part
		Sound PPfocus2 70 70 .7

	End


	Event
		EName 	hand2
		Type	Local 
		At	WepR
		Bhvr	behaviors/genericParticleFade.bhvr
		Part5	:Largepowerpunch3.part
	End

	Event
		EName 	Glow1
		Type	Local 
		At	WepL
		Bhvr	behaviors/genericParticleFade.bhvr
		Part1	:Largepowerpunch1.part
		Part3   :LargepowerpunchGlow.part
		Part4   :LargepowerpunchRings2.part
		
		
		LifeSpan	50

	End


	Event
		EName 	Glow2
		Type	Local 
		At	WepR
		Bhvr	behaviors/genericParticleFade.bhvr
		Part1	:Largepowerpunch1.part
		Part3   :LargepowerpunchGlow.part
		Part4   :LargepowerpunchRings2.part
		
		LifeSpan	50
	End

End

Condition
	On 	Time
	Time 	63

	Event
		EName 	handA
		Type	Local 
		At	WepR
		Bhvr	behaviors/genericParticleFade.bhvr
		
		Part3   :QuickRings.part
		
		Part2	:Largepowerpunch2.part
		
		LifeSpan	5

	End

	Event
		EName 	handB
		Type	Local 
		At	WepL
		Bhvr	behaviors/genericParticleFade.bhvr
		
		Part3   :QuickRings.part
		
		Part2	:Largepowerpunch2.part
		
		LifeSpan	5

	End

End

Condition
	On 	Time
	Time 	95

	Event
		EName 	Hand1
		Type	Destroy

	End

	Event
		EName 	Hand2
		Type	Destroy

	End

End	


Condition
	On 	Time
	Time 	200

	Event
		EName 	all
		Type	Destroy

	End

End


End		