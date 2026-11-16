#########################################################
##	iceweapon
########################################################
FxInfo

#LifeSpan	30

Input  
	Inpname	Origin
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime8
		Type	posit
		At	Origin
	#		Part 	:ripple.part
		Part 	:ripple2.part
		Part 	:ripple3.part
		Part	:ripple4.part
		Part	:FrontSplash_mist.part
		Part    :FrontSplash_fill.part
		Part	:FrontSplash_mistBright.part
		Part	:FrontSplash_mistFlat.part
		Sound Swim_Loop 50 50 .15
	End


End


End
