#########################################################
##	
##

FxInfo
LifeSpan  200

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
		Sound purpletoss 120 120 .76
	End
End



Condition
	On 	Time
	Time 	20

	Event
		EName 	Prime
		Type	local 
		At	wepR
		geom	Brute_Crystal_Purple
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
		geom	Brute_Crystal_Purple
		bhvr	behaviors/CrystalLobbprojectile2.bhvr
		Part1	:BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root

	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Fire
		Type	Posit
		At	T_Root
		Part1	:PurpleFireExplosion.part
		
		Lifespan 8
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part1	:PurpleGernadeLauncherSparks.part
		#Part2	:GernadeLauncherSpark.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
		
	End


End


##################################

Condition
	On 	Time
	Time	34

	Event
		Ename 	Blast
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	43

	Event
		Ename 	tracer
		Type	Destroy
	End

End

Condition
	On 	PrimeHit
	
	Event
		
		Type	start
		At	T_Root
		BHVR	Behaviors/RootToChestAdjustment.bhvr
	
		part	:PurpleStaminaRingOut.part
		part	:PurpleJaggedRingOut.part
		part	:PurpleJaggedRingOut2.part
		part	:PurpleShrapnelRingOut.part

	End

	Event
		
		Type	start
		At	T_Root
		BHVR	Behaviors/RootToChestAdjustment.bhvr
	
		part	:PurpleStaminaRingOut.part
		part	:PurpleJaggedRingOut.part
		part	:PurpleJaggedRingOut2.part
		part	:PurpleShrapnelRingOut.part

	End

	Event
		Ename 	Prime
		Type	Destroy
	End

	
End


End							