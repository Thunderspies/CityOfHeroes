#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Condition
	
	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR

		Geom	Staff_06		
		
		#LifeSpan 45
	End

End

Condition
	On 	Time
	Time 	46
	Event
		EName	SwordFlash2
		Type	Local
		At	InitialSwordFire
		Altpiv	1
		
		Part1	:WhiteFlash.part
		LifeSpan 14
	End

	Event
		Type	posit
		At	SwordFlash2

		Part	:StreaksMagnet.part
		Part	:DarkStreaksBlueMagnet.part
		Part	:DarkStreaksRedMagnet.part
		Part	:DarkStreaksMagnet.part

		Magnet	Target
		LookAt	Target

		LifeSpan 14
	End
End

###########################################################
Condition
	On	Time
	Time	0

	Event
		EName	core
		Type	Local
		At	InitialSwordFire
		Altpiv	1
		Geom	PsionicRigging	
		#BHVR	Powers/ItemsandTempPowers/BlastOffset.bhvr
		
	End

End
	
Condition
	On	Time
	Time	48

	Event
		EName	prime
		Type	Start
		At	Core
		   Altpiv   2
		Geom	FrontFacingLocalPivot
		bhvr	behaviors/PsionicLance.bhvr
		Magnet	T_Head
		LookAt  T_Head
		Sound DiviningBlast 80 80 .77
	End

	Event
		Type	Local
		At	Prime
		  AltPiv 1
		BHVR	Behaviors/CrystalScale.bhvr
		Geom	PsiLance
		Part1	:RingStreakSmall.part
		Part2	:CrystalStreakGlow.part
		Part	:SMALLWhiteCometRingsThin.part
		Part	:SMALLWhiteCometRings.part

	End
	
End

Condition
	On	PrimeHit
		
	Event
		EName	Prime
		Type	Destroy
	End

		
	Event
		EName	all
		Type	Destroy
	End

End

#Condition
#	On Time
#	Time 32
#	
#	Event
#		EName	Core
#		Type	Destroy
#	End
#
#	Event
#		EName	Core2
#		Type	Destroy
#	End
#
#	Event
#		EName	Core3
#		Type	Destroy
#	End
#
#	Event
#		EName	Flash1
#		Type	Destroy
#	End
#
#	Event
#		EName	Flash2
#		Type	Destroy
#	End
#
#	
#End
#
#Condition
#	On Time
#	Time 66
#
#	Event
#		EName	RingsAndParticles
#		Type	Destroy
#	End
#
#End
#
#Condition
#	On Time
#	Time 28
#	Event
#		EName	RingsAndParticles3
#		Type	Destroy
#	End
#
#	Event
#		EName	RingsAndParticles1
#		Type	Destroy
#	End
#
#	Event
#		EName	RingsAndParticles2
#		Type	Destroy
#	End
#	
#End
	

End	


			