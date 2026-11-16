#########################################################
##	cot_waterfall_med.fx
##
FxInfo
	
LifeSpan	110

Input  
	InpName	Origin
End


Input  
	InpName	Target
End


Input  
	InpName	head
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

###########################################################

Condition
	On	Time
	Time	10

	Event
		EName	faceringsRigging
		Type	local
		At	Head
		geom	LocalPivot01
				
	End

		
		
	Event
		EName	RingsAndParticles
		Type	Local
		At	faceringsRigging
			altpiv   2		
		Part1	:InfuseRings3.part
		Part2	:PsiGlow2.part
		Part3	:InfuseLight1.part
		Part4	:EnergyCoreFlash.part
		Part5	:PsionicBubblesSmall.part
	End

		
End

Condition
	On Time
	Time 61

	Event
		EName	RingsAndParticles
		Type	Destroy
	End

End

Condition
	On Time
	Time 108

	Event
		EName	All
		Type	Destroy
	End
End


End			