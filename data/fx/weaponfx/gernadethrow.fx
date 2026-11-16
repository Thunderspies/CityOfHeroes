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
	Time	10

	Event
		Type	Local	
		At	Origin
		Sound grtoss1 60 60 .7
	End
End



Condition
	On 	Time
	Time 	10

	Event
		EName 	Prime
		Type	local 
		At	wepR
		geom	grenade
		bhvr	behaviors/grenadeFade.bhvr
		lifespan	22
	End

End

Condition
	On 	Time
	Time 	32

	Event
		EName 	Prime
		Type	start 
		At	wepR
		geom	grenade
		bhvr	behaviors/MorterLobbprojectile2.bhvr
		Part1	:BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root

	End

End


##################################

Condition
	On 	PrimeHit

	Event
		EName 	Fire
		Type	Posit
		At	T_Root
		Part1	:FireExplosion.part
		Part2	powers/firecontrol/FireWeaponHitSpark.part
		Lifespan 2
	End

#	Event
#		EName 	Sparks
#		Type	Posit
#		At	T_Root
#		Part1	:GernadeLauncherSparks.part
#		#Part2	:GernadeLauncherSpark.part
#		Sound	grexplo2 120.0 30.0 1.0
#		Lifespan 5
#		
#	End
#
	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part	:FireExplosionFlash.part
		#Part2	:GernadeLauncherSpark.part
		Sound grexplo2 120.0 120.0 1.0
		
		Lifespan 5
		
	End

	
	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_Root
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		15
			PhysForcePower		300
			PhysForcePowerJitter	50
		End

		Lifespan	1

	End
End

#Condition
#	On	Primehit
#		
#	Repeat	24
#	
#	Event
#		EName	Sparkout
#		Type 	Start
#		At	T_Root
#		BhvrOverride
#			PositionOffset	0 1.5 0
#		End
#		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
#		
#	End
#
#End
#

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
		Ename 	Prime
		Type	Destroy
	End


End


End							