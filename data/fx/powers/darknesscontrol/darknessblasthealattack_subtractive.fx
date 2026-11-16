#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	300


#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Hand1
		Type	Local 
		At	UarmR
		Part1	:DarkHandsA.part
		Part2	:DarkHandsB.part
		Part3	:DarkHandsEmberb.part
		Part	:DarkHandsEmberBlast_Subtractive.part
		Sound miasma3 100 100 .8
		
		PMagnet	LarmR
	End

	Event
		EName 	Hand3
		Type	Local
		At	UarmL
		Part1	:DarkHandsA.part
		Part2	:DarkHandsB.part
		Part3	:DarkHandsEmberb.part
		Part	:DarkHandsEmberBlast_Subtractive.part
				
		PMagnet	LarmL
	End

End

Condition
	On 	Time
	Time 	15

	Event
		EName 	Hand2
		Type	Local 
		At	LarmR
		Part1	:DarkHands.part
		Part2	:DarkHands2.part
		Part3	:DarkHandsEmberb.part
		Part	:DarkHandsEmberBlast_Subtractive.part
		
		PMagnet	WepR
	End

	

	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		Part1	:DarkHands.part
		Part2	:DarkHands2.part
		Part3	:DarkHandsEmberb.part
		Part	:DarkHandsEmberBlast_Subtractive.part
		PMagnet	WepL
	End

End


Condition
	On 	Time
	Time	93

	Event
		EName 	Hand1
		Type	Destroy

	End

	Event
		EName 	Hand2
		Type	Destroy

	End

	Event
		EName 	Hand3
		Type	Destroy

	End

	Event
		EName 	Hand4
		Type	Destroy

	End

End

End	


			