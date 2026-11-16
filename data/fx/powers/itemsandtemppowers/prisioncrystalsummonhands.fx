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
		At	wepL

		BHVR	:CrystalOffsetScale2.bhvr
		Geom	cot_LargeCrystalSpin
		
		Part1	Powers/ability/Stamina2.part
		Part2	Powers/ability/StaminaSmall2.part
		Part3	Powers/ability/AbilityDots.part
		Part4	Powers/ability/AbilitySparklingDots.part
		Sound heal4 70 70 .55
		LifeSpan	55
	End		
	
End    


Condition
	On 	Time
	Time 	0
	
	Event
		Type	local
		At	wepR

		ChildFx	POWERS\RadiationControl\Prisioncrystal.fx
		#LifeSpan	20		
		
	End

End  

End			