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
		Sound blacktoss 120 120 .78
	End
End



Condition
	On 	Time
	Time 	20

	Event
		EName 	Prime
		Type	local 
		At	wepR
		geom	Brute_Crystal_Red
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
		geom	Brute_Crystal_Red
		bhvr	behaviors/CrystalLobbprojectile2.bhvr
		Part1	:BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root

	End

End

Condition
	On 	PrimeHit
	
	Event
		
		Type	start
		At	T_Root
		BHVR	Behaviors/RootToChestAdjustment.bhvr
	
		part	:RedStaminaRingOut.part
		part	:RedJaggedRingOut.part
		part	:RedJaggedRingOut2.part
		part	:RedShrapnelRingOut.part

	End

	Event
		
		Type	start
		At	T_Root
		BHVR	Behaviors/RootToChestAdjustment.bhvr
	
		part	:RedStaminaRingOut.part
		part	:RedJaggedRingOut.part
		part	:RedJaggedRingOut2.part
		part	:RedShrapnelRingOut.part

	End

	Event
		Ename 	Prime
		Type	Destroy
	End


End


End							