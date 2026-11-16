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

LifeSpan	200


############################################################

Condition
	On	Time
	Time	0
		
	Event	

		ENAME	Base
		Type	local
		At	WepR
		Sound darkwindup2 60 60 .3
		Part1	:DarkHand.part
		Part3	:DarkHandsEmber.part
		LifeSpan	50

	End

	Event	

		ENAME	Base
		Type	local
		At	WepL
		
		Part1	:DarkHand.part
		Part3	:DarkHandsEmber.part
		LifeSpan	50
	End

End

Condition
	On 	Time
	Time 	20

	Event
		EName 	core
		Type	local 
		At	T_Root
		geom	centerdummy
		bhvr	:Smoke_spinFast.bhvr
		
		lifeSpan	260
				
	End

	Event
		EName 	Orb
		Type	Local 
		At	core
		altpiv	1
		part	:DarkessChillMistTrail.part
		part	:DarkessChillMistTrail2.part
		Sound miasma_loop 100 100 .5
		
		lifeSpan	80	
		
	End

	Event
		EName 	Orb
		Type	Local 
		At	T_Root
		part	:DarkStreaks.part
		
		lifeSpan	80	
		
	End

	Event	

		ENAME	Base
		Type	start
		At	T_Root
		
		Bhvr 	:DarkScale3.bhvr
		Geom	FX_DarkWave
		Sound miasmaburst 100 100 .6
		
		lifeSpan	110
	End

	Event	

		ENAME	BaseTrail
		Type	start
		At	T_Root

		Part1	powers/DarknessControl/CreateDarknessTrail.part
		Part2	powers/DarknessControl/CreateDarknessTrail2.part
		Sound miasma_loop 100 100 .5
		
		lifeSpan	80

	End

	Event
		EName	darkskirt
		Type	Start
		At	T_Root
		
		Part	powers/DarknessControl/CreateDarknessPuddleFlat.part
		Part	powers/DarknessControl/CreateDarknessPuddle2Flat.part
		Part	:CreateDarknessPuddle.part
		Part	:CreateDarknessPuddle2.part
		
		lifeSpan	80
	End

End

#############################################################################################################################

End			