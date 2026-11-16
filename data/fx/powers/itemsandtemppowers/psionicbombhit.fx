#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	100

Input  
	InpName	Root
End    

Condition
	On 	Time
	Time 	0

		
	Event
		Type	local
		At	Root

		Part	:PsionicBombLight.part
		Part	:PsionicBombLightRing.part
		Part	:PsionicBombLightFlat.part
		Part	:PsionicBombLightRingFlat.part
		Sound Psionicbomb 70 70 .93
		
		
	End
		
	Event
		ENAME	Sparkles
		Type	local
		At	Root

		
		Part	:PsionicBombStreaks.part
	
		LifeSpan		10

	End
	
	Event
		ENAME	Sparkles
		Type	local
		At	Root
		
		Part	:PurpleRingsUp.part
		Part	:BigPurpleRingsUp.part
		
		LifeSpan		27

	End

	Event
		ENAME	Sparkles
		Type	local
		At	Root
		
		Part	:BigPurpleRingsUp2.part
		
		LifeSpan		8

	End

	Event
		ENAME	Sparkles2
		Type	local
		At	Root

		Part4	:PsionicBombLight3.part
		
		
		LifeSpan		26

	End

End	

Condition
	On 	Time
	Time 	15

Event
		ENAME	Sparkles
		Type	local
		At	Root
		
		Part	:BigPurpleRingsUp2.part
		
		LifeSpan		8

	End

End


End			