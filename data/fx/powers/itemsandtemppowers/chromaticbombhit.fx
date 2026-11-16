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

		Part	:WiloWisp01.part
		Part	:WiloWisp02.part
		Part	:WiloWisp01Flat.part
		Part	:WiloWisp02Flat.part
		LifeSpan	20		
		
	End

End    

Condition
	On 	Time
	Time 	7

	Event
		Type	local
		At	Root

		Part	:ChromaticBombLight.part
		Part	:ChromaticBombLightFlat.part
		Part	:ChromaticBombLightRing.part
		Part	:ChromaticBombLightRingFlat.part
		Part	:ChromaticStreaksBase.part

		LifeSpan		7

	End

	
End	

Condition
	On 	Time
	Time 	0

	Event
		ENAME	Sparkles2
		Type	local
		At	Root

		Part4	:ChromaticBombLight3.part
		Part	:PsionicBombflashLarge2.part
		Sound chromaticgrenade 70 70 .94
		LifeSpan		37

	End

End	

Condition
	On 	Time
	Time 	10

	Event
		Type	local
		At	Root

		Part	:ChromaticBombLight.part
		Part	:ChromaticBombLightFlat.part
		Part	:ChromaticBombLightRing.part
		Part	:ChromaticBombLightRingFlat.part
		Part	:ChromaticStreaksBase.part

		LifeSpan		7

	End

End

Condition
	On 	Time
	Time 	7

	Event
		ENAME	Sparkles
		Type	local
		At	Root
		
		Part	:ChromaticStreaks.part
		Part	:ChromaticStreaksFlat.part

		LifeSpan		10

	End
End

End			