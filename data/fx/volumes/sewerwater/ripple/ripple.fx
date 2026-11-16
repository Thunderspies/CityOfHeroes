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
		Part1 	:Greenripple.part
		Part2 	:Greenripple2.part
		Part3 	:Greenripple3.part
		Part4   :GreenFrontSplash_mist.part
		Part5   :GreenFrontSplash_fill.part
	End

	Event
		EName 	Prime8
		Type	posit
		At	Origin
		Part1 	:GreenFrontSplash_mistFlat.part
		
	End

End


End
