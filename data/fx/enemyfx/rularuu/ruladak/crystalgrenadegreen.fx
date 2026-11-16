#########################################################
##	
##

FxInfo
LifeSpan  200


Input  
	InpName	Origin
End

Input  
	InpName	T_Root
End

Input  
	InpName	WepR
End

##################################


Condition
	On	Time
	Time	25

	Event
		Type	Local	
		At	Origin
		Sound grtoss2 120 120 .76
	End
End


Condition
	On	Time
	Time	46

	Event
		Type	Local	
		At	Origin
		Sound redtoss 120 120 .76
	End
End



Condition
	On 	Time
	Time 	20

	Event
		Type	local 
		At	wepR
		geom	Brute_Crystal_Green
		bhvr	behaviors/grenadeFade.bhvr
		lifespan	30
	End

End

Condition
	On 	Time
	Time 	52

	Event
		EName 	Prime
		Type	start 
		At	wepR
		geom	Brute_Crystal_Green
		bhvr	behaviors/CrystalLobbprojectile2.bhvr
		Part1	:BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root

	End

End


##################################

Condition
	On 	PrimeHit
	
	Event
		
		Type	start
		At	T_Root
		BHVR	Behaviors/RootToChestAdjustment.bhvr
	
		part	:GreenStaminaRingOut.part
		part	:GreenJaggedRingOut.part
		part	:GreenJaggedRingOut2.part
		part	:GreenShrapnelRingOut.part

	End

	Event
		
		Type	start
		At	T_Root
		BHVR	Behaviors/RootToChestAdjustment.bhvr
	
		part	:GreenStaminaRingOut.part
		part	:GreenJaggedRingOut.part
		part	:GreenJaggedRingOut2.part
		part	:GreenShrapnelRingOut.part

	End

	Event
		Ename 	Prime
		Type	Destroy
	End


End


End							