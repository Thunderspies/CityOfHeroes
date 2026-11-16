#########################################################
##	Fire_Ball
##
FxInfo


Input  
	InpName	wepR
End

Input  
	InpName	wepL
End

Input  
	InpName	UARMR
End

Input  
	InpName	UARML
End

Input  
	InpName	LARML
End

Input  
	InpName	LARMR
End


	
LifeSpan 15

##########################################################################
##Audio
##################################################################
Condition
	On	Time
	Time	14

	Event	
		Type	Local
		At 	LarmL
		Sound Shadowpunch 50 50 .9
	End
End

##########################################################################
##Arms
##################################################################

Condition
	On	Time
	Time	0
	

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:SmokeHands.part
		part2	:SmokeHands2.part
		Part	:SmokeHands2_Subtractive.part
		Part	:DarkBlastHitTendril_Subtractive.part
		Part3	:DarkBlastHitTendril.part
	
	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:SmokeHands.part
		part2	:SmokeHands2.part
		Part	:SmokeHands2_Subtractive.part
		Part	:DarkBlastHitTendril_Subtractive.part
		Part3	:DarkBlastHitTendril.part
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:SmokeHands.part
		part2	:SmokeHands2.part
		Part	:SmokeHands2_Subtractive.part
		Part	:DarkBlastHitTendril_Subtractive.part
		Part3	:DarkBlastHitTendril.part

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:SmokeHands.part
		part2	:SmokeHands2.part
		Part	:SmokeHands2_Subtractive.part
		Part	:DarkBlastHitTendril_Subtractive.part
		Part3	:DarkBlastHitTendril.part
	End

	Event
		EName	15
		Type	Local
		At	WepL
		part1	:SmokeHands.part
		part2	:SmokeHands2.part
		Part	:SmokeHands2_Subtractive.part
		Part	:DarkBlastHitTendril_Subtractive.part
		Part3	:DarkBlastHitTendril.part
	End

	Event
		EName	16
		Type	Local
		At	WepR
		part1	:SmokeHands.part
		part2	:SmokeHands2.part
		Part	:SmokeHands2_Subtractive.part
		Part	:DarkBlastHitTendril_Subtractive.part
		Part3	:DarkBlastHitTendril.part
	End

End    



End	

