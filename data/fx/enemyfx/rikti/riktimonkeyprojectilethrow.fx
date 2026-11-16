#########################################################
##	
##

FxInfo
LifeSpan  200


##################################
##SOUND#####
######################################################################
Condition
	On	Time
	Time	10

	Event
		Type	Local	
		At	Origin
		#Sound	grtoss1 60 50 .7
	End
End

###################################################################

Condition
	On 	Time
	Time 	32

	Event
		EName 	Prime
		Type	start 
		At	wepR
		Geom	FX_Stone_01
		bhvr	:RiktiMonkeyProjectile.bhvr
		#Part1	:fire.part
		Magnet	T_Root
		LookAt	T_Root

	End

	Event
		Type	local 
		At	Prime		
		Part1	:fire.part	
		Part1	:fire.part
		
	End

End


##################################

#Condition
#	On 	PrimeHit
#
#	Event
#		EName 	Fire
#		Type	Posit
#		At	T_Root
#		Part1	:FireExplosion.part
#		#Part2	powers/firecontrol/FireWeaponHitSpark.part
#		Lifespan 8
#	End
#
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
#	Event
#		EName 	Sparks
#		Type	Posit
#		At	T_Root
#		Part	:FireExplosionFlash.part
#		#Part2	:GernadeLauncherSpark.part
#		
#		Lifespan 5
#		
#	End
#End

Condition
	On 	PrimeHit
	
	Event
		Ename 	Prime
		Type	Destroy
	End


End


End							