#########################################################
##	
##
FxInfo


Input  
	InpName	LARML
End

Input  
	InpName	LARMR
End

Input  
	InpName	wepR
End

Input  
	InpName	wepL
End


####################################################

Condition
	On 	Time
	Time 	0

		Event
		Type	Local 
		At	LARMR
		Bhvr 	behaviors/powers/coldcontrol/icehands.bhvr
		geom	IceHandR
		
		Part2	:ColdArmMist2.part
			#part3	:IceArmorHandBitsSmall.part
		Sound Icepunch3 70 70 .7

	End

		Event
		Type	Local 
		At	LARML
		Bhvr 	behaviors/powers/coldcontrol/iceHands.bhvr
		geom	IceHandL
		
		Part2	:ColdArmMist2.part
			#part3	:IceArmorHandBitsSmall.part

	End

		Event
		Type	Local 
		At	WepL
		part1   :Snowfall.part
		part3	:IceArmorHandBitsA.part

		LifeSpan	35

	End

		Event
		Type	Local 
		At	WepR
		part1   :Snowfall.part
		part3	:IceArmorHandBitsA.part
	
		LifeSpan	35

	End



End

Condition
	On 	Time
	Time 	35
	
	Event
	
	Type	Destroy
	EName	all
	
	End


End




End			