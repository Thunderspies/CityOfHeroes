#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	root
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

##################################
Condition
	On	Time
	Time	0
		

	Event	

		ENAME	Base
		Type	Posit
		At	root


		
		#Geom	FX_CreateDarkness
	
		Part1	:DarkPuddle.part
		Part2	:DarkPuddle2.part
		Part3	:DarkPuddleRadiusRing.Part
		Part4	:DarknessTendrill.Part

	End
	
	Event	

		ENAME	BaseTrail
		Type	Posit
		At	root

		Part1	:DarkPuddleTrail.part
		Part2	:DarkPuddle2Trail.part
		

	End
	
	Event	

		ENAME	Base
		Type	local
		At	WepR

		Part1	:DarkHand.part
		Part3	:DarkHandsEmber.part

	End

	Event	

		ENAME	Base
		Type	local
		At	WepL
		
		Part1	:DarkHand.part
		Part3	:DarkHandsEmber.part
	End

	


End

End			