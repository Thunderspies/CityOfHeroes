#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	WepL
End
	
LifeSpan	42

#####################
##Hands
############################################

Condition
	On 	Time
	Time 	10
		
	Event
		EName 	Prime2
		Type	Posit
		At	WepL
		Part1	:HandGlows.part
		Sound forcehands7 100.0 100.0 .6
	End	

End

Condition
	On 	Time
	Time 	20

	Event
		EName 	Prime4
		Type	start
		At	WepL
		
		Geom	FX_ForceBubble
		Bhvr 	behaviors/ForceBubbleHands2.bhvr

		LifeSpan	9

	End

End


End
