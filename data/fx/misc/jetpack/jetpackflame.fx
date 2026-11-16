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
		Part1 	:ripple.part
		Part2 	:ripple2.part
		Part3 	:ripple3.part
		Part4   :FrontSplash_mist.part
		Part5   :FrontSplash_fill.part
	End

	Event
		EName 	Prime8
		Type	posit
		At	Origin
		Part1 	:FrontSplash_mistFlat.part
		#Part2 	:ripple2.part
		#Part3 	:ripple3.part
		#Part4   :FrontSplash_mist.part
		#Part5   :FrontSplash_fill.part
	End

End


End
